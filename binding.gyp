{
  "targets": [
    {
      "target_name": "analytics",
      "include_dirs": [
        "lib/include"
      ],
      'conditions': [
        [ 'OS=="mac"', {

          'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11', '-stdlib=libc++'],
            'OTHER_LDFLAGS': ['-stdlib=libc++'],
             'MACOSX_DEPLOYMENT_TARGET': '10.7'
            },
            "libraries" : [
              "../lib/lib/libta_abstract_csr.a",
              "../lib/lib/libta_common_csr.a",
              "../lib/lib/libta_func_csr.a",
              "../lib/lib/libta_libc_csr.a"
            ],
        }],
        [ 'OS=="win"', {

          'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11', '-stdlib=libc++'],
            'OTHER_LDFLAGS': ['-stdlib=libc++'],
             'MACOSX_DEPLOYMENT_TARGET': '10.7'
            },
            "libraries" : [
              "../lib/lib/ta_abstract_csr.lib",
              "../lib/lib/ta_common_csr.lib",
              "../lib/lib/ta_func_csr.lib",
              "../lib/lib/ta_libc_csr.lib"
            ],
        }],
      ],
      "sources": [
        "src/analytics.cpp",
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
      ]
    }
  ]
}
