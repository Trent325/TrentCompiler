#ifndef CST_H
#define CST_H

#include <iostream>
#include <vector>

// I gave ChatGPT Alan's CST file and asked it to create a cst.h

class TreeNode {
public:
    std::string name;
    std::vector<TreeNode*> children;
    TreeNode* parent;
    int line;
    int position;

    TreeNode(std::string name);
    ~TreeNode();

};

class Tree {
public:
    TreeNode* root;
    TreeNode* cur;

    Tree();
    void addNode(std::string name, std::string kind);
    void addNodeBeginnging(std::string name, std::string kind);
    void endChildren();
    std::string toString();
    ~Tree();
    std::vector<std::string> getElements();
    void getElements(TreeNode* node, std::vector<std::string>& elements); 
    void getElements1(TreeNode* node, std::vector<std::tuple<std::string, int, int>>& elements, int line, int position);
    std::vector<std::tuple<std::string, int, int>> getElements1();
    void addNodeLocation(std::string name, std::string kind, int line, int position);

};

#endif // CST_H