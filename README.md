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
