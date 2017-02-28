// Copyright 2016 DSP Synthesizers Sweden. 
// 
// Author: Jan Ostman 
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version. 
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 


//--------- Ringbuf parameters ----------
uint8_t Ringbuffer[256];
uint8_t RingWrite=0;
uint8_t RingRead=0;
volatile uint8_t RingCount=0;
//-----------------------------------------


const uint8_t drum1[] PROGMEM =
{
  128,129,129,129,129,130,130,131,132,132,137,132,126,100,140,249,79,164,144,144,113,142,125,114,123,73,116,66,83,55,79,59,67,60,70,63,72,66,86,94,101,116,107,136,182,141,158,223,186,199,207,227,237,239,239,240,235,223,198,196,188,183,171,168,156,150,141,132,124,120,105,100,89,75,71,70,56,46,44,33,29,29,28,25,22,23,22,23,25,28,32,39,48,51,57,66,
  71,79,87,97,105,112,119,128,134,141,151,156,168,168,188,181,201,196,227,221,238,235,234,238,238,235,234,229,224,212,207,193,188,184,177,170,165,158,151,142,137,128,121,113,108,102,95,88,84,77,75,67,65,57,56,49,49,44,43,42,34,36,32,35,37,36,33,40,38,43,42,48,48,52,54,55,63,69,70,74,80,84,89,92,98,104,107,111,115,119,120,129,123,134,137,140,144,144,147,150,
  154,152,155,158,157,162,158,159,162,161,160,160,162,162,161,162,163,164,163,168,167,169,169,173,171,174,171,176,176,177,175,179,176,175,174,173,172,172,166,166,162,158,156,152,148,143,140,134,131,125,121,117,113,108,104,100,95,91,87,82,80,74,72,67,64,60,55,54,51,49,46,42,44,43,43,41,42,43,45,46,48,49,51,55,58,61,66,72,76,81,86,92,100,104,112,116,123,128,135,140,
  145,150,155,160,163,168,171,176,178,180,182,185,187,188,189,190,188,190,189,188,186,188,183,184,180,179,175,174,169,168,165,160,158,155,151,148,145,142,138,135,132,129,126,124,121,118,116,114,112,110,109,106,106,104,104,103,103,102,102,102,102,103,103,104,105,105,106,106,108,109,111,112,113,114,115,116,118,119,121,121,123,124,125,126,127,128,129,130,131,132,132,133,133,134,134,134,135,134,134,134,
  134,134,133,133,133,132,132,132,131,131,130,130,130,129,129,128,128,128,128,128,127,127,127,127,127,127,127,127,127,127,127,128,128,128,128,129,129,130,130,130,131,131,132,132,133,133,134,134,135,135,135,136,136,136,136,136,136,136,136,136,136,136,136,136,136,135,135,135,135,134,134,134,134,134,134,133,133,133,133,133,133,133,133,133,133,133,132,132,132,132,132,132,132,132,131,131,131,131,131,131,
  130,130,130,130,130,130,129,129,129,129,129,129,129,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,129,129,129,129,129,129,130,130,130,130,131,131,131,131,131,132,132,132,132,132,132,133,133,133,133,133,133,133,133,133,134,134,134,134,134,134,134,134,134,134,134,134,134,134,134,134,134,134,134,134,133,133,133,133,133,133,133,133,133,132,132,132,132,
  132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
  131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,132,132,132,132,132,132,132,132,132,132,132,132,132,132,132,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,130,130,130,130,130,130,130,130,130,131,131,131,131,131,130,130,130,130,130,130,130,130,130,130,130,130,130,130,129,
  129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,128,128,128,128,128,128,128,128,128,128,128,128,128
};

