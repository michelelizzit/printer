/**
 *   3D printer firmware with closed loop control - lizzit.it/printer
 *   Copyright (C) 2017  Michele Lizzit
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


//thermistor.ino

//Conversion from ADC units to degrees and vice versa using a conversion table


// Thermistor lookup table for RepRap Temperature Sensor Boards (http://make.rrrf.org/ts)
// Made with createTemperatureLookup.py (http://svn.reprap.org/trunk/reprap/firmware/Arduino/utilities/createTemperatureLookup.py)
// ./createTemperatureLookup.py --r0=100000 --t0=25 --r1=0 --r2=4700 --beta=3950 --max-adc=1023
// r0: 100000
// t0: 25
// r1: 0
// r2: 4700
// beta: 3950
// max adc: 1023
#define NUMTEMPS 20
short temptable[NUMTEMPS][2] = {
   {1, 938},
   {54, 267},
   {107, 217},
   {160, 191},
   {213, 172},
   {266, 158},
   {319, 147},
   {372, 137},
   {425, 127},
   {478, 119},
   {531, 111},
   {584, 103},
   {637, 96},
   {690, 88},
   {743, 80},
   {796, 72},
   {849, 62},
   {902, 50},
   {955, 35},
   {1008, 2}
};

void copyVals(short dest[2][2], short index) {
  dest[0][0] = temptable[index - 1][0];
  dest[0][1] = temptable[index - 1][1];
  dest[1][0] = temptable[index][0];
  dest[1][1] = temptable[index][1];
}

//Convert from ADC units to degrees
short ADCtoTemp(short adcVal) {
  short tempVals[2][2];
  
  copyVals(tempVals, NUMTEMPS - 1);
  for (int i = 0; i < NUMTEMPS; i++) {
    if (temptable[i][0] > adcVal) {
      if (i == 0) {
        copyVals(tempVals, 1);
      }
      else {
        copyVals(tempVals, i);
      }
      break;
    }
  }
  
  short res = adcVal;
  res -= tempVals[0][0];
  res *= (tempVals[0][1] - tempVals[1][1]);
  res /= (tempVals[0][0] - tempVals[1][0]);
  res += tempVals[1][1];
  return res;
}

//Convert from degrees to ADC units
short tempToADC(short tempVal) {
  short tempVals[2][2];
  
  copyVals(tempVals, NUMTEMPS - 1);
  for (int i = 0; i < NUMTEMPS; i++) {
    if (temptable[i][1] < tempVal) {
      if (i == 0) {
        copyVals(tempVals, 1);
      }
      else {
        copyVals(tempVals, i);
      }
      break;
    }
  }
  
  short res = tempVal;
  res -= tempVals[1][1];
  res *= (tempVals[0][0] - tempVals[1][0]);
  res /= (tempVals[0][1] - tempVals[1][1]);
  res += tempVals[0][0];
  //res = map(tempVal, tempVals[1][1], tempVals[0][1], tempVals[1][0], tempVals[0][0]);
  return res;
}

