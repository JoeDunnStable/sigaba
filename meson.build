project('sigaba', 'cpp', default_options : ['cpp_std=c++11'],
         version : '1.2.3')

compiler=meson.get_compiler('cpp').get_id()

if compiler == 'gcc' or compiler == 'clang'
elif compiler == 'msvc'
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

