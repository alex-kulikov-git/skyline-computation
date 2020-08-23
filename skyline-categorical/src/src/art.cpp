/*
 * art.cpp
 * Created on: August 22, 2018
 * Author: Oleksii Kulikov
 */

#include <cstddef>
#include <vector>
#include <iostream>
#include <math.h>
#include <queue>
#include <assert.h>
#include <string.h> // memmove

#include "art.hpp"

ART::ART(const std::vector<int> &attributes) : attributes(attributes) {
	assert(attributes.size() > 0);
	root = new Node4;
	tupleID = 0;
}

ART::~ART(){
	// delete root and all subsequent children
	freeMemory();
}

unsigned int ART::score(const std::vector<int> &tuple){
	//2^(n-i) * t[Ai]
	unsigned long sum = 0;
	for(unsigned int i = 0; i < tuple.size(); i++){
		// t[Ai] are all integers
		sum += (int) (pow(2.0, (double) (tuple.size()-i)) * tuple[i]);
	}
	return sum;
}

// find the child of the given parent, the edge to which we follow next
Node* ART::findChild(Node* parent, const int &attribute){
	switch(parent->type){
		case NodeType4: {
			Node4* node = static_cast<Node4*>(parent);
			for (unsigned i = 0; i < node->count; i++){
				if (node->key[i] == attribute){
					return node->children[i];
				}
			}
			return NULL;
		}
		case NodeType16: {
			Node16* node = static_cast<Node16*>(parent);
			// TODO: Maybe build in binary search here
//			int childIndex = binarySearch(node->key, 0, node->count-1, attribute);
//			if(childIndex >= 0){
//				return node->children[childIndex];
//			}
			for (unsigned i = 0; i < node->count; i++){
				if (node->key[i] == attribute){
					return node->children[i];
				}
			}
			return NULL;
		}
		case NodeType48: {
			Node48* node = static_cast<Node48*>(parent);
			if (node->childIndex[attribute] != emptyMarker){
				return node->children[node->childIndex[attribute]];
			}
			else
				return NULL;
		}
		case NodeType256: {
			Node256* node = static_cast<Node256*>(parent);
			return node->children[attribute];
		}
		default: {
			return NULL;
		}
	}
}

//unsigned created_counter = 1; // for debugging only

// add new child to the given node and return a pointer to the new child
Node* ART::newChild(Node *&node, const int &attribute){
	Node4* child = new Node4();
//	created_counter++; // for debugging only
	switch(node->type){
		case NodeType4: {
			Node4* parent = static_cast<Node4*>(node);
			unsigned pos;
			// make free space for the new child entry
			for (pos = 0; (pos < parent->count) && (parent->key[pos] < attribute); pos++);
			memmove(parent->key+pos+1, parent->key+pos, parent->count-pos);
			memmove(parent->children+pos+1, parent->children+pos, (parent->count-pos)*sizeof(Node*));
			parent->key[pos] = attribute;
			parent->children[pos] = child;
			parent->count++;
			break;
		}
		case NodeType16: {
			Node16* parent = static_cast<Node16*>(node);
			unsigned pos;
			// make free space for the new child entry
			for (pos = 0; (pos < parent->count) && (parent->key[pos] < attribute); pos++);
			memmove(parent->key+pos+1, parent->key+pos, parent->count-pos);
			memmove(parent->children+pos+1, parent->children+pos, (parent->count-pos)*sizeof(Node*));
			parent->key[pos] = attribute;
			parent->children[pos] = child;
			parent->count++;
			break;
		}
		case NodeType48: {
			Node48* parent = static_cast<Node48*>(node);
			unsigned pos = parent->count;
			// if there are empty slots inbetween, use them instead of appending the child pointer at the end
			if(parent->children[pos]){
				for(pos = 0; parent->children[pos] != NULL; pos++);
			}
			parent->children[pos] = child;
			parent->childIndex[attribute] = pos;
			parent->count++;
			break;
		}
		case NodeType256: {
			Node256* parent = static_cast<Node256*>(node);
			parent->children[attribute] = child;
			parent->count++;
			break;
		}
		default:
			break;
	}
	return child;
}

