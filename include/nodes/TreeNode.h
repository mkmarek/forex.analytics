#pragma once
#include "../indicators/BaseIndicator.h"

class TreeNode
{
public:
	static std::random_device r;

	std::mt19937 engine;

TreeNode();
virtual ~TreeNode();

TreeNode * left;
TreeNode * right;

virtual bool Evaluate(
    const std::map<std::string, IndicatorData>& data) const = 0;

virtual void GenerateRandomValue() = 0;

virtual void Copy(TreeNode *) const = 0;
virtual TreeNode * Copy() const = 0;

virtual void ToJs(v8::Local<v8::Object>& object) const = 0;

static TreeNode * FromJs(
    const std::vector<BaseIndicator *>& indicators,
    const v8::Local<v8::Object>& input);
};
