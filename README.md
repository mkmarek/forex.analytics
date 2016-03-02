Forex analytics native module for Node.js
==========

Node.js native library performing technical analysis over an OHLC dataset with use of genetic algorithm. The result of technical analysis are two binary trees describing strategies for buy and sell signals
which produced profit in a certain period of time specified by the input OHLC data set.

The Node.js version used is 5.3.0.

Running the source
---------------

First if you don't have it yet. Install node-gyp for comiling the c++ source code;

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
node-gyp build
```

You can run embedded examples which are located in the examples folder

```
cd examples
node example
```

Usage
---------------

Import the the module

```javascript
var analytics = require('../build/Release/analytics.node');
```

or with ES6 modules syntax

```javascript
import analytics from '../build/Release/analytics.node'
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
 indicators: [ 'CCI', 'MACD', 'RSI', 'SMA15_SMA50' ]
}
```

**progressCallback** parameter has to be a function. This function is invoked when one generation passes.
 It contains three arguments: **strategy, fitness, generation**. Where **strategy** stands for the currently best
 strategy found in a certain generation. **Fitness** is a fitness value of a certain strategy calculated by fitness evaluation algorithm.
 **Generation** is the number of generation which was just completed.

 ```javascript
 function progressCallback(strategy, fitness, generation) {
   console.log('Fitness: ' + fitness + '; Generation: ' + generation);
 }
 ```

 could print

 ```
Fitness: 0.00010751596495091384; Generation: 285
 ```

The returning value is a promise which when it's resolved passes one arguments with the best found strategy.

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
