#include "AVLTree.h"
#include "AVLTree.h"

#include <ios>
#include <iostream>
#include <string>
#include <bits/ios_base.h>


size_t AVLTree::AVLNode::numChildren() const {
    size_t numChildren = 0;
    if (this->left != nullptr)
        numChildren++; // counting the left child, if it exists
    if (this->right != nullptr)
        numChildren++; // counting the right child, if it exists
    return numChildren; // will return 0, 1, or 2
}

bool AVLTree::AVLNode::isLeaf() const {
    return this->left == nullptr && this->right == nullptr; // if a node has no children, it is a leaf.
}

int AVLTree::AVLNode::getHeight(const AVLNode *node) {
    if (node == nullptr)
        return -1; // the height of a null node is -1
    return node->height;
}

void updateHeight(AVLTree::AVLNode *node) {
    int leftHeight = AVLTree::AVLNode::getHeight(node->left); // recursively finds the size of the left and right sub trees
    int rightHeight = AVLTree::AVLNode::getHeight(node->right);
    node->height = std::max(leftHeight, rightHeight) + 1; // uses whichever is larger to calculate the height of the current node
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

        current->height = AVLNode::getHeight(current);
        balanceNode(current);

        return true; // we already deleted the one we needed to so return
    }
    delete toDelete;

    return true;
}

bool AVLTree::addNode(AVLNode *node, AVLNode *&current) {
    if (current == nullptr) { // finds the empty space where the new key should be and places the node there.
        current = node; // this is a base case.
        return true;
    }

    if (node->key == current->key) { // if the key we're trying to place already exist, do not insert the same key again.
        return false; // this is the other base case.
    }

    bool inserted = false;

    if (node->key > current->key) // recursively finds the correct position for the new key
        inserted = addNode(node, current->right);
    else
        inserted = addNode(node, current->left);

    if (!inserted)
        return false;

    updateHeight(current);
    balanceNode(current);

    return inserted;
}


bool AVLTree::remove(AVLNode *&current, KeyType key) { // remove helper function
    if (current == nullptr) // if the key is not found, there is no value to remove.
        return false;
    if (current->key == key) // found correct key, passing to main remove function
        return removeNode(current);
    bool removed = false;
    if (current->key < key) // recursively finds the target key
        removed = remove(current->right, key);
    else
        removed = remove(current->left, key);
    if (!removed)
        return false;
    updateHeight(current); // fixes the tree after removal changes heights and balance
    balanceNode(current);
    return removed;
}

bool AVLTree::insert(const std::string &key, size_t value) { // insert helper function
    AVLNode *newNode = new AVLNode(key, value); // creates node to insert

    if (addNode(newNode, root)) { // calls main insert function, pases new node
        numNodes++;
        return true;
    }
    delete newNode; // if the key already existed within the tree, delete the node to prevent data leaks
    return false;
}

bool AVLTree::remove(const std::string &key) { // remove helper function
    return remove(root, key);
}

bool AVLTree::contains(const std::string &key) const { // contains helper function
    return contains(key, root);
}

bool AVLTree::contains(const std::string &key, AVLNode *node) const {
    if (node == nullptr) // if the node is null, the key does not exist in the tree.
        return false; // base case.
    if (node->key == key) // if the key we're looking for and the current key are the same, this is the value we were looking for.
        return true; // base case.
    if (node->key > key) // recursively finds the searched key.
        return contains(key, node->left);
    return contains(key, node->right);
}

std::optional<size_t> AVLTree::get(const std::string &key) const { // get helper function
    return get(key, root);
}

std::optional<size_t> AVLTree::get(const std::string &key, AVLNode *node) const {
    if (node == nullptr) // if the current node is null the key does not exist in the tree.
        return std::nullopt; // base case.
    if (node->key == key) // if the node's key = the searched key, return the value of the key.
        return std::make_optional(node->value); // base case.
    if (node->key > key) // recursively finds the searched key.
        return get(key, node->left);
    return get(key, node->right);
}

size_t &AVLTree::operator[](const std::string &key) { // operator[] helper function
    return getBrackets(key, root);
}

size_t &AVLTree::getBrackets(const std::string &key, AVLNode *node) {
    if (node == nullptr) // if the node is null, they key does not exist in the tree.
        throw std::invalid_argument("AVLTree::operator[]: Key not found"); // base case.
    if (node->key == key) // if the searched key = the node's key, return the value.
        return node->value; // base case.
    if (node->key > key) // recursively finds the searched key.
        return getBrackets(key, node->left);
    return getBrackets(key, node->right);
}

