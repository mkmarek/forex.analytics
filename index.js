var analytics = require('./build/Release/analytics.node');

/**
 * Translate findStrategy callback signature to promise due to weird behaviour
 * in native where promises for some reason weren't resolved
 */
function findStrategy() {
  const args = arguments;

  return new Promise((resolve, reject) => {

    function cb(result, error) {
      result ? resolve(result) : reject(error);
    }

    const a = [args[0], args[1], args[2], cb];

    analytics.findStrategy.apply(this, a);
  });
}

module.exports = {
  convertOHLC : analytics.convertOHLC,
  getMarketStatus : analytics.getMarketStatus,
  getTrades : analytics.getTrades,
  findStrategy : findStrategy,
  getIndicatorData : analytics.getIndicatorData
};