const uint8_t drum2[] PROGMEM =
{
  128,164,168,168,163,142,145,75,24,39,25,39,51,76,78,90,103,129,149,163,194,207,213,177,181,178,194,185,231,220,227,225,210,161,150,151,133,119,110,87,75,62,49,22,20,10,35,18,16,21,19,21,31,60,66,73,87,108,117,134,155,169,234,237,255,242,238,243,240,231,222,175,182,163,147,122,104,96,87,78,58,37,13,26,20,27,18,25,18,31,44,88,111,109,114,152,
  194,171,150,194,239,213,238,232,216,225,236,208,151,164,129,125,97,100,81,81,73,48,19,44,57,22,40,22,41,28,59,49,85,72,93,120,129,134,128,151,169,184,165,170,172,175,172,157,148,142,145,175,141,137,140,122,119,112,96,99,73,50,71,29,44,41,30,28,58,60,83,95,113,103,97,110,118,131,154,160,151,150,177,163,186,187,196,208,233,195,218,164,144,144,130,128,114,89,68,93,
  68,44,73,41,30,61,47,74,95,80,108,90,134,112,139,126,137,165,161,157,176,174,173,157,150,179,171,145,158,143,149,143,104,122,106,96,89,99,108,97,75,65,103,106,93,93,125,104,114,112,105,116,132,126,162,129,106,129,137,140,130,145,128,151,134,145,175,166,174,168,169,153,145,154,148,128,130,110,111,96,100,76,89,77,75,81,89,82,101,92,113,101,125,137,144,137,160,153,147,153,
  149,149,197,182,167,159,176,161,146,147,147,131,130,128,100,114,117,100,109,82,104,90,76,73,101,97,96,107,116,128,127,148,149,141,164,155,164,152,157,156,154,148,148,152,129,154,142,136,147,132,126,140,128,131,126,118,120,116,105,120,111,112,115,109,118,111,111,106,129,112,129,127,123,125,132,127,129,140,139,131,140,136,154,142,135,146,150,151,148,152,147,138,142,144,134,129,131,128,113,113,
  117,111,108,104,98,106,99,109,101,114,123,118,142,133,133,138,144,147,137,142,154,142,142,159,138,145,150,148,139,136,133,128,118,122,125,122,120,112,115,111,115,112,99,114,109,126,125,120,125,140,136,136,144,135,133,142,146,146,146,134,145,132,133,134,131,131,127,134,127,129,124,136,132,128,140,132,131,131,128,133,129,124,127,121,130,126,125,122,124,127,128,128,124,127,130,130,129,128,127,136,
  131,139,143,142,141,138,141,145,141,141,141,139,140,131,131,126,128,123,121,125,118,117,120,117,120,117,114,121,121,130,121,132,128,131,132,140,136,140,138,145,137,138,139,138,135,134,132,128,129,125,125,125,126,123,126,120,124,120,131,124,124,128,131,132,129,130,135,132,144,136,137,134,141,136,132,133,136,136,130,128,127,129,123,132,128,126,131,129,129,128,132,130,132,134,133,131,134,132,130,134,
  134,131,133,130,134,126,129,128,132,128,129,128,122,127,129,122,128,131,129,132,134,134,135,135,136,138,137,138,139,142,139,134,135,134,133,131,133,127,124,127,126,123,123,126,120,123,127,125,127,128,129,130,133,132,139,134,137,135,134,134,135,130,132,131,131,129,128,130,126,127,124,127,126,125,128,129,127,126,128,129,131,131,134,132,133,131,134,134,135,130,134,131,131,134,131,131,130,128,129,128,
  127,128,124,126,127,128,128,129,128,132,131,133,135,132,134,138,137,136,134,134,134,134,132,130,130,131,129,129,127,127,128,128,126,129,130,128,128,129,131,131,130,131,133,133,133,134,134,131,133,132,130,130,129,130,129,128,127,128,126,126,125,127,127,126,127,128,129,129,131,130,131,131,132,131,132,131,131,130,130,130,129,129,129,128,126,128,128,127,127,127,127,127,127,128,128,128,128,128,128,129,
  129,129,130,130,130,130,130,130,130,129,129,128,128,128,128,128,128,128,128,127,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128
};

