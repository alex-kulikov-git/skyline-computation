/*
 * generator.hpp
 * Created on: July 5, 2018
 * Author: Oleksii Kulikov
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include <random>
#include <algorithm>

#include "operator.hpp"

class Generator: public Operator {
public:
	Generator(Operator *parent, const std::size_t &n, const std::size_t &dimension);
	~Generator(){
		;
	}
	void init();
	std::vector<double> getNext();
	void produce();
	void produceParallel(); // dummy
	void consume(const std::vector<double> &input); // dummy
	void set_parent(Operator *parent);
	std::vector<std::vector<double>> getTuples(); // FOR TESTING
	void print(); // FOR TESTING
private:
	Operator *parent;
	const std::size_t &n, &dimension;
	std::size_t counter;
	std::vector<std::vector<double>> storage; // FOR TESTING
};

#endif // GENERATOR_H
