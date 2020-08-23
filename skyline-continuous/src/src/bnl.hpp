/*
 * bnl.hpp
 * Created on: Sptember 20, 2018
 * Author: Oleksii Kulikov
 */

#ifndef BNL_H
#define BNL_H

#include <cstddef>
#include <vector>

#include "operator.hpp"

class BNL: public Operator {
public:
	BNL(Operator *parent, Operator *child);
	~BNL(){
		;
	}
	std::vector<double> getNext();
	void computeSkyline();
	void computeSkylineProduce();
//	void computeSkylineProduceParallel();
	void produce();
	void produceParallel(); // dummy
	void consume(const std::vector<double> &inputTuple);
	void set_parent(Operator *parent);
private:
	Operator *parent, *child;
	std::size_t counter;
	std::vector<std::vector<double>> storage;
	bool dominates(const std::vector<double> &dominator, const std::vector<double> &dominated);
};

#endif // BNL_H

