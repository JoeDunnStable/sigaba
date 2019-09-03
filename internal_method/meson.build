internal_method = executable('internal_method', 
                        'internal_mathod.cpp',
                        dependencies : boostdep) 


my_timeout = 100

test('internal_method_26_10', internal_method, args : ['-s', '26', '-n', '10'] , timeout: my_timeout)
test('internal_method_10_5', internal_method, args : ['-s', '10', '-n', '5'] , timeout: my_timeout)


