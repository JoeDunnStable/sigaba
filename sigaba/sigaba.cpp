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
  /* Index Rotor 1 */ "7591482630",
  /* Index Rotor 2 */ "3810592764",
  /* Index Rotor 3 */ "4086153297",
  /* Index Rotor 4 */ "3980526174",
  /* Index Rotor 5 */ "6497135280"
};

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

