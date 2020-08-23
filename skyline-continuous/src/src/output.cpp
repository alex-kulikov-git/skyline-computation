/*
 * output.cpp
 * Created on: July 5, 2018
 * Author: Oleksii Kulikov
 */

#include <iostream>

#include "output.hpp"

Output::Output(Operator *child) : my_child(child){
	storage = {};
}

void Output::getTuples(){
	while(store(my_child->getNext())){
		;
	}
}

void Output::getTuplesCP(){
	storage = {};
    my_child->produce();
}

void Output::getTuplesParallelCP(){
	storage = {};
	my_child->produceParallel();
}

void Output::consume(const std::vector<double> &inputTuple){
	store(inputTuple);
}

bool Output::store(const std::vector<double> &tuple){
	if(!tuple.empty()){
		storage.push_back(tuple);
		return true;
	}
	else{
		return false;
	}
}

// dummy method
void Output::produce(){
	;
}

// dummy method
void Output::produceParallel(){
	;
}

// dummy method
std::vector<double> Output::getNext(){
	return {};
}

void Output::print(){
	std::cout << "Resulting Skyline: \n";
	for(std::vector<std::vector<double>>::size_type i = 0; i < storage.size(); i++){
		std::cout << "Tuple[" << i << "] is (";
		for(std::vector<double>::size_type j = 0; j < storage[i].size(); j++){
			std::cout << storage[i][j] << ' ';
		}
		std::cout << ")\n";
	}
	std::cout << std::endl;
}

// FOR TESTING
std::vector<std::vector<double>> Output::getStorage(){
	return storage;
}



