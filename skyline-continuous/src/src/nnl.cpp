/*
 * nnl.cpp
 * Created on: June 29, 2018
 * Author: Oleksii Kulikov
 */

#include "nnl.hpp"

#include <tbb/tbb.h>

using namespace tbb;

NNL::NNL(Operator *parent, Operator *child) : parent(parent), child(child){
	counter = 0;
	storage = {};
}

/**
 * Checks whether one tuple dominates the other and returns true|false
 * @param dominator the tuple to check for dominating
 * @param dominated the tuple to check for being dominated
 */
bool NNL::dominates(const std::vector<double> &dominator, const std::vector<double> &dominated){
	// dominated not worse in all
	bool flag = true;
	for(std::vector<int>::size_type i = 0; i< dominated.size(); i++){
		if(dominator[i] > dominated[i]) return false;
		if(dominated[i] > dominator[i]) flag = false;
	}
	if(flag) return false;
	return true;
}

/**
 * Naive-Nested-Loops Produce/Consume
 */
void NNL::computeSkylineProduce(){
	for(std::size_t i = 0; i < storage.size(); i++){
		bool not_dominated = true;
		for(std::size_t j = 0; j < storage.size(); j++){
			if(i != j){
				if(dominates(storage[j], storage[i])){
					not_dominated = false;
					break;
				}
			}
		}
		if(not_dominated){
			parent->consume(storage[i]);
		}
	}
}

/**
 * Naive-Nested-Loops Produce/Consume Parallelized
 */
void NNL::computeSkylineProduceParallel(){
	const std::vector<std::vector<double>> storage = this->storage;
	Operator *parent = this->parent;
	parallel_for(std::size_t(0), storage.size(), [this, storage, parent]( std::size_t i ) {
		bool not_dominated = true;
		bool flag = false;
		for(std::size_t j = 0; j < storage.size() && !flag; j++){
			if(i != j){
				if(dominates(storage[j], storage[i])){
					not_dominated = false;
					flag = true;
				}
			}
		}
		// the mutex somewhat slows down the parallelization
		// if mutex is taken out, the programmer has to make sure no race condition occurs between threads
		static spin_mutex mtx;
		spin_mutex::scoped_lock lock(mtx);
		if(not_dominated){
			parent->consume(storage[i]);
		}
	} );
}

void NNL::consume(const std::vector<double> &inputTuple){
	storage.push_back(inputTuple);
}

void NNL::produce(){
	storage = {};
	child->produce();
	computeSkylineProduce();
}

void NNL::produceParallel(){
	storage = {};
	child->produce();
	computeSkylineProduceParallel();
}

void NNL::set_parent(Operator *parent){
	this->parent = parent;
}

// dummy method
std::vector<double> NNL::getNext(){
	return {};
}