const uint8_t drum3[] PROGMEM =
{
  128,187,132,118,145,112,103,172,95,106,37,137,114,109,133,148,103,148,107,135,58,154,114,117,92,174,181,119,152,175,122,130,146,108,96,121,110,107,66,74,111,61,110,105,134,85,121,141,125,136,203,130,141,124,159,165,171,149,110,153,99,108,104,184,115,28,87,94,27,43,115,130,148,94,137,148,97,134,150,127,83,180,107,200,115,112,87,159,97,144,44,148,84,128,70,136,
  108,114,134,80,155,110,103,150,127,136,70,101,72,140,146,109,87,110,24,142,109,99,160,152,98,186,71,160,116,166,19,211,113,147,83,181,115,130,84,128,101,127,131,116,98,139,67,143,148,142,140,125,103,166,123,114,91,116,137,146,64,104,81,198,93,134,110,93,126,154,128,149,108,116,130,129,116,116,123,127,173,109,99,114,113,120,146,120,82,144,35,120,105,120,124,137,74,146,98,121,
  122,98,154,120,99,114,145,77,125,152,58,72,106,64,145,178,112,89,84,113,131,99,136,143,120,194,118,132,130,127,118,82,77,134,41,138,96,170,20,130,110,150,105,149,164,116,108,143,112,107,88,111,98,108,81,152,85,107,126,171,67,126,161,164,138,121,79,156,102,188,37,145,71,172,67,170,58,123,99,168,124,173,102,170,115,138,96,101,84,121,90,87,99,132,82,142,85,184,32,146,
  117,195,109,119,160,113,126,113,82,125,141,186,122,134,82,82,185,115,110,157,127,130,127,155,160,133,105,111,126,89,46,161,104,146,108,108,102,161,100,160,125,152,199,126,94,128,103,184,86,79,116,65,135,142,103,147,103,140,112,108,111,144,110,116,132,124,93,112,115,104,130,114,103,172,171,135,98,133,92,137,90,121,113,137,197,110,108,104,117,128,128,85,111,149,104,122,95,145,172,130,
  44,145,110,186,140,154,127,44,112,124,80,142,119,114,66,122,140,209,89,123,132,140,86,151,98,128,138,113,103,140,155,118,104,131,183,94,136,128,109,116,75,136,161,102,82,128,114,133,134,169,75,141,145,110,90,199,86,207,92,107,125,112,167,129,115,75,86,149,142,124,106,99,86,126,169,119,110,135,155,130,102,143,127,123,106,65,116,79,143,151,186,90,142,139,73,157,160,138,117,122,
  91,155,49,163,78,87,118,122,182,127,109,112,158,79,155,135,122,157,162,118,131,138,136,143,118,143,100,76,153,136,128,64,120,86,161,122,164,212,114,117,154,107,153,120,149,82,105,119,129,107,155,86,131,86,130,108,158,147,116,98,107,99,109,145,135,108,132,89,109,143,97,109,122,139,130,157,127,162,152,122,94,135,166,133,153,131,95,124,132,120,103,128,132,126,217,95,134,120,128,108,
  126,185,138,126,106,84,141,108,132,137,115,100,104,184,179,101,192,104,115,157,146,119,129,91,131,120,105,159,140,79,139,78,136,131,164,143,112,121,150,138,134,151,147,88,121,124,140,106,92,133,119,87,188,114,154,104,125,147,129,139,157,146,137,104,121,136,112,120,135,71,115,112,154,111,118,87,171,94,200,127,137,81,154,156,92,117,144,133,147,90,175,99,116,161,152,79,97,120,146,126,
  152,132,133,127,189,158,162,95,200,117,120,92,141,83,133,118,193,66,131,164,129,105,158,126,129,109,111,140,106,154,132,131,88,117,148,143,108,147,142,120,98,141,117,92,119,126,194,125,152,133,151,94,122,106,141,116,137,111,74,119,167,133,129,160,135,122,121,209,82,103,87,124,150,97,161,144,94,141,175,107,141,148,163,81,109,138,141,91,139,119,156,113,133,155,114,86,137,100,151,105,
  181,112,103,164,141,107,132,70,147,88,143,108,136,92,129,101,130,112,173,154,131,106,148,134,119,95,111,55,157,102,151,119,160,138,162,91,179,148,144,110,155,106,124,98,208,92,133,67,160,114,114,134,129,62,153,127,144,146,148,150,158,109,130,146,146,156,122,112,71,88,133,112,130,138,116,118,158,158,129,96,179,110,122,111,120,131,143,57,161,125,109,158,206,128,118,111,141,134,146,134,
  138,94,139,155,163,154,115,127,68,100,130,160,115,143,167,127,113,175,169,116,124,128,139,111,116,134,132,74,125,131,115,114,191,123,148,139,148,122,151,142,132,94,117,109,154,82,135,117,110,135,189,155,121,116,144,104,130,113,148,138,153,145,161,105,115,120,146,78,137,129,91,107,146,127,102,136,146,131,127,118,161,150,117,121,115,91,114,181,145,96,175,157,121,127,144,110,138,114,137,151,
  137,138,153,97,91,132,162,136,129,128,135,57,145,116,86,119,113,146,157,128,141,178,93,134,152,118,132,128,129,120,92,139,120,133,151,126,114,114,158,148,118,119,159,148,73,160,162,149,104,149,172,169,95,147,126,90,120,139,147,116,115,160,134,131,145,172,117,147,173,97,129,121,133,143,50,151,164,125,131,165,113,133,120,145,122,97,147,153,108,146,123,146,149,155,213,145,107,184,162,123,
  145,151,129,97,100,121,152,83,167,142,95,125,129,165,137,127,171,147,101,169,152,141,115,152,136,125,134,180,131,90,135,86,133,118,137,148,137,151,150,123,135,162,150,111,156,148,131,116,113,122,103,126,149,140,118,137,147,138,132,142,138,139,101,143,143,128,136,94,116,130,107,133,141,111,111,117,139,147,112,143,114,132,148,163,142,178,126,155,124,102,125,132,118,130,149,117,129,153,105,131,
  84,159,162,133,144,134,132,131,105,144,106,158,128,139,122,153,159,141,127,112,101,143,120,151,118,144,153,132,144,132,154,149,108,154,105,137,132,146,97,93,124,128,134,151,124,156,125,143,127,124,141,143,149,138,125,140,126,115,127,141,112,157,142,136,157,104,139,146,108,151,137,137,144,121,144,158,108,142,109,120,121,165,133,118,125,127,144,127,137,140,131,153,127,139,92,119,127,124,120,129,
  115,134,144,141,130,134,126,136,116,134,162,117,152,113,126,137,118,137,129,127,129,125,115,132,131,130,126,152,131,154,121,161,136,126,128,143,119,131,125,113,121,118,113,130,113,137,145,117,161,129,148,135,127,124,131,130,131,127,130,124,119,137,125,130,126,127,123,140,113,137,137,125,129,140,120,133,139,128,122,124,127,125,126,121,129,118,126,131,134,137,126,140,133,129,131,131,126,128,129,126,
  117,139,121,127,123,133,128,130,132,134,127,136,126,134,130,126,127,128,126,130,135,125,126,130,121,133,125,130,128,131,129,128,130,132,128,132,127,130,125,130,127,128,126,127,126,130,127,129,126,130,129,130,128,128,131,128,128,128,128,128,127,128,128,128,128,128
};

