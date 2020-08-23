/*
 * generator.cpp
 * Created on: July 5, 2018
 * Author: Oleksii Kulikov
 */

#include <iostream>

#include "generator.hpp"

Generator::Generator(Operator *parent, const std::size_t &n, const std::size_t &dimension) : parent(parent), n(n), dimension(dimension){
	counter = 0; // for getNext()
	storage = {};
	init();
}

// Generates the tuples.
void Generator::init(){
	const double range_from = -100;
	const double range_to = 100;
	std::random_device rand_dev;
	std::mt19937 generator(rand_dev());
	std::uniform_real_distribution<double> distr(range_from, range_to);
	storage = {};

	for(std::size_t i = 0; i < n; i++){
		std::vector<double> tuple;
		for(std::size_t j = 0; j < dimension; j++){
			tuple.push_back(distr(generator));
		}
		storage.push_back(tuple);
	}
}

std::vector<double> Generator::getNext(){
	while(counter < n){
		return storage[counter++];
	}
	return {}; // if no more tuples can be produced, return an empty vector
}

void Generator::produce(){
	for(std::size_t i = 0; i < storage.size(); i++){
		parent->consume(storage[i]);
	}
}

void Generator::set_parent(Operator *parent){
	this->parent = parent;
}

// FOR TESTING ONLY
std::vector<std::vector<double>> Generator::getTuples(){
	return storage;
}

// FOR TESTING ONLY
void Generator::print(){
	std::cout << "Generated tuples: \n";
	for(std::vector<std::vector<double>>::size_type i = 0; i < storage.size(); i++){
		std::cout << "Tuple[" << i << "] is (";
		for(std::vector<double>::size_type j = 0; j < storage[i].size(); j++){
			std::cout << storage[i][j] << ' ';
		}
		std::cout << ")\n";
	}
	std::cout << std::endl;
}

// dummy method
void Generator::consume(const std::vector<double> &input){
	;
}

// dummy method
void Generator::produceParallel(){
	;
}


