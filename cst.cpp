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
//add loc with node
void Tree::addNodeLocation(std::string name, std::string kind, int line, int position) {
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

    node->line = line;
    node->position = position;
}

void Tree::addNodeBeginnging(std::string name, std::string kind){
    TreeNode* node = new TreeNode(name);

    if (root == nullptr) {
        root = node;
    } else {
        node->parent = cur;
        cur->children.insert(cur->children.begin(), node);
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

std::vector<std::pair<std::string, int>> Tree::toScope() {
    std::vector<std::pair<std::string, int>> traversalResult;
    std::function<void(TreeNode*, int)> expand = [&](TreeNode* node, int scope) {
        if (node->children.size() == 0) {
            traversalResult.push_back(std::make_pair("[" + node->name + "]", scope));
        } else {
            traversalResult.push_back(std::make_pair("<" + node->name + ">", scope));
            for (int i = 0; i < node->children.size(); i++) {
                expand(node->children[i], scope + 1);
            }
        }
    };
    expand(root, 0);
    return traversalResult;
}


//added a method to get the elements of a tree into a vector
void Tree::getElements(TreeNode* node, std::vector<std::string>& elements) {
    elements.push_back(node->name);

    for (int i = 0; i < node->children.size(); i++) {
        getElements(node->children[i], elements);
    }
}
//added a method to get the elements of a tree into a vector
std::vector<std::string> Tree::getElements() {
    std::vector<std::string> elements;

    if (root != nullptr) {
        getElements(root, elements);
    }

    return elements;
}

//get elements with scope?
void Tree::getElements1(TreeNode* node, std::vector<std::tuple<std::string, int, int>>& elements, int line, int position) {
    elements.push_back(std::make_tuple(node->name, line, position));

    for (int i = 0; i < node->children.size(); i++) {
        getElements1(node->children[i], elements, node->children[i]->line, node->children[i]->position);
    }
}

std::vector<std::tuple<std::string, int, int>> Tree::getElements1() {
    std::vector<std::tuple<std::string, int, int>> elements;

    if (root != nullptr) {
        getElements1(root, elements, root->line, root->position);
    }

    return elements;
}
