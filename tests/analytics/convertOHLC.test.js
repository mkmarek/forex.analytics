var assert = require('assert'),
  chai = require('chai'),
  expect = chai.expect,
  should = chai.should(),
  analytics = require('../../index'),
  candlesticks = require('../candlesticks');


describe('analytics', function() {
  describe('#convertOHLC', function() {
    describe('()', function() {
      it('should throw validation error because at least two parameters are required', function() {
        var error = null;

        try {
          analytics.convertOHLC();
        } catch(e) {
          error = e;
        }

        expect('' + error).to.be.equal('TypeError: Wrong number of arguments');
      });
    });

    describe('(candlesticks)', function() {
      it('should throw validation error because at least two parameters are required', function() {
        var error = null;

        try {
          analytics.convertOHLC([]);
        } catch(e) {
          error = e;
        }

        expect('' + error).to.be.equal('TypeError: Wrong number of arguments');
      });
    });

    describe('(candlesticks, timeframe)', function() {
      it('should return empty set if empty array is passed in', function() {
        var error = null;
        var ohlc = null;

        try {
          ohlc = analytics.convertOHLC([], 1);
        } catch(e) {
          error = e;
        }

        expect(error).to.be.equal(null);
        expect(ohlc.length).to.be.equal(0);
      });
    });

    describe('(candlesticks, timeframe)', function() {

      var error = null;
      var ohlc = null;

      before(function() {
        var candlesticks = [
          {"open":1.11399,"low":1.1122,"high":1.11421,"close":1.11393,"time":0},
          {"open":1.11393,"low":1.1129,"high":1.11431,"close":1.1129,"time":100}
        ];

        try {
          ohlc = analytics.convertOHLC(candlesticks, 200);
        } catch(e) {
          error = e;
        }
      });

      it('should merge two predefined candlesticks when converting to larger timeframe', function() {
        expect(ohlc.length).to.equal(1);
      });

      it('should have the opeb price set to the open price of the first candlestick', function() {
        expect(ohlc[0].open).to.equal(1.11399);
      });

      it('should have the high price set to the highest price in specified range', function() {
        expect(ohlc[0].high).to.equal(1.11431);
      });

      it('should have the low price set to the lowest price in a specified range', function() {
        expect(ohlc[0].low).to.equal(1.1122);
      });

      it('should have the close price set to the close price of the last candlestick', function() {
        expect(ohlc[0].close).to.equal(1.1129);
      });

      it('should have time set to the first candlestck', function() {
        expect(ohlc[0].time).to.equal(0);
      });
    });
  });
});
