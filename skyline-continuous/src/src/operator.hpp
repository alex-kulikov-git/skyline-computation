/*
 * operator.hpp
 * Created on: July 5, 2018
 * Author: Oleksii Kulikov
 */

#ifndef OPERATOR_H
#define OPERATOR_H

#include <vector>

class Operator{
public:
	virtual ~Operator(){
		;
	}
	virtual std::vector<double> getNext() = 0;
	virtual void produce() = 0;
	virtual void produceParallel() = 0;
	virtual void consume(const std::vector<double> &input) = 0;
private:

};

#endif // OPERATOR_H
