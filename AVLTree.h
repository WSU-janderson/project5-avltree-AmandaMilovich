/**
* AVLTree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H
#include <string>

class AVLTree {
public:
    using KeyType = std::string;
    using ValueType = size_t;

protected:
    class AVLNode {
    public:
        KeyType key;
        ValueType value;
        size_t height;

        AVLNode* left;
        AVLNode* right;

        // 0, 1 or 2
        [[nodiscard]] size_t numChildren() const;
        // true or false
        [[nodiscard]] bool isLeaf() const;
        // number of hops to deepest leaf node
        [[nodiscard]] size_t getHeight() const;

        AVLNode(KeyType key, ValueType value) {
            this->key = key;
            this->value = value;
            this->height = 0;
            this->left = nullptr;
            this->right = nullptr;
        };
    };

public:
private:
    AVLNode* root;

    /* Helper methods for remove */
    // this overloaded remove will do the recursion to remove the node
    bool remove(AVLNode*& current, KeyType key);

    void insert(std::string key, size_t value);

    // removeNode contains the logic for actually removing a node based on the numebr of children
    bool addNode(AVLNode* node);

    bool addNode(AVLNode *node, AVLNode *&current);

    bool removeNode(AVLNode*& current);
    // You will implement this, but it is needed for removeNode()
    void balanceNode(AVLNode*& node);

    friend void updateHeight(AVLNode* node);
};

#endif //AVLTREE_H