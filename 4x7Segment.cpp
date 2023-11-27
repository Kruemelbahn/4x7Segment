/*
||
|| @file 4x7Segment.cpp
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
|| |           1         C0				MSD (most significant digit, left)
|| |           2         C1
|| |           3         C2
|| |           4         C3				LSD (last significant digit, right)
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

#include "4x7Segment.h"
#include "4x7SegmentChar.h"

#if not defined clockCyclesPerMicrosecond
	#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#endif

const uint8_t MAX_DIGITS = 4;

#define SegA  PD3
#define SegB  PD4
#define SegC  PD5
#define SegD  PB1
#define SegE  PB2
#define SegF  PB3
#define SegG  PB4
#define SegDP PB5

#define Digit1 PC0  // MSD
#define Digit2 PC1
#define Digit3 PC2
#define Digit4 PC3  // LSD

volatile uint8_t ui8_CurrentDigit;
volatile uint8_t ui8_CountOfDisplays;
volatile uint8_t digitToShow [MAX_DIGITS];
volatile uint8_t ui8_ShiftOffset;

volatile unsigned long timer2_2millis = 0;

/* eine leere ISR kostet: 50 Takte (3.125 µs gesamt bei 16 MHz) */
// Aufruf alle 2ms
ISR(TIMER2_COMPA_vect)
{
	if(ui8_CountOfDisplays)
	{
		// Digit ausschalten:
		PORTC |= ((1 << Digit1) | (1 << Digit2) | (1 << Digit3) | (1 << Digit4));

		// Anzeige ausgeben:
		PORTB &= ~((1 << SegD) | (1 << SegE) | (1 << SegF) | (1 << SegG) | (1 << SegDP));
		PORTB |= ((digitToShow[ui8_CurrentDigit] & 0xF8) >> 2);
		PORTD &= ~((1 << SegA) | (1 << SegB) | (1 << SegC));
		PORTD |= ((digitToShow[ui8_CurrentDigit] & 0x07) << 3);

		// Digit einschalten:
		PORTC &= ~(1 << (ui8_CurrentDigit + ui8_ShiftOffset));

		// nächstes Digit vorwählen:
		++ui8_CurrentDigit;
		if(ui8_CurrentDigit >= ui8_CountOfDisplays)
			ui8_CurrentDigit = 0;
	}

	// inkrement 2ms-Zähler
	++timer2_2millis;
}

// siehe '...\hardware\arduino\avr\cores\arduino\wiring.c'
unsigned long millis2()
{
	unsigned long m;
	uint8_t oldSREG(SREG);

	// disable interrupts while we read timer2_2millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer2_2millis)
	cli();
	m = timer2_2millis;
	SREG = oldSREG;	// restores isr-flags too

	return (m << 1); // increment is each 2ms -> so multiply with 2 to get 'true' millis
}

SevenSegment::SevenSegment()
: m_b_LeadingZero(false)
, m_bRightBound(false)
{
  ui8_ShiftOffset = 0;
	ui8_CountOfDisplays = 0;
	ui8_CurrentDigit = 0;
  clear();
  init();
}

void SevenSegment::init(uint8_t ui8_count, bool bLeadingZero, boolean bRightBound)
{
	cli();

	m_b_LeadingZero = bLeadingZero;
  m_bRightBound = bRightBound;

	if(ui8_count <= MAX_DIGITS)
	{
		ui8_CountOfDisplays = ui8_count;
	}

	if(ui8_CountOfDisplays)
	{
		// setup ports:
		DDRB |= (1 << SegD) | (1 << SegE) | (1 << SegF) | (1 << SegG) | (1 << SegDP);
		DDRC |= (1 << Digit1) | (1 << Digit2) | (1 << Digit3) | (1 << Digit4);
		DDRD |= (1 << SegA) | (1 << SegB) | (1 << SegC);

    // calculate Display shift offset
    if(m_bRightBound)
      ui8_ShiftOffset = MAX_DIGITS - ui8_CountOfDisplays;
	}

	// set up Timer 2:
	TCCR2A = 0;							// normal operation
	TCCR2B = bit(WGM22) | bit(CS20) | bit(CS22);			// CTC, pre-scaling = 128
	OCR2A  = 249 * clockCyclesPerMicrosecond() / 16;	// compare A register value (8-Bit!) (250 * clock speed / 128)
	TIMSK2 = bit(OCIE2A);	// interrupt on Compare A Match

	sei(); // set interrupts flag
}

void SevenSegment::clear()
{
	for(uint8_t i = 0; i < MAX_DIGITS; i++)
		digitToShow[i] = digitCodeMap[DIGIT_BLANK];
}

void SevenSegment::setDigit(int8_t ui8_DisplayIndex, uint8_t ui8_IndexToCharMap)
{
	if(ui8_DisplayIndex >= MAX_DIGITS)	// Index 0...MAX_DIGITS-1
		return;
	else if(ui8_IndexToCharMap > MAX_DIGITCODE)
		digitToShow[ui8_DisplayIndex] = digitCodeMap[DIGIT_BLANK]; // BLANK
	else if(ui8_IndexToCharMap == DIGIT_ZERO)
		digitToShow[ui8_DisplayIndex] = (m_b_LeadingZero || (ui8_DisplayIndex == (ui8_CountOfDisplays - 1)) ? digitCodeMap[DIGIT_ZERO] : digitCodeMap[DIGIT_BLANK]);	// '0' oder BLANK
	else 
		digitToShow[ui8_DisplayIndex] = digitCodeMap[ui8_IndexToCharMap];
}

