/*
 * nnl.hpp
 * Created on: June 29, 2018
 * Author: Oleksii Kulikov
 */

#ifndef NNL_H
#define NNL_H

#include <cstddef>
#include <vector>

#include "cat_operator.hpp"

class NNL: public CatOperator {
public:
	NNL(CatOperator *parent, CatOperator *child);
	~NNL(){
		;
	}
	std::vector<int> getNext();
	void computeSkylineProduce();
	void computeSkylineProduceParallel();
	void produce();
	void produceParallel();
	void consume(const std::vector<int> &inputTuple);
	void set_parent(CatOperator *parent);
private:
	CatOperator *parent, *child;
	std::size_t counter;
	std::vector<std::vector<int>> storage;
	bool dominates(const std::vector<int> &dominator, const std::vector<int> &dominated);
};

#endif // NNL_H

