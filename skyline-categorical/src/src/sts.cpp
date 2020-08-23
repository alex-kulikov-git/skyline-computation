/*
 * sts.cpp
 * Created on: August 12, 2018
 * Author: Oleksii Kulikov
 */

#include "sts.hpp"

#include <algorithm>
#include "tbb/parallel_sort.h"

STS::STS(CatOperator *parent, CatOperator *child, NTree &tree) : parent(parent), child(child), tree(tree) {
	storage = {};
}

// returns the sum of all attributes of the given tuple
int STS::sum(const std::vector<int> &tuple){
	int sum = 0;
	for(std::size_t i = 0; i < tuple.size(); i++){
		sum += tuple[i];
	}
	return sum;
}

// returns the min attribute value of the given tuple
int STS::min(const std::vector<int> &tuple){
	return tuple[std::distance(tuple.begin(), std::min_element(tuple.begin(), tuple.end()))];
}

// returns the max attribute value of the given tuple
int STS::max(const std::vector<int> &tuple){
	return tuple[std::distance(tuple.begin(), std::max_element(tuple.begin(), tuple.end()))];
}

// sorts all tuples according to minC(t) = (min_Ai{t[Ai]}, sum(t[Ai]))
void STS::sort(std::vector<std::vector<int>> &tuples){
	// sort tuples according to smallest attribute first and sum of attributes as tie-breaker
	std::sort(tuples.begin(), tuples.end(), [this](const std::vector<int> &a, const std::vector<int> &b){
		if(min(a) != min(b)){
			return min(a) < min(b);
		}
		else{
			return sum(a) < sum(b);
		}
	});
}

/**
 * Computes the Skyline with the ST-S algorithm
 */
void STS::computeSkylineProduce(){
	// pre-sort tuples in place (storage)
	sort(storage);
	std::vector<int> t_stop = storage[0];
	tree.insert(storage[0], tree.root, 0);
	parent->consume(storage[0]);
	for(std::size_t i = 1; i < storage.size(); i++){
		// stop if all the tuples left are dominated à-priori
		if((max(t_stop) <= min(storage[i])) && (t_stop != storage[i])){
			return;
		}
		// check for dominance
		if(!tree.is_dominated(storage[i], tree.root, 0, tree.score(storage[i]))){
			parent->consume(storage[i]);
			tree.insert(storage[i], tree.root, 0);
			if(max(storage[i]) < max(t_stop)){
				t_stop = storage[i];
			}
		}
	}
}

void STS::consume(const std::vector<int> &inputTuple){
	storage.push_back(inputTuple);
}

void STS::produce(){
	storage = {};
	child->produce();
	computeSkylineProduce();
}

void STS::set_parent(CatOperator *parent){
	this->parent = parent;
}

// dummy method
std::vector<int> STS::getNext(){
	return {};
}

// dummy method
void STS::produceParallel(){
	;
}

