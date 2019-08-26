//
/// @file sigaba.cpp
///  @package sigaba
//
//  Created by Joseph Dunn on 8/2/19.
//  Copyright © 2019 Joseph Dunn.
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


#include <stdio.h>

#include "sigaba.h"

/*
const vector<string> BigRotor::wiring_strs = {
  "YCHLQSUGBDIXNZKERPVJTAWFOM",
  "INPXBWETGUYSAOCHVLDMQKZJFR",
  "WNDRIOZPTAXHFJYQBMSVEKUCGL",
  "TZGHOBKRVUXLQDMPNFWCJYEIAS",
  "YWTAHRQJVLCEXUNGBIPZMSDFOK",
  "QSLRBTEKOGAICFWYVMHJNXZUDP",
  "CHJDQIGNBSAKVTUOXFWLEPRMZY",
  "CDFAJXTIMNBEQHSUGRYLWZKVPO",
  "XHFESZDNRBCGKQIJLTVMUOYAPW",
  "EZJQXMOGYTCSFRIUPVNADLHWBK"
};

const vector<string> IndexRotor::wiring_strs = {
   "7591482630",
   "3810592764",
   "4086153297",
   "3980526174",
   "6497135280"
};
*/

bool Sigaba::initialized = false;

// This is the wiring table that Rich Pekelney developed for his simulator
// translated from string form to zero based integers
// The government did not supplie any of the large rotors actually used
const array< array<int, 26>, 10> Sigaba::left_wiring = {
  array<int,26>{24,2,7,11,16,18,20,6,1,3,8,23,13,25,10,4,17,15,21,9,19,0,22,5,14,12},
  array<int,26>{8,13,15,23,1,22,4,19,6,20,24,18,0,14,2,7,21,11,3,12,16,10,25,9,5,17},
  array<int,26>{22,13,3,17,8,14,25,15,19,0,23,7,5,9,24,16,1,12,18,21,4,10,20,2,6,11},
  array<int,26>{19,25,6,7,14,1,10,17,21,20,23,11,16,3,12,15,13,5,22,2,9,24,4,8,0,18},
  array<int,26>{24,22,19,0,7,17,16,9,21,11,2,4,23,20,13,6,1,8,15,25,12,18,3,5,14,10},
  array<int,26>{16,18,11,17,1,19,4,10,14,6,0,8,2,5,22,24,21,12,7,9,13,23,25,20,3,15},
  array<int,26>{2,7,9,3,16,8,6,13,1,18,0,10,21,19,20,14,23,5,22,11,4,15,17,12,25,24},
  array<int,26>{2,3,5,0,9,23,19,8,12,13,1,4,16,7,18,20,6,17,24,11,22,25,10,21,15,14},
  array<int,26>{23,7,5,4,18,25,3,13,17,1,2,6,10,16,8,9,11,19,21,12,20,14,24,0,15,22},
  array<int,26>{4,25,9,16,23,12,14,6,24,19,2,18,5,17,8,20,15,21,13,0,3,11,7,22,1,10}};

array< array<int,26>, 10> Sigaba::right_wiring;

// This is the wiring table for the index rotors supplied by the government
const array< array<int,10>, 5> Sigaba::index_wiring = {
  array<int,10>{7,5,9,1,4,8,2,6,3,0},
  array<int,10>{3,8,1,0,5,9,2,7,6,4},
  array<int,10>{4,0,8,6,1,5,3,2,9,7},
  array<int,10>{3,9,8,0,5,2,6,1,7,4},
  array<int,10>{6,4,9,7,1,3,5,2,8,0}};

// This table has the wiring between the left side of the control rotor
// bank to the left side of the index rotor.  This table is for a CSP-889.
const array<int,26> Sigaba::control_index_889 =
{9,1,2,3,3,4,4,4,5,5,5,6,6,6,6,7,7,7,7,7,8,8,8,8,8,8}; // index

// This table has the wiring between the left side of the control rotor
// bank to the left side of the index rotor.  This table is for a CSP-2900.
// On the CSP-2900, P, Q and R are not connected. They are 9 in the table, but
// the code below handles the exception.
const array<int, 26> Sigaba::control_index_2900 =
{9,1,2,3,3,4,4,4,5,5,5,6,6,6,6,9,9,9,7,7,0,0,8,8,8,8};  // index

// This table has the wiring between the right side of the index rotor bank
// to the magnets that rotate the cipher rotors.
const array<int, 10> Sigaba::index_mag =
{1,5,5,4,4,3,3,2,2,1};  // rotor stepping magnet

