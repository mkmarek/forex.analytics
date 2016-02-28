#include "../../include/models/Candlestick.h"

void Candlestick::CreateFromArray(
  std::vector<Candlestick>& candlesticks,
  const v8::Local<v8::Array>& input,
  v8::Isolate* isolate) {

  unsigned candlestickCount = input->Length();

  candlesticks.clear();

  for (unsigned i = 0; i < candlestickCount; i++) {

    //All members of an array have to be integers
    if (!input->Get(i)->IsObject()) {
      isolate->ThrowException(v8::Exception::TypeError(
        v8::String::Concat(v8::String::NewFromUtf8(isolate, "Wrong type of member in array on index: "),
          v8::Integer::New(isolate, i)->ToString())
        ));
      return;
    }

    v8::Local<v8::Object> candlestick = v8::Local<v8::Object>::Cast(input->Get(i));

    Candlestick c = Candlestick::CreateFromObject(candlestick, isolate);

    candlesticks.push_back(c);
  }
}

Candlestick Candlestick::CreateFromObject(
  const v8::Local<v8::Object>& input,
  v8::Isolate* isolate) {

  Candlestick c = Candlestick();

  c.Open = input->Get(v8::String::NewFromUtf8(isolate, "open"))->NumberValue();
  c.Low = input->Get(v8::String::NewFromUtf8(isolate, "low"))->NumberValue();
  c.High = input->Get(v8::String::NewFromUtf8(isolate, "high"))->NumberValue();
  c.Close = input->Get(v8::String::NewFromUtf8(isolate, "close"))->NumberValue();
  c.Time = input->Get(v8::String::NewFromUtf8(isolate, "time"))->Int32Value();

  return c;
}

void Candlestick::ToArray(
  const std::vector<Candlestick>& candlesticks,
  v8::Local<v8::Array>& output,
  v8::Isolate* isolate) {

  unsigned candlestickCount = candlesticks.size();

  for (unsigned i = 0; i < candlestickCount; i++) {

    v8::Local<v8::Object> object = v8::Object::New(isolate);

    const Candlestick& candlestick = candlesticks[i];

    Candlestick::ToObject(candlestick, object, isolate);

    output->Set(i, object);
  }
}

void Candlestick::ToObject(
  const Candlestick& candlestick,
  v8::Local<v8::Object>& output,
  v8::Isolate* isolate) {

  output->Set(v8::String::NewFromUtf8(isolate, "open"),
    v8::Number::New(isolate, candlestick.Open));
  output->Set(v8::String::NewFromUtf8(isolate, "low"),
    v8::Number::New(isolate,candlestick.Low));
  output->Set(v8::String::NewFromUtf8(isolate, "high"),
    v8::Number::New(isolate,candlestick.High));
  output->Set(v8::String::NewFromUtf8(isolate, "close"),
    v8::Number::New(isolate,candlestick.Close));
  output->Set(v8::String::NewFromUtf8(isolate, "time"),
    v8::Int32::New(isolate,candlestick.Time));

}
