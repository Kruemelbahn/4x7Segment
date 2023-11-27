#include "4x7Segment.h"

SevenSegment sevenSeg = SevenSegment();

void setup()
{
}

void loop()
{
  sevenSeg.printInt(9876);
  delay(1000);
  sevenSeg.printInt(543);
  delay(1000);
  sevenSeg.printInt(21);
  delay(1000);
  sevenSeg.printInt(0);
  delay(1000);

  sevenSeg.printInt(-123);
  sevenSeg.setDP(2, true);  
  delay(1000);
  sevenSeg.printInt(-45);
  sevenSeg.setDP(2, false);  
  delay(1000);
  sevenSeg.printInt(-5);
  delay(1000);

  sevenSeg.print(F("HIGH"));
  delay(1000);
  sevenSeg.print("AFFE");
  delay(1000);
  sevenSeg.setDigit(0, 49);
  delay(1000);
  sevenSeg.showDash();
  delay(1000);
  sevenSeg.showErr();
  delay(1000);
}
