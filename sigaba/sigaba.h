//
///  @file sigaba.h
///  @package sigaba
//
///  For the this C++ port of only the cryptological elements
///  @author Joseph Dunn on 8/2/19.
///  @copyright © 2019 Joseph Dunn.
///
///  Original Java version: https://maritime.org/tech/ecmapp.txt
///  @author Richard Pekelney
///  @copyright (C) 1996, by Richard Pekelney.

/*
 * The MIT License
 *
 * Copyright 2019 Joseph Dunn.
 * Copyright 1996 Richard Pekelney
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

// From https://maritime.org/tech/ecmapp.htm
/***********************************************************************
* This program emulates the operation of an ECM Mark II model CSP 889/2900 (a.k.a. SIGABA).
* More information on this machine may be found on the ECM Mark II page. This program and
* its source code are provided for the benefit of cryptologic history researchers, it is
* not recommended that this algorithm be used for modern cryptography.
***********************************************************************/

// Preface in the java source code
/***********************************************************************
* ECMApp - Emulation of ECM Mark II (aka SIGABA)
* Copyright (C) 1996, by Richard Pekelney
* All Rights Reserved
*
* SUMMARY:
*
* This program emulates an ECM Mark II (aka SIGABA).  It emulates the CSP-2900 version
* that is now on USS Pampanito SS-383 in San Francisco.  The emulation is based on the
* artifact (the machine) not engineering drawings or other documentation.  This leaves
* the possibility that some of the behavior is a bug in this particular machine rather
* than the generic design.  All the documents I have are consistent with the machine's
* functioning, but the documents are not a complete machine definition.
*
* A description of the machine on Pampanito may be found at:
* https://maritime.org/tech/ecm2.htm
* I would suggest reading the web page before the code.
* ....
***********************************************************************/


#ifndef sigaba_h
#define sigaba_h
#include <array>
using std::array;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::ostream;
using std::endl;
#include <iomanip>
using std::setw;

inline int mod (int a, int b)
{
  int ret = a % b;
  if(ret < 0)
    ret+=b;
  return ret;
}


/// class for operation common to all rotors
template<int N>
class Rotor {
public:
  /// constructor given wiring array and orientation
  Rotor(const array<int, N>& left_wiring, ///< wiring array: N int from 0-N
        const array<int, N>& right_wiring,///< inverse of left wiring array
        bool reversed,           ///< is rotor reversed
        int wiring_num           ///< wiring number for display
        ) :
  pos(0), reversed(reversed), wiring_num(wiring_num),
  left(left_wiring), right(right_wiring){}
  
  /// encrypt one integer,  Used by cipher and index rotors
  int encrypt(int in  ///< integer to encrypt from 0-25
              ) {
    if (reversed)
      return mod(pos - right[mod(pos - in,N)], N);
    else
      return mod(left[mod(in + pos,N)] - pos, N);
  }
  /// decrypt an integer.  (used only for cipher and control wheels
  int decrypt(int in   ///< integer to decrypt
              ) {
    if (reversed) {
      return mod(pos - left[mod(pos - in, N)], N);
    }
    else {
      return mod(right[mod(in + pos, N)] - pos, N);
    }
  }
  /// rotor position for 0 to N
  int pos;
  // rotate clockwise by n, (coutercloskwise if n<0)
  void rotate(int n  ///< number of clockwise steps
             ) {
    if (reversed) {             // Reversed rotors increase counter clockwise.
      pos = mod(pos + n, N);
    }
    else {
      pos = mod(pos - n, N);
    }
  }
  
  bool reversed;
  
  friend ostream& operator<< (ostream& os, Rotor<N>& r) {
    os << r.wiring_num << (r.reversed ? 'R' : 'N');
    return os;
  }
  
  int wiring_num;

private:
  const array<int, N>& left;
  const array<int, N>& right;
};  //Rotor


/// The cryptological elements of an ECM Mark II machine
class Sigaba {
public:
  enum Direction {ENCRYPT =0, DECRYPT =1};
  enum MachineType {CSP889=0, CSPNONE=1, CSP2900=2 };
  