// grow the given node to the next highest type
void ART::grow(Node *&parent, Node *&node, const int &indexOfCurrent){
	Node* newNode;
	switch(node->type){
		case NodeType4:
			newNode = new Node16();
			newNode->count = 4;
			for(std::size_t i = 0; i < 4; i++){
				static_cast<Node16*>(newNode)->key[i] = static_cast<Node4*>(node)->key[i];
			}
			for(std::size_t i = 0; i < 4; i++){
				static_cast<Node16*>(newNode)->children[i] = static_cast<Node4*>(node)->children[i];
			}
			break;
		case NodeType16:
			newNode = new Node48();
			newNode->count = 16;
			for(std::size_t i = 0; i < 16; i++){
				static_cast<Node48*>(newNode)->children[i] = static_cast<Node16*>(node)->children[i];
			}
			for (unsigned i = 0; i < node->count; i++){
				static_cast<Node48*>(newNode)->childIndex[static_cast<Node16*>(node)->key[i]] = i;
			}
			break;
		case NodeType48:
			newNode = new Node256();
			newNode->count = 48;
			for (unsigned i = 0; i < 256; i++){
				if (static_cast<Node48*>(node)->childIndex[i] != 48){ // slot not empty
					static_cast<Node256*>(newNode)->children[i] = static_cast<Node48*>(node)->children[static_cast<Node48*>(node)->childIndex[i]];
				}
			}
			break;
		default:
			break;
	}
	newNode->minScore = node->minScore;
	newNode->maxScore = node->maxScore;
	for(std::size_t i = 0; i < node->tupleIDs.size(); i++){
		newNode->tupleIDs[i] = node->tupleIDs[i];
	}

	if(node != root){
		updateParent(parent, newNode, indexOfCurrent);
	}
	delete node;
	node = newNode;
}

void ART::updateParent(Node *&parent, Node* newChild, const int &indexOfChild){
	switch(parent->type){
	case NodeType4:{
		Node4* parent4 = static_cast<Node4*>(parent);
		unsigned pos;
		for (pos = 0; parent4->key[pos] < indexOfChild; pos++);
		parent4->children[pos] = newChild;
		break;
	}
	case NodeType16:{
		Node16* parent16 = static_cast<Node16*>(parent);
		unsigned pos;
		for (pos = 0; parent16->key[pos] < indexOfChild; pos++);
		parent16->children[pos] = newChild;
		break;
	}
	case NodeType48:{
		Node48* parent48 = static_cast<Node48*>(parent);
		parent48->children[parent48->childIndex[indexOfChild]] = newChild;
		break;
	}
	case NodeType256:{
		Node256* parent256 = static_cast<Node256*>(parent);
		parent256->children[indexOfChild] = newChild;
		break;
	}
	default:
		break;
	}
}

void ART::insert(const std::vector<int> &tuple, Node *&parent, Node *&current, unsigned int level){
	if(level == 0){
		current->minScore = 0;
		current->maxScore = 0;
		for(std::size_t i = 0; i < tuple.size(); i++){
			current->maxScore += (int) (pow(2.0, (double) (tuple.size()-i)) * attributes[attributes.size()-1]);
		}
	}
	else{
		current->minScore = 0;
		for(std::size_t i = 0; i < level; i++){
			current->minScore += (int) (pow(2.0, (double) (tuple.size()-i)) * tuple[i]);
		}
		current->maxScore = current->minScore;
		for(std::size_t i = level; i < tuple.size(); i++){
			current->maxScore += (int) (pow(2.0, (double) (tuple.size()-i)) * attributes[attributes.size()-1]);
		}
	}
	if(level == tuple.size()){
		current->tupleIDs.push_back(tupleID++);
	}
	else{
		Node* child = findChild(current, tuple[level]);
		// if child null, grow node if needed and create new child
		if(!child){
			switch(current->type){
				case NodeType4:
					if(current->count == 4) grow(parent, current, tuple[level-1]);
					break;
				case NodeType16:
					if(current->count == 16) grow(parent, current, tuple[level-1]);
					break;
				case NodeType48:
					if(current->count == 48) grow(parent, current, tuple[level-1]);
					break;
				case NodeType256:
				default:
					break;
			}
			child = newChild(current, tuple[level]);
		}
		insert(tuple, current, child, level+1);
	}
}

