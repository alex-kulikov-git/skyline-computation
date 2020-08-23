/*
 * parallel_ParallelSARTS.cpp
 * Created on: September 4, 2018
 * Author: Oleksii Kulikov
 */

#include "parallel_sarts.hpp"

#include <algorithm>
#include <iostream>
#include "tbb/parallel_sort.h"
#include "tbb/tbb.h"
#include <future>
#include <cstddef>

ParallelSARTS::ParallelSARTS(CatOperator *parent, CatOperator *child, ART &art) : parent(parent), child(child), art(art) {
	storage = {};
}

// returns the sum of all attributes of the given tuple
int ParallelSARTS::sum(const std::vector<int> &tuple){
	int sum = 0;
	for(std::size_t i = 0; i < tuple.size(); i++){
		sum += tuple[i];
	}
	return sum;
}

// returns the min attribute value of the given tuple
int ParallelSARTS::min(const std::vector<int> &tuple){
	return tuple[std::distance(tuple.begin(), std::min_element(tuple.begin(), tuple.end()))];
}

// returns the max attribute value of the given tuple
int ParallelSARTS::max(const std::vector<int> &tuple){
	return tuple[std::distance(tuple.begin(), std::max_element(tuple.begin(), tuple.end()))];
}

// sorts all tuples according to minC(t) = (min_Ai{t[Ai]}, sum(t[Ai]))
void ParallelSARTS::sort(std::vector<std::vector<int>> &tuples){
	// sort tuples according to smallest attribute first and sum of attributes as tie-breaker
	tbb::parallel_sort(tuples.begin(), tuples.end(), [this](const std::vector<int> &a, const std::vector<int> &b){
		if(min(a) != min(b)){
			return min(a) < min(b);
		}
		else{
			return sum(a) < sum(b);
		}
	});
}

/**
 * Divides the original input set of tuples into subsets and calculates for each of them individually the skyline.
 * Then, merges the subresults with the SARTS algorthm utilizing the ART tree and produces the final skyline result.
 */
void ParallelSARTS::computeSkylineProduce(){
	const std::vector<std::vector<int>> storage = this->storage;
	const std::size_t number_of_threads = NUMBER_OF_THREADS;

	std::vector<ART*> subtrees;
	for(std::size_t i = 0; i < NUMBER_OF_THREADS; i++){
		ART* subtree = new ART(art.get_attributes());
		subtrees.push_back(subtree);
	}
	std::future<void> futures[NUMBER_OF_THREADS];

	// compute subquery skylines
//	tbb::parallel_for(std::size_t(0), number_of_threads, [this, storage, number_of_threads, subtrees](std::size_t i) {
	for(std::size_t i = 0; i < NUMBER_OF_THREADS; i++){
		std::vector<std::vector<int>> subset;
		if(i == number_of_threads-1){ // if last thread, then also hand over the tuples left in the division rest
			subset.resize(storage.size() / number_of_threads + storage.size() % number_of_threads);
		}
		else{
			subset.resize(storage.size() / number_of_threads);
		}
		for(std::size_t j = 0; j < subset.size(); j++){
			subset[j] = storage[i*(storage.size()/number_of_threads) + j];
		}
//		computeSkylineSubset(i, subset, subtrees[i]);
		futures[i] = std::async(std::launch::async, &ParallelSARTS::computeSkylineSubset, this, i, subset, subtrees[i]);
	}
	for(std::size_t i = 0; i < NUMBER_OF_THREADS; i++){
		futures[i].get();
	}

	// compute final skyline
	std::vector<std::vector<int>> input;
	for(std::size_t i = 0; i < subset_results.size(); i++){
		if(!subset_results[i].empty()){
			input.push_back(subset_results[i]);
		}
	}
	sort(input);
	std::vector<int> t_stop = input[0];
	art.insert(input[0], art.root, art.root, 0);
	parent->consume(input[0]);
	for(std::size_t i = 1; i < input.size(); i++){
		if((max(t_stop) <= min(input[i])) && (t_stop != input[i])){
			return;
		}
		if(!art.is_dominated(input[i], art.root, 0, art.score(input[i]))){
			parent->consume(input[i]);
			art.insert(input[i], art.root, art.root, 0);
			if(max(input[i]) < max(t_stop)){
				t_stop = input[i];
			}
		}
	}

	// free memory
	for(std::size_t i = 0; i < subtrees.size(); i++){
		if(subtrees[i] != nullptr) delete subtrees[i];
	}
}

// computes the skyline for a subset of the original query with the SARTS algorithm
void ParallelSARTS::computeSkylineSubset(unsigned threadNumber, std::vector<std::vector<int>> tuples, ART* tree){
	// pre-sort tuples in place
	sort(tuples);
	std::vector<int> t_stop = tuples[0];
	tree->insert(tuples[0], tree->root, tree->root, 0);
	subset_results[threadNumber * (subset_results.size() / NUMBER_OF_THREADS) + 0] = tuples[0];
	for(std::size_t k = 1; k < tuples.size(); k++){
		// stop if all tuples left are dominated a-priori
		if((max(t_stop) <= min(tuples[k])) && (t_stop != tuples[k])){
			return;
		}
		// check for dominance
		if(!tree->is_dominated(tuples[k], tree->root, 0, tree->score(tuples[k]))){
			subset_results[threadNumber * (subset_results.size() / NUMBER_OF_THREADS) + k] = tuples[k];
			tree->insert(tuples[k], tree->root, tree->root, 0);
			if(max(tuples[k]) < max(t_stop)){
				t_stop = tuples[k];
			}
		}
	}
}

void ParallelSARTS::consume(const std::vector<int> &inputTuple){
	storage.push_back(inputTuple);
}

void ParallelSARTS::produce(){
	storage = {};
	child->produce();
	subset_results.resize(storage.size());
	computeSkylineProduce();
}

void ParallelSARTS::set_parent(CatOperator *parent){
	this->parent = parent;
}

std::vector<int> ParallelSARTS::getNext(){
	return {};
}

// FOR TESTING ONLY
void ParallelSARTS::printSubsetResults(){
	for(std::size_t i = 0; i < subset_results.size(); i++){
		std::cout << "Tuple[" << i << "] is (";
		for(std::vector<int>::size_type j = 0; j < subset_results[i].size(); j++){
			std::cout << subset_results[i][j];
			if(j!=subset_results[i].size()-1){
				std::cout << ' ';
			}
		}
		std::cout << ")\n";
	}
	std::cout << std::endl;
}

// dummy method
void ParallelSARTS::produceParallel(){
	;
}



