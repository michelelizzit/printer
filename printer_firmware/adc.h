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


//adc.h

//set ADC channel to 6 (pin A6)
#define SET_ADC_THERMISTOR() ADMUX = B01000110;
//set ADC channel to 7 (pin A7)
#define SET_ADC_Z_ENDSTOP() ADMUX = B01100111;
//start a new ADC conversion
#define FORCE_START_CONVERSION() ADCSRA |= B11000000;

//variables to hold ADC results
volatile short zEndstopRes = 0;
volatile unsigned short thermistorRes = 0;

//target temperature is degrees
short targetTemp = 0;
//target temperature in "ADC units"
short targetTempADCval = 1023;
