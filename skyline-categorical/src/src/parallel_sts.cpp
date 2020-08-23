/*
 * ParallelSTS.cpp
 * Created on: August 12, 2018
 * Author: Oleksii Kulikov
 */

#include "parallel_sts.hpp"

#include <algorithm>
#include "tbb/parallel_sort.h"
#include "tbb/tbb.h"
#include <future>
#include <cstddef>

ParallelSTS::ParallelSTS(CatOperator *parent, CatOperator *child, NTree &ntree) : parent(parent), child(child), ntree(ntree) {
	storage = {};
}

// returns the sum of all attributes of the given tuple
int ParallelSTS::sum(const std::vector<int> &tuple){
	int sum = 0;
	for(std::size_t i = 0; i < tuple.size(); i++){
		sum += tuple[i];
	}
	return sum;
}

// returns the min attribute value of the given tuple
int ParallelSTS::min(const std::vector<int> &tuple){
	return tuple[std::distance(tuple.begin(), std::min_element(tuple.begin(), tuple.end()))];
}

// returns the max attribute value of the given tuple
int ParallelSTS::max(const std::vector<int> &tuple){
	return tuple[std::distance(tuple.begin(), std::max_element(tuple.begin(), tuple.end()))];
}

// sorts all tuples according to minC(t) = (min_Ai{t[Ai]}, sum(t[Ai]))
void ParallelSTS::sort(std::vector<std::vector<int>> &tuples){
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
 * Then, merges the subresults with the ST-S algorthm utilizing the N-Tree and produces the final skyline result.
 */
void ParallelSTS::computeSkylineProduce(){
	const std::vector<std::vector<int>> storage = this->storage;
	const std::size_t number_of_threads = NUMBER_OF_THREADS;

	std::vector<NTree*> subtrees;
	for(std::size_t i = 0; i < NUMBER_OF_THREADS; i++){
		NTree* subtree = new NTree(ntree.get_attributes());
		subtrees.push_back(subtree);
	}
	std::future<void> futures[NUMBER_OF_THREADS];

	// compute subquery skylines
	for(std::size_t i = 0; i < number_of_threads; i++){
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
		futures[i] = std::async(std::launch::async, &ParallelSTS::computeSkylineSubset, this, i, subset, subtrees[i]);
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
	ntree.insert(input[0], ntree.root, 0);
	parent->consume(input[0]);
	for(std::size_t i = 1; i < input.size(); i++){
		if((max(t_stop) <= min(input[i])) && (t_stop != input[i])){
			return;
		}
		if(!ntree.is_dominated(input[i], ntree.root, 0, ntree.score(input[i]))){
			parent->consume(input[i]);
			ntree.insert(input[i], ntree.root, 0);
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

// computes the skyline for a subset of the original query with the ST-S algorithm using N-Tree
void ParallelSTS::computeSkylineSubset(unsigned threadNumber, std::vector<std::vector<int>> tuples, NTree* tree){
	// pre-sort tuples in place
	sort(tuples);
	std::vector<int> t_stop = tuples[0];
	tree->insert(tuples[0], tree->root, 0);
	subset_results[threadNumber * (subset_results.size() / NUMBER_OF_THREADS) + 0] = tuples[0];
	for(std::size_t k = 1; k < tuples.size(); k++){
		// stop if all tuples left are dominated a-priori
		if((max(t_stop) <= min(tuples[k])) && (t_stop != tuples[k])){
			return;
		}
		// check for dominance
		if(!tree->is_dominated(tuples[k], tree->root, 0, tree->score(tuples[k]))){
			subset_results[threadNumber * (subset_results.size() / NUMBER_OF_THREADS) + k] = tuples[k];
			tree->insert(tuples[k], tree->root, 0);
			if(max(tuples[k]) < max(t_stop)){
				t_stop = tuples[k];
			}
		}
	}
}

void ParallelSTS::consume(const std::vector<int> &inputTuple){
	storage.push_back(inputTuple);
}

void ParallelSTS::produce(){
	storage = {};
	child->produce();
	subset_results.resize(storage.size());
	computeSkylineProduce();
}

void ParallelSTS::set_parent(CatOperator *parent){
	this->parent = parent;
}

// dummy method
std::vector<int> ParallelSTS::getNext(){
	return {};
}

// dummy method
void ParallelSTS::produceParallel(){
	;
}