void SevenSegment::setDP(uint8_t ui8_DisplayIndex, boolean bSet)
{
	if(ui8_DisplayIndex >= MAX_DIGITS)
		return;
	if(bSet)
		digitToShow[ui8_DisplayIndex] &= digitCodeMap[DIGIT_PERIOD];
	else
		digitToShow[ui8_DisplayIndex] |= digitCodeMap[8];
}

void SevenSegment::printInt(int16_t i_value)
{
	if((i_value < -999) || (i_value > 9999))
	{
		showErr();
		return;
	}
	
  boolean bMinus(false);
  if(i_value < 0)
  {
    bMinus = true;
    i_value = abs(i_value);
  }

  bool bTempLeadingZero(m_b_LeadingZero);
  bool bFirstNonZeroDigit(false);
	if(i_value >= 1000)
	{
		setDigit(ui8_CountOfDisplays - 4, i_value / 1000);
		i_value %= 1000;
    bFirstNonZeroDigit = true;
    m_b_LeadingZero = true;
	}
	else
  {
    if(!bMinus)
      setDigit(ui8_CountOfDisplays - 4, DIGIT_BLANK);
    else
    {
      if(i_value > 100)
      {
        setDigit(ui8_CountOfDisplays - 4, DIGIT_DASH);
        bMinus = false;
      }
      else
        setDigit(ui8_CountOfDisplays - 4, DIGIT_BLANK);
    }
  }
	
	if(i_value >= 100)
	{
		setDigit(ui8_CountOfDisplays - 3, i_value / 100);
		i_value %= 100;
    bFirstNonZeroDigit = true;
    m_b_LeadingZero = true;
	}
  else
  {
    if(!bMinus)
      setDigit(ui8_CountOfDisplays - 3, bFirstNonZeroDigit ? DIGIT_ZERO : DIGIT_BLANK);
    else
    {
      if(i_value > 10)
      {
        setDigit(ui8_CountOfDisplays - 3, DIGIT_DASH);
        bMinus = false;
      }
      else
        setDigit(ui8_CountOfDisplays - 3, bFirstNonZeroDigit ? DIGIT_ZERO : DIGIT_BLANK);
    }
  }
	
	if(i_value >= 10)
	{
		setDigit(ui8_CountOfDisplays - 2, i_value / 10);
		i_value %= 10;
	}
  else
    setDigit(ui8_CountOfDisplays - 2, bMinus ? DIGIT_DASH : (bFirstNonZeroDigit ? DIGIT_ZERO : DIGIT_BLANK));
	
	setDigit(ui8_CountOfDisplays - 1, i_value);

  m_b_LeadingZero = bTempLeadingZero; // restore flag
}

void SevenSegment::print(const __FlashStringHelper *p)
{
	PGM_P p1(reinterpret_cast<PGM_P>(p));

	for(uint8_t i = 0; i < ui8_CountOfDisplays; i++)
	{
		register uint8_t c = pgm_read_byte(p1++);
		if(c)
			printChar(i, c);
	}
}

void SevenSegment::print(const char* p)
{
	for(uint8_t i = 0; i < ui8_CountOfDisplays; i++, p++)
		if(*p)
			printChar(i, *p);
}

void SevenSegment::printChar(uint8_t ui8Pos, unsigned char ch)
{
	if((ch >= '0') && (ch <= '9'))
	{
		boolean b(m_b_LeadingZero);
		m_b_LeadingZero = true;
		setDigit(ui8Pos, ch - 48);
		m_b_LeadingZero = b;
	}
	else if((ch >= 'A') && (ch <= 'Z'))
		setDigit(ui8Pos, ch - 55);
	else if(ch == 'u')
		setDigit(ui8Pos, 47);
	else if((ch >= 'a') && (ch <= 'z'))
		setDigit(ui8Pos, ch - 55);
	else if(ch == ' ')
		setDigit(ui8Pos, DIGIT_BLANK);
	else if(ch == '-')
		setDigit(ui8Pos, DIGIT_DASH);
	else if(ch == '.')
		setDigit(ui8Pos, DIGIT_PERIOD);
	else if(ch == '/')
		setDigit(ui8Pos, 39);
	else if(ch == '=')
		setDigit(ui8Pos, 40);
	else if(ch == '?')
		setDigit(ui8Pos, 41);
	else if(ch == '[')
		setDigit(ui8Pos, 42);
	else if(ch == '\\')
		setDigit(ui8Pos, 43);
	else if(ch == ']')
		setDigit(ui8Pos, 44);
	else if(ch == '^')
		setDigit(ui8Pos, 45);
	else if(ch == '_')
		setDigit(ui8Pos, 46);
	else if(ch == '|')
		setDigit(ui8Pos, 48);
//else if(ch == '°')
//	setDigit(ui8Pos, 49);
	else
		setDigit(ui8Pos, DIGIT_BLANK);
}

void SevenSegment::showDash()
{
	for(uint8_t i = 0; i < MAX_DIGITS; i++)
		setDigit(i, DIGIT_DASH);
}

void SevenSegment::showErr()
{
  setDigit(0, 14);  //'E'
  setDigit(1, 27);  //'r'
  setDigit(2, 27);  //'r'
  setDigit(3, DIGIT_BLANK);
}
