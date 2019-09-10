Forex analytics native module for Node.js
==========
[![Build Status](https://travis-ci.org/mkmarek/forex.analytics.svg?branch=master)](https://travis-ci.org/mkmarek/forex.analytics)
[![npm version](https://badge.fury.io/js/forex.analytics.svg)](https://badge.fury.io/js/forex.analytics)

Node.js native library performing technical analysis over an OHLC dataset with use of genetic algorithm. The result of technical analysis are two binary trees describing strategies for buy and sell signals
which produced profit in a certain period of time specified by the input OHLC data set.

Running the source
---------------

First if you don't have it yet. Install node-gyp for compiling the c++ source code;

```
npm install -g node-gyp
```

Then run the install npm command to download and install all dependencies. It also compiles
the ta-lib dependency and builds the source code

```
npm install
```

To do further builds you can use

```
node-gyp configure && node-gyp build
```

You can run embedded examples which are located in the examples folder

```
cd examples
node example
```

NPM
---------------
You can install forex.analytics via npm:

```
npm install forex.analytics
```


Usage
---------------

Import the the module

```javascript
var analytics = require('forex.analytics');
```

or with ES6 modules syntax

```javascript
import analytics from 'forex.analytics'
```
Analytics object will give you several functions to use.

findStrategy(candlesticks, options, progressCallback)
---------------

Finds the optimal strategy for a certain period defined by the *candlesticks* array.

**candlesticks** parameter should contain an array of objects representing one candlestick in OHLC chart.
```javascript
{
  open: 1.113990,
  high: 1.113990,
  low: 1.113890,
  close: 1.113890,
  time: 1435701600
}
```

**options** parameter lists several properties that influence the genetic algorithm
```javascript
{
 populationCount: 100,
 generationCount: 300,
 selectionAmount: 10,
 leafValueMutationProbability: 0.5,
 leafSignMutationProbability: 0.3,
 logicalNodeMutationProbability: 0.3,
 leafIndicatorMutationProbability: 0.2,
 crossoverProbability: 0.03,
 indicators: [ 'CCI', 'MACD', 'RSI', 'SMA15_SMA50' ],
 strategy: { buy : {...}, sell : {...} }
}
```

**progressCallback** parameter has to be a function. This function is invoked when one generation passes.
 It contains three arguments: **strategy, fitness, generation**. Where **strategy** stands for the currently best
 strategy found in a certain generation. **Fitness** is a fitness value of a certain strategy calculated by fitness evaluation algorithm.
 **Generation** is the number of generation which was just completed. The **strategy** parameter describes which
 strategy it shall use as a referential. This parameter is not mandatory.

 ```javascript
 function progressCallback(strategy, fitness, generation) {
   console.log('Fitness: ' + fitness + '; Generation: ' + generation);
 }
 ```

 could print

 ```
Fitness: 0.00010751596495091384; Generation: 285
 ```

The returning value is a promise which when it's resolved passes one argument with the best found strategy.

**Full example:**
```javascript
analytics.findStrategy(candlesticks, {
  populationCount: 100,
  generationCount: 300,
  selectionAmount: 10,
  leafValueMutationProbability: 0.5,
  leafSignMutationProbability: 0.3,
  logicalNodeMutationProbability: 0.3,
  leafIndicatorMutationProbability: 0.2,
  crossoverProbability: 0.03,
  indicators: indicators

}, function(strategy, fitness, generation) {
  console.log('Fitness: ' + fitness + '; Generation: ' + generation);
})
.then(function(strategy) {
  console.log('------------Strategy-------------')
  console.log(strategy);
});
```

This could print something like:

```
Fitness: 0.0; Generation: 1
....
Fitness: 0.00010751596495091384; Generation: 297
Fitness: 0.00010751596495091384; Generation: 298
Fitness: 0.00010751596495091384; Generation: 299
Fitness: 0.00010751596495091384; Generation: 300
------------Strategy-------------
{
   "buy":{
      "operator":"And",
      "left":{
         "operator":"Or",
         "left":{
           "indicator":"Momentum",
           "sign":">",
           "value":-0.8390790893276773
         },
         "right":{
           "indicator":"RSI",
           "sign":">",
           "value":74.22093161865811
         }
      },
      "right":{
         "operator":"And",
         "left":{
           "indicator":"Momentum",
           "sign":"<",
           "value":-0.6815039536729026
         },
         "right":{
           "indicator":"Momentum",
           "sign":"<",
           "value":-0.32888175664540553
         }
      }
   },
   "sell":{
      "operator":"And",
      "left":{
         "operator":"Or",
         "left":{
           "indicator":"Momentum",
           "sign":">",
           "value":-0.8390790893276773
         },
         "right":{
           "indicator":"RSI",
           "sign":">",
           "value":74.22093161865811
         }
      },
      "right":{
         "operator":"And",
         "left":{
           "indicator":"Momentum",
           "sign":"<",
           "value":-0.6815039536729026
         },
         "right":{
           "indicator":"Momentum",
           "sign":"<",
           "value":-0.32888175664540553
         }
      }
   }
}
```
convertOHLC(candlesticks, targetTimeframe)
---------------
Converts OHLC data set to a larger timeframe
(e.g. from 5 minute interval to 30 minute interval)

**candlesticks** parameter should contain an array of objects representing one candlestick in OHLC chart.
```javascript
{
  open: 1.113990,
  high: 1.113990,
  low: 1.113890,
  close: 1.113890,
  time: 1435701600
}
```

**targetTimeframe** parameter defines the target interval in seconds

**Example:**
```javascript
/**
 * Converts candlesticks from lower timeframe to 30 minute timeframe
 */
function convertTo30MOhlc(candlesticks) {
  return analytics.convertOHLC(candlesticks, 1800);
}
```

This function can be used for converting ticks to OHLC as well with a simple trick.

```javascript
/**
 * Converts ticks to one minute OHLC
 * Input could be for example: { time : 0, value : 1.225 },  { time : 10, value : 1.226 },
 *  { time : 20, value : 1.227 },  { time : 30, value : 1.228 }, ...
 */
function ticksTo1MOhlc(ticks) {
  return analytics.convertOHLC(candlesticks
    .map(t => {
      open: t.value,
      high: t.value,
      low: t.value,
      close: t.value,
      time: t.time
    }), 60 /* one minute interval */);
}
```

getMarketStatus(candlesticks, options)
---------------

Returns suggestion whether to buy or sell current for the last candlestick in the
**candlesticks** array passed in as a first parameter.

**candlesticks** parameter should contain an array of objects representing one candlestick in OHLC chart.
```javascript
{
  open: 1.113990,
  high: 1.113990,
  low: 1.113890,
  close: 1.113890,
  time: 1435701600
}
```

**options** parameter lists one property
```javascript
{
 strategy: { buy : [...], sell : [...] }
}
```
**strategy** is the result from the **findStrategy** function and defines when to buy and when to sell.

**Example:**
```javascript
var status = analytics.getMarketStatus(candlesticks, { strategy: strategy });
console.log(status);
```
Can output:
```
{ shouldBuy: true, shouldSell: false }
```

getTrades(candlesticks, options)
---------------

Returns an array of trades that were performed on a provided candlestick array
with given strategy
**candlesticks** array passed in as a first parameter.

**candlesticks** parameter should contain an array of objects representing one candlestick in OHLC chart.
```javascript
{
  open: 1.113990,
  high: 1.113990,
  low: 1.113890,
  close: 1.113890,
  time: 1435701600
}
```

**options** parameter lists one property
```javascript
{
 strategy: { buy : [...], sell : [...] }
}
```
**strategy** is the result from the **findStrategy** function and defines when to buy and when to sell.

**Example:**
```javascript
var trades = analytics.getTrades(candlesticks, {
  strategy: strategy
});
console.log(trades);
```

Can output:

```
[
   {
      "Buy":true,
      "Revenue":0.0031200000000000117,
      "MaximumLoss":0.0023200000000000998,
      "MaximumProfit":0.005009999999999959,
      "ProfitBeforeLoss" : true,
      "start":{
         "open":1.10604,
         "low":1.10586,
         "high":1.10762,
         "close":1.10711,
         "time":1435782600
      },
      "end":{
         "open":1.10833,
         "low":1.10833,
         "high":1.11044,
         "close":1.11023,
         "time":1435824000
      }
   }
]
```

Where **buy** stands for whether the specific trade was made on a profit from a
rising or falling market.

**Revenue** is the the revenue that was obtained at the end of a trade.

**MaximumLoss** describes how far the price movement went against the wanted direction.
**MaximumProfit** describes how far the price movement went on the wanted direction.
**start** and **end** are the candlesticks describing the boundaries of a given trade.
**ProfitBeforeLoss** indicates whether the maximum profit was reached before maximum loss

Roadmap
---------------
- [ ] Stabilize the solution / fix bugs
- [ ] Make the algorithm more abstract (support for different kind of chromosomes and tree nodes)
- [ ] Delegate indicator generation to a different library (perhaps use [node-talib](https://github.com/oransel/node-talib))
- [ ] Implementing support for generating indicators (math formulas with OHLC as an input) based on successful trades
- [ ] Think of more stuff to add. :)
