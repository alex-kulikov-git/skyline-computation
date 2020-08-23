/*
 * ntree.cpp
 * Created on: August 11, 2018
 * Author: Oleksii Kulikov
 */

#include <cstddef>
#include <vector>
#include <iostream>
#include <math.h>
#include <queue>
#include <assert.h>

#include "ntree.hpp"

NTree::NTree(const std::vector<int> &attributes) : attributes(attributes) {
	assert(attributes.size() > 0);
	root = new node;
	tupleID = 0;
}

NTree::~NTree(){
	freeMemory();
}

void NTree::insert(const std::vector<int> &tuple, node* p, unsigned int level){
	if(level == 0){
		p->minScore = 0;
		p->maxScore = 0;
		for(std::size_t i = 0; i < tuple.size(); i++){
			p->maxScore += (int) (pow(2.0, (double) (tuple.size()-i)) * attributes[attributes.size()-1]);
		}
	}
	else{
		p->minScore = 0;
		for(std::size_t i = 0; i < level; i++){
			p->minScore += (int) (pow(2.0, (double) (tuple.size()-i)) * tuple[i]);
		}
		p->maxScore = p->minScore;
		for(std::size_t i = level; i < tuple.size(); i++){
			p->maxScore += (int) (pow(2.0, (double) (tuple.size()-i)) * attributes[attributes.size()-1]);
		}
	}
	if(level == tuple.size()){
		p->tupleIDs.push_back(tupleID++);
	}
	else{
		if(p->children.empty()){
			p->children.resize(attributes.size());
		}
		if(!p->children[tuple[level]]){
		   p->children[tuple[level]]=new node();
		}
		insert(tuple, p->children[tuple[level]], level+1);
	}
}

unsigned int NTree::score(const std::vector<int> &tuple){
	//2^(n-i) * t[Ai]
	unsigned long sum = 0;
	for(unsigned int i = 0; i < tuple.size(); i++){
		// t[Ai] are all integers
		sum += (int) (pow(2.0, (double) (tuple.size()-i)) * tuple[i]);
	}
	return sum;
}

// note: we are looking for minimal tuples
bool NTree::is_dominated(const std::vector<int> &tuple, node* p, unsigned int level, unsigned int currentScore){
	if(p==nullptr || (currentScore < p->minScore)){
		return false;
	}
	if((level == tuple.size()) && (score(tuple) != p->minScore)){
		return true;
	}
	if((level == tuple.size()) && (score(tuple) == p->minScore)){
		return false;
	}
	// search the subtrees from left to right
	unsigned int weight = (int) (pow(2.0, (double) (tuple.size()-level)) * tuple[level]);
	for(int i = 0; i < tuple[level]; i++){
		if(is_dominated(tuple, p->children[i], level+1, currentScore + weight)){
			return true;
		}
	}
	if(is_dominated(tuple, p->children[tuple[level]], level+1, currentScore)){
		return true;
	}
	return false;
}

std::vector<int> NTree::get_attributes(){
	return attributes;
}

// Prints the tree by Breadth-First-Search
void NTree::print(){
	int counter = 0;
	std::queue<node const *> queue;
	queue.push(root);
	while(!queue.empty()){
		// take an element from the queue and print it
		node const *n = queue.front();
		queue.pop();
		std::cout << "Node " << counter++ << ": ";
		std::cout << "minScore " << n->minScore;
		std::cout << ", maxScore " << n->maxScore;
		if(n->tupleIDs.size() > 0){
			std::cout << ", tupleIDs ";
			for(std::size_t i = 0; i < n->tupleIDs.size(); i++){
				std::cout << n->tupleIDs[i];
				if(i != n->tupleIDs.size()-1){
					std::cout << ", ";
				}
			}
		}
		std::cout << "\n";
		// push children from left to right into the queue
		if(!n->children.empty()){
			for(std::size_t i = 0; i < attributes.size(); i++){
				if(n->children[i] != nullptr){
					queue.push(n->children[i]);
				}
			}
		}
	}
	std::cout << "\n";
}

// frees the memory previously used by the n-tree
void NTree::freeMemory(){
	std::queue<node const *> queue;
	queue.push(root);
	while(!queue.empty()){
		// take an element from the queue, puts its children into the queue, and then delete the element
		node const *n = queue.front();
		queue.pop();
		if(!n->children.empty()){
			for(std::size_t i = 0; i < attributes.size(); i++){
				if(n->children[i] != nullptr){
					queue.push(n->children[i]);
				}
			}
		}
		delete n;
	}
}