const uint8_t drum4[] PROGMEM =
{
  128,142,62,59,95,60,79,91,166,123,135,161,238,246,165,189,136,154,183,89,63,96,94,45,83,59,59,78,74,84,99,95,111,108,139,130,130,176,182,186,218,222,249,158,162,214,237,214,223,201,174,178,106,156,131,108,84,119,30,21,38,17,40,44,98,40,49,57,71,86,93,110,103,145,157,162,143,176,161,182,200,214,198,206,178,159,169,145,135,116,129,88,101,81,54,15,
  26,18,10,29,41,30,47,62,64,87,88,94,105,120,122,162,162,243,186,195,190,209,196,191,172,173,165,159,146,130,111,88,96,122,76,34,39,49,26,28,31,36,44,58,66,83,94,101,125,135,130,161,183,186,190,192,219,197,190,185,189,185,177,166,160,145,141,108,107,95,73,59,23,42,50,38,41,42,56,65,71,87,102,101,118,136,146,142,166,177,188,220,242,212,232,233,194,187,184,176,
  169,156,147,138,120,111,58,68,59,46,37,31,42,38,49,53,64,71,83,96,97,114,130,136,143,159,166,181,182,198,221,218,228,215,190,186,179,173,159,150,141,124,108,92,90,65,54,48,46,39,41,43,52,59,68,73,85,94,108,112,125,134,144,148,163,183,184,183,189,214,194,200,199,186,178,172,161,153,139,124,123,104,76,76,67,47,44,45,45,44,50,59,64,74,82,90,100,106,117,130,
  143,148,157,168,184,186,183,187,209,191,188,184,183,173,168,156,144,133,122,113,87,71,74,66,59,50,53,52,55,58,67,72,83,90,98,110,116,122,133,148,149,166,171,165,181,182,183,196,190,186,185,184,179,171,158,143,141,124,110,100,91,79,72,77,67,59,59,57,62,68,73,83,88,100,105,114,124,131,136,143,151,162,171,178,181,186,189,197,188,184,187,182,173,163,155,150,140,126,111,106,
  93,89,75,71,64,65,63,66,66,73,80,83,93,103,112,118,123,130,142,147,155,160,168,178,184,181,194,190,192,182,182,178,174,165,160,148,144,129,115,105,102,94,88,76,81,71,67,68,70,71,78,84,92,100,108,116,124,135,138,145,155,161,165,168,176,176,187,188,185,185,179,180,178,170,160,155,147,137,132,119,108,100,95,85,75,75,71,79,72,73,77,84,91,96,104,112,116,124,132,140,
  147,152,156,161,172,170,175,178,179,181,181,181,179,175,170,158,156,149,143,134,123,113,104,100,93,81,85,78,76,77,78,80,86,89,95,100,108,116,125,128,138,143,145,152,154,160,166,171,172,176,180,174,176,176,170,168,165,156,153,144,138,130,123,113,111,101,96,90,87,88,83,82,83,83,85,90,98,104,110,116,122,127,134,140,146,148,153,162,164,165,171,168,171,173,172,171,170,169,165,161,
  152,147,137,136,128,119,111,103,101,96,91,88,86,86,86,85,88,91,97,102,107,113,119,123,130,136,140,148,152,155,157,161,166,167,168,168,168,169,167,163,164,160,157,151,144,139,133,126,122,116,104,100,101,92,93,89,88,87,84,88,92,97,103,108,112,117,122,132,134,136,144,148,153,155,159,160,162,166,166,165,164,164,163,160,158,153,150,145,139,134,127,122,118,114,107,108,104,97,98,96,
  95,97,96,98,102,104,109,112,118,120,125,129,135,139,142,144,147,151,152,153,152,156,158,157,158,156,154,152,152,149,145,140,136,134,130,124,121,118,112,109,105,104,105,105,102,103,104,104,107,108,111,114,119,123,125,129,134,136,139,141,142,144,145,148,149,148,148,149,148,147,147,148,146,144,142,140,137,133,130,128,124,122,119,116,112,111,111,110,110,109,109,109,112,113,114,117,120,123,125,127,
  129,131,134,134,139,139,140,141,142,143,143,144,143,142,142,142,141,141,140,139,136,134,132,129,127,125,122,120,118,118,116,116,116,116,117,116,116,116,117,118,120,122,124,125,126,128,130,131,133,134,136,136,136,138,137,138,137,138,138,137,137,137,136,136,135,134,132,131,130,128,126,126,125,124,123,122,121,122,120,121,121,121,121,122,122,123,124,125,126,127,128,129,129,130,131,131,131,132,132,132,
  133,133,133,133,133,133,132,132,132,132,131,131,130,129,129,128,128,127,126,126,126,126,125,125,125,125,125,125,125,125,125,126,126,127,127,127,128,128,128,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,129,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128
};

