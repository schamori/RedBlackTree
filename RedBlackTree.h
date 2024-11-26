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

// Lambda for checking if a node is red
auto isRed = [](const std::shared_ptr<const Node>& node) -> bool {
    return node && node->color == RED;
};

// Lambda for left rotation
auto rotateLeft = [](const std::shared_ptr<const Node>& node) -> std::shared_ptr<const Node> {
    auto x = node->right;
    return std::make_shared<Node>(
            x->key, x->count, node->color,
            std::make_shared<Node>(node->key, node->count, RED, node->left, x->left),
            x->right
    );
};

// Lambda for right rotation
auto rotateRight = [](const std::shared_ptr<const Node>& node) -> std::shared_ptr<const Node> {
    auto x = node->left;
    return std::make_shared<Node>(
            x->key, x->count, node->color,
            x->left,
            std::make_shared<Node>(node->key, node->count, RED, x->right, node->right)
    );
};

// Lambda for color flipping
auto colorFlip = [](const std::shared_ptr<const Node>& node) -> std::shared_ptr<const Node> {
    return std::make_shared<Node>(
            node->key, node->count, RED,
            std::make_shared<Node>(node->left->key, node->left->count, BLACK, node->left->left, node->left->right),
            std::make_shared<Node>(node->right->key, node->right->count, BLACK, node->right->left, node->right->right)
    );
};

auto fixUp = [](const std::shared_ptr<const Node>& node) -> std::shared_ptr<const Node> {
    auto n = node;

    if (isRed(n->right) && !isRed(n->left)) n = rotateLeft(n);
    if (isRed(n->left) && isRed(n->left->left)) n = rotateRight(n);
    if (isRed(n->left) && isRed(n->right)) n = colorFlip(n);

    return n;
};

std::function<std::shared_ptr<const Node>(
        const std::shared_ptr<const Node>&, const std::string&, bool&)>
        insert = [](const std::shared_ptr<const Node>& root, const std::string& val, bool& increased) -> std::shared_ptr<const Node> {
    if (!root) {
        increased = true;
        return std::make_shared<Node>(val, 1, RED);
    }

    if (val == root->key) {
        return std::make_shared<Node>(root->key, root->count + 1, root->color, root->left, root->right);
    } else if (val < root->key) {
        auto newLeft = insert(root->left, val, increased);
        auto newNode = std::make_shared<Node>(root->key, root->count, root->color, newLeft, root->right);
        return fixUp(newNode);
    } else {
        auto newRight = insert(root->right, val, increased);
        auto newNode = std::make_shared<Node>(root->key, root->count, root->color, root->left, newRight);
        return fixUp(newNode);
    }
};

// Lambda for getting the count of a value
std::function<int(const std::shared_ptr<const Node>&, const std::string&)>
        getCount  = [](const std::shared_ptr<const Node>& root, const std::string& val) -> int {
    if (!root) return 0;
    if (val == root->key) return root->count;
    if (val < root->key) return getCount(root->left, val);
    return getCount(root->right, val);
};

std::function<bool(const std::shared_ptr<const Node>&, const std::string&)>
        find = [](const std::shared_ptr<const Node>& root, const std::string& val) -> bool {
    if (!root) return false;
    if (val == root->key) return true;
    if (val < root->key) return find(root->left, val);
    return find(root->right, val);
};

std::function<void(const std::shared_ptr<const Node>&, std::ostream&)>
        printTree = [](const std::shared_ptr<const Node>& root, std::ostream& output) {
    if (!root) return;
    printTree(root->left, output);
    output << root->key << " count: " << root->count << "\n";
    printTree(root->right, output);
};



#endif // PROJEKT_REDBLACKTREE_H
