/*
 * cat_output.cpp
 * Created on: August 13, 2018
 * Author: Oleksii Kulikov
 */

#include <iostream>

#include "cat_output.hpp"

CatOutput::CatOutput(CatOperator *child) : my_child(child){
	storage = {};
}

void CatOutput::getTuples(){
	while(store(my_child->getNext())){
			;
	}
}

void CatOutput::getTuplesCP(){
	storage = {};
    my_child->produce();
}

void CatOutput::getTuplesParallelCP(){
	storage = {};
	my_child->produceParallel();
}

void CatOutput::consume(const std::vector<int> &inputTuple){
	store(inputTuple);
}

bool CatOutput::store(const std::vector<int> &tuple){
	if(!tuple.empty()){
		storage.push_back(tuple);
		return true;
	}
	else{
		return false;
	}
}

void CatOutput::print(){
	std::cout << "Resulting Skyline: \n";
	for(std::vector<std::vector<int>>::size_type i = 0; i < storage.size(); i++){
		std::cout << "Tuple[" << i << "] is (";
		for(std::vector<int>::size_type j = 0; j < storage[i].size(); j++){
			std::cout << storage[i][j];
			if(j!=storage[i].size()-1){
				std::cout << ' ';
			}
		}
		std::cout << ")\n";
	}
	std::cout << std::endl;
}

// FOR TESTING ONLY
std::vector<std::vector<int>> CatOutput::getStorage(){
	return storage;
}

// dummy method
void CatOutput::produce(){
	;
}

// dummy method
std::vector<int> CatOutput::getNext(){
	return {};
}

// dummy method
void CatOutput::produceParallel(){
	;
}



