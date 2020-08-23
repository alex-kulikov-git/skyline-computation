/*
 * cat_generator.hpp
 * Created on: August 13, 2018
 * Author: Oleksii Kulikov
 */

#ifndef CATEGORICAL_GENERATOR_H
#define CATEGORICAL_GENERATOR_H

#include <random>
#include <algorithm>

#include "cat_operator.hpp"

class CatGenerator: public CatOperator {
public:
	CatGenerator(CatOperator *parent, const std::size_t &n, const std::size_t &dimension);
	~CatGenerator(){
		;
	}
	void init();
	void produce();
	void produceParallel(); // dummy method
	void consume(const std::vector<int> &input); // dummy method
	std::vector<int> getNext();
	void set_parent(CatOperator *parent);
	std::vector<int> getAttributes(); // for ST-S and SARTS
	std::vector<std::vector<int>> getTuples(); // FOR TESTING ONLY
	void print(); // FOR TESTING ONLY
private:
	CatOperator *parent;
	const std::size_t &n, &dimension;
	int range_from;
	int range_to;
	std::vector<std::vector<int>> storage; // FOR TESTING
	std::size_t counter;
};

#endif // CATEGORICAL_GENERATOR_H
