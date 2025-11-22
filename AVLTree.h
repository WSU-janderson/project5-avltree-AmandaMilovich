/**
* AVLTree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H
#include <optional>
#include <string>
#include <vector>

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
    bool insert(const std::string& key, size_t value);

    bool remove(const std::string& key);

    bool contains(const std::string& key) const;

    std::optional<size_t> get(const std::string& key) const;

    size_t& operator[](const std::string& key);

    std::vector<std::string> findRange(const std::string& lowKey, const std::string& highKey) const;

    std::vector<std::string> keys() const;

    size_t size() const;

    size_t getHeight() const;

    AVLTree(const AVLTree& other);

    AVLTree& operator=(const AVLTree& other);

    ~AVLTree();

    friend std::ostream& operator<<(std::ostream& os, const AVLTree& tree);

private:
    AVLNode* root;

    size_t numNodes;

    /* Helper methods for remove */

    // this overloaded remove will do the recursion to remove the node

    bool remove(AVLNode*& current, KeyType key);

    // removeNode contains the logic for actually removing a node based on the numebr of children

    bool addNode(AVLNode* node);

    bool addNode(AVLNode *node, AVLNode *&current);

    bool removeNode(AVLNode*& current);

    void clear(AVLNode* currentNode);

    // You will implement this, but it is needed for removeNode()

    void balanceNode(AVLNode*& node);

    void rightRotation(AVLNode *&node);

    void leftRotation(AVLNode *&node);

    void cloneTree(AVLNode*& currentNode, AVLNode* otherNode);

    void addKeysRange(AVLNode* node, const std::string& highKey, const std::string& lowKey, std::vector<std::string>& keys) const;

    void addKeys(AVLNode* node, std::vector<std::string>& keys) const;

    bool contains(const std::string &key, AVLNode* node) const;

    std::optional<size_t> get(const std::string &key, AVLNode *node) const;

    size_t &getBrackets(const std::string &key, AVLNode *node);

    friend void updateHeight(AVLNode* node);
};

#endif //AVLTREE_H