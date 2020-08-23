/*
 * dnc.hpp
 * Created on: September 7, 2018
 * Author: Oleksii Kulikov
 */

#ifndef DNC_H
#define DNC_H

#include <cstddef>
#include <vector>

#include "operator.hpp"

class DNC: public Operator {
public:
	DNC(Operator *parent, Operator *child);
	~DNC(){
		;
	}
	std::vector<std::vector<double>> computeSkyline(const std::vector<std::vector<double>> &M, const int &dimension);
	std::vector<double> getNext(); // dummy
	void produce();
	void produceParallel(); // dummy
	void consume(const std::vector<double> &inputTuple);
	void set_parent(Operator *parent);
private:
	Operator *parent, *child;
	std::vector<std::vector<double>> storage;
	bool dominates(const std::vector<double> &dominator, const std::vector<double> &dominated);
	std::vector<double> median(std::vector<std::vector<double>> tuples, const int &dimension);
	std::pair<std::vector<std::vector<double>>, std::vector<std::vector<double>>> partition(const std::vector<std::vector<double>> &tuples, const int &dimension, const std::vector<double> &pivot);
	std::vector<std::vector<double>> mergeBasic(std::vector<std::vector<double>> S_1, const std::vector<std::vector<double>> &S_2, const int &dimension);
};

#endif // DNC_H