  /// construct a machine with the indicated rotors and orientation
  Sigaba(string cipher_str,    ///< Five cipher rotors of form "nrnrnrnrnr" ,
                               ///< where n is rotor number & r="R" for reversed
         string control_str,   ///< Five cipher rotors of form "nrnrnrnrnr" ,
                               ///< where n is rotor number & r="R" for reversed
         string index_str,     ///< Five index rotors of form "nrnrnrnrnr" ,
                               ///< where n is rotor number & r="R" for reversed
         MachineType machine = CSP889  /// Machine type (CSP889, CSP2900 or CSPNONE)
         ) : machine(machine){
    if (!initialized) {
      for (int i=0; i<10; ++i) {
        for (int j=0; j<26; ++j)
          right_wiring[i][left_wiring[i][j]] = j;
      }
      initialized = true;
    }
    for (int i=0; i<5; ++i) {
      int num = cipher_str[2*i]-'0';
      cipher_rotors.push_back(Rotor<26>(left_wiring[num], right_wiring[num], cipher_str[2*i+1]=='R', num));
    }
    for (int i=0; i<5; ++i) {
      int num = control_str[2*i]-'0';
      control_rotors.push_back(Rotor<26>(left_wiring[num], right_wiring[num], control_str[2*i+1]=='R', num));
    }
    for (int i=0; i<5; ++i) {
      int num = index_str[2*i]-'0';
      index_rotors.push_back(Rotor<10>(index_wiring[num], index_wiring[num], cipher_str[2*i+1]=='R', num));
    }

  }
  /// sete ciphe and control rotors to "OOOOO"
  void zeroize() {
    set_cipher_pos("OOOOO");
    set_control_pos("OOOOO");
  }
  
  /// rotate cipher and control rotor 1 position until they're zero
  void Z_cycle() {
    for (int i=0; i<5; ++i){
      if (cipher_rotors.at(i).pos != 14)
        cipher_rotors.at(i).rotate(1);
      if (control_rotors.at(i).pos != 14)
        control_rotors.at(i).rotate(1);
    }
    
  }
  
  
  /// ctl_cycle is used when zeroize is off and master switch is in R position
  /// it steps the cipher rotors  & rotates the control rotor indicated by key
  /// returns the increment to the counter.
  void ctl_cycle (string key) {
    // The doc indicates that nothing should happen here, but Blank moves the
    // cipher rotors, this is a quirk, not a feature of the real hardware.
    if (key == "Blank"){
      // Rotate 1 to 4 cipher rotors.
      step_cipher_rotors();
      return;
    }
    // The 1-5 keys work in R position
    size_t j = string("12345").find(key);
    if ( j != string::npos) {
      // Rotate 1 to 4 cipher rotors.
      step_cipher_rotors();
      // change the position of a control rotor
      control_rotors.at(j).rotate(1);
    }
    
    // Keys other than 1-5 and Blank do nothing
    return;
  }

 
 /// set the cipher position
 void set_cipher_pos(string cipher_pos  ///< positon string of five from A-Z
                    )
 {
   for(int i = 0; i < 5; i++) {
   
    // if the first or last rotor changes clear the cipherCount
    if ( (i == 0) || (i == 4) ) {
     if (cipher_rotors[i].pos != (int) cipher_pos[i] - 'A') {
      count = 0;
     }
    }
    
    // now update the cipher bank
    cipher_rotors[i].pos = (int) cipher_pos[i] - 'A';
    
   }
  }

  /// get a string with the cipher position
  string get_cipher_pos() {
    string out;
    for( int i = 0; i<5; ++i)
      out += cipher_rotors[i].pos + 'A';
    return out;
  }
 
 
  /// set control rotor postions
  void set_control_pos(string control_pos  ///< position string of five from A-Z
                       ) {
    for (int i=0; i<5; ++i) {
      control_rotors[i].pos = control_pos[i]-'A';
    }
  }

