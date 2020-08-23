/*
 * sts.hpp
 * Created on: August 12, 2018
 * Author: Oleksii Kulikov
 */

#ifndef STS_H
#define STS_H

#include <cstddef>
#include <vector>

#include "cat_operator.hpp"
#include "ntree.hpp"

class STS: public CatOperator {
public:
	STS(CatOperator *parent, CatOperator *child, NTree &tree);
	~STS(){
		;
	}
	void computeSkylineProduce();
	void produce();
	void produceParallel(); // dummy
	void consume(const std::vector<int> &inputTuple);
	std::vector<int> getNext();
	void set_parent(CatOperator *parent);
private:
	CatOperator *parent, *child;
	NTree &tree;
	std::vector<std::vector<int>> storage;
	int sum(const std::vector<int> &tuple);
	int min(const std::vector<int> &tuple);
	int max(const std::vector<int> &tuple);
	void sort(std::vector<std::vector<int>> &tuples);
};

#endif // STS_H
