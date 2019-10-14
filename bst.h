#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */


template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed within the deleteAll() helper method in the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree(); //TODO
    virtual ~BinarySearchTree(); //TODO
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO
    virtual void remove(const Key& key); //TODO
    void clear(); //TODO
    bool isBalanced() const; //TODO
    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const; // TODO
    Node<Key, Value> *getSmallestNode() const;  // TODO
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); // TODO
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here
    int height(Node<Key,Value>* current) const;
    void clearHelp(Node<Key,Value> *node);


protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() :  current_(NULL)
{
}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return current_ == rhs.current_;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
	return current_!= rhs.current_;
}


/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
	Node<Key,Value> *next = current_;
	if(next->getRight() != NULL)
	{
		next = next->getRight();
		//go as far down leftside as possible
		while(next->getLeft() != NULL)
		{
			next = next->getLeft();
		}
		current_ = next;
		return *this;
	}
	else
	{
		Node<Key, Value> *parent = next->getParent();
		while(parent != NULL && next == parent->getRight())
		{
			next = parent;
			parent = parent->getParent();
		}
		current_ = parent;
		return *this;
	}
}



/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() : root_(NULL) 
{

}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    clear();

}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
	Node<Key, Value> *curr;
	if(empty())
	{ 
		curr = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
		root_ = curr;
	}
	else
	{
		curr = root_;
		bool done = false;
		Node<Key,Value> *find = internalFind(keyValuePair.first);
		if(find != NULL)
		{
			curr = find;
	    	curr->setValue(keyValuePair.second);
		}
		else
		{
			while(!done)
	    	{
		    	if(keyValuePair.first > curr->getKey())
		    	{
		    		Node<Key,Value> *temp = curr;
		    		curr = curr->getRight();
		    		if(curr == NULL)
		    		{
		    			curr = temp;
		    			done = true;
		    		}
		    	}
		    	else if(keyValuePair.first < curr->getKey())
		    	{
		    		Node<Key,Value> *temp = curr;
		    		curr = curr->getLeft();
		    		if(curr == NULL)
		    		{
		    			curr = temp;
		    			done = true;
		    		}
		    	}
	   		}
		    Node<Key, Value> *leaf = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, curr);

		    if(keyValuePair.first > curr->getKey())
			{
				curr->setRight(leaf); 
			}
			else if(keyValuePair.first < curr->getKey())
			{
				curr->setLeft(leaf);
			}
		}
	}
}


