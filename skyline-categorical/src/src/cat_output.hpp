/*
 * cat_output.hpp
 * Created on: August 13, 2018
 * Author: Oleksii Kulikov
 */

#ifndef CATEGORICAL_OUTPUT_H
#define CATEGORICAL_OUTPUT_H

#include <vector>

#include "cat_operator.hpp"

class CatOutput: public CatOperator{
public:
	CatOutput(CatOperator *child);
	~CatOutput(){
		;
	}
	void getTuples();
	void getTuplesCP();
	void getTuplesParallelCP();
	void consume(const std::vector<int> &inputTuple);
	void produce(); // dummy
	void produceParallel(); // dummy
	std::vector<int> getNext(); // dummy
	bool store(const std::vector<int> &tuple);
	void print();
	std::vector<std::vector<int>> getStorage(); // FOR TESTING ONLY
private:
	CatOperator *my_child;
	std::vector<std::vector<int>> storage;
};

#endif // CATEGORICAL_OUTPUT_H
