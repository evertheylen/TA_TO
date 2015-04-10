/*
 * suffix.h
 *
 *  Created on: 6 Apr 2015
 *      Author: stijn
 */

#include <string>
#include <iostream>
#include <list>

#ifndef SUFFIX_SUFFIX_H_
#define SUFFIX_SUFFIX_H_
class Node {
public:

	Node(std::string tag);
	/*
	 * Creates a node of a suffixtree with tag as the suffix. Tag should consist of the suffixstring + '$' + starting index in
	 * the original string.
	 */

	~Node();
	/*
	 * Destructor for a node, frees all heap blocks.
	 */

	Node* get_firstchild();
	/*
	 * Returns a pointer to the firstchild of this node. If this returns a nullptr, we know that the node has no children.
	 */

	void add_child(Node* child);
	/*
	 * Adds a child to the node. If it is the first child to be added then it will automatically become the node's firstchild.
	 */

	std::string get_tag();
	/*
	 * Returns the tag of this node, including $ and indexnumber.
	 */

	void set_tag(std::string tag);
	/*
	 * Allows you to change the tag of this node.
	 */

	std::list <Node*> children;
	/*
	 * A public list with Node* to all of the node's children.
	 */


	friend std::ostream& operator<<(std::ostream& stream, Node& node);
	/*
	 * Output operator overloading, optimised for output in dot-language (.gv-file).
	 */

private:
	Node* _firstchild;
	std::string _tag;
};

class SuffixTree {
public:
	SuffixTree(std::string text);
	/*
	 * Creates a suffixtree on the base of 'text', the result will be a tree with all the possible suffixes and their starting places.
	 */

	~SuffixTree();
	/*
	 * Destroys the suffixtree and makes sure all heap blocks are freed.
	 */

	void add_node(std::string tag, Node& current_node);
	/*
	 * Adds a new_node to the suffixtree. Normally all nodes are added in the constructor so you shouldn't use this function.
	 * Searches for the correct location and splits or creates the necessary branches.
	 */

	Node* get_root();
	/*
	 * returns a pointer to the root of the suffix tree.
	 */

	void fix_leaves(Node* current_node = nullptr);
	/*
	 * This function is called at the end of the constructor.
	 * It traverses all branches and at the current leaf of a branch he splits the tag of the node.
	 * Then creates a new leaf so that the original tag is the string, and the new leaf the index. The $ is deleted.
	 */

	friend std::ostream& operator<<(std::ostream& stream, SuffixTree& tree);
	/*
	 * Output operator overloading, optimised for dot-language (.gv-file).
	 */

private:
	Node* _root;
};

#endif /* SUFFIX_SUFFIX_H_ */
