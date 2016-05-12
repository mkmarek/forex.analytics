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
        "src_cpp/Analytics.cpp",
        "src_cpp/TradingSimulator.cpp",
        "src_cpp/BinaryTreeFitness.cpp",
        "src_cpp/BinaryTreeChromosome.cpp",
        "src_cpp/BinaryTreeGeneticAlgo.cpp",
        "src_cpp/Export.cpp",
        "src_cpp/FitnessEvaluation.cpp",

        "src_cpp/nodes/IndicatorTreeNode.cpp",
        "src_cpp/nodes/OperatorTreeNode.cpp",
        "src_cpp/nodes/TreeNode.cpp",

        "src_cpp/utils/HeapSort.cpp",

        "src_cpp/functions/findStrategy.cpp",
        "src_cpp/functions/getMarketStatus.cpp",
        "src_cpp/functions/getTrades.cpp"
      ]
    }
  ]
}
