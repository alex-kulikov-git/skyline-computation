/*
 * node.hpp
 * Created on: August 11, 2018
 * Author: Oleksii Kulikov
 */

#ifndef NODE_H
#define NODE_H

#include <vector>

struct node{
	unsigned int minScore;
	unsigned int maxScore;
	std::vector<int> tupleIDs;
	std::vector<node*> children;
};

#endif // NODE_H
