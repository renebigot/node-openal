{
  'targets': [
    {
      'target_name': 'openal',
      'sources': [ 
        "src/openal.cc", 
        "src/NodeWavData.cpp", 
        "src/NodeOpenALDevice.cpp", 
        "src/NodeOpenALContext.cpp", 
        "src/NodeOpenALSource.cpp",
        "src/NodeOpenALStream.cpp"
      ],
      'include_dirs' : [
        '<!(node -e \"require(\'nan\')\")'
      ],
      'conditions': [
        ['OS=="linux"',
         {
           'cflags_cc!': [
             '-fno-exceptions'
           ],
           'defines': [
             '__LINUX_ALSASEQ__'
           ],
           'link_settings': {
             'libraries': [
               '-lopenal'
             ]
           }
         }
        ],
        ['OS=="mac"',
         {
           'defines': [
             '__MACOSX_CORE__'
           ],
           'xcode_settings': {
             'MACOSX_DEPLOYMENT_TARGET':'10.9',
             'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
           },
           'link_settings': {
             'libraries': [
               'OpenAL.framework',
             ],
           }
         }
        ],
        ['OS=="win"',
         {
           'defines': [
             '__WINDOWS_MM__'
           ],
           'link_settings': {
             'libraries': [
               '-lAL.lib'
             ],
           }
         }
        ]
      ]
    }
  ]
}
