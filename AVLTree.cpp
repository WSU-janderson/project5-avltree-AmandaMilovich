#include "AVLTree.h"
#include "AVLTree.h"

#include <ios>
#include <string>
#include <bits/ios_base.h>

size_t AVLTree::AVLNode::numChildren() const {
    size_t numChildren = 0;
    if (this->left != nullptr)
        numChildren++;
    if (this->right != nullptr)
        numChildren++;
    return numChildren;
}

bool AVLTree::AVLNode::isLeaf() const {
    return this->left == nullptr && this->right == nullptr;
}

size_t AVLTree::AVLNode::getHeight() const {
    if (this == nullptr)
        return -1;
    return height;
}

void updateHeight(AVLTree::AVLNode *node) {
    size_t leftHeight = node->left->getHeight();
    size_t rightHeight = node->right->getHeight();
    node->height = std::max(leftHeight, rightHeight) + 1;
}

bool AVLTree::removeNode(AVLNode *&current) {
    if (!current) {
        return false;
    }

    AVLNode *toDelete = current;
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
        AVLNode *smallestInRight = current->right;
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


bool AVLTree::addNode(AVLNode *node) {
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
    return addNode(node, root->left);
}

bool AVLTree::addNode(AVLNode *node, AVLNode *&current) {
    if (node == current)
        return false;

    if (current == nullptr) {
        current = node;
        return true;
    }

    bool inserted = false;

    if (node > current)
        inserted = addNode(node, current->right);
    else
        inserted = addNode(node, current->left);

    if (!inserted)
        return false;

    updateHeight(current);
    balanceNode(current);

    return inserted;
}


bool AVLTree::remove(AVLNode *&current, KeyType key) {
    if (current == nullptr)
        return false;
    if (current->key == key)
        return removeNode(current);
    if (current->key < key)
        return remove(current->right, key);
    return remove(current->left, key);
}

bool AVLTree::insert(const std::string& key, size_t value) {
    AVLNode *newNode = new AVLNode(key, value);

    if (addNode(newNode)) {
        numNodes++;
        return true;
    }
    return false;
}

bool AVLTree::remove(const std::string &key) {
    return remove(root, key);
}

bool AVLTree::contains(const std::string &key) const {
}

std::optional<size_t> AVLTree::get(const std::string &key) const {
}

size_t & AVLTree::operator[](const std::string &key) {
}

std::vector<std::string> AVLTree::findRange(const std::string &lowKey, const std::string &highKey) const {
}

std::vector<std::string> AVLTree::keys() const {
}

size_t AVLTree::size() const {
}

size_t AVLTree::getHeight() const {
}

AVLTree::AVLTree(const AVLTree &other) {
}

AVLTree & AVLTree::operator=(const AVLTree &other) {
    clear(root);

    root = nullptr;
    numNodes = 0;

    cloneTree(root, other.root);
}

void AVLTree::cloneTree(AVLNode *&current, AVLNode *other) {
    if (other == nullptr)
        return;
    current = new AVLNode(other->key, other->value);
    cloneTree(current->left, other->left);
    cloneTree(current->right, other->right);
}

AVLTree::~AVLTree() {
    clear(root);
}

std::ostream & operator<<(std::ostream &os, const AVLTree &tree) {
}

void AVLTree::clear(AVLNode* currentNode) {
    if (!currentNode) {
        return;
    }
    clear(currentNode->left);
    clear(currentNode->right);
    delete currentNode;
}

void AVLTree::balanceNode(AVLNode *&node) {
    if (node->left->getHeight() - node->right->getHeight() >= 2) {
        if (node->left->left->getHeight() - node->left->right->getHeight() == -1)
            leftRotation(node->left);
        rightRotation(node);
    } else if (node->left->getHeight() - node->right->getHeight() <= -2)
        if (node->right->left->getHeight() - node->right->right->getHeight() == 1)
            rightRotation(node->right);
    leftRotation(node);
}

void AVLTree::rightRotation(AVLNode *&node) {
    AVLNode *hook = node->left;
    node->left = hook->right;
    hook->right = node;
    node = hook;
}

void AVLTree::leftRotation(AVLNode *&node) {
    AVLNode *hook = node->right;
    node->right = hook->left;
    hook->left = node;
    node = hook;
}
