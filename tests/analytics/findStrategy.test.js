var assert = require('assert'),
  chai = require('chai'),
  expect = chai.expect,
  should = chai.should(),
  analytics = require('../../index'),
  candlesticks = require('../candlesticks');


var indicators = [
  'CCI',
  'MACD',
  'MACD_Signal',
  'MACD_Histogram',
  'Momentum',
  'RSI',
  'BOP',
  'ATR',
  'SAR',
  'SMA15_SMA50',
  'Stochastic'
];

describe('analytics', function() {
  describe('#findStrategy', function() {

    describe('()', function() {
      var error;

      before(function(done) {
        analytics.findStrategy()
          .then(function() {
            error = null;
            done();
          })
          .catch(function(e) {
            error = e;
            done();
          });
      });

      it('should throw validation error because candlestick parameter is required', function() {
        expect('' + error).to.be.equal('TypeError: Wrong first argument. Expecting array of candlesticks');
      });
    });

    describe('(candlesticks)', function() {
      var error;
      var strategy;

      before(function(done) {

        this.timeout(0);

        analytics.findStrategy(candlesticks.splice(0, 60))

        .then(function(s) {
          strategy = s;
          error = null;
          done();
        })

        .catch(function(e) {
          strategy = null;
          error = e;
          done();
        });
      });

      it('should throw validation error because options parameter is required', function() {
        expect('' + error).to.be.equal('TypeError: Wrong second argument. Expecting object with genetic algorithm configuration');
      });
    });

    describe('(candlesticks, { })', function() {
      var error;
      var strategy;

      before(function(done) {

        this.timeout(0);

        analytics.findStrategy(candlesticks.splice(0, 60))

        .then(function(s) {
          strategy = s;
          error = null;
          done();
        })

        .catch(function(e) {
          strategy = null;
          error = e;
          done();
        });
      });

      it('should throw validation error because options parameter is required', function() {
        expect('' + error).to.be.equal('TypeError: Wrong second argument. Expecting object with genetic algorithm configuration');
      });
    });

    describe('(candlesticks, { pipInDecimals : 0 })', function() {
      var error;
      var strategy;

      before(function(done) {

        this.timeout(0);

        analytics.findStrategy(candlesticks, { pipInDecimals: 0 })

        .then(function(s) {
          strategy = s;
          error = null;
          done();
        })

        .catch(function(e) {
          strategy = null;
          error = e;
          done();
        });
      });

      it('should throw validation error because pipInDecimals can\'t be zero or lower', function() {
        expect('' + error).to.be.equal('TypeError: Expecting pipInDecimals to be larger than 0');
      });
    });

    describe('(candlesticks, { pipInDecimals : 0.0001 })', function() {
      var error;
      var strategy;

      before(function(done) {

        this.timeout(0);

        analytics.findStrategy(candlesticks, { pipInDecimals : 0.001 })

        .then(function(s) {
          strategy = s;
          error = null;
          done();
        })

        .catch(function(e) {
          strategy = null;
          error = e;
          done();
        });
      });

      it('should not throw validation error', function() {
        expect(error).to.be.equal(null);
      });

      it('should return strategy object', function() {
        expect(strategy).to.not.be.equal(null);
      });

      it('should return strategy object with buy strategy', function() {
        expect(strategy.buy).to.not.be.equal(null);
      });

      it('should return strategy object with sell strategy', function() {
        expect(strategy.sell).to.not.be.equal(null);
      });
    });

    describe('(candlesticks, options, callback)', function() {
      var error;
      var strategy;
      var statusCounter = 0;
      var generationCount = 5;

      before(function(done) {

        this.timeout(0);

        analytics.findStrategy(candlesticks.splice(0, 50), {
          generationCount,
          pipInDecimals : 0.001
        }, () => {
          statusCounter++;
        })

        .then(function(s) {
          strategy = s;
          error = null;
          done();
        })

        .catch(function(e) {
          strategy = null;
          error = e;
          done();
        });
      });

      it('should not throw validation error', function() {
        expect(error).to.be.equal(null);
      });

      it('should return strategy object', function() {
        expect(strategy).to.not.be.equal(null);
      });

      it('should return strategy object with buy strategy', function() {
        expect(strategy.buy).to.not.be.equal(null);
      });

      it('should return strategy object with sell strategy', function() {
        expect(strategy.sell).to.not.be.equal(null);
      });

      it('should fire status callback with each completed generation', function() {
        expect(statusCounter).to.be.equal(generationCount);
      });
    });

    for (var indicator of indicators) {
      describe('(candlesticks, options)', function() {

        var error = null;

        before(function(done) {

          this.timeout(0);

          analytics.findStrategy([], {
            indicators: [indicator],
            pipInDecimals : 0.001
          })

          .then(function(s) {
            error = null;
            done();
          })

          .catch(function(e) {
            error = e;
            done();
          });
        });

        it('should throw an error when ' + indicator + ' indicator doesn\'t get sufficient number of candlesticks to perform', function() {
          expect('' + error).to.be.equal('TypeError: Unsufficient amount of input candlesticks');
        });
      });
    }
  });
});
