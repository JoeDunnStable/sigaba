//
/// @file main.cpp
/// @package sigaba
//
/// @author Joseph Dunn on 8/2/19.
//

/*
 * The MIT License
 *
 * Copyright 2019 jdunn.
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
using std::cin;
using std::endl;
#include <fstream>
using std::ifstream;
#include <exception>
using std::exception;

#include <boost/program_options.hpp>

#include <boost/filesystem.hpp>
using boost::filesystem::path;

#include "sigaba.h"

///Groups the given text into n-letter groups separated by spaces.
string group_text(string text, int n){
  string out;
  for (int i=0; i<static_cast<int>(text.length()); i+=n) {
    out += ((i!=0)?" ":"") + text.substr(i,n);
  }
  return out;
}

/// determine whter the order string is well formed with rotor numers
/// in the correct range and orientations R or N
bool validate_order(string order, int n, string name) {
  if (order.length() != 10) {
    cout << name + " must be of length 10" << endl;
    return false;
  }
  for (int i = 0; i<10; i+=2) {
    if (order[i] < '0' || order[i] >= '0' + n) {
      cout << name << " rotors must be numbers >=0 and < " << n << endl;
      return false;
    }
    if (order[i+1] != 'R' && order[i+1] != 'N') {
      cout << name << " orientatations must be N or R" << endl;
      return false;
    }
  }
  return true;
}

/// determine whether the order strings are well formed without
/// a duplication of rotors.
bool validate_orders(string cipherOrder,
                     string controlOrder,
                     string indexOrder) {
  if (!validate_order(cipherOrder, 10, "cipherOrder"))
    return false;
  if (!validate_order(controlOrder, 10, "cipherOrder"))
    return false;
  if (!validate_order(indexOrder, 5, "indexOrder"))
    return false;
  
  // Check that the big rotors are used only once
  bool big_used[10];
  for (int i=0; i<10; ++i) {
    big_used[i] = false;
  }
  for (int i=0;  i<5; ++i) {
    int c = cipherOrder[2*i]-'0';
    if ( big_used[c] ){
      cout << "Big rotors can only be used once" << endl;
      return false;
    } else {
      big_used[c] = true;
    }
  }
  
  for (int i=0;  i<5; ++i) {
    int c = controlOrder[2*i]-'0';
    if ( big_used[c] ){
      cout << "Big rotors can only be used once" << endl;
      return false;
    } else {
      big_used[c] = true;
    }
  }
  
  // Check that the little rotors are used only once
  bool little_used[5];
  for (int i=0; i<5; ++i) {
    little_used[i] = false;
  }
  for (int i=0; i<5; ++i) {
    int c = indexOrder[2*i] - '0';
    if (little_used[c]) {
      cout << "Index rotors can only be used once" << endl;
      return false;
    } else {
      little_used[c] = true;
    }
  }
  return true;
} // validate_order

/// determine whether the position strings are well formed
bool validate_pos(string cipherPos, string controlPos, string indexPos) {
  if (cipherPos.length() != 5) {
    cout << "cipherPos must be 5 characters" << endl;
    return false;
  }
  if (controlPos.length() != 5) {
    cout << "controlPos must be 5 characters" << endl;
    return false;
  }
  if (indexPos.length() != 5) {
    cout << "indexPos must be 5 characters" << endl;
    return false;
  }
  for (int i=0; i<5; ++i) {
    if (cipherPos[i] < 'A' || cipherPos[i] > 'Z') {
      cout << "Cipher poistion must be >= A & <= Z" << endl;
      return false;
    }
    if (controlPos[i] < 'A' || controlPos[i] > 'Z') {
      cout << "Control poistion must be >= A & <= Z" << endl;
      return false;
    }
    if (indexPos[i] < '0' || indexPos[i] > '9') {
      cout << "Index poistion must be >= 0 & <= 9" << endl;
      return false;
    }
  }
  return true;
}  // validate_pos



/// Entry point for the command-line ECM Mark II (Sigaba) simulation.
int main(int argc, const char* const argv[]) {
  path p(argv[0]);
  string program_name = p.filename().string();

  using namespace boost::program_options;
  bool encrypt{false}, decrypt{false}, trace(false);
  string cipherOrder, controlOrder, indexOrder;
  bool NavyInit(false);
  string cipherPos, controlPos, indexPos;
  string machine_str;
  string input_text, input_file;
  
  
  options_description desc{program_name + ": ECM Mark II cipher machine simulation"};
  
  desc.add_options()
  ("help,h", "display help message")
  ("encrypt,e", bool_switch(&encrypt), "perform an encrypt operation")
  ("decrypt,d", bool_switch(&decrypt), "perform a decrypt operation")
  ("cipherOrder",value<string>(&cipherOrder)->default_value("0N1N2N3N4N"),"cipher rotors e.g. 0N1R2N3R4N")
  ("controlOrder",value<string>(&controlOrder)->default_value("5N6N7N8N9N"),"control rotors e.g. 5N6R7N8R9N")
  ("indexOrder",value<string>(&indexOrder)->default_value("0N1N2N3N4N"),"index rotors e.g. 0N1R2N3R4N")
  ("machine",value<string>(&machine_str)->default_value("CSP889"),"machine type: CSP889, CSP2900, or CSPNONE")
  ("navyInit", bool_switch(&NavyInit)->default_value(false), "use the Navy initialiation procedure.  CipherPos is unused or overriden")
  ("cipherPos",value<string>(&cipherPos)->default_value("OOOOO"),"cipher rotors pos e.g. AAAAA")
  ("controlPos",value<string>(&controlPos)->default_value("OOOOO"),"control rotors pos e.g. AAAAA")
  ("indexPos",value<string>(&indexPos)->default_value("00000"),"index rotors pos e.g. 01234")
  ("text,t",value<string>(&input_text),"input text to encrypt/decrypt")
  ("input,i", value<string>(&input_file),"file to read input text from, - for stdin")
  ("trace", bool_switch(&trace),"show trace of rotor positions");
  
  variables_map vm;
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);

  if (vm.count("help")) {
    cout << desc << endl;
    return 0;
  }

  if (!decrypt && !encrypt) {
    cout << program_name << ": " << "Specify encrypt or decrypt" << endl;
    return 1;
  }
  if (encrypt && decrypt) {
    cout << program_name << ": " << "Supply either -e or -d, not both" << endl;
    return 1;
  }
  Sigaba::Direction direction = encrypt ? Sigaba::ENCRYPT : Sigaba::DECRYPT;

  if (!vm.count("text") && !vm.count("input")) {
    cout << program_name << ": " << "Supply either -t or -i" << endl;
    return 1;
  }
  
  if (vm.count("text") && vm.count("input")) {
    cout << program_name << ": " << "Supply either -t or -i, not both" << endl;
    return 1;
  }
  
  Sigaba::MachineType machine = Sigaba::CSP889;
  if (vm.count("machine")) {
    if (machine_str == "CSP889")
      machine = Sigaba::CSP889;
    else if (machine_str == "CSP2900")
      machine = Sigaba::CSP2900;
    else if (machine_str == "CSPNONE")
      machine = Sigaba::CSPNONE;
    else {
      cout << program_name << ": " << "Invalid machine type: " + machine_str << endl;
      return 1;
    }
  }
  
  if (!validate_orders(cipherOrder, controlOrder, indexOrder))
    return 1;
  
  // Create sigaba cipher machine object
  Sigaba sigaba(cipherOrder, controlOrder, indexOrder, machine);
  if (trace)
    sigaba.start_trace();
   
  if (!validate_pos(cipherPos, controlPos, indexPos))
    return 1;
    
  // set the inital position of the rotors
  sigaba.set_index_pos(indexPos);
  if (NavyInit)
    sigaba.navy_init(controlPos);
  else {
    sigaba.set_cipher_pos(cipherPos);
    sigaba.set_control_pos(controlPos);
  }
  
  ifstream f_in;
  bool file_in = (vm.count("input"));
  if (file_in) {
    f_in.open(input_file);
    if (!f_in) {
      cout << program_name << ": unable to open file " + input_file << endl;
      return 1;
    }
    
  }
  
  string str_in, result;
  if(file_in) {
    while (f_in) {
      string line;
      getline(f_in,line);
      str_in += line+" ";
    }
    result = sigaba.cycle(str_in, direction);
  }
  else {
    result = sigaba.cycle(input_text, direction);
  }
  
  // post process to prettify the output
  if (direction==Sigaba::ENCRYPT) {
    int n = 5;
    result = group_text(result, n);
    
    int width = 70;
    // Never break groups when grouping
    int line_length = (n+1)*(width/(n+1));
    for (int i =0; i<static_cast<int>(result.length()); i+=line_length)
      cout << result.substr(i, line_length) << endl;
  } else { // decrypt
    // break lines at word boundaries
    size_t i=0;
    string line;
    while (i < result.size()) {
      size_t j = result.find(" ", i);
      j= (j!=result.npos)? j : result.size()-1;
      if (j-i+line.size() > 70) {
        cout << line << endl;
        line = result.substr(i,j-i+1);
        i=j+1;
      } else {
        line += result.substr(i,j-i+1);
        i=j+1;
      }
    }
    cout << line;
  }

} //main

