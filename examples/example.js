var analytics = require('../build/Release/analytics.node');
var fs = require('fs');

var inputFile = './data/minute_values_092015.csv';
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
    var dailyCandlesticks = convertTo15MOhlc(candlesticks);

      var strategy = analytics.findStrategy(dailyCandlesticks, {
        populationCount : 100,
        generationCount : 120,
        selectionAmount : 25,
        leafValueMutationProbability : 1.8,
        leafSignMutationProbability : 0.3,
        logicalNodeMutationProbability : 0.6,
        leafIndicatorMutationProbability : 0.4,
        crossoverProbability : 0.03,
        indicators: ['CCI', 'MACD', 'MACD_Signal', 'MACD_Histogram', 'Momentum', 'RSI', 'SAR', 'SMA', 'Stochastic_K', 'Stochastic_D']
      });

    var trades = analytics.getTrades(dailyCandlesticks, {
        strategy : strategy,
        indicators: ['CCI', 'MACD', 'MACD_Signal', 'MACD_Histogram', 'Momentum', 'RSI', 'SAR', 'SMA', 'Stochastic_K', 'Stochastic_D']
      });

      console.log(JSON.stringify(strategy));
      console.log(JSON.stringify(trades));
  });

function parseCsv(text) {
  var candlesticks = [];
  var lines = text.split('\n');

  for (var i = 0; i < lines.length; i++) {
    var parts = lines[i].split(',');

    var date = new Date(parts[0]+ ' ' + parts[1]);
    var time = (date.getTime()) / 1000;

    candlesticks.push({
      open : parts[2],
      high : parts[3],
      low : parts[4],
      close : parts[5],
      time : time
    });
  }

  return candlesticks;
}

function convertTo15MOhlc(candlesticks) {
  return analytics.convertOHLC(candlesticks, 900);
}
