#ifndef PROJEKT_REDBLACKTREE_H
#define PROJEKT_REDBLACKTREE_H

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>

enum Color { RED, BLACK };

struct Node {
    const std::string key;
    const int count;
    const Color color;
    const std::shared_ptr<const Node> left;
    const std::shared_ptr<const Node> right;

    Node(const std::string& k, int c, Color col,
         std::shared_ptr<const Node> l = nullptr,
         std::shared_ptr<const Node> r = nullptr)
            : key(k), count(c), color(col), left(l), right(r) {}
};

class RedBlackTree {
private:
    std::shared_ptr<const Node> root;

    // Insert function
    std::shared_ptr<const Node> insert(const std::shared_ptr<const Node>& node, const std::string& val, bool& increased) {
        if (!node) {
            increased = true;
            return std::make_shared<Node>(val, 1, RED);
        }

        if (val == node->key) {
            // Increase the count
            return std::make_shared<Node>(node->key, node->count + 1, node->color, node->left, node->right);
        } else if (val < node->key) {
            auto newLeft = insert(node->left, val, increased);
            auto newNode = std::make_shared<Node>(node->key, node->count, node->color, newLeft, node->right);
            return fixUp(newNode);
        } else {
            auto newRight = insert(node->right, val, increased);
            auto newNode = std::make_shared<Node>(node->key, node->count, node->color, node->left, newRight);
            return fixUp(newNode);
        }
    }

    // Fix-up function
    std::shared_ptr<const Node> fixUp(const std::shared_ptr<const Node>& node) {
        auto n = node;

        if (isRed(n->right) && !isRed(n->left)) {
            n = rotateLeft(n);
        }
        if (isRed(n->left) && isRed(n->left->left)) {
            n = rotateRight(n);
        }
        if (isRed(n->left) && isRed(n->right)) {
            n = colorFlip(n);
        }

        return n;
    }

    bool isRed(const std::shared_ptr<const Node>& node) const {
        if (!node) return false;
        return node->color == RED;
    }

    std::shared_ptr<const Node> rotateLeft(const std::shared_ptr<const Node>& node) {
        auto x = node->right;
        return std::make_shared<Node>(
                x->key, x->count, node->color,
                std::make_shared<Node>(node->key, node->count, RED, node->left, x->left),
                x->right
        );
    }

    std::shared_ptr<const Node> rotateRight(const std::shared_ptr<const Node>& node) {
        auto x = node->left;
        return std::make_shared<Node>(
                x->key, x->count, node->color,
                x->left,
                std::make_shared<Node>(node->key, node->count, RED, x->right, node->right)
        );
    }

    std::shared_ptr<const Node> colorFlip(const std::shared_ptr<const Node>& node) {
        return std::make_shared<Node>(
                node->key, node->count, RED,
                std::make_shared<Node>(node->left->key, node->left->count, BLACK, node->left->left, node->left->right),
                std::make_shared<Node>(node->right->key, node->right->count, BLACK, node->right->left, node->right->right)
        );
    }

    // Helper function for getCount
    int getCount(const std::shared_ptr<const Node>& node, const std::string& val) const {
        if (!node) return 0;
        if (val == node->key) return node->count;
        if (val < node->key) return getCount(node->left, val);
        return getCount(node->right, val);
    }

    // Printing helper function
    void printHelper(const std::shared_ptr<const Node>& node, std::ostream& output) const {
        if (!node) return;
        printHelper(node->left, output);
        output << node->key << " count: " << node->count << "\n";
        std::cout << node->key << " count: " << node->count << std::endl;
        printHelper(node->right, output);
    }

    // Search helper function
    bool find(const std::shared_ptr<const Node>& node, const std::string& val) const {
        if (!node) return false;
        if (val == node->key) return true;
        if (val < node->key) return find(node->left, val);
        return find(node->right, val);
    }

public:
    RedBlackTree() : root(nullptr) {}

    // Public insert function
    void insert(const std::string& val) {
        bool increased = false;
        root = insert(root, val, increased);
        // Ensure the root is black
        if (root->color == RED) {
            root = std::make_shared<Node>(root->key, root->count, BLACK, root->left, root->right);
        }
    }

    // Print the tree
    void printTree(std::ostream& output) const {
        printHelper(root, output);
    }

    // Find a value
    bool find(const std::string& val) const {
        return find(root, val);
    }

    // Get the count of a value
    int getCount(const std::string& val) const {
        return getCount(root, val);
    }
};

#endif //PROJEKT_REDBLACKTREE_H
