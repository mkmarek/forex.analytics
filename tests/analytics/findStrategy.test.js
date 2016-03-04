var assert = require('assert')
  , chai = require('chai')
  , expect = chai.expect
  , should = chai.should()
  , analytics = require('../../build/Release/analytics.node')
  , candlesticks = require('../candlesticks');


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

describe('analytics', function() {
  describe('#findStrategy', function () {

    describe('()', function () {
      var error;

      before(function(done){
        analytics.findStrategy()
        .then(function() { error = null; done(); })
        .catch(function(e) {
          error = e;
          done();
        });
      });

      it('should throw validation error because at least candlestick parameter is required', function () {
        expect(''+error).to.be.equal('TypeError: Wrong number of arguments');
      });
    });

    describe('(candlesticks)', function () {
      var error;
      var strategy;

      before(function(done){

        this.timeout(0);

        analytics.findStrategy(candlesticks.splice(0,50))

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

      it('should not throw validation error', function () {
        expect(error).to.be.equal(null);
      });

      it('should return strategy object', function () {
        expect(strategy).to.not.be.equal(null);
      });

      it('should return strategy object with buy strategy', function () {
        expect(strategy.buy).to.not.be.equal(null);
      });

      it('should return strategy object with sell strategy', function () {
        expect(strategy.sell).to.not.be.equal(null);
      });
    });

    describe('(candlesticks, options, callback)', function () {
      var error;
      var strategy;
      var statusCounter = 0;
      var generationCount = 5;

      before(function(done){

        this.timeout(0);

        analytics.findStrategy(candlesticks.splice(0,50), { generationCount }, () => { statusCounter++; } )

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

      it('should not throw validation error', function () {
        expect(error).to.be.equal(null);
      });

      it('should return strategy object', function () {
        expect(strategy).to.not.be.equal(null);
      });

      it('should return strategy object with buy strategy', function () {
        expect(strategy.buy).to.not.be.equal(null);
      });

      it('should return strategy object with sell strategy', function () {
        expect(strategy.sell).to.not.be.equal(null);
      });

      it('should fire status callback with each completed generation', function () {
        expect(statusCounter).to.be.equal(generationCount);
      });
    });

    /*
      TODO: validate indicator values
     for (var indicator of indicators) {
      it('should throw an error when '+indicator+' indicator doesn\'t get sufficient number of candlesticks to perform', function () {

        var factory = () => analytics.findStrategy([]);

        should.Throw(factory);
      });
    }*/
  });
});
