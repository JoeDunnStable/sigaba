/// @file internal_mathod.h
/// @package internal_method
//
/// @author Joseph Dunn on 8/18/19.
/// @copyright Joseph Dunn 2019.
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

#ifndef internal_method_h
#define internal_method_h

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::ostream;
#include <random>
using std::mt19937;
using std::uniform_int_distribution;
#include <algorithm>
using std::find;

/// check whether a string or vector of ints satisfies the internal method
class Checker {
public:
  /// consructor from string of letters and char base
  Checker(string str, char base) {
    init(str2vector(str, base));
  }
  
  /// constructor for a vector of zero-based integers
  Checker(vector<int> vec ) {
    init(vec);
  }
  
  /// transfer results to an ostream
  friend ostream& operator<< (ostream& os, Checker chk) {
    for (int i=0; i<chk.diff_used.size(); ++i) {
      if (chk.diff_used[i] != 1)
        os << "// diff " << i << " used " << chk.diff_used[i] << " times." << endl;
    }
    os << "// permuatation is "<< (chk.permutation_ok ?"OK":"Bad")
    << ", discrepancy: " << chk.score << endl;
    return os;
  }
  
  /// calculate a mod b with 0<= result < b.
  static int mod(int a, int b) {
    int out = a % b;
    if (out < 0)
      out += b;
    return out;
  }
  
private:
  vector<int> str2vector(string str, char base) {
    vector<int> izero(str.length());
    for (int i=0; i<str.length(); ++i)
      izero[i] = static_cast<int>(str[i]-base);
    return izero;
  }
  
  void init(vector<int> vec) {
    int n = static_cast<int>(vec.size());
    
    vector<bool> used(n,false);
    diff_used.resize(n,0);
    for (int i=0; i<n; ++i) {
      if (used.at(vec[i]))
        permutation_ok = false;
      else
        used.at(vec[i]) = true;
      diff_used.at(mod(vec[i] -i , n))++;
    }
    vector<int> diffs_used(n,0);
    for (int i=0; i<n; ++i)
      diffs_used[diff_used[i]]++;
    vector<int> ideal_diffs_used(n,0);
    if (0 == n%2) {
      ideal_diffs_used[0]=1;
      ideal_diffs_used[1]=n-2;
      ideal_diffs_used[2]=1;
    } else {
      ideal_diffs_used[1] = n;
    }
    for (int i=0; i<n; ++i)
      score += abs(diffs_used[i]-ideal_diffs_used[i]);
  }
  
  vector<int> diff_used;
  bool permutation_ok =true;
  int score=0;
  
};

/// Generate random rotor wiring using internal method
class InternalMethod {
public:
  
  /// construct wiring using backtracking algorithm making
  /// random choices at each stage
  template<class URNG>
  InternalMethod(int size,    ///< the size of the rotor
                 URNG& gen    ///< the uniform random number generator
                 ) : size(size) {
    
    vector<int> chosen(size);
    vector<int> available0(size);
    for (int i=0; i<size; ++i)
      available0[i] = i;
    
    vector< vector<int> > available(size);
    available.at(0) = available0;
    j=0;
    
    for (i=0; j>=0 && j<size; ++i) {
      // at this point every entry in available[i] showuld be okay
      if (available.at(j).size() == 0) {
        j = j-1;
        continue;
      }
      {
        // pick one of the available at random as the next chosen
        uniform_int_distribution<int> dist_j(0,static_cast<int>(available.at(j).size())-1);
        int k = dist_j(gen);
        chosen.at(j) = available.at(j).at(k);
        available.at(j).erase(available.at(j).begin()+k);
        /*
         for (int c : chosen)
         cout << c << " ";
         cout << "     ";
         for (int c: available.at(j))
         cout << c << " ";
         cout << endl;
         */
      }
      if (j == size-1)
        break;
      // build the available choices for next level
      vector<int> availablejp1 = available0;
      // first get rid of the already chosen ones except when size is even and j=size-2
      if (1 == size%2 || (0 == size%2 && j != size-2)) {
        for (int l=0; l<=j; ++l) {
          auto it = find(availablejp1.begin(), availablejp1.end(), chosen.at(l));
          if (it < availablejp1.end())
            availablejp1.erase(it);
        }
      }
      // Get rid of incosistent ones
      available.at(j+1).clear();
      for (int k =0; k<availablejp1.size(); ++k) {
        bool okay_k = true;
        for (int l=0; l<=j; ++l) {
          if ((chosen.at(l) +l - availablejp1.at(k) -j-1) % size == 0) {
            okay_k = false;
            break;
          }
        }
        if (okay_k)
          available.at(j+1).push_back(availablejp1.at(k));
      }
      if (available.at(j+1).size() >0 ) {
        j=j+1;
      } else if(available.at(j).size() == 0) {
        j=j-1;
      } else {
        continue;
      }
    }
    if (j == size-1) {
      if (0 == size%2) {
        // move the duplicated entry to a rnadom location
        uniform_int_distribution<int> dist(0,size-1);
        int k = dist(gen);
        diff.resize(size);
        for (int l=0; l<size; ++l)
          diff.at(l) = chosen.at(mod(k+l,size));
      } else {
        diff = chosen;
      }
      perm.resize(size);
      for (int l=0; l<size; ++l)
        perm.at(l) = mod(diff.at(l)+l, size);
    }
  } // InternalMethod
  
  /// calculate a mod b using with 0<= result < b
  static int mod(int a, int b) {
    int out = a % b;
    if (out < 0)
      out += b;
    return out;
  }
  
  /// return the resulting permutation
  vector<int> get_perm() {
    return perm;
  }
  
  /// transfers various statistics about the construction to a ostream
  friend ostream& operator<< (ostream& os, const InternalMethod& im) {
    if (im.j != im.size-1) {
      os << "j = " << im.j << " indicating failure." << endl
      << "i = " << im.i << " iterations." << endl;
    } else {
      os << "j = " << im.j << " indicating success." << endl
      << "i = " << im.i << " iterations." << endl;
      os << setw(10) << " " <<  setw(10) << "perm" << setw(10) << "diff" << endl;
      for (int k=0; k<im.size; ++k) {
        os << setw(10) << k << setw(10) << im.perm.at(k) << setw(10) << im.diff.at(k) << endl;
      }
      Checker chk(im.perm);
      os << chk << endl;
    }
    return os;
  }

private:
  int size; //the size of the permutation
  int j;    // j is the number of diffs chosen
  size_t i;    // the number of iterations
  vector<int> diff;  // the chosen diffs
  vector<int> perm;    // the resulting permutation

};

#endif /* internal_method_h */