const uint8_t drum5[] PROGMEM =
{
  128,122,164,121,123,49,34,101,250,246,152,60,49,111,58,167,243,126,103,68,138,107,113,121,135,96,147,138,114,110,115,90,125,85,53,91,165,246,143,91,89,70,97,54,129,241,102,103,167,113,90,131,147,149,117,81,111,112,35,94,196,103,158,168,112,44,120,56,138,226,108,107,61,99,147,82,54,129,13,101,250,184,82,140,81,179,56,81,80,147,198,98,141,18,46,56,
  189,254,168,36,66,79,53,144,253,192,89,10,140,167,53,77,163,202,164,73,29,98,165,119,18,91,84,89,98,209,192,61,73,25,142,237,166,219,135,97,74,22,152,180,84,79,18,108,145,178,100,41,146,158,198,133,92,93,131,73,58,131,142,158,70,82,91,188,156,136,81,54,175,172,64,165,89,15,71,219,174,86,127,159,67,84,121,151,250,145,96,44,75,132,128,74,133,184,107,138,
  209,148,138,62,135,120,24,69,192,204,112,29,122,86,128,134,172,133,145,122,85,108,151,111,72,82,88,200,163,106,52,120,196,196,130,46,103,187,156,56,110,116,180,116,99,118,142,140,110,102,162,175,128,62,104,183,143,61,154,175,142,51,160,151,123,71,118,126,176,110,144,170,153,149,127,49,53,165,172,102,158,159,118,115,98,140,88,94,124,143,109,231,146,52,104,157,172,153,100,89,
  127,143,147,110,153,130,87,127,107,139,182,117,126,118,126,122,114,132,128,121,131,147,121,123,115,170,121,78,130,117,122,128,158,132,113,142,135,154,122,101,102,120,101,99,154,178,147,108,125,125,142,140,109,122,137,137,132,93,117,163,135,124,93,126,145,162,106,123,100,146,138,123,134,155,133,130,115,139,135,130,98,138,152,102,120,129,126,153,127,120,125,136,133,136,138,147,132,138,134,124,
  100,123,135,137,125,137,146,149,138,122,115,116,125,136,136,147,121,126,136,143,144,131,110,132,120,130,142,170,127,112,114,120,136,143,136,140,136,110,121,152,141,149,115,115,129,137,144,156,124,113,132,134,146,127,124,131,127,136,135,133,137,123,123,146,144,142,129,117,127,119,138,150,129,124,131,126,129,150,129,128,121,116,127,146,145,132,129,134,149,136,130,123,130,132,121,126,142,137,130,128,
  133,140,127,124,126,125,136,136,124,123,139,138,137,129,139,131,112,124,138,132,129,125,137,139,125,125,135,122,126,135,135,132,132,132,128,134,133,126,127,129,127,129,131,132,122,126,133,133,125,124,135,136,133,129,126,131,130,130,130,128,129,130,130,130,128,128,131,130,129,129,128,128,127,128,129,131,130,128,129,128,130,129,129,130,129,129,127,129,129,128,127,128,128,129,128,128,129,129,129,129,
  129,128,128,128,128,129,128,128,128,128,128,128,128,128
};