  /// get a string with the control position
  string get_control_pos() {
    string out;
    for( int i = 0; i<5; ++i)
      out += control_rotors[i].pos + 'A';
    return out;
  }
  
  /// set index rotor positions
  void set_index_pos(string index_pos   ///< postion string of five from '0'-'9'
                     ) {
    for (int i=0; i<5; ++i) {
      index_rotors[i].pos = index_pos[i] - '0';
    }
  }
  
  /// get index rotor positions
  string get_index_pos() {
    string out;
    for (int i=0; i<5; ++i) {
      out.push_back(index_rotors[i].pos + '0');
    }
    return out;
  }
  
  /// set the machine type.  Used if machine switch is moved after setup
  void set_machine_type(MachineType new_type) {
    machine = new_type;
  }
  
  string get_machine_type() {
    switch(machine) {
      case CSP889:
        return "CSP889";
      case CSP2900:
        return "CSP2900";
      case CSPNONE:
      default:
        return "CSPNONE";
    }
  }
  
  /// process one character and step the rotors
  string cycle(string str_in,   ///< characters to process for a-z, A-Z, ' "
            Direction direction ///< direction [ENCRYPT or DECRYPT)
            )
  {
    string str = filter_in(direction, str_in);
    string out;
    for (int i=0; i<static_cast<int>(str.size()); ++i) {
      int in = str[i]-'A';
      out.push_back(cipher_path(direction, in) +'A');
      step_cipher_rotors();
      step_control_rotors();
      if (trace) {
        cout << get_cipher_pos() << " " << get_control_pos() << endl;
      }
    }
    return filter_out(direction, out);
  }
  
  /// Initialize the control rotors one step at a time while cycling
  /// the cipher rotors
  void navy_init(string control_pos) {
    zeroize();
    for (int j = 0; j<5 ; ++j) {
      int target = control_pos[j]-'A';
      while (control_rotors.at(j).pos != target) {
        step_cipher_rotors();
        control_rotors.at(j).rotate(1) ;
        if (trace) {
          cout << get_cipher_pos() << " " << get_control_pos() << endl;
        }
      }
    }
  }
  
  /// print the machine state to an ostream
  friend   ostream& operator<< (ostream& os, Sigaba sig) {
    os << "Cipher Order:     ";
    for (int i=0; i<5; ++i)
      os << sig.cipher_rotors[i];
    os << endl;
    os << "Control Order:    ";
    for (int i=0; i<5; ++i)
      os << sig.control_rotors[i];
    os << endl;
    os << "Index Order:      ";
    for (int i=0; i<5; ++i)
      os << sig.index_rotors[i];
    os << endl;
    os << "Machine type:     " << sig.get_machine_type() << endl;
    os << endl;
    os << "Cipher Position:  " << sig.get_cipher_pos() << endl;
    os << "Control Position: " << sig.get_control_pos() << endl;
    os << "Index Position:   " << sig.get_index_pos() << endl;
    return os;
  }
  
  void start_trace() {
    trace=true;
  }
  
  void stop_trace() {
    trace = false;
  }
  
private:
  
  /// filter text handling spaces etc
  static string filter_in(Direction direction, string str) {
    string out;
    for (auto itr=str.begin(); itr<str.end(); ++itr) {
      char c = toupper(*itr);
      if (!isupper(c) && c!=' ')
        continue;
      if ( direction == ENCRYPT ) {
        // Convert Z to X. There are only 26 cipher text characters.
        c = (c=='Z') ? 'X' : c;
        // Convert Space Bar to Z. Spaces are more important than Z. Note that the
        // deciphered plaintext can never have a Z.
        c = (c==' ') ? 'Z' : c;
        out.push_back(c);
      } // end of encipher
      else {
        // decipher
        // ignore spaces
        if (c!=' ') {
          out.push_back(c);
        }
      }   // end of decipher
    }
    return out;
  }
  
