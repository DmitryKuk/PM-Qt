// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "passwordtree.h"

PasswordTree::Node::~Node()
{
	if (this->parent_ != nullptr)
		this->parent_->child_deleted(this->id());
}


void PasswordTree::Node::setParent(Node *parent)
{
	if (this->parent() == parent) return;
	
	auto it = this->find_in_parent();
	if (it == this->parent()->end()) return;
	
	std::swap(this->parent_, parent);	// parent is old parent now
	this->parent()->emplace_back(std::move(*it));	// Move object from old parent emplacing
	parent->erase(it);								// to the new parent.
}


std::list<std::unique_ptr<Node>>::iterator PasswordTree::Node::findInParent() const
{
	auto it = this->parent()->begin(), end = this->parent()->end();
	while (it != end)
		if (it->id->get() == this)	// Compare two pointers
			return it;
	if (it == end) return end;
}
