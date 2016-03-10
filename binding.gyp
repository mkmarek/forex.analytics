{
  "targets": [
    {
      "target_name": "analytics",
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "lib/ta-lib/include"
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'conditions': [
        [ 'OS=="mac"', {

          'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11', '-stdlib=libc++'],
            'OTHER_LDFLAGS': ['-stdlib=libc++'],
            'MACOSX_DEPLOYMENT_TARGET': '10.7',
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
            },
            "libraries" : [
              "../lib/ta-lib/lib/libta_abstract_csr.a",
              "../lib/ta-lib/lib/libta_common_csr.a",
              "../lib/ta-lib/lib/libta_func_csr.a",
              "../lib/ta-lib/lib/libta_libc_csr.a"
            ],
        }],
        [ 'OS=="win"', {
            "libraries" : [
              "../lib/ta-lib/lib/ta_abstract_csr.lib",
              "../lib/ta-lib/lib/ta_common_csr.lib",
              "../lib/ta-lib/lib/ta_func_csr.lib",
              "../lib/ta-lib/lib/ta_libc_csr.lib"
            ],
        }],
      ],
      "sources": [
        "src/BinaryTreeFitness.cpp",
        "src/BinaryTreeChromosome.cpp",
        "src/BinaryTreeGeneticAlgo.cpp",
        "src/TradingSystem.cpp",
        "src/TradingSimulator.cpp",

        "src/indicators/CCI.cpp",
        "src/indicators/MACD.cpp",
        "src/indicators/Momentum.cpp",
        "src/indicators/RSI.cpp",
        "src/indicators/SAR.cpp",
        "src/indicators/SMA.cpp",
        "src/indicators/Stochastic.cpp",

        "src/indicators/factories/IndicatorFactory.cpp",

        "src/models/Candlestick.cpp",

        "src/nodes/IndicatorTreeNode.cpp",
        "src/nodes/OperatorTreeNode.cpp",
        "src/nodes/TreeNode.cpp",

        "src/utils/HeapSort.cpp",

        "src/export.cpp",
        "src/functions/convertOHLC.cpp",
        "src/functions/findStrategy.cpp",
        "src/functions/getIndicatorData.cpp",
        "src/functions/getMarketStatus.cpp",
        "src/functions/getTrades.cpp",
      ]
    }
  ]
}
