/*
 * parallel_sarts.hpp
 * Created on: September 4, 2018
 * Author: Oleksii Kulikov
 */

#ifndef PARALLEL_SARTS_H
#define PARALLEL_SARTS_H

#include <cstddef>
#include <vector>

#include "art.hpp"
#include "cat_operator.hpp"

class ParallelSARTS: public CatOperator {
public:
	ParallelSARTS(CatOperator *parent, CatOperator *child, ART &art);
	~ParallelSARTS(){
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
	ART &art;
	std::vector<std::vector<int>> storage;
	std::vector<std::vector<int>> subset_results;
	const std::size_t NUMBER_OF_THREADS = 4;
	int sum(const std::vector<int> &tuple);
	int min(const std::vector<int> &tuple);
	int max(const std::vector<int> &tuple);
	void sort(std::vector<std::vector<int>> &tuples);
	void computeSkylineSubset(unsigned threadNumber, std::vector<std::vector<int>> tuples, ART* subtree);
	void printSubsetResults(); // for debugging only
};

#endif // PARALLEL_SARTS_H
