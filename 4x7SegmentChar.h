/*
||
|| @file 4x7SegmentChar.h
|| @version 1.0
|| @author Michael Zimmermann
|| @contact michael.zimmermann.sg@t-online.de
||
|| @description
|| |
|| #
||
|| @license
|| |	Copyright (c) 2018 Michael Zimmermann <http://www.kruemelsoft.privat.t-online.de>
|| |	All rights reserved.
|| |	
|| |	This program is free software: you can redistribute it and/or modify
|| |	it under the terms of the GNU General Public License as published by
|| |	the Free Software Foundation, either version 3 of the License, or
|| |	(at your option) any later version.
|| |	
|| |	This program is distributed in the hope that it will be useful,
|| |	but WITHOUT ANY WARRANTY; without even the implied warranty of
|| |	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|| |	GNU General Public License for more details.
|| |	
|| |	You should have received a copy of the GNU General Public License
|| |	along with this program. If not, see <http://www.gnu.org/licenses/>.
|| #
||
*/

#ifndef _KS_FOURx7SEGMENTCHAR_H
#define _KS_FOURx7SEGMENTCHAR_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

const uint8_t DIGIT_ZERO = 0;
const uint8_t DIGIT_BLANK = 36;
const uint8_t DIGIT_DASH = 37;
const uint8_t DIGIT_PERIOD = 38;

// The codes below indicate which segments must be illuminated to display each number.
const uint8_t MAX_DIGITCODE = 50;
static const uint8_t digitCodeMap[MAX_DIGITCODE] = {
//   HGFEDCBA   Segments      7-segment map
		B11000000, // 48  "0"          AAA			0
		B11111001, // 49  "1"         F   B
		B10100100, // 50  "2"         F   B
		B10110000, // 51  "3"          GGG
		B10011001, // 52  "4"         E   C
		B10010010, // 53  "5"         E   C			5
		B10000010, // 54  "6"          DDD
		B11111000, // 55  "7"
		B10000000, // 56  "8"
		B10010000, // 57  "9"
		B10001000, // 65  'A'										10
		B10000011, // 66  'b'
		B10100111, // 67  'c'
		B10100001, // 68  'd'
		B10000110, // 69  'E'
		B10001110, // 70  'F'										15
		B11000010, // 71  'G'
		B10001001, // 72  'H'
		B11111001, // 73  'I'
		B11110001, // 74  'J'
		B10001001, // 75  'K'  Same as 'H'			20
		B11000111, // 76  'L'
		B11111111, // 77  'M'  NO DISPLAY
		B10101011, // 78  'n'
		B11000000, // 79  'O'
		B10001100, // 80  'P'										25
		B10011000, // 81  'q'
		B10101111, // 82  'r'
		B10010010, // 83  'S'
		B10000111, // 84  't'
		B11000001, // 85  'U'										30
		B11000001, // 86  'V'  Same as 'U'
		B11111111, // 87  'W'  NO DISPLAY
		B10001001, // 88  'X'  Same as 'H'
		B10010001, // 89  'y'
		B10100100, // 90  'Z'  Same as '2'			35
		B11111111, // 32  ' '  BLANK
		B10111111, // 45  '-'  DASH
		B01111111, // 46  '.'  PERIOD
		B10101101, // 47  '/'
		B10110111, // 61  '='  									40
		B10101100, // 63  '?'
		B11000110, // 91  '['
		B10011011, // 92  '\'
		B11110000, // 93  ']'
		B11011100, // 94  '^'										45										
		B11110111, // 95  '_'
		B11100011, // 117 'u'
		B11001111, // 124 '|'
		B10011100, // 176 '°'
};

#endif
