#include "../../include/stdafx.h"
#include "../../include/nodes/TreeNode.h"
#include "../../include/nodes/OperatorTreeNode.h"
#include "../../include/nodes/IndicatorTreeNode.h"

#include <iostream>


TreeNode::TreeNode()
{
  this->left = nullptr;
  this->right = nullptr;
}

TreeNode::~TreeNode()
{
  if (this->left != nullptr)
    delete this->left;

  if (this->right != nullptr)
    delete this->right;
}

void TreeNode::ToJs(v8::Local<v8::Object>& input, v8::Isolate* isolate) const {

  if (this->left != nullptr) {
    v8::Local<v8::Object> left = v8::Object::New(isolate);

    this->left->ToJs(left, isolate);

    input->Set(v8::String::NewFromUtf8(isolate, "left"),left);
  }

  if (this->right != nullptr) {
    v8::Local<v8::Object> right = v8::Object::New(isolate);

    this->right->ToJs(right, isolate);

    input->Set(v8::String::NewFromUtf8(isolate, "right"), right);
  }
}

TreeNode* TreeNode::FromJs(
  const std::vector<BaseIndicator*>& indicators,
  const v8::Local<v8::Object>& input,
  v8::Isolate* isolate) {

    //try to parse operator tree node first
    TreeNode* node = OperatorTreeNode::FromJs(input, isolate);

    if (node == nullptr) {
      node = IndicatorTreeNode::FromJs(indicators, input, isolate);
    }

    if (input->Has(v8::String::NewFromUtf8(isolate, "left"))) {

      v8::Handle<v8::Object> left =
        v8::Handle<v8::Object>::Cast(input->Get(v8::String::NewFromUtf8(isolate, "left")));

      node->left = TreeNode::FromJs(indicators, left, isolate);
    }

    if (input->Has(v8::String::NewFromUtf8(isolate, "right"))) {

      v8::Handle<v8::Object> right =
        v8::Handle<v8::Object>::Cast(input->Get(v8::String::NewFromUtf8(isolate, "right")));

      node->right = TreeNode::FromJs(indicators, right, isolate);
    }

    return node;
}