const uint8_t drum6[] PROGMEM =
{
  128,149,117,85,141,185,112,43,133,218,116,10,125,251,128,2,109,237,143,15,101,223,148,25,94,211,163,31,88,205,164,53,74,198,177,55,71,183,183,74,62,167,191,93,48,164,193,94,63,145,195,113,53,137,199,117,59,126,188,139,59,106,196,148,63,97,186,166,75,75,180,184,83,62,163,193,96,55,156,192,103,67,138,191,119,66,133,188,123,68,124,185,131,71,118,181,
  142,72,111,180,142,80,110,170,145,88,107,163,149,95,103,155,154,98,102,153,150,106,106,144,145,113,112,135,145,118,111,137,143,120,114,134,139,122,115,132,140,124,116,129,138,127,118,129,136,127,121,125,135,131,121,126,133,130,124,126,132,129,127,128,130,129
};

const uint8_t drum7[] PROGMEM =
{
  128,129,112,108,86,114,157,124,159,206,159,121,96,93,46,41,167,172,164,178,227,178,165,116,38,8,79,124,133,148,186,255,230,163,96,39,8,18,59,85,125,241,248,243,201,128,83,4,6,38,52,92,172,243,247,229,165,76,13,8,7,29,66,142,238,247,242,194,123,48,6,11,9,33,103,164,247,244,238,160,92,49,13,9,17,58,134,216,246,241,194,139,73,35,12,12,
  29,91,163,242,237,227,162,122,79,16,13,17,67,126,175,230,236,183,157,104,72,21,19,41,95,148,178,219,212,169,150,108,52,34,40,80,110,146,172,182,185,166,127,97,60,52,71,93,127,140,166,174,176,153,117,87,76,75,90,99,125,142,159,175,154,139,114,97,88,92,96,107,118,145,156,160,148,127,118,110,104,94,97,108,126,143,153,144,143,131,126,116,104,102,98,114,129,136,143,141,
  137,140,129,120,103,103,107,116,127,129,136,142,143,140,130,118,109,105,115,116,123,128,134,144,146,139,127,116,115,113,112,116,117,130,138,145,142,135,128,120,117,115,111,114,121,131,143,142,141,130,129,125,118,113,110,116,127,134,140,140,137,136,128,125,114,112,112,120,129,136,138,139,137,136,130,120,115,111,118,123,131,134,137,140,140,134,128,117,115,116,119,126,128,136,137,141,139,131,124,118,
  116,120,121,126,129,135,141,139,137,128,123,121,121,121,123,125,132,135,138,136,131,128,125,124,122,123,123,127,132,135,134,134,129,129,127,125,124,123,126,129,131,132,132,131,130,128,128,125,125,125,127,129,130,130,130,130,130,128,127,126,126,127,128,129,129,129,130,130,129,128,127,127,127,128,128,128,129,129,129,129,129,128,128,128,128,128,128,128,128,129,128,128,128,128,128,128,128,128
};

