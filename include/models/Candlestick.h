#pragma once
#include "../stdafx.h"

#include <node.h>

/**
 * Class representing candlestick model with open low hight and close values
 */
class Candlestick
{
public:
double Open;
double Low;
double High;
double Close;
long Time;

/**
 * Populates a vector with candlesticks based on the input v8 array
 * @param candlesticks The target vector for converted candlesticks
 * @param input        The input v8 array used as a source for candlesticks
 * @param isolate      v8 isolate pointer
 */
static void CreateFromArray(
    std::vector<Candlestick>& candlesticks,
    const v8::Local<v8::Array>& input);

/**
 * Creates a candlestick instance from a v8 object
 * @param  input   The input v8 object
 * @param  isolate The isolate object
 * @return         New instance of candlestick class
 */
static Candlestick CreateFromObject(const v8::Local<v8::Object>& input);

/**
 * Converts vector of candlesticks to v8 array of objects
 * @param candlesticks The input vector of candlesticks
 * @param output       The output v8 array
 * @param isolate      The isolate object
 */
static void ToArray(
    const std::vector<Candlestick>& candlesticks,
    v8::Local<v8::Array>& output);

/**
 * Converts a single candlestick to a corresponding v8 object
 * @param candlestick Input candlestick object
 * @param output      V8 output object
 * @param isolate     The isolate object
 */
static void ToObject(
    const Candlestick& candlestick,
    v8::Local<v8::Object>& output);
};
