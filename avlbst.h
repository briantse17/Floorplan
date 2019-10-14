#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;
 
protected:
    char balance_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/** 
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item);
    virtual void remove(const Key& key);
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    void rotateLeft(AVLNode<Key,Value> *n1);
    void rotateRight (AVLNode<Key,Value> *n1);
    void insertFix (AVLNode<Key,Value> *current, AVLNode<Key,Value> *parent);
    void removeFix (AVLNode<Key,Value> *parent, int diff);
};

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key,Value> *curr = static_cast<AVLNode<Key,Value>*>(this->root_);
    //check if this is first node in tree
    if(this->empty())
    {
        curr = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        this->root_ = curr;
        return;
    }
    else
    {
        bool done = false;
        while(!done)
        {
            if(new_item.first > curr->getKey())
            {
                AVLNode<Key,Value> *temp = curr;
                curr = curr->getRight();
                //check if we've reachd leaf node
                if(curr == NULL)
                {
                    curr = temp;
                    done = true;
                }
            }
            else if(new_item.first < curr->getKey())
            {
                AVLNode<Key,Value> *temp = curr;
                curr = curr->getLeft();
                //check if we've reached a leaf node
                if(curr == NULL)
                {
                    curr = temp;
                    done = true;
                }
            }
            //if key already exists, update value
            else if(curr->getKey() == new_item.first)
            {
                curr->setValue(new_item.second);
                done = true;
                return;
            }
        }
        //node doesn't currently exist
        //create a new AVLNode with curr as parent;
        AVLNode<Key, Value> *leaf = new AVLNode<Key, Value>(new_item.first, new_item.second, curr);
        if(new_item.first > curr->getKey())
        {
            curr->setRight(leaf);
            leaf->setBalance(0);
            if(curr->getBalance() == 0)
            {
                curr->updateBalance(1); 
                insertFix(curr, leaf);
            }
            //if new leaf node balances out the parent
            else if(curr->getBalance() == -1)
            {
                curr->setBalance(0);
            }
        }
        else if(new_item.first < curr->getKey())
        {
            curr->setLeft(leaf);
            leaf->setBalance(0);
            if(curr->getBalance() == 0)
            {
                curr->updateBalance(-1);
                insertFix(curr, leaf);
            }
            else if(curr->getBalance() == 1)
            {
                curr->setBalance(0);
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    int diff = 0;
    AVLNode<Key,Value> *curr = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if(!this->empty() && curr!=NULL)
    {
        AVLNode<Key,Value> *parent = curr->getParent();
        if(parent!=NULL)
        {
            if(curr == parent->getLeft())
            {
                diff = 1;
            }
            else if(curr == parent->getRight())
            {
                diff = -1;
            }
        }
        if(curr == this->root_)
        {
            //if there are no subtrees, just delete root 
            if(curr->getLeft() == NULL && curr->getRight() == NULL)
            {
                delete curr;
                this->root_ = NULL;
            }
            //if there's a right but no left subtree, promote right child as new root
            else if(curr->getLeft() == NULL && curr->getRight() != NULL)
            {
                AVLNode<Key, Value> *temp = curr->getRight();
                delete curr;
                this->root_ = temp;
                temp->setParent(NULL);
            }
            //if there's a left but no right subtree, promote left child as new root
            else if(curr->getLeft() != NULL && curr->getRight() == NULL)
            {
                AVLNode<Key, Value> *temp = curr->getLeft();
                delete curr;
                this->root_ = temp;
                temp->setParent(NULL);
            }
            //if there's a left and right child
            else
            {
                AVLNode<Key, Value> *pred = static_cast<AVLNode<Key,Value>*>(this->predecessor(curr));
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
                    diff = -1;
                    
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
                    diff = 1; 
                }
            removeFix(curr->getParent(), diff);
            delete curr;
            }
        }
        //target is not the root
        else
        {
            bool right = false;
            if(curr == parent->getRight())
            {
                right = true;
            }
            else
            {
                right = false;
            }
            //if left but no right subtree exists
            if(curr->getRight() == NULL && curr->getLeft() != NULL)
            {
                //if target is a right child
                if(right)
                {
                    //promote current left child as parent's right child
                    curr->getParent()->setRight(curr->getLeft());
                    curr->getLeft()->setParent(curr->getParent());
                }
                //if target is a left child
                else if (!right)
                {
                    curr->getParent()->setLeft(curr->getLeft());
                    curr->getLeft()->setParent(curr->getParent());
                }
                removeFix(curr->getParent(),diff);
                delete curr;   

            }
            //if right but no left subtre exists
            else if(curr->getRight() != NULL && curr->getLeft() == NULL)
            {
                if(right)
                {
                    curr->getParent()->setRight(curr->getRight());
                    curr->getRight()->setParent(curr->getParent());
                }
                else if (!right)
                {
                    curr->getParent()->setLeft(curr->getRight());
                    curr->getRight()->setParent(curr->getParent());
                }
                removeFix(curr->getParent(), diff);
                delete curr;

            }
            //if both left and right subtree exists
            else if (curr->getRight() != NULL && curr->getLeft() != NULL)
            {
                nodeSwap(curr,static_cast<AVLNode<Key,Value>*>(this->predecessor(curr)));
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
                diff = -1;
                removeFix(curr->getParent(), diff);
                delete curr;
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
                    diff = 1;
                    removeFix(curr->getParent(), diff);
                    delete curr;
                }
            }
            //target's a leaf node
            else
            {
                //if target is a right child
                if(right)
                {
                    curr->getParent()->setRight(NULL);
                    diff = -1;
                    removeFix(curr->getParent(), diff);
                    delete curr;
                }
                else
                {
                    curr->getParent()->setLeft(NULL);
                    diff = 1;
                    removeFix(curr->getParent(), diff);
                    delete curr;
                }
            }
        }
    }
}




template<class Key, class Value>
void AVLTree<Key, Value>::removeFix (AVLNode<Key,Value> *parent, int diff)
{
    //check if we've recursed past the root of the tree
    if(parent == NULL)
    {
        return;
    }
    int ndiff = 0;
    AVLNode<Key,Value> *grand = parent->getParent();
    if(grand != NULL)
    {
        if(parent == grand->getLeft())
        {
            ndiff = 1;
        }
        else
        {
            ndiff = -1;
        }
    }
    //left subtree longer than right subtree
    if(parent->getBalance() + diff == -2)
    {
        AVLNode<Key,Value> *tallc = parent->getLeft();
        int c_bal = int(tallc->getBalance());
        if(c_bal == -1 || c_bal == 0)
        {
            rotateRight(parent);
            if(c_bal == -1)
            {
                parent->setBalance(0);
                tallc->setBalance(0);
                removeFix(grand, ndiff);
            }
            //taller child's balance is 0
            else if (c_bal == 0)
            {
                parent->setBalance(-1);
                tallc->setBalance(1);
            }
        }
        //zig zag left
        else if(c_bal == 1)
        {
            AVLNode<Key,Value> *c_child = tallc->getRight();
            int g_bal = int(c_child->getBalance());
            rotateLeft(tallc);
            rotateRight(parent);
            if(g_bal == 1)
            {
                parent->setBalance(0);
                tallc->setBalance(-1);
                c_child->setBalance(0);
            }
            else if (g_bal == 0)
            {
                parent->setBalance(0);
                tallc->setBalance(0);
                c_child->setBalance(0);
            }
            else if (g_bal == -1)
            {
                parent->setBalance(1);
                tallc->setBalance(0);
                c_child->setBalance(0);
            }
        removeFix(grand, ndiff);
        }
    }
    else if(parent->getBalance() + diff == -1)
    {
        parent->setBalance(-1);
    }
    else if (parent->getBalance() + diff == 0)
    {
        parent->setBalance(0);
        removeFix(grand, ndiff);
    }
    else if(parent->getBalance() + diff == 1)
    {
        parent->setBalance(1);
    }
    //right subtree longer than left subtree
    else if(parent->getBalance() + diff == 2)
    {
        AVLNode<Key,Value> *tallc = parent->getRight();
        int c_bal = int(tallc->getBalance());
        //zig zig
        if(c_bal == 1 || c_bal == 0)
        {
            rotateLeft(parent);
            if(c_bal == 1)
            {
                parent->setBalance(0);
                tallc->setBalance(0);
                removeFix(grand, ndiff);
            }
            //taller child's balance is 0
            else if(c_bal == 0)
            {
                parent->setBalance(1);
                tallc->setBalance(-1);
            }
        }
        //zig zag
        else if(c_bal == -1)
        {
            AVLNode<Key,Value> *c_child = tallc->getLeft();
            int g_bal = int(c_child->getBalance());
            rotateRight(tallc);
            rotateLeft(parent);
            if(g_bal == 1)
            {
                parent->setBalance(-1);
                tallc->setBalance(0);
                c_child->setBalance(0);
            }
            else if (g_bal == 0)
            {
                parent->setBalance(0);
                tallc->setBalance(0);
                c_child->setBalance(0);
            }
            else if (g_bal == -1)
            {
                parent->setBalance(0);
                tallc->setBalance(1);
                c_child->setBalance(0);
            }
        removeFix(grand, ndiff);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix (AVLNode<Key,Value> *parent, AVLNode<Key,Value> *current)
{
    if(parent == NULL || parent->getParent() == NULL)
    {
        return;
    } 
    AVLNode<Key,Value> *grand = parent->getParent();
    if(parent == grand->getRight())
    {
        grand->updateBalance(1);
    }
    else if(parent == grand->getLeft())
    {
        grand->updateBalance(-1);
    }
    //if grandparent's balance is even after addition of node, return
    if(grand->getBalance() == 0)
    {
        return;
    }
    else if(abs(grand->getBalance()) == 1)
    {
        //check next ancestor node
        insertFix(grand, parent);
    }
    //if grandparent's balance is out of range [-1,1]
    else if (grand->getBalance() == -2)
    {
        //zig-zig left
        if(current == parent->getLeft())
        {
            rotateRight(grand);
            parent->setBalance(0);
            grand->setBalance(0);

        }
        //zig-zag left
        else if (current == parent->getRight())
        {
            rotateLeft(parent);
            rotateRight(grand);
            if(current->getBalance() == -1)
            {
                parent->setBalance(0);
                grand->setBalance(1);
                current->setBalance(0);
            }
            else if(current->getBalance() == 0)
            {
                parent->setBalance(0);
                grand->setBalance(0);
                current->setBalance(0);
            }
            else if(current->getBalance() == 1)
            {
                parent->setBalance(-1);
                grand->setBalance(0);
                current->setBalance(0);
            }
        }
    return;
    }
    else if (grand->getBalance() == 2)
    {
        //zig-zig right
        if(current == parent->getRight())
        {
            rotateLeft(grand);
            parent->setBalance(0);
            grand->setBalance(0);
        }
        //zig-zag right
        else if (current == parent->getLeft())
        {
            rotateRight(parent);
            rotateLeft(grand);
            if(current->getBalance() == -1)
            {
                parent->setBalance(1);
                grand->setBalance(0);
                current->setBalance(0);
            }
            else if(current->getBalance() == 0)
            {
                parent->setBalance(0);
                grand->setBalance(0);
                current->setBalance(0);
            }
            else if(current->getBalance() == 1)
            {
                parent->setBalance(0);
                grand->setBalance(-1);
                current->setBalance(0);
            }
        }
    return;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft (AVLNode<Key,Value> *n1)
{
    AVLNode<Key,Value> *parent = n1->getRight();
    AVLNode<Key,Value> *temp = parent->getLeft();
    AVLNode<Key,Value> *grandparent = n1->getParent();
    //if parent is not the root node
    if(grandparent != NULL)
    {
        if(n1 == grandparent->getLeft())
        {
            grandparent->setLeft(parent);
        }
        else
        {
            grandparent->setRight(parent);
        }
    }
    else
    {
        this->root_ = parent;
    }
    //update new parent's pointers
    parent->setLeft(n1);
    parent->setParent(n1->getParent());
    //update previous parent's pointers
    n1->setParent(parent);
    n1->setRight(temp);
    //update previous parent's new right child if it exists
    if(temp != NULL)
    {
        temp->setParent(n1);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight (AVLNode<Key,Value> *n1)
{
    //rotate left but mirrored
    AVLNode<Key,Value> *parent = n1->getLeft();
    AVLNode<Key,Value> *temp = parent->getRight();
    AVLNode<Key,Value> *grandparent = n1->getParent();

    if(grandparent != NULL)
    {
        if(n1 == grandparent->getLeft())
        {
            grandparent->setLeft(parent);
        }
        else
        {
            grandparent->setRight(parent);
        }
    }
    else
    {
        this->root_ = parent;
    }
    parent->setRight(n1);
    parent->setParent(n1->getParent());
    n1->setParent(parent);
    n1->setLeft(temp);
    if(temp != NULL)
    {
        temp->setParent(n1);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
