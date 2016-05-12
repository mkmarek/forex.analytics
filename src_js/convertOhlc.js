"use strict"

function validate(candlesticks, resolution) {

  if (!candlesticks) {
    throw new TypeError('Candlesticks parameter is required for convertOhlc function')
  }

  if (!Array.isArray(candlesticks)) {
    throw new TypeError('Candlesticks parameter has to be an array')
  }

	if (!resolution || resolution < 0) {
		throw new TypeError('Resolution parameter for convertOhlc is required and cannot be lower than 1')
	}

  if (!Number(resolution)) {
		throw new TypeError('Resolution parameter for convertOhlc has to be a number')
	}

}

/**
* Converts OLHC candlesticks from lower timeframe to larger timeframe
*/
function convertOhlc(candlesticks, resolution) {

  validate(candlesticks, resolution)

  const convertedBars = []

  let currentTickInterval = -1, currentBarOpen = 0, currentBarHigh = 0,
      currentBarLow = 0, currentBarClose = 0, boundaryAdjustedTime = 0

  if (!candlesticks.length)
    return convertedBars

  for (let i = 0; i < candlesticks.length; i++) {
    const bar = candlesticks[i]
    const thisTickInterval = Math.floor(bar.time / resolution)

    if (thisTickInterval !== currentTickInterval) {
      if (currentTickInterval != -1) {
				convertedBars.push({
          time : boundaryAdjustedTime,
          open : currentBarOpen,
          high : currentBarHigh,
          low : currentBarLow,
          close : currentBarClose
        })
			}

      currentTickInterval = thisTickInterval;
			boundaryAdjustedTime = currentTickInterval * resolution;

			currentBarOpen = bar.open
			currentBarHigh = bar.high
			currentBarLow = bar.low
			currentBarClose = bar.close

    } else {
			currentBarHigh = bar.high > currentBarHigh ? bar.high : currentBarHigh
			currentBarLow = bar.low < currentBarLow ? bar.low : currentBarLow
			currentBarClose = bar.close
		}
  }

  convertedBars.push({
    time : boundaryAdjustedTime,
    open : currentBarOpen,
    high : currentBarHigh,
    low : currentBarLow,
    close : currentBarClose
  })

  return convertedBars
}


module.exports = convertOhlc
