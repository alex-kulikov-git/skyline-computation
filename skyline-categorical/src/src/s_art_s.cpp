/*
 * s_art_s.cpp
 * Created on: August 18, 2018
 * Author: Oleksii Kulikov
 */

#include "s_art_s.hpp"

#include <algorithm>
#include <iostream>
#include "tbb/parallel_sort.h"

SARTS::SARTS(CatOperator *parent, CatOperator *child, ART &art) : parent(parent), child(child), art(art) {
	storage = {};
}

// returns the sum of all attributes of the given tuple
int SARTS::sum(const std::vector<int> &tuple){
	int sum = 0;
	for(std::size_t i = 0; i < tuple.size(); i++){
		sum += tuple[i];
	}
	return sum;
}

// returns the min attribute value of the given tuple
int SARTS::min(const std::vector<int> &tuple){
	return tuple[std::distance(tuple.begin(), std::min_element(tuple.begin(), tuple.end()))];
}

// returns the max attribute value of the given tuple
int SARTS::max(const std::vector<int> &tuple){
	return tuple[std::distance(tuple.begin(), std::max_element(tuple.begin(), tuple.end()))];
}

// sorts all tuples according to minC(t) = (min_Ai{t[Ai]}, sum(t[Ai]))
void SARTS::sort(std::vector<std::vector<int>> &tuples){
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
 * Computes the Skyline with the ST-S algorithm using ART
 */
void SARTS::computeSkylineProduce(){
	// pre-sort tuples in place (storage)
	sort(storage);
	std::vector<int> t_stop = storage[0];

	// insert first tuple
	art.insert(storage[0], art.root, art.root, 0);
	parent->consume(storage[0]);

	for(std::size_t i = 1; i < storage.size(); i++){
		// stop if all the tuples left are dominated à-priori
		if((max(t_stop) <= min(storage[i])) && (t_stop != storage[i])){
			return;
		}
		// check for dominance
		if(!art.is_dominated(storage[i], art.root, 0, art.score(storage[i]))){
			parent->consume(storage[i]);
//			Node4* temp = static_cast<Node4*>(art.root); // for debugging only
//			Node16* temp16 = static_cast<Node16*>(art.root); // for debugging only
			art.insert(storage[i], art.root, art.root, 0);
			if(max(storage[i]) < max(t_stop)){
				t_stop = storage[i];
			}
		}
	}
}

void SARTS::consume(const std::vector<int> &inputTuple){
	storage.push_back(inputTuple);
}

void SARTS::produce(){
	storage = {};
	child->produce();
	computeSkylineProduce();
}

void SARTS::set_parent(CatOperator *parent){
	this->parent = parent;
}

// dummy method
std::vector<int> SARTS::getNext(){
	return {};
}

// dummy method
void SARTS::produceParallel(){
	;
}

