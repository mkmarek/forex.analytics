var analytics = require('../index.js');
var fs = require('fs');

/**
 * Indicators selected for calculation
 * @type {Array}
 */
var indicators = [
  'CCI',
  'MACD',
  'MACD_Signal',
  'MACD_Histogram',
  'Momentum',
  'RSI',
  'SAR',
  'SMA15_SMA50',
  'Stochastic_K',
  'Stochastic_D'
];

var stopLoss = 0.0015;

/**
 * Training csv file worth of 6 month data
 * @type {String}
 */
var trainingFile = './data/DAT_MT_EURUSD_M1_2015.csv';

/**
 * Testing data file worth of 1 month data.
 * The strategy trained on the training data set will be applied
 * to this data set to verify the output strategy
 *
 * @type {String}
 */
var testingFile = './data/DAT_MT_EURUSD_M1_201601.csv';

/**
 * Loads candlestick data from a file and then fires a callback with the data
 * as a parameter
 * @param  {String}   inputFile Path to the csv file
 * @param  {Function} callback  Callback function that is fired after data
 *                              is loaded
 */
function loadCsvData(inputFile, callback) {
  var csvContent = '';

  var stream = fs.createReadStream(inputFile)
    .on('readable', function() {
      var buf = stream.read();

      if (buf) {
        csvContent += buf.toString();
      }
    })
    .on('end', function() {
      var candlesticks = parseCsv(csvContent);

      callback(candlesticks);
    });
}

/**
 * Returns an object representing buy/sell strategy
 * @param  {Object} candlesticks Input candlesticks for strategy estimation
 */
function createStrategy(candlesticks) {
  return analytics.findStrategy(candlesticks, {
    populationCount: 400,
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
  });
}

/**
 * Gets an array of trades using a set of candlesticks and strategy
 * @param  {Object} candlesticks Input candlesticks for trade estimation
 * @param  {Object} strategy     Strategy obtained by the findStrategy function
 */
function getTrades(candlesticks, strategy) {
  return analytics.getTrades(candlesticks, {
    strategy: strategy,
    indicators: indicators
  });
}

/**
 * Parses the given csv and returns array of candlesticks
 * @param  {String} text The csv file content
 */
function parseCsv(text) {
  var candlesticks = [];
  var lines = text.split('\n');

  for (var i = 0; i < lines.length; i++) {
    var parts = lines[i].split(',');

    var date = new Date(parts[0] + ' ' + parts[1]);
    var time = (date.getTime()) / 1000;

    candlesticks.push({
      open: parts[2],
      high: parts[3],
      low: parts[4],
      close: parts[5],
      time: time
    });
  }

  return candlesticks;
}

/**
 * Converts candlesticks from lower timeframe to 30 minute timeframe
 */
function convertTo30MOhlc(candlesticks) {
  return analytics.convertOHLC(candlesticks, 1800);
}

console.log('Loading training data set');
loadCsvData(trainingFile, function(candlesticks) {

  var thirtyMinuteCandlesticks = convertTo30MOhlc(candlesticks);

  console.log('Calculating strategy')
  createStrategy(thirtyMinuteCandlesticks)
  .then(function(strategy) {
    console.log('------------Strategy-------------');
    console.log(JSON.stringify(strategy));
    console.log('---------------------------------');

    console.log('Loading testing data set');
    loadCsvData(testingFile, function(candlesticks) {

      var thirtyMinuteCandlesticks = convertTo30MOhlc(candlesticks);

      console.log('Calculating trades')
      var trades = getTrades(thirtyMinuteCandlesticks, strategy);

      var totalRevenue = 0;
      var totalNoOfTrades = 0;
      var numberOfProffitTrades = 0;
      var numberOfLossTrades = 0;
      var maximumLoss = 0;

      for (var i = 0; i < trades.length; i++) {

        var revenue;

        if (stopLoss < trades[i].MaximumLoss) {
          revenue = -stopLoss;
        } else {
          revenue = trades[i].Revenue || 0;
        }

        if (revenue > 0) numberOfProffitTrades++;
        else numberOfLossTrades++;

        totalNoOfTrades++;

        totalRevenue += revenue;

        if (maximumLoss < trades[i].MaximumLoss)
          maximumLoss = trades[i].MaximumLoss;
      }

      console.log('Total theoretical revenue is: ' + totalRevenue + ' PIPS');
      console.log('Maximum theoretical loss is: ' + maximumLoss + ' PIPS');
      console.log('Total number of proffitable trades is: ' + numberOfProffitTrades);
      console.log('Total number of loss trades is: ' + numberOfLossTrades);
      console.log('Total number of trades is: ' + totalNoOfTrades);
    });
  });
});
