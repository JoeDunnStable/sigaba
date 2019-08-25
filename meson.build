project('sigaba', 'cpp', default_options : ['cpp_std=c++11'],
         version : '1.2.2')

compiler=meson.get_compiler('cpp').get_id()

if compiler == 'gcc' or compiler == 'clang'
  add_global_arguments(
      ['-Wno-sign-compare' ,
       '-Wno-missing-braces'
      ] ,
      language : 'cpp' )
elif compiler == 'msvc'
  add_global_arguments(
      ['/wd4244' ,
      ] ,
      language : 'cpp' )
  add_global_link_arguments(
       '/STACK:16000000',
      language : 'cpp' )
endif


#Boost header and some libraries.
#I had to define BOOST_ROOT to get this to work.
boostdep = dependency('boost', modules : ['program_options', 'timer', 'system'],  required : true)

incdir=include_directories(['internal_method',
                            'sigaba'])
subdir('doc')
subdir('sigaba')
subdir('test_sigaba')
subdir('internal_method')