/**
* A remove method to remove a specific key from a Binary Search Tree.
* The tree may not remain balanced after removal.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    if(!empty() && internalFind(key)!= NULL)
    {
    	Node<Key, Value> *curr = root_;
        //if the target is the root
    	if(curr->getKey() == key)
    	{
            //if there are no subtrees, just delete root 
    		if(curr->getLeft() == NULL && curr->getRight() == NULL)
    		{
    			delete curr;
                root_ = NULL;
    		}
            //if there's a right but no left subtree, promote right child as new root
    		else if(curr->getLeft() == NULL && curr->getRight() != NULL)
    		{
    			Node<Key, Value> *temp = curr->getRight();
    			delete curr;
    			root_ = temp;
    			temp->setParent(NULL);
    		}
            //if there's a left but no right subtree, promote left child as new root
    		else if(curr->getLeft() != NULL && curr->getRight() == NULL)
    		{
    			Node<Key, Value> *temp = curr->getLeft();
    			delete curr;
    			root_ = temp;
    			temp->setParent(NULL);
    		}
            //if there's a left and right child
    		else
    		{
                Node<Key, Value> *pred = predecessor(curr);
    			nodeSwap(curr, pred);
                //if node swapped was a right child
                if(curr == curr->getParent()->getRight())
                {
                    //promote left child of node that was swapped
                    if(curr->getLeft()!= NULL)
                    {
                        curr->getLeft()->setParent(curr->getParent());
                        curr->getParent()->setRight(curr->getLeft());
                    }
                    //promote right child of node that was swapped
                    else if(curr->getRight()!=NULL)
                    {
                        curr->getRight()->setParent(curr->getParent());
                        curr->getParent()->setRight(curr->getRight());
                    }
                    //otherwise node was a leaf node, so just delete
                    else
                    {
                        curr->getParent()->setRight(NULL);
                    }
                }
                //if node swapped was a left child
                else if(curr == curr->getParent()->getLeft())
                {
                    if(curr->getLeft() != NULL)
                    {
                        curr->getParent()->setLeft(curr->getLeft());
                        curr->getLeft()->setParent(curr->getParent());
                    }
                    else if(curr->getRight() != NULL)
                    {
                        curr->getParent()->setLeft(curr->getRight());
                        curr->getRight()->setParent(curr->getParent());
                    }
                    else
                    {
                        curr->getParent()->setLeft(NULL);
                    }

                }
    			delete curr;
    		}
    	}
        //target is not the root
        else
        {
        	bool found = false;
            //bool to check if target is a right or left child
        	bool right = false;

            while(curr != NULL && !found)
            {
            	if(curr->getKey() < key)
            	{
            		curr = curr->getRight();
            		right = true;
            	}
            	else if(curr->getKey() > key)
            	{
            		curr = curr->getLeft();
            		right = false;
            	}
                //found the target
            	else if (curr->getKey() == key)
            	{
                    //if left but no right subtree exists
            		if(curr->getRight() == NULL && curr->getLeft() != NULL)
            		{
                        //if target is a right child
            			if(right)
            			{
                            //promote current left child as parent's right child
            				curr->getParent()->setRight(curr->getLeft());
            				curr->getLeft()->setParent(curr->getParent());
            				delete curr;
            			}
                        //if target is a left child
            			else if (!right)
            			{
            				curr->getParent()->setLeft(curr->getLeft());
            				curr->getLeft()->setParent(curr->getParent());
            				delete curr;
            			}
            		}
                    //if right but no left subtre exists
            		else if(curr->getRight() != NULL && curr->getLeft() == NULL)
            		{
            			if(right)
            			{
            				curr->getParent()->setRight(curr->getRight());
            				curr->getRight()->setParent(curr->getParent());
            				delete curr;
            			}
            			else if (!right)
            			{
            				curr->getParent()->setLeft(curr->getRight());
            				curr->getRight()->setParent(curr->getParent());
            				delete curr;
            			}
            		}
                    //if both left and right subtree exists
            		else if (curr->getRight() != NULL && curr->getLeft() != NULL)
            		{
            			nodeSwap(curr,predecessor(curr));
                        //if target is the right child
                        if(curr == curr->getParent()->getRight())
                        {
                            if(curr->getRight()!=NULL)
                            {
                                curr->getRight()->setParent(curr->getParent());
                                curr->getParent()->setRight(curr->getRight());
                            }
                            else if(curr->getLeft()!= NULL)
                            {
                                curr->getLeft()->setParent(curr->getParent());
                                curr->getParent()->setRight(curr->getLeft());
                            }
                            else
                            {
                                curr->getParent()->setRight(NULL);
                            }
                        }
                        //if target is the left child
                        else if(curr == curr->getParent()->getLeft())
                        {
                            if(curr->getLeft() != NULL)
                            {
                                curr->getLeft()->setParent(curr->getParent());
                                curr->getParent()->setLeft(curr->getLeft());

                            }
                            else if(curr->getRight()!= NULL)
                            {
                                curr->getRight()->setParent(curr->getParent());
                                curr->getParent()->setLeft(curr->getRight());
                            }
                            else
                            {
                                curr->getParent()->setLeft(NULL);
                            }
                        }
            			delete curr;
            		}
                    //target's a leaf node
            		else
            		{
                        //if target is a right child
            			if(right)
            			{
            				curr->getParent()->setRight(NULL);
            				delete curr;
            			}
            			else
            			{
            				curr->getParent()->setLeft(NULL);
            				delete curr;
            			}
            		}
            		found = true;
            	}
            }
        }
    }
}



template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
	if(current == NULL)
	{
		return NULL;
	}
	else if(current->getLeft() != NULL)
    {
    	current = current->getLeft();
    	while(current->getRight() != NULL)
		{
			current = current->getRight();
		}
		return current;
    }
    else
    {
    	Node<Key, Value> *parent = current->getParent();
    	while(parent != NULL)
    	{
    		if(current == parent->getRight())
    		{
    			return parent;
    		}
    		current = parent;
    		parent = parent->getParent();
    	}
    	return NULL;
    }
}


/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    clearHelp(root_);
    root_ = NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearHelp(Node<Key,Value> *node)
{
	if(node == NULL)
	{
		return;
	}
	clearHelp(node->getLeft());
	clearHelp(node->getRight());
	delete node;
}

/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
	Node<Key, Value> *curr = root_;
	while(curr->getLeft() != NULL)
	{
		curr = curr->getLeft();
	}
	return curr;
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    Node<Key, Value> *find;
    find = root_;
    if(find == NULL)
    {
    	return NULL;
    }
    else
    {
    	while(find != NULL)
    	{
    		if(key < find->getKey())
    		{
    			find = find->getLeft();
    		}
    		else if (key > find->getKey())
    		{
    			find = find->getRight();
    		}
    		else if (key == find->getKey())
    		{
    			return find;
    		}
    	}
    }
    return NULL;
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
	if (root_ == NULL)
	{
		return true;
	}
	else if(height(root_) > -1)
	{
		return true;
	}
	return false;
}

template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::height(Node<Key,Value> *current) const
{
	if(current == NULL)
	{
		return 0;
	}
	int leftheight = height(current->getLeft());
	int rightheight = height(current->getRight());
	if(leftheight < 0 || rightheight < 0)
	{
		return -1;
	}
	if(abs(leftheight - rightheight) > 1)
	{
		return -1;
	}
	else
	{
		if(leftheight > rightheight)
		{
			return leftheight +1;
		}
		else
		{
			return rightheight + 1;
		}
	}
}


template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif
