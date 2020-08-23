/*
 * bnl.cpp
 * Created on: September 20, 2018
 * Author: Oleksii Kulikov
 */

#include "bnl.hpp"

BNL::BNL(Operator *parent, Operator *child) : parent(parent), child(child){
	counter = 0;
	storage = {};
}

/**
 * Checks whether one tuple dominates the other and returns true|false
 * @param dominator the tuple to check for dominating
 * @param dominated the tuple to check for being dominated
 */
bool BNL::dominates(const std::vector<double> &dominator, const std::vector<double> &dominated){
	// dominated not worse in all
	bool flag = true;
	for(std::vector<double>::size_type i = 0; i< dominated.size(); i++){
		if(dominator[i] > dominated[i]) return false;
		if(dominated[i] > dominator[i]) flag = false;
	}
	if(flag) return false;
	return true;
}

/**
 * Block-Nested-Loops Iterator Model
 */
void BNL::computeSkyline(){
	// storage is the window here
	storage.push_back(child->getNext());
	while(true){
		std::vector<double> tuple = child->getNext();
		if(!tuple.empty()){
			storage.push_back(tuple);
			for(std::size_t j = 0; j < storage.size()-1; j++){
				if(dominates(storage.back(), storage[j])){
					storage.erase(storage.begin() + j);
					j--;
				}
				else if (dominates(storage[j], storage.back())){
					storage.erase(storage.begin() + storage.size()-1);
					break;
				}
			}
		}
		else break;
	}
}

std::vector<double> BNL::getNext(){
	if(counter == 0){
		computeSkyline();
	}
	while(counter < storage.size()){
		return storage[counter++];
	}
	return {};
}

/**
 * Block-Nested-Loops Produce/Consume
 */
void BNL::computeSkylineProduce(){
	// storage is the window here
	std::vector<std::vector<double>> window;
	window.push_back(storage[0]);
	for(std::size_t i = 1; i < storage.size(); i++){
		std::vector<double> tuple = storage[i];
		window.push_back(tuple);
		for(std::size_t j = 0; j < window.size()-1; j++){
			if(dominates(window.back(), window[j])){
				window.erase(window.begin() + j);
				j--;
			}
			else if (dominates(window[j], window.back())){
				window.erase(window.begin() + window.size()-1);
				break;
			}
		}
	}
	for(std::size_t i = 0; i < window.size(); i++){
		parent->consume(window[i]);
	}
}

void BNL::consume(const std::vector<double> &inputTuple){
	storage.push_back(inputTuple);
}

void BNL::produce(){
	storage = {};
	child->produce();
	computeSkylineProduce();
}

void BNL::set_parent(Operator *parent){
	this->parent = parent;
}

// dummy method
void BNL::produceParallel(){
	;
}

