/*
  Hacker Nowful
*/
#ifndef blesdlib_h
#define blesdlib_h

#include <SPI.h>
#include <SD.h>
#include <string.h>
#include <HijelHID_BLEKeyboard.h>

extern HijelHID_BLEKeyboard keyboard;
extern File myFile;
extern boolean first;

void Line(String l);
void Press(String b);

void attack();

#endif
