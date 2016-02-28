#include "../../include/stdafx.h"
#include "../../include/nodes/OperatorTreeNode.h"
#include "../../include/nodes/IndicatorTreeNode.h"


OperatorTreeNode::OperatorTreeNode()
{
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
  }

  return false;
}

void OperatorTreeNode::GenerateRandomValue()
{
  this->value = static_cast<Operator>(rand() % 2);
}

void OperatorTreeNode::Copy(TreeNode* destination) const
{
  OperatorTreeNode* node = (OperatorTreeNode*)destination;

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
  v8::Local<v8::Object>& object,
  v8::Isolate* isolate) const {

  object->Set(v8::String::NewFromUtf8(isolate, "operator"),
    v8::String::NewFromUtf8(isolate, OperatorStrings[this->value]));

  TreeNode::ToJs(object, isolate);
}

TreeNode* OperatorTreeNode::FromJs(
  const v8::Local<v8::Object>& input,
  v8::Isolate* isolate) {

    if (!input->Has(v8::String::NewFromUtf8(isolate, "operator")))
      return nullptr;

    std::string oper =
      std::string(*v8::String::Utf8Value(input->Get(v8::String::NewFromUtf8(isolate, "operator"))
        ->ToString()));
    Operator op;

    for (int i = 0; i < 2; i++) {
      if (oper == OperatorStrings[i]) {
        op = (Operator)i;
      }
    }

    OperatorTreeNode* node = new OperatorTreeNode();
    node->value = op;

    return node;
}
