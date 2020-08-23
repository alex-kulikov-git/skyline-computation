/*
 * cat_generator.cpp
 * Created on: August 12, 2018
 * Author: Oleksii Kulikov
 */

#include "cat_generator.hpp"

#include <iostream>


CatGenerator::CatGenerator(CatOperator *parent, const std::size_t &n, const std::size_t &dimension) : parent(parent), n(n), dimension(dimension){
	storage = {};
	init();
	counter = 0;
}

// Generates the tuples.
// The newer RNG with mt19937 is better for producing unbiased random numbers. It is however not compatible with the current Valgrind version.
void CatGenerator::init(){
	range_from = 0; // the entire range has to be no bigger than 256 elements due to the structure of the ART tree
	range_to = 255;
	std::random_device rand_dev;
	std::mt19937 generator(rand_dev());
	std::uniform_int_distribution<int> distr(range_from, range_to);
	storage = {};

	for(std::size_t i = 0; i < n; i++){
		std::vector<int> tuple;
		for(std::size_t j = 0; j < dimension; j++){
			tuple.push_back(distr(generator));
//			tuple.push_back(range_from + (rand() % static_cast<int>(range_to - range_from + 1)));
		}
		storage.push_back(tuple);
	}
}

void CatGenerator::produce(){
	for(std::size_t i = 0; i < storage.size(); i++){
		parent->consume(storage[i]);
	}
}

std::vector<int> CatGenerator::getNext(){
	while(counter < n){
		return storage[counter++];
	}
	return {}; // if no more tuples can be produced, return an empty vector
}

void CatGenerator::set_parent(CatOperator *parent){
	this->parent = parent;
}

std::vector<int> CatGenerator::getAttributes(){
	std::vector<int> attributes;
	for(auto i = range_from; i <= range_to; i++){
		attributes.push_back(i);
	}
	return attributes;
}

// FOR TESTING ONLY
std::vector<std::vector<int>> CatGenerator::getTuples(){
	return storage;
}

// FOR TESTING ONLY
void CatGenerator::print(){
	std::cout << "Generated tuples: \n";
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

// dummy method
void CatGenerator::consume(const std::vector<int> &input){
	;
}

// dummy method
void CatGenerator::produceParallel(){
	;
}


