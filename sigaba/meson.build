sigaba_main = executable('sigaba', 
                       [ 'main.cpp', '../sigaba/sigaba.cpp'],
                        dependencies : boostdep) 


my_timeout = 100

test('sigaba_help', sigaba_main, args : '--help' , timeout: my_timeout)
test('sigaba_hello', sigaba_main, args : ['-e', '--text', 'HELLO WORLD'] , timeout: my_timeout)
test('sigaba_no_file', sigaba_main, args : ['-e', '--input', '../data/dummy.txt'], timeout: my_timeout,
    should_fail : true)
test('sigaba_file_text', sigaba_main, args : ['-e', '--text', 'HELLO WORLD', '-input', '../data/text.txt'], timeout: my_timeout,
    should_fail : true)
test('sigaba_file_e', sigaba_main, args : ['-e', '--input', '../data/test.txt'], timeout: my_timeout)
test('sigaba_file_d', sigaba_main, args : ['-d', '--input', '../data/test.out'], timeout: my_timeout)
test('sigaba_no_e_d', sigaba_main, args : ['-text', 'HELLO WORLD'] ,
    timeout: my_timeout, should_fail: true)
test('sigaba_e_d', sigaba_main, args : ['-e' ,'-d','--text', 'HELLO WORLD'] ,
    timeout: my_timeout, should_fail: true)
test('sigaba_order', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherOrder', '0N2R4N6R8N',
					  '--controlOrder', '1N3R5N7R9N',
					  '--indexOrder', '4N3R2N1R0N'] , timeout: my_timeout)
test('sigaba_short_corder', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherOrder', '0N2R4N6R',
					  '--controlOrder', '1N3R5N7R9N',
					  '--indexOrder', '4N3R2N1R0N'] ,
					timeout: my_timeout, should_fail : true)
test('sigaba_long_corder', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherOrder', '0N2R4N6R8NXX',
					  '--controlOrder', '1N3R5N7R9N',
					  '--indexOrder', '4N3R2N1R0N'] ,
					timeout: my_timeout, should_fail : true)
test('sigaba_bad_corder_char', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherOrder', 'AN2R4N6R8N',
					  '--controlOrder', '1N3R5N7R9N',
					  '--indexOrder', '4N3R2N1R0N'] ,
					timeout: my_timeout, should_fail : true)
test('sigaba_bad_order_num', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherOrder', '9N2R4N6R8N',
					  '--controlOrder', '1N3R5N7R9N',
					  '--indexOrder', '4N3R2N1R0N'] ,
					timeout: my_timeout, should_fail : true)
test('sigaba_short_iorder', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherOrder', '0N2R4N6R8N',
					  '--controlOrder', '1N3R5N7R9N',
					  '--indexOrder', '4N3R2N1R'] ,
					timeout: my_timeout, should_fail : true)
test('sigaba_long_iorder', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherOrder', '0N2R4N6R8N',
					  '--controlOrder', '1N3R5N7R9N',
					  '--indexOrder', '4N3R2N1R0N5N'] ,
					timeout: my_timeout, should_fail : true)
test('sigaba_bad_iorder_char', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherOrder', '0N2R4N6R8N',
					  '--controlOrder', '1N3R5N7R9N',
					  '--indexOrder', 'XN3R2N1R0N'] ,
					timeout: my_timeout, should_fail : true)
test('sigaba_bad_iorder_num', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherOrder', '0N2R4N6R8N',
					  '--controlOrder', '1N3R5N7R9N',
					  '--indexOrder', '0N3R2N1R0N'] ,
					timeout: my_timeout, should_fail : true)
test('sigaba_pos', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherPos', 'ABCDE',
					  '--controlPos', 'FGHIJ',
					  '--indexPos', '01234'] , timeout: my_timeout)
test('sigaba_long_cpos', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherPos', 'ABCDEF',
					  '--controlPos', 'FGHIJ',
					  '--indexPos', '01234'] , 
					  timeout: my_timeout, should_fail : true)
test('sigaba_short_cpos', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherPos', 'ABCD',
					  '--controlPos', 'FGHIJ',
					  '--indexPos', '01234'] , 
					  timeout: my_timeout, should_fail : true)
test('sigaba_bad_cpos_char', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherPos', 'ABCD1',
					  '--controlPos', 'FGHIJ',
					  '--indexPos', '01234'] , 
					  timeout: my_timeout, should_fail : true)
test('sigaba_long_ipos', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherPos', 'ABCDE',
					  '--controlPos', 'FGHIJ',
					  '--indexPos', '012345'] , 
					  timeout: my_timeout, should_fail : true)
test('sigaba_short_ipos', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherPos', 'ABCDE',
					  '--controlPos', 'FGHIJ',
					  '--indexPos', '0123'] , 
					  timeout: my_timeout, should_fail : true)
test('sigaba_bad_ipos_char', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--cipherPos', 'ABCDE',
					  '--controlPos', 'FGHIJ',
					  '--indexPos', '0123X'] , 
					  timeout: my_timeout, should_fail : true)
test('sigaba_navy_init', sigaba_main, args : ['-e', '--text', 'HELLO WORLD',
                                          '--navyInit',
					  '--controlPos', 'FGHIJ'] , timeout: my_timeout)
test('sigaba_CSP2900', sigaba_main, args : ['-e', '-text', 'HELLO WORLD',
                                         '--machine', 'CSP2900'] , timeout: my_timeout)

