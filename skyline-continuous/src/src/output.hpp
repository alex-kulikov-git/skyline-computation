/*
 * output.hpp
 * Created on: July 5, 2018
 * Author: Oleksii Kulikov
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include <vector>

#include "operator.hpp"

class Output: public Operator{
public:
	Output(Operator *child);
	~Output(){
		;
	}
	void getTuples();
	void getTuplesCP();
	void getTuplesParallelCP();
	void consume(const std::vector<double> &inputTuple);
	void produce(); // dummy
	void produceParallel(); // dummy
	bool store(const std::vector<double> &tuple);
	std::vector<double> getNext(); // dummy
	void print();
	std::vector<std::vector<double>> getStorage(); // FOR TESTING
private:
	Operator *my_child;
	std::vector<std::vector<double>> storage;
};

#endif // OUTPUT_H