const uint8_t drum8[] PROGMEM =
{
  128,128,129,129,129,128,128,128,129,128,128,128,128,127,128,127,130,129,129,130,128,128,128,127,129,129,129,123,130,127,127,126,131,128,127,128,130,125,129,127,125,131,124,126,126,128,124,124,128,125,129,133,130,121,130,122,135,126,120,127,123,117,137,129,119,138,122,125,135,129,142,113,140,121,112,148,107,142,135,108,151,105,137,124,114,141,142,114,130,138,113,126,127,150,118,127,
  110,143,115,137,124,130,122,126,121,120,124,123,120,129,117,127,133,138,130,130,104,133,135,119,148,129,119,142,126,126,128,143,117,132,119,128,138,128,122,134,126,138,142,124,156,106,112,150,132,118,189,101,118,127,106,150,117,118,131,113,140,128,131,115,103,171,103,146,116,148,104,111,124,163,75,127,144,148,124,118,101,170,124,143,133,133,129,128,125,143,152,116,98,153,140,100,115,111,160,93,
  111,157,157,138,168,97,158,113,116,157,143,184,99,132,183,70,155,133,118,111,109,100,88,111,134,125,163,79,134,112,60,174,128,123,144,116,166,114,147,132,31,144,57,120,166,121,114,127,77,171,118,48,191,62,147,143,159,105,134,132,208,84,90,221,88,157,156,67,99,179,83,177,139,150,107,103,181,78,122,162,106,214,135,122,88,123,179,79,143,178,55,169,115,132,159,66,133,224,88,175,
  95,162,124,57,177,120,125,151,140,70,224,95,246,98,60,130,110,88,150,102,118,69,134,118,108,152,106,70,91,144,106,213,138,88,89,150,170,179,85,144,96,90,106,124,177,84,168,110,145,148,69,183,75,126,137,127,113,90,78,174,73,92,193,87,131,161,97,146,158,124,139,157,86,158,120,93,80,105,104,152,81,177,41,164,178,143,91,128,143,111,56,126,137,106,114,123,132,143,107,152,
  129,159,137,88,79,114,177,61,168,116,128,76,123,149,171,83,160,160,115,140,101,113,144,154,147,134,97,135,120,134,113,121,112,181,77,159,124,155,73,168,155,137,112,135,95,152,158,100,143,132,123,128,150,117,116,127,141,102,113,158,121,121,145,104,100,112,138,123,133,132,120,125,126,127,118,121,119,135,93,129,120,128,133,127,118,127,128,131,131,133,116,127,140,120,130,122,132,122,128,128,
  128,127,123,130,125,124,122,136,121,130,130,127,120,123,133,126,122,125,132,123,130,125,129,123,133,125,126,127,130,125,127,129,127,127,124,126,126,129,125,127,121,135,128,123,130,123,128,126,125,125,128,121,130,123,123,126,125,128,123,126,124,127,122,129,123,125,129,125,126,124,129,126,128,126,130,123,128,127,127,128,127,129,127,127,128,127,126,130,125,128,127,129,125,128,126,127,128,129,127,127,
  129,125,129,128,128,126,127,127,127,127,127,126,128,126,125,127,125,128,126,125,127,128,125,126,127,127,125,127,126,126,127,126,126,127,127,126,126,126,127,127,127,127,127,127,126,128,128,126,128,128,127,127,127,129,127,127,128,127,128,128,127,128,128,127,128,129,127,127,128,127,127,128,127,128,127,128,128,126,127,127,127,127,127,127,126,127,127,127,128,127,127,127,127,128,127,128,127,127,127,127,
  127,128,128,127,127,127,127,128,127,128,128,128,128,127,128,128,127,128,128,127,127,128,128,127,128,127,128,128,128,128,127,128,128,128,127,127,128,127,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128
};



void setup() {
  OSCCAL=254;
    

  // Set up Timer/Counter0 for PWM output
  TCCR0A = ((1<<COM0B1)|(1<<COM0A1)|(1<<WGM01)|(1<<WGM00));    
  TCCR0B = ((0<<CS01)|(1<<CS00));

  //A0=Pitch 3.2KHz - 20KHz
  //A1=Accent 6dB - 48dB

  pinMode(10, INPUT_PULLUP);     //Drum1 trig
  pinMode(9, INPUT_PULLUP);      //Drum2 trig
  pinMode(8, OUTPUT);            // Enable PWM output pin
  pinMode(7, INPUT_PULLUP);      //Drum3 trig
  pinMode(6, INPUT_PULLUP);      //Drum4 trig
  pinMode(5, INPUT_PULLUP);      //Drum5 trig
  pinMode(4, INPUT_PULLUP);      //Drum6 trig
  pinMode(3, INPUT_PULLUP);      //Drum7 trig
  pinMode(2, INPUT_PULLUP);      //Drum8 trig

  // Set up Timer 1 to send a sample every interrupt.
  cli();
  // Set CTC mode
  // Have to set OCR1A *after*, otherwise it gets reset to 0!
  TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
  TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));    
  // No prescaler
  TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  // Set the compare register (OCR1A).
  // OCR1A is a 16-bit register, so we have to do this with
  // interrupts disabled to be safe.
  //OCR1A = F_CPU / SAMPLE_RATE; 
  // Enable interrupt when TCNT1 == OCR1A
  TIMSK1 |= _BV(OCIE1A);   
  sei();
  OCR1A = 800; //20KHz Samplefreq

  // Disable Timer0 interrupts
  TIMSK0 = 0;


  OCR0A=128;


  uint16_t dummy=analogRead(0);
  
  
}

