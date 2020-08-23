/*
 * cat_operator.hpp
 * Created on: August 13, 2018
 * Author: Oleksii Kulikov
 */

#ifndef CATEGORICAL_OPERATOR_H
#define CATEGORICAL_OPERATOR_H

#include <vector>

class CatOperator{
public:
	virtual ~CatOperator(){
		;
	}
	virtual void produce() = 0;
	virtual void produceParallel() = 0;
	virtual void consume(const std::vector<int> &input) = 0;
	virtual std::vector<int> getNext() = 0;
private:

};

#endif // CATEGORICAL_OPERATOR_H
