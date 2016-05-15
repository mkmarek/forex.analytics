{
  "targets": [
    {
      "target_name": "analytics",
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
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
            }
        }]
      ],
      "sources": [
        "src/cpp/Analytics.cpp",
        "src/cpp/TradingSimulator.cpp",
        "src/cpp/BinaryTreeFitness.cpp",
        "src/cpp/BinaryTreeChromosome.cpp",
        "src/cpp/BinaryTreeGeneticAlgo.cpp",

        "src/cpp/nodes/IndicatorTreeNode.cpp",
        "src/cpp/nodes/OperatorTreeNode.cpp",
        "src/cpp/nodes/TreeNode.cpp",

        "src/cpp/utils/HeapSort.cpp",

        "src/cpp/export/findStrategy/export.cpp",
        "src/cpp/export/findStrategy/FindStrategyAsyncWorker.cpp",
        "src/cpp/export/findStrategy/FindStrategyBaton.cpp",

        "src/cpp/export/getMarketStatus/export.cpp",
        "src/cpp/export/getTrades/export.cpp",
        "src/cpp/export/Export.cpp",

        "src/cpp/utils/Utils.cpp",
      ]
    }
  ]
}