  static string filter_out(Direction direction, string str) {
    string out;
    if (direction==ENCRYPT)
      out = str;
    else {
      for (auto itr=str.begin(); itr < str.end(); itr++) {
        // Convert Z to space
        out.push_back((*itr == 'Z') ? ' ' : *itr);
      }
    }
    return out;
  }

  void step_control_rotors(){
    if (control_rotors[2].pos == (int) 'O' - 'A') {    // medium rotor moves
      if (control_rotors[3].pos == (int) 'O' - 'A') {// slow rotor moves
        control_rotors[1].rotate(1);
      }
      control_rotors[3].rotate(1);
    }
    control_rotors[2].rotate(1);   // fast rotor always moves
    
  }
  
  void step_cipher_rotors(){
    array<bool, 5> move;
    move.fill(false);
    
    // The movements are stored in move[] because more than one of the paths through
    // the control and index banks can connect with a single cipher rotor magnet at the
    // same time.  Using the move[] array allows the program to be sequential even though
    // the machine is concurrent and thereby avoid extra motions of the rotor.
    if (machine == CSP889) {
      for (int j = (int) 'F' - 'A' ; j <= (int) 'I' - 'A' ; j++) {
        move[index_mag[index_path(control_index_889[control_path(j)])]-1] = true;
      }
      
      // Between 1 and 4 cipher rotors will rotate.
      for (int i = 0 ; i < 5 ; i++) {
        if (move[i]) {
          cipher_rotors[i].rotate(1);
          // clear the cipher rotor movement counter if the first or last rotor turn.
          if (i == 0 || i == 4) {
            count = 0;
          }
        }
      }
    }
    else {  // This is a CSP-2900, there are three changes.
      //1 Six contacts are on instead of four.
      for (int j = (int) 'D' - 'A' ; j <= (int) 'I' - 'A' ; j++) {
        //2 The control/index wiring is changed and contacts P, Q and R are not connected.
        int k = control_path(j);
        if ( (k == (int) 'P' - 'A') || (k == (int) 'Q' - 'A') || (k == (int) 'R' - 'A') ) {
          continue ;  // Skip contacts P, Q and R since they are not connected.
        }
        move[index_mag[index_path(control_index_2900[k])]-1] = true;
      }
      // Between 1 and 4 cipher rotors will rotate.
      //3 In a 2900 rotors 2 and 4 ( array index 1 and 3) rotate backwards.
      if (move[0]) {
        cipher_rotors[0].rotate(1);
        count = 0;
      }
      if (move[1]) {
        cipher_rotors[1].rotate(-1);
      }
      if (move[2]) {
        cipher_rotors[2].rotate(1);
      }
      if (move[3]) {
        cipher_rotors[3].rotate(-1);
      }
      if (move[4]) {
        cipher_rotors[4].rotate(1);
        count = 0;
      }
    }
    return;
  }

  
  int cipher_path(Direction direction, int c){
    if (direction == ENCRYPT) {
      // encrypt from left to right
      for (int i = 0 ; i < 5 ; ++i)
        c = cipher_rotors[i].encrypt(c);
    }
    else {
      // decrypt from right to left
      for (int i = 4 ; i >= 0 ; i--)
        c = cipher_rotors[i].decrypt(c);
    }
    return c;
  }
  
  int control_path(int c){
    for (int i=4; i>=0; i--)
      c = control_rotors[i].decrypt(c);
    return c;
  }
  
  int index_path(int c) {
    for (int i=0; i<5; ++i)
      c = index_rotors[i].encrypt(c);
    return c;
  }
  
public:
  static const array< array<int, 26>, 10> left_wiring;
  static array< array<int, 26> ,10> right_wiring;
  static const array< array<int, 10>, 5> index_wiring;

private:
  vector<Rotor<26> > cipher_rotors;
  vector<Rotor<26> > control_rotors;
  vector<Rotor<10> > index_rotors;
  static bool initialized;
  static const array<int,26> control_index_889;
  static const array<int,26> control_index_2900;
  static const array<int,10> index_mag;
  MachineType machine;
  int count = 0;
  bool trace = false;
  
};


#endif /* sigaba_h */
