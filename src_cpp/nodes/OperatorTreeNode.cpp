#include "nan.h"

#include "../../include/stdafx.h"
#include "../../include/nodes/OperatorTreeNode.h"

OperatorTreeNode::OperatorTreeNode(): value()
{
	operator_dist = std::uniform_int_distribution<int>(0, 1);
}


OperatorTreeNode::~OperatorTreeNode()
{
}

bool OperatorTreeNode::Evaluate(
  const std::map<std::string, IndicatorData>& data) const
{
  switch (this->value) {
    case Operator::And:
      return   this->left->Evaluate(data) &&
              this->right->Evaluate(data);

    case Operator::Or:
      return   this->left->Evaluate(data) ||
              this->right->Evaluate(data);

	case Operator::Xor:
		return   this->left->Evaluate(data) !=
			this->right->Evaluate(data);

  }

  return false;
}

void OperatorTreeNode::GenerateRandomValue()
{
  this->value = static_cast<Operator>(operator_dist(engine));
}

void OperatorTreeNode::Copy(TreeNode* destination) const
{
  OperatorTreeNode* node = static_cast<OperatorTreeNode*>(destination);

  node->value = this->value;
  this->left->Copy(node->left);
  this->right->Copy(node->right);
}

TreeNode* OperatorTreeNode::Copy() const
{
  OperatorTreeNode* node = new OperatorTreeNode();

  node->value = this->value;
  node->left = this->left->Copy();
  node->right = this->right->Copy();

  return node;
}

void OperatorTreeNode::ToJs(
  v8::Local<v8::Object>& object) const {

  object->Set(Nan::New<v8::String>("operator").ToLocalChecked(),
    Nan::New<v8::String>(OperatorStrings[this->value]).ToLocalChecked());

  TreeNode::ToJs(object);
}

TreeNode* OperatorTreeNode::FromJs(
  const v8::Local<v8::Object>& input) {

    if (!input->Has(Nan::New<v8::String>("operator").ToLocalChecked()))
      return nullptr;

    std::string oper =
      std::string(*v8::String::Utf8Value(input->Get(Nan::New<v8::String>("operator").ToLocalChecked())
        ->ToString()));

    Operator op = Operator::And;

    for (int i = 0; i < 3; i++) {
      if (oper == OperatorStrings[i]) {
        op = static_cast<Operator>(i);
      }
    }

    OperatorTreeNode* node = new OperatorTreeNode();
    node->value = op;

    return node;
}
