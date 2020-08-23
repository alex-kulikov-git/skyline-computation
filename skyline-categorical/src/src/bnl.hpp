/*
 * bnl.hpp
 * Created on: Sptember 20, 2018
 * Author: Oleksii Kulikov
 */

#ifndef BNL_H
#define BNL_H

#include <cstddef>
#include <vector>

#include "cat_operator.hpp"

class BNL: public CatOperator {
public:
	BNL(CatOperator *parent, CatOperator *child);
	~BNL(){
		;
	}
	std::vector<int> getNext();
	void computeSkyline();
	void computeSkylineProduce();
	void produce();
	void produceParallel(); // dummy
	void consume(const std::vector<int> &inputTuple);
	void set_parent(CatOperator *parent);
private:
	CatOperator *parent, *child;
	std::size_t counter;
	std::vector<std::vector<int>> storage;
	bool dominates(const std::vector<int> &dominator, const std::vector<int> &dominated);
};

#endif // BNL_H

