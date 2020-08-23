/*
 * nnl.hpp
 * Created on: June 29, 2018
 * Author: Oleksii Kulikov
 */

#ifndef NNL_H
#define NNL_H

#include <cstddef>
#include <vector>

#include "operator.hpp"

class NNL: public Operator {
public:
	NNL(Operator *parent, Operator *child);
	~NNL(){
		;
	}
	std::vector<double> getNext(); // dummy
	void computeSkylineProduce();
	void computeSkylineProduceParallel();
	void produce();
	void produceParallel();
	void consume(const std::vector<double> &inputTuple);
	void set_parent(Operator *parent);
private:
	Operator *parent, *child;
	std::size_t counter;
	std::vector<std::vector<double>> storage;
	bool dominates(const std::vector<double> &dominator, const std::vector<double> &dominated);
};

#endif // NNL_H

