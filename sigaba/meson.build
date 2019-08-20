sigaba_main = executable('sigaba', 
                       [ 'main.cpp', '../sigaba/sigaba.cpp'],
                        dependencies : boostdep) 


my_timeout = 100

test('sigaba_main', sigaba_main, args : ['-e', '-text', 'HELLO WORLD'] , timeout: my_timeout)


