/*
 * ntree.hpp
 * Created on: August 11, 2018
 * Author: Oleksii Kulikov
 */

#ifndef NTREE_H
#define NTREE_H

#include <cstddef>

#include "node.hpp"

class NTree{
public:
	NTree(const std::vector<int> &attributes);
	~NTree();
	node *root;
	void insert(const std::vector<int> &element, node* p, unsigned int level);
	unsigned int score(const std::vector<int> &tuple);
	bool is_dominated(const std::vector<int> &tuple, node* p, unsigned int level, unsigned int currentScore);
	void print();
	std::vector<int> get_attributes();
private:
	const std::vector<int> attributes;
	unsigned int tupleID;
	void freeMemory();
};

#endif // NTREE_H
