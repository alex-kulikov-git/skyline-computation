/*
 * art.hpp
 * Created on: August 22, 2018
 * Author: Oleksii Kulikov
 */

#ifndef ART_H
#define ART_H

#include <cstddef>

#include "art_nodes.hpp"

class ART{
public:
	ART(const std::vector<int> &attributes);
	~ART();
	Node *root;
	void insert(const std::vector<int> &element, Node *&parent, Node *&current, unsigned int level);
	unsigned int score(const std::vector<int> &tuple);
	bool is_dominated(const std::vector<int> &tuple, Node* p, unsigned int level, unsigned int currentScore);
	void print();
	std::vector<int> get_attributes();
private:
	const std::vector<int> attributes;
	unsigned int tupleID;
	Node* findChild(Node* parent, const int &attribute);
	void grow(Node *&parent, Node *&current, const int &indexOfCurrent);
	void updateParent(Node *&parent, Node* newChild, const int &indexOfChild);
	Node* newChild(Node *&parent, const int &attribute);
	int binarySearch(uint8_t arr[], unsigned l, unsigned r, const int& value);
	void freeMemory();
};

#endif // ART_H
