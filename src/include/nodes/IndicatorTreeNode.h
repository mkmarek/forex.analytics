#pragma once
#include "TreeNode.h"

static const char * SignStrings[] = { ">", "<" };

/**
 * A sign enum used for comparism of strategy required values
 * with real values
 */
enum Sign {
	Gt = 0,
	Lt = 1
};

class IndicatorTreeNode :
	public TreeNode
{
private:
	const std::vector<std::string>& indicators;

	std::uniform_int_distribution<int> sign_uniform_dist;
	std::uniform_int_distribution<int> indicator_uniform_dist;

public:
	std::string indicator;
	double value;
	Sign sign;


	IndicatorTreeNode(const std::vector<std::string>& indicators);
	~IndicatorTreeNode();

	// Inherited via TreeNode
	virtual bool Evaluate(
		const std::unordered_map<std::string, double>& data) const override;

	virtual void GenerateRandomValue() override;
	virtual void GenerateRandomSign();
	virtual void GenerateRandomIndicator();
	virtual void GenerateRandomNumbericValue();

	// Inherited via TreeNode
	virtual void Copy(TreeNode * destination) const override;
	virtual TreeNode * Copy() const override;

	virtual void ToJs(v8::Local<v8::Object>& object) const override;

	static TreeNode * FromJs(
	  const std::vector<std::string>& indicators,
		const v8::Local<v8::Object>& input);
};