void loop() {
  uint16_t samplecnt1,samplecnt2,samplecnt3,samplecnt4,samplecnt5,samplecnt6,samplecnt7,samplecnt8;
  uint16_t samplepnt1,samplepnt2,samplepnt3,samplepnt4,samplepnt5,samplepnt6,samplepnt7,samplepnt8;
  uint8_t dread1,dread2,dread3,dread4,dread5,dread6,dread7,dread8;
  uint8_t odread1,odread2,odread3,odread4,odread5,odread6,odread7,odread8;

  uint8_t Accent;
  uint8_t MUX;

  sbi(ADCSRA, ADSC); //start next conversation
  while(1) {
    if (RingCount<32) {  //if space in ringbuffer
      int16_t total=0;
      if (samplecnt1) {
        total+=(pgm_read_byte_near(drum1 + samplepnt1++)-128);
        samplecnt1--;
      }
      if (samplecnt2) {
        total+=(pgm_read_byte_near(drum2 + samplepnt2++)-128);
        samplecnt2--;
      }
      if (samplecnt3) {
        total+=(pgm_read_byte_near(drum3 + samplepnt3++)-128);
        samplecnt3--;
      }
      if (samplecnt4) {
        total+=(pgm_read_byte_near(drum4 + samplepnt4++)-128);
        samplecnt4--;
      }
      if (samplecnt5) {
        total+=(pgm_read_byte_near(drum5 + samplepnt5++)-128);
        samplecnt5--;
      }
      if (samplecnt6) {
        total+=(pgm_read_byte_near(drum6 + samplepnt6++)-128);
        samplecnt6--;
      }
      if (samplecnt7) {
        total+=(pgm_read_byte_near(drum7 + samplepnt7++)-128);
        samplecnt7--;
      }
      if (samplecnt8) {
        total+=(pgm_read_byte_near(drum8 + samplepnt8++)-128);
        samplecnt8--;
      }     
      total>>=1;
      total>>=Accent;
      total+=128;  
      if (total>255) total=255;
      cli();
      Ringbuffer[RingWrite]=total;
      RingWrite++;
      RingCount++;
      sei();
    }
      dread1=digitalRead(10);
      dread2=digitalRead(9);
      dread3=digitalRead(7);
      dread4=digitalRead(6);
      dread5=digitalRead(5);
      dread6=digitalRead(4);
      dread7=digitalRead(3);
      dread8=digitalRead(2);

      if (dread1!=odread1) {
        odread1=dread1;
        if (odread1) {
          samplepnt1=0;
          samplecnt1=sizeof(drum1);
        }
      }
      if (dread2!=odread2) {
        odread2=dread2;
        if (odread2) {
          samplepnt2=0;
          samplecnt2=sizeof(drum2);
        }
      }
      if (dread3!=odread3) {
        odread3=dread3;
        if (odread3) {
          samplepnt3=0;
          samplecnt3=sizeof(drum3);
        }
      }
      if (dread4!=odread4) {
        odread4=dread4;
        if (odread4) {
          samplepnt4=0;
          samplecnt4=sizeof(drum4);
        }
      }
      if (dread5!=odread5) {
        odread5=dread5;
        if (odread5) {
          samplepnt5=0;
          samplecnt5=sizeof(drum5);
        }
      }
      if (dread6!=odread6) {
        odread6=dread6;
        if (odread6) {
          samplepnt6=0;
          samplecnt6=sizeof(drum6);
        }
      }
      if (dread7!=odread7) {
        odread7=dread7;
        if (odread7) {
          samplepnt7=0;
          samplecnt7=sizeof(drum7);
        }
      }
      if (dread8!=odread8) {
        odread8=dread8;
        if (odread8) {
          samplepnt8=0;
          samplecnt8=sizeof(drum8);
        }
      }
      
      if (!(ADCSRA & 64)) {
        if (MUX==0) OCR1A = 800+((1023-(ADCL+(ADCH<<8)))<<2);
        if (MUX==1) Accent = 7-((ADCL+(ADCH<<8))>>7);
        MUX++;
        MUX&=1;
        ADMUX = MUX; //Select MUX
        sbi(ADCSRA, ADSC); //start next conversation
      }

  }
}

ISR(TIM1_COMPA_vect) {
 //-------------------  Ringbuffer handler -------------------------
    
    if (RingCount) {                            //If entry in FIFO..
      OCR0A = Ringbuffer[(RingRead++)];          //Output LSB of 16-bit DAC
      RingCount--;
    }
    
    //-----------------------------------------------------------------
}
