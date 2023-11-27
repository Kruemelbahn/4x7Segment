volatile boolean state;
boolean state13;

#define LEDPIN 6

// toggles every 2ms
ISR(TIMER2_COMPA_vect)
{
  state = !state;
  digitalWrite(LEDPIN, state);
}

void setup()
{
  pinMode(LEDPIN, OUTPUT);
  pinMode(13, OUTPUT);
  state = true;
  state13 = true;

  cli();
  // set up Timer 2:
  TCCR2A = 0;                       // normal operation
  TCCR2B = bit(WGM22) | bit(CS20) | bit(CS22);  		// CTC, pre-scaling = 128
  OCR2A =  249 * clockCyclesPerMicrosecond() / 16;	// compare A register value (8-Bit!) (250 * clock speed / 128)
  TIMSK2 = bit(OCIE2A);             // interrupt on Compare A Match

  sei();  // set interrupts flag
}

void loop()
{
  // toggles prox. all 4.1µs
  state13 = !state13;
  digitalWrite(13, state13);
}
