// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDTREE_H
#define PASSWORDTREE_H

#include <list>
#include <memory>

#include "types.h"


class PasswordTree
{
	class Node;
public:
	
};


class PasswordTree::Node:
	public Password,
	public std::list<std::unique_ptr<Node>>
{
public:
	Node(const Password &p, Node *parent = nullptr);
	~Node();
	
	inline Node * parent() const;
	void setParent(Node *parent);
protected:
	std::list<std::unique_ptr<Node>>::iterator findInParent() const;
private:
	Node *parent_;
};


#include "passwordtree.hpp"

#endif // PASSWORDTREE_H
