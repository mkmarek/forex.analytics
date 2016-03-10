function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var analytics = require('./build/Release/analytics.node');
var analytics2 = _interopRequireDefault(analytics);

function findStrategy() {
  const args = arguments;

  return new Promise((resolve, reject) => {

    function cb(result, error) {
      result ? resolve(result) : reject(error);
    }

    const a = [args[0], args[1], args[2], cb];

    analytics2.default.findStrategy.apply(this, a);
  });
}

module.exports = {
  convertOHLC : analytics2.default.convertOHLC,
  getMarketStatus : analytics2.default.getMarketStatus,
  getTrades : analytics2.default.getTrades,
  findStrategy : findStrategy,
  getIndicatorData : analytics2.default.getIndicatorData
};
