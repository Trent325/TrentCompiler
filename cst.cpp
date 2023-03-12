#include "cst.h"
#include <functional>

// I gave ChatGPT Alan's CST file and asked it to create a cst.cpp

TreeNode::TreeNode(std::string name) {
    this->name = name;
    parent = nullptr;
}

TreeNode::~TreeNode() {
    for (int i = 0; i < children.size(); i++) {
        delete children[i];
    }
}

Tree::Tree() {
    root = nullptr;
    cur = nullptr;
}

void Tree::addNode(std::string name, std::string kind) {
    TreeNode* node = new TreeNode(name);

    if (root == nullptr) {
        root = node;
    } else {
        node->parent = cur;
        cur->children.push_back(node);
    }

    if (kind == "branch") {
        cur = node;
    }
}

void Tree::endChildren() {
    if (cur->parent != nullptr && cur->parent->name != "") {
        cur = cur->parent;
    }
}

std::string Tree::toString() {
    std::string traversalResult;
    std::function<void(TreeNode*, int)> expand = [&](TreeNode* node, int depth) {
        for (int i = 0; i < depth; i++) {
            traversalResult += "-";
        }

        if (node->children.size() == 0) {
            traversalResult += "[" + node->name + "]";
            traversalResult += "\n";
        } else {
            traversalResult += "<" + node->name + "> \n";
            for (int i = 0; i < node->children.size(); i++) {
                expand(node->children[i], depth + 1);
            }
        }
    };
    expand(root, 0);
    return traversalResult;
}

Tree::~Tree() {
    delete root;
}