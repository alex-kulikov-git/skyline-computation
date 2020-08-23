/*
 * parallel_sts.hpp
 * Created on: August 12, 2018
 * Author: Oleksii Kulikov
 */

#ifndef PARALLEL_STS_H
#define PARALLEL_STS_H

#include <cstddef>
#include <vector>

#include "cat_operator.hpp"
#include "ntree.hpp"

class ParallelSTS: public CatOperator {
public:
	ParallelSTS(CatOperator *parent, CatOperator *child, NTree &tree);
	~ParallelSTS(){
		;
	}
	void computeSkylineProduce();
	void produce();
	void produceParallel(); // dummy
	void consume(const std::vector<int> &inputTuple);
	std::vector<int> getNext(); // dummy
	void set_parent(CatOperator *parent);
private:
	CatOperator *parent, *child;
	NTree &ntree;
	std::vector<std::vector<int>> storage;
	std::vector<std::vector<int>> subset_results;
	const std::size_t NUMBER_OF_THREADS = 4;
	int sum(const std::vector<int> &tuple);
	int min(const std::vector<int> &tuple);
	int max(const std::vector<int> &tuple);
	void sort(std::vector<std::vector<int>> &tuples);
	void computeSkylineSubset(unsigned threadNumber, std::vector<std::vector<int>> tuples, NTree* tree);
};

#endif // PARALLEL_STS_H
