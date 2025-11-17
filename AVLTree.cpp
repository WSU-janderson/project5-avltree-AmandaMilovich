#include "AVLTree.h"

#include <string>

size_t AVLTree::AVLNode::numChildren() const {
    return 0;
}

bool AVLTree::AVLNode::isLeaf() const {
    return false;
}

size_t AVLTree::AVLNode::getHeight() const {
    return 0;
}

bool AVLTree::removeNode(AVLNode*& current){
    if (!current) {
        return false;
    }

    AVLNode* toDelete = current;
    auto nChildren = current->numChildren();
    if (current->isLeaf()) {
        // case 1 we can delete the node
        current = nullptr;
    } else if (current->numChildren() == 1) {
        // case 2 - replace current with its only child
        if (current->right) {
            current = current->right;
        } else {
            current = current->left;
        }
    } else {
        // case 3 - we have two children,
        // get smallest key in right subtree by
        // getting right child and go left until left is null
        AVLNode* smallestInRight = current->right;
        // I could check if smallestInRight is null,
        // but it shouldn't be since the node has two children
        while (smallestInRight->left) {
            smallestInRight = smallestInRight->left;
        }
        std::string newKey = smallestInRight->key;
        int newValue = smallestInRight->value;
        remove(root, smallestInRight->key); // delete this one

        current->key = newKey;
        current->value = newValue;

        current->height = current->getHeight();
        balanceNode(current);

        return true; // we already deleted the one we needed to so return
    }
    delete toDelete;

    return true;
}


bool AVLTree::addNode(AVLNode* node) {
    if (root == nullptr) {
        root = node;
        root->left = nullptr;
        root->right = nullptr;
        return true;
    }

    if (root == node)
        return false;

    if (node > root->right) {
        if (!root->right) {
            root->right = node;
            return true;
        }

        return addNode(node, root->right);

    }
    if (!root->left) {
        root->left = node;
        return true;
    }
    return addNode(node,   root->left);
}

bool AVLTree::addNode(AVLNode* node, AVLNode*& current) {

    if (node == current)
        return false;

    if (node > current) {
        if (!current->right) {
            current->right = node;
            return true;
        }
        return addNode(node, current->right);
    }
    if (!current->left) {
        current->left = node;
        return true;
    }
    return addNode(node, current->left);
}

bool AVLTree::remove(AVLNode *&current, KeyType key) {
    return false;
}



void AVLTree::balanceNode(AVLNode *&node) {
}
