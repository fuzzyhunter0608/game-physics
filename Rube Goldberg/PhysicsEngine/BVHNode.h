#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "RigidBody.h"
#include "PotentialContact.h"

struct PotentialContact;

template<class T>
class BVHNode
{
public:
	BVHNode(BVHNode *parent, const T &volume, RigidBody *body = nullptr);
	~BVHNode();

	bool IsLeaf() const { return body != nullptr; }

	unsigned GetPotentialContacts(PotentialContact *contacts, unsigned limit) const;
	void Insert(RigidBody *newBody, const T &newVolume);

	BVHNode *children[2];
	T volume;
	RigidBody *body;
	BVHNode *parent;

protected:
	bool Overlaps(const BVHNode<T> *other) const;
	unsigned GetPotentialContactsWith(const BVHNode<T> *other, PotentialContact *contacts, unsigned limit) const;

	void RecalculateBoundingVolume(bool recurse = true);
};

// Because class is templated, implementations need to be accessible to anything that includes header

template <class T>
BVHNode<T>::BVHNode(BVHNode* parent, const T& volume, RigidBody* body)
	:volume(volume)
	, body(body)
	, parent(parent)
{
	children[0] = children[1] = NULL;
}

template <class T>
BVHNode<T>::~BVHNode()
{
	// If we don't have a parent, we ignore the sibling processing
	if (parent)
	{
		// Find our sibling
		BVHNode<T> *sibling;
		if (parent->children[0] == this)
			sibling = parent->children[1];
		else
			sibling = parent->children[0];

		// Write its data to our parent
		parent->volume = sibling->volume;
		parent->body = sibling->body;
		parent->children[0] = sibling->children[0];
		parent->children[1] = sibling->children[1];

		// Delete the sibling (blank its parent and children to avoid processing/deleting them
		sibling->parent = NULL;
		sibling->body = NULL;
		sibling->children[0] = NULL;
		sibling->children[1] = NULL;
		delete sibling;

		// Recalculate the parent's bounding volume
		parent->RecalculateBoundingVolume();
	}

	// Delete our children
	if (children[0])
	{
		children[0]->parent = NULL;
		delete children[0];
	}
	if (children[1])
	{
		children[1]->parent = NULL;
		delete children[1];
	}
}

template <class T>
unsigned BVHNode<T>::GetPotentialContacts(PotentialContact* contacts, unsigned limit) const
{
	// Early out if we don't have the room for contacts, or if we're a leaf node
	if (IsLeaf() || limit == 0) return 0;

	// Get the potential contacts of our children with the other
	return children[0]->GetPotentialContactsWith(children[1], contacts, limit);
}

template <class T>
void BVHNode<T>::Insert(RigidBody* newBody, const T& newVolume)
{
	// If we are a leaf, only option is to spawn two new children,
	// and place the new body in one
	if (IsLeaf())
	{
		// Child one is a copy of us
		children[0] = new BVHNode<T>(this, volume, body);

		// Child two holds the new body
		children[1] = new BVHNode<T>(this, newVolume, newBody);

		// Lose the body (we aren't a leaf anymore)
		this->body = NULL;

		// Recalculate bounding volume
		RecalculateBoundingVolume();
	}

	// Otherwise we need to work out which child gets to keep the inserted body
	// Given to whoever would grow the least to incorporate it
	else
	{
		if (children[0]->volume.GetGrowth(newVolume) < children[0]->volume.GetGrowth(newVolume))
		{
			children[0]->Insert(newBody, newVolume);
		}
		else
		{
			children[1]->Insert(newBody, newVolume);
		}
	}
}

template <class T>
bool BVHNode<T>::Overlaps(const BVHNode<T>* other) const
{
	return volume->Overlaps(other->volume);
}

template <class T>
unsigned BVHNode<T>::GetPotentialContactsWith(const BVHNode<T>* other, PotentialContact* contacts, unsigned limit) const
{
	// Early out if we don't overlap or we have no room to report contacts
	if (!Overlaps(other) || limit == 0) return 0;

	// If we're both at leaf nodes, then we have a potential contact
	if(IsLeaf() && other->IsLeaf())
	{
		contacts->body[0] = body;
		contacts->body[1] = other->body;
		return 1;
	}

	// Determine which node to descend into.
	// If either is a leaf, then we descend into the other.
	// If both are branches, use the one with the largest size
	if (other->IsLeaf() || (!IsLeaf() && volume->GetSize() >= other->volume->GetSize()))
	{
		// Recurse into ourself
		unsigned count = children[0]->GetPotentialContactsWith(other, contacts, limit);

		// Check we have enough slots to do the other side too
		if (limit > count)
		{
			return count + children[1]->GetPotentialContactsWith(other, contacts + count, limit - count);
		}
		else
		{
			return count;
		}
	}
	else
	{
		// Recurse into the other node
		unsigned count = GetPotentialContactsWith(other->children[0], contacts, limit);

		// Check we have enough slots to do the other side too
		if (limit > count)
		{
			return count + GetPotentialContactsWith(other->children[1], contacts + count, limit - count);
		}
		else
		{
			return count;
		}
	}
}

template <class T>
void BVHNode<T>::RecalculateBoundingVolume(bool recurse)
{
	if (IsLeaf()) return;

	// Use the bounding volume combining constructor
	volume = T(children[0]->volume, children[1]->volume);

	// Recurse up the tree
	if (parent)
		parent->RecalculateBoundingVolume(true);
}
#endif