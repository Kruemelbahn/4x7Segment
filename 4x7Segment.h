/*
||
|| @file 4x7Segment.h
|| @version 1.0
|| @author Michael Zimmermann
|| @contact michael.zimmermann.sg@t-online.de
||
|| @description
|| | controls up to 4 seven segment-Displays
|| | uses Timer 2 with ISR for multiplexing, uses fixed outputs for 7-Segment-Display with common anode
|| |
|| | Connect segments to ports using resistor:
|| |           A         D3
|| |           B         D4
|| |           C         D5
|| |           D         B1
|| |           E         B2
|| |           F         B3
|| |           G         B4
|| |           dp        B5
|| |
|| | Connect anode to ports using pnp-transistor:
|| |           1         C0				MSD (most significant digit)
|| |           2         C1
|| |           3         C2
|| |           4         C3				LSD (last significant digit)
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

#ifndef _KS_FOURx7SEGMENT_H
#define _KS_FOURx7SEGMENT_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

unsigned long millis2(void);

class SevenSegment {
  public:

    SevenSegment();
    void init(uint8_t ui8_count = 4, boolean bLeadingZero = false, boolean bRightBound = false);
    void clear();
		void print(const __FlashStringHelper *p);
    void print(const char* p);
    void printInt(int16_t i_value);
		void printChar(uint8_t ui8Pos, unsigned char ch);

		void setDigit(int8_t ui8_DisplayIndex, uint8_t ui8_IndexToCharMap);
		void setDP(uint8_t ui8_DisplayIndex, boolean bSet);

		void showDash();
		void showErr();

	protected:
		boolean m_b_LeadingZero;
    boolean m_bRightBound;
};

#endif
