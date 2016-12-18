/**********************************************************
 * AVL Tree Implementation file
 *
 *  ** riffed and modified heavily by Aaron Crandall - 2016
 *  ** All implementation in header because of templating
 *
 **********************************************************/

#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <queue>
#include <iostream>

using namespace std;

template<class T>
class AVLNode {
public:
    // Default blank AVLNode constructor
    AVLNode() {
        left = right = nullptr;
        height = 0;
    }

    // Constructor with provided element (data) and children
    AVLNode(const T& el, AVLNode *l = nullptr, AVLNode *r = nullptr) {
        key = el;
        left = l;
        right = r;
        height = 0;
    }

    T key;                  // Key to compare/insert on of type <T>
    AVLNode *left, *right;  // Children of this node
    int height;             // Height of this node in the tree
    int getheight()         // Sets and returns height of this node and its children
    {
      if (this == nullptr)
        return -1;
      else
      { int l_height = this->left->getheight();
        int r_height = this->right->getheight();
        this->height = (l_height > r_height) ? (l_height + 1) : (r_height + 1);
      }
      return this->height;
    }
};

/*
 * Main AVLTree class that stores AVLNode objects
 *  -- REQUIRES: implementation of rotation functions
 *  -- REQUIRES: implementation of node heights
 *  -- REQUIRES: implementation of contains function
 *  -- REQUIRES: implementation of remove function
 */
template<class T>
class AVLTree {
  public:
    AVLTree() { root = nullptr; }

    void insert(const T& key) { insert(root, key); }
    void printTree() { printTree(root); }
    // Bad cohesion, but this function returns the left-most node
    // and makes adjustements to the tree as necessary for deletion
    AVLNode<T>* getmin(AVLNode<T>* &root)
    {
      AVLNode<T>* current = root;
      AVLNode<T>* previous = current;

      if (root->left == nullptr) // If the given node is the left most
      {
        if (root->right != nullptr)
        {
          root = root->right; // replace with the right-child
          balance(root);
        }
        return root;
      }
      while (current->left != nullptr) // walk until left-most pointer
      {
        previous = current;
        current = current->left;
      }
      if (current->right != nullptr) // if left-most has a right child
        previous->left = current->right;
      balance(previous);
      return current;
    }

    bool contains( const T& key ){ return(contains(root, key)); }
    void remove( const T& key ){ remove(root, key); }
    void balance (AVLNode<T>* &temp) // Balances the tree
    {
        temp->getheight(); // Verify node heights are updated
        int balance_factor = diff(temp); // Check for imbalance
        if (balance_factor > 1) // If tree is "left heavy"
        {
          if (diff(temp->left) > 0) // If "left-left" heavy
            rightRotation(temp);
          else
          {
            leftRotation(temp->left); // If "left-right" heavy
            rightRotation(temp);
          }
        }
        else if (balance_factor < -1) // If tree is "right heavy"
        {
          if (diff (temp->right) > 0) // if "right-left" heavy
          {
            rightRotation(temp->right);
            leftRotation(temp);
          }
          else
            leftRotation(temp); // if "left-left" heavy
        }

    }
    int diff (AVLNode<T> *item) // Returns difference between right and left children
    {
        if (item == nullptr)
        {
          return 0;
        }
        int l_height = (item->left->getheight());
        int r_height = (item->right->getheight());
        int balance_factor = l_height - r_height;
        return balance_factor;
      }

  private:
    AVLNode<T>* root;

    void rightRotation(AVLNode<T>* &node)
    {
      AVLNode<T>* left = node->left;

      node->left = left->right;
      left->right = node;
      node = left;
      node->getheight();
    }

    void leftRotation(AVLNode<T>* &node)
    {
      AVLNode<T>* right = node->right;

      node->right = right->left;
      right->left = node;
      node = right;
      node->getheight();
    }

    /*
     *  Insert function needs updating!
     *  This function needs to update heights as it returns from recursive insert.
     *  If the heights of a node are more than 2 different, rotate to fix
     *  After fixing, repair the heights of all nodes in the rotated tree
     */
    void insert(AVLNode<T>* &node, const T& key)
    {
      if (contains(node, key) == true)
      {
        cout << "ERROR: Node already exists" << endl;
        return;
      }
      if(node == nullptr)
      {
        node = new AVLNode<T>(key);
      }
      else if(key > node->key)
      {
        insert(node->right, key);
        balance(node);
      }
      else
      {
        insert(node->left, key);
        balance(node);
      }
      node->getheight(); // update heights
    }

    //Checks if given key is contained in the tree
    bool contains(AVLNode<T>* root, const T& key) {
      if (root == nullptr)
        return(false);
      if (root->key == key)
        return true;
      else
        if (key > root->key)
          return contains(root->right, key);
        else
          return contains(root->left, key);
    }

      // Removes given key
    void remove( AVLNode<T>* &root, const T& key)
    {
      if (contains(root, key) == false)
      {
        cout << "ERROR: Tree does not contain " << key << endl;
      }
      else
      {
        if (root->key == key)
        {
          AVLNode<T>* temp = nullptr;
          if (root->left == nullptr && root->right == nullptr) // No children
          {
            temp = root;
            root = nullptr;
            delete(temp); // Delete root
            return;
          }
          if(root->left == nullptr || root->right == nullptr) // One child
          {
            temp = root;
            if (root->left == nullptr) // Replace with the child and delete
              root = root->right;
            else
              root = root->left;
            delete(temp);
            temp = nullptr;
            return;
          }
          else // Two children
          {
            temp = getmin(root->right); // Find left-most right-child
            root->key = temp->key;
            remove(root->right, root->key); // Delete left-most right-child
            balance(root);
            return;
          }
        }
        if (key > root->key)
          remove(root->right, key);
        else
          remove(root->left, key);
        balance(root);
      }
    }

    // Should do a level order printout with actual depth (no alignment)
    void printTree(AVLNode<T>* node) {
      queue<AVLNode<T>*> bufQueue;
      int curr_height = node->height;
      bufQueue.push(node);
      while( !bufQueue.empty() ) {
        AVLNode<T>* curr = bufQueue.front();
        if( curr->left  != nullptr ){ bufQueue.push(curr->left ); }
        if( curr->right != nullptr ){ bufQueue.push(curr->right); }
        if( curr->height < curr_height ){
          curr_height = curr->height;
        }
        cout << curr->key << "  ";
        bufQueue.pop();
      }
      cout << endl;
    }
  // end private
};

#endif
