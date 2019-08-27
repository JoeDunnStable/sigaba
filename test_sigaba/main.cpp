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
  for (int i=0; i<static_cast<int>(Sigaba::left_wiring.size()); ++i) {
    vector<int> wiring(26);
    for (int j=0; j<26; ++j)
      wiring.at(j)= Sigaba::left_wiring.at(i).at(j);
    Checker chk(wiring);
    cout << setw(12) << "Big Rotor " << setw(3) << i << ": " << endl
    << chk;
  }
  for (int i=0; i<static_cast<int>(Sigaba::index_wiring.size()); ++i) {
    vector<int> wiring(10);
    for (int j=0; j<10; ++j)
      wiring.at(j) = Sigaba::index_wiring.at(i).at(j);
    Checker chk(wiring);
    cout << setw(12) << "Index Rotor " << setw(3)<< i << ": " << endl
    << chk;
  }
  
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
  for (int i=0; i<static_cast<int>(str.size()); ++i) {
    out +=sig.cycle(str.substr(i,1), Sigaba::ENCRYPT);
    if (sig.get_cipher_pos() != test_cipher_pos[i+1]) {
      cout << "cipher_pos(" << i << ") = "<< sig.get_cipher_pos() << " != " << test_cipher_pos[i+1] << endl;
    }
    if (sig.get_control_pos() != test_control_pos[i+1]) {
      cout << "control_pos(" << i << ") = "<< sig.get_control_pos() << " != " << test_control_pos[i+1] << endl;
    }
  }
  
  sig.zeroize();
  string str_out = sig.cycle(out, Sigaba::DECRYPT);
  cout << str << endl;
  cout << out << endl;
  cout << str_out << endl;
  
  // compare results to those from the Java simulator
  
  int ret = 0;
  
  string default_cipherOrder = "0N1N2N3N4N";
  string default_controlOrder = "5N6N7N8N9N";
  string default_indexOrder = "0N1N2N3N4N";
  {// CSP889  HELLO WORLD => FLqGF QUEQC H
    Sigaba tst(default_cipherOrder, default_controlOrder, default_indexOrder);
    tst.zeroize();
    string str = tst.cycle("HELLO WORLD", Sigaba::ENCRYPT);
    if (str != "FLQGFQUEQCH") {
      cout << "Failure in tst1" << endl;
      cout << str << " != " << "FLMGUIGFVWW" << endl;
      ret = 1;
    }
  }
  {
  // CSP2900 HELLO WORLD => FLMGU IGFVW W
    Sigaba tst(default_cipherOrder, default_controlOrder, default_indexOrder, Sigaba::CSP2900);
    tst.zeroize();
    string str = tst.cycle("HELLO WORLD", Sigaba::ENCRYPT);
    if (str != "FLMGUIGFVWW") {
      cout << "Failure in tst2" << endl;
      cout << str << " != " << "FLMGUIGFVWW" << endl;
      ret = 1;
    }
  }
  {
  // CSP889 --navyInit -controlOrder ABCDE HELLO WORLD => COTUC RAXLV I
    Sigaba tst(default_cipherOrder, default_controlOrder, default_indexOrder);
    string target = "COTUCRAXLVI";
    tst.zeroize();
    tst.navy_init("ABCDE");
    string str = tst.cycle("HELLO WORLD", Sigaba::ENCRYPT);
    if (str != target) {
      cout << "Failure in tst3" << endl;
      cout << str << " != " << target << endl;
      ret = 1;
    }

  }
  {
  // CSP889 --cipherOrder ABCDE --controlOrder ABCDE HELLO WORLD => PHXZJ OJXYV A
    Sigaba tst(default_cipherOrder, default_controlOrder, default_indexOrder);
    string target = "PHXZJOJXYVA";
    tst.zeroize();
    tst.set_cipher_pos("ABCDE");
    tst.set_control_pos("ABCDE");
    string str = tst.cycle("HELLO WORLD", Sigaba::ENCRYPT);
    if (str != target) {
      cout << "Failure in tst4" << endl;
      cout << str << " != " << target << endl;
      ret = 1;
    }
    
  }
  
  return ret;
}