// note: we are looking for minimal tuples
bool ART::is_dominated(const std::vector<int> &tuple, Node* p, unsigned int level, unsigned int currentScore){
	if(p==NULL || (currentScore < p->minScore)){
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
		Node* child = findChild(p, i);
		if(child){ // if child not null
			if(is_dominated(tuple, child, level+1, currentScore + weight)){
				return true;
			}
		}
	}
	Node* child = findChild(p, tuple[level]);
	if(child){
		if(is_dominated(tuple, child, level+1, currentScore)){
			return true;
		}
	}
	return false;
}

// returns the index to the searched value, -1 if it's not in the array
int ART::binarySearch(uint8_t arr[], unsigned l, unsigned r, const int& value){
	if(r > l){
		int mid = l + (r - l)/2;
		if (arr[mid] == value){
			return mid;
		}
		if (arr[mid] > value){
			return binarySearch(arr, l, mid-1, value);
		}
		else{
			return binarySearch(arr, mid+1, r, value);
		}
	}
	return -1;
}

std::vector<int> ART::get_attributes(){
	return attributes;
}

// Prints the tree by Breadth-First-Search
void ART::print(){
	int counter = 0;
	std::queue<Node *> queue;
	queue.push(root);
	while(!queue.empty()){
		// take an element from the queue and print it
		Node *n = queue.front();
		queue.pop();
		std::cout << "Node " << counter++ << ": ";
		std::cout << "type " << (int) (n->type);
		std::cout << ", minScore " << n->minScore;
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
		switch(n->type){
			case NodeType4:
				if(sizeof(static_cast<Node4*>(n)->children) <= 0) break;
				for(std::size_t i = 0; i < n->count; i++){
					if(static_cast<Node4*>(n)->children[i]){
						queue.push(static_cast<Node4*>(n)->children[i]);
					}
				}
				break;
			case NodeType16: {
				if(sizeof(static_cast<Node16*>(n)->children) <= 0) break;
				for(std::size_t i = 0; i < n->count; i++){
					if(static_cast<Node16*>(n)->children[i]){
						queue.push(static_cast<Node16*>(n)->children[i]);
					}
				}
				break;
			}
			case NodeType48: {
				Node48* temp = static_cast<Node48*>(n);
				if(sizeof(temp->children) <= 0) break;
				for(std::size_t i = 0; i < 48; i++){
					if(temp->children[i]){
						queue.push(temp->children[i]);
					}
				}
				break;
			}
			case NodeType256: {
				Node256* temp = static_cast<Node256*>(n);
				if(sizeof(temp->children) <= 0) break;
				for(std::size_t i = 0; i < 256; i++){
					if(temp->children[i]){
						queue.push(temp->children[i]);
					}
				}
				break;
			}
		}
	}
	std::cout << "\n";
}

// frees the memory previously used by the art tree
void ART::freeMemory(){
//	unsigned deleted_counter = 0; // for debugging only
	std::queue<Node *> queue;
	queue.push(root);
	while(!queue.empty()){
		// take an element from the queue, puts its children into the queue, and then delete the element
		Node *n = queue.front();
		queue.pop();
		switch(n->type){
			case NodeType4:
				if(sizeof(static_cast<Node4*>(n)->children) <= 0) break;
				for(std::size_t i = 0; i < n->count; i++){
					if(static_cast<Node4*>(n)->children[i]){
						queue.push(static_cast<Node4*>(n)->children[i]);
					}
				}
				break;
			case NodeType16: {
				if(sizeof(static_cast<Node16*>(n)->children) <= 0) break;
				for(std::size_t i = 0; i < n->count; i++){
					if(static_cast<Node16*>(n)->children[i]){
						queue.push(static_cast<Node16*>(n)->children[i]);
					}
				}
				break;
			}
			case NodeType48: {
				Node48* temp = static_cast<Node48*>(n);
				if(sizeof(temp->children) <= 0) break;
				for(std::size_t i = 0; i < 48; i++){
					if(temp->children[i]){
						queue.push(temp->children[i]);
					}
				}
				break;
			}
			case NodeType256: {
				Node256* temp = static_cast<Node256*>(n);
				if(sizeof(temp->children) <= 0) break;
				for(std::size_t i = 0; i < 256; i++){
					if(temp->children[i]){
						queue.push(temp->children[i]);
					}
				}
				break;
			}
		}
		delete n;
//		deleted_counter++; // for debugging only
	}
//	std::cout << "\nTotal art nodes created: " << created_counter; // for debugging only
//	std::cout << "\nTotal art nodes deleted: " << deleted_counter << "\n\n"; // for debugging only
}











