#pragma once

#include <unordered_map>

#include "TreeNode.h"

enum Operator {  And = 0,  Or = 1, Xor = 2};
static const char * OperatorStrings[] = { "And", "Or", "Xor"};

class OperatorTreeNode :
    public TreeNode
{
public:
Operator value;

std::uniform_int_distribution<int> operator_dist;

OperatorTreeNode();
~OperatorTreeNode();

// Inherited via TreeNode
virtual bool Evaluate(
    const std::unordered_map<std::string, double>& data) const override;

virtual void GenerateRandomValue() override;

// Inherited via TreeNode
virtual void Copy(TreeNode * destination) const override;
virtual TreeNode * Copy() const override;

virtual void ToJs(v8::Local<v8::Object>& object) const override;

static TreeNode * FromJs(const v8::Local<v8::Object>& input);
};
