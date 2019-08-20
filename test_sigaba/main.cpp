//
/// @file  main.cpp
/// @package test_sigaba
//
///  Created by Joseph Dunn on 8/7/19.
//

/*
 * The MIT License
 *
 * Copyright 2019 Joseph Dunn.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <iostream>
using std::cout;
using std::endl;

#include "../sigaba/sigaba.h"
#include "../internal_method/internal_method.h"


int main(int argc, const char * argv[]) {
  
  vector<string> test_cipher_pos ={
    "OOOOO",
    "NONON",
    "MOMOM",
    "MOLNL",
    "LOLMK",
    "KOKMJ",
    "JNKLJ",
    "IMJKJ",
    "HMJJJ",
    "GLJJI",
    "FLIIH"};
  
  vector<string> test_control_pos = {
    "OOOOO",
    "ONNNO",
    "ONMNO",
    "ONLNO",
    "ONKNO",
    "ONJNO",
    "ONINO",
    "ONHNO",
    "ONGNO",
    "ONFNO",
    "ONENO"};
  
  
  // first check the wiring
  for (int i=0; i<BigRotor::wiring_strs.size(); ++i)
    cout << setw(12) << "Big Rotor " << setw(3) << i << ": " << endl
    << Checker(BigRotor::wiring_strs.at(i), 'A');
  for (int i=0; i<IndexRotor::wiring_strs.size(); ++i)
    cout << setw(12) << "Index Rotor " << setw(3)<< i << ": " << endl
    << Checker(IndexRotor::wiring_strs.at(i), '0');
  
  string cipherOrder = getenv("CipherOrder")==NULL ? "" : getenv("CipherOrder");
  if (cipherOrder.length() != 10) {
    cipherOrder = "0N1N2N3N4N";
  }
  string controlOrder = getenv("ControlOrder")==NULL ? "" : getenv("ControlOrder");
  if (controlOrder.length() != 10) {
    controlOrder = "5N6N7N8N9N";
  }
  string indexOrder = getenv("IndexOrder")==NULL ? "" : getenv("IndexOrder");
  if (indexOrder.length() != 10) {
    indexOrder = "0N1N2N3N4N";
  }
  
  Sigaba sig(cipherOrder, controlOrder, indexOrder);
  
  sig.zeroize();
  
  cout << sig << endl;
  if (sig.get_cipher_pos() != test_cipher_pos[0]) {
    cout << "cipher_pos(" << 0 << ") = " << sig.get_cipher_pos() << " != " << test_cipher_pos[0] << endl;
  }
  if (sig.get_control_pos() != test_control_pos[0]) {
    cout << "control_pos(" << 0 << ") = " << sig.get_control_pos() << " != " << test_control_pos[0] << endl;
  }
  string str="HELLOWORLD";
  string out;
  for (int i=0; i<str.size(); ++i) {
    out +=sig.cycle(str.substr(i,1), Sigaba::ENCRYPT);
    if (sig.get_cipher_pos() != test_cipher_pos[i+1]) {
      cout << "cipher_pos(" << i << ") = "<< sig.get_cipher_pos() << " != " << test_cipher_pos[i+1] << endl;
    }
    if (sig.get_control_pos() != test_control_pos[i+1]) {
      cout << "control_pos(" << i << ") = "<< sig.get_control_pos() << " != " << test_control_pos[i+1] << endl;
    }
  }
  
  sig.zeroize();
  string str2 = sig.cycle(out, Sigaba::DECRYPT);
  cout << str << endl;
  cout << out << endl;
  cout << str2 << endl;
  
  return 0;
}



