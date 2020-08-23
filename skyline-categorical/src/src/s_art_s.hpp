/*
 * s_art_s.hpp
 * Created on: August 18, 2018
 * Author: Oleksii Kulikov
 */

#ifndef SARTS_H
#define SARTS_H

#include <cstddef>
#include <vector>

#include "art.hpp"
#include "cat_operator.hpp"

class SARTS: public CatOperator {
public:
	SARTS(CatOperator *parent, CatOperator *child, ART &art);
	~SARTS(){
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
	int sum(const std::vector<int> &tuple);
	int min(const std::vector<int> &tuple);
	int max(const std::vector<int> &tuple);
	void sort(std::vector<std::vector<int>> &tuples);
};

#endif // SARTS_H