std::vector<size_t> AVLTree::findRange(const std::string &lowKey, const std::string &highKey) const { // findRange helper function
    std::vector<size_t> values;
    addKeysRange(root, highKey, lowKey, values);
    return values;
}

void AVLTree::addKeysRange(AVLNode *node, const std::string &highKey, const std::string &lowKey,
                           std::vector<size_t> &values) const {
    if (node == nullptr) // base case
        return;
    if (lowKey <= node->key && node->key <= highKey) {
        addKeysRange(node->left, highKey, lowKey, values); // recursively finds all keys' values between the searched keys
        values.push_back(node->value);
        addKeysRange(node->right, highKey, lowKey, values);
    } else if (lowKey > node->key) { // recursively finds the keys' values of nodes where the parents are outside the range
        addKeysRange(node->right, highKey, lowKey, values); // but the children are not
    } else if (highKey < node->key) {
        addKeysRange(node->left, highKey, lowKey, values);
    }
}

std::vector<std::string> AVLTree::keys() const { // keys helper function
    std::vector<std::string> keys;
    addKeys(this->root, keys);
    return keys;
}

void AVLTree::addKeys(AVLNode *node, std::vector<std::string> &keys) const {
    if (node == nullptr) // base case
        return;
    addKeys(node->left, keys); // recursively finds all the keys and adds them to the list of keys
    keys.push_back(node->key); // automatically sorts alphabetically
    addKeys(node->right, keys);
}

size_t AVLTree::size() const {
    return numNodes;
}

int AVLTree::getHeight() const {
    return AVLNode::getHeight(root);
}

AVLTree::AVLTree() { // default constructor
    root = nullptr;
    numNodes = 0;
}

AVLTree::AVLTree(const AVLTree &other) { // copy constructor helper function
    root = nullptr;
    numNodes = other.numNodes;
    cloneTree(root, other.root);
}

AVLTree &AVLTree::operator=(const AVLTree &other) { // copy constructor helper function
    clear(root);

    root = nullptr;
    numNodes = other.numNodes;
    cloneTree(root, other.root);
    return *this;
}

void AVLTree::cloneTree(AVLNode *&current, AVLNode *other) {  // copy constructor
    if (other == nullptr) // base case
        return;
    current = new AVLNode(other->key, other->value); // recursively adds all keys to the new tree in
    cloneTree(current->left, other->left); // the same order as the cloned tree
    cloneTree(current->right, other->right);
}

AVLTree::~AVLTree() { // destructor
    clear(root);
}

std::ostream &operator<<(std::ostream &os, const AVLTree &tree) { // print helper function
    printOperator(os, tree.root, 0);
    return os;
}

void printOperator(std::ostream &os, const AVLTree::AVLNode *node, int level) {
    if (node == nullptr) // base case
        return;
    printOperator(os, node->right, level + 1); // recursively prints all nodes
    for (int i = 0; i < level; i++) // indents the node correctly based on the node's level
        os << "    ";
    os << node->key << ", " << node->value << std::endl;
    printOperator(os, node->left, level + 1);
}

void AVLTree::clear(AVLNode *currentNode) { // destructor
    if (!currentNode) {
        return;
    }
    clear(currentNode->left);
    clear(currentNode->right);
    delete currentNode;
}

void AVLTree::balanceNode(AVLNode *&node) {
    if (AVLNode::getHeight(node->left) - AVLNode::getHeight(node->right) >= 2) { // checks left sub tree balance
        if (AVLNode::getHeight(node->left->left) - AVLNode::getHeight(node->left->right) == -1) // checks if a left-right rotation is needed
            leftRotation(node->left);
        rightRotation(node);
    } else if (AVLNode::getHeight(node->left) - AVLNode::getHeight(node->right) <= -2) { // checks right sub tree balance
        if (AVLNode::getHeight(node->right->left) - AVLNode::getHeight(node->right->right) == 1) // checks if a right-left rotation is needed
            rightRotation(node->right);
        leftRotation(node);
    }
}

void AVLTree::rightRotation(AVLNode *&node) {
    AVLNode *hook = node->left; // finds the hook node
    node->left = hook->right; // problem node's new left child is the hook's right child
    hook->right = node; // hook's new right child is the problem node
    updateHeight(node); // updates the height of the problem and hook nodes
    node = hook;
    updateHeight(node);
}

void AVLTree::leftRotation(AVLNode *&node) {
    AVLNode *hook = node->right; // finds the hook node
    node->right = hook->left; // problem node's new right child is the hook's left child
    hook->left = node; // hook's new right child is the problem node
    updateHeight(node); // updates the height of the problem and hook nodes
    node = hook;
    updateHeight(node);
}
