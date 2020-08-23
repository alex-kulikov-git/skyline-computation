/**
 *art_nodes.hpp
 */

#include <stdint.h>    // integer types
#include <string.h>    // memset, memcpy
#include <algorithm>

// Constants for the node types
static const int8_t NodeType4=0;
static const int8_t NodeType16=1;
static const int8_t NodeType48=2;
static const int8_t NodeType256=3;

// Shared header of all inner nodes
struct Node {
   uint16_t count; // number of child entries/pointers in a node
   int8_t type;
   unsigned int minScore;
   unsigned int maxScore;
   std::vector<int> tupleIDs;

   Node(int8_t type) : count(0),type(type),minScore(0),maxScore(0) {}
};

// Node with up to 4 children
struct Node4 : Node {
   uint8_t key[4]; // could be changed to uint16_t, but not really sensible
   Node* children[4];

   Node4() : Node(NodeType4) {}
};

// Node with up to 16 children
struct Node16 : Node {
   uint8_t key[16]; // could be changed to uint16_t, but not really sensible
   Node* children[16];

   Node16() : Node(NodeType16) {}
};

const uint8_t emptyMarker=48;

// Node with up to 48 children
struct Node48 : Node {
   uint8_t childIndex[256];
   Node* children[48];

   Node48() : Node(NodeType48) {
      memset(childIndex,emptyMarker,sizeof(childIndex));
      memset(children,0,sizeof(children));
   }
};

// Node with up to 256 children
struct Node256 : Node {
   Node* children[256];

   Node256() : Node(NodeType256) {
      memset(children,0,sizeof(children));
   }
};
