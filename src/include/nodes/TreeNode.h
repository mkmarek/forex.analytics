#ifndef _TREE_NODE_H
#define _TREE_NODE_H

#include <node.h>
#include <random>
#include <unordered_map>

class TreeNode
{
public:
	static std::random_device r;

	std::mt19937 engine;

TreeNode();
virtual ~TreeNode();

TreeNode * left;
TreeNode * right;

virtual bool Evaluate(const std::unordered_map<std::string, double>& data) const = 0;
virtual void GenerateRandomValue() = 0;
virtual void Copy(TreeNode *) const = 0;
virtual TreeNode * Copy() const = 0;

virtual void ToJs(v8::Local<v8::Object>& object) const = 0;
static TreeNode * FromJs(
	const std::vector<std::string>& indicators,
	const v8::Local<v8::Object>& input);
};

#endif
