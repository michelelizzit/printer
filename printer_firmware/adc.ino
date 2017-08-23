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


//adc.ino

//write ADC configuration to ADC registers
void initADC() {
  SET_ADC_THERMISTOR(); //set ADC channel to 6 (pin A6)
  ADCSRB = B00000000;
  ADCSRA = B11001111; //enable interrupt, prescaler set to 128
}

//ADC conversion completed ISR
ISR(ADC_vect) {
  if ((byte)(ADMUX & B00000111) == (byte)6) { //if adc channel is thermistor
   thermistorRes = ADCW; //read ADC value from ADC registers
   if (thermistorRes > targetTempADCval) { //if temperature is lower than target temperature
     HEATER_HIGH(); //turn the heater on
   }
   else { //if temperature is higher than target temperature
     HEATER_LOW(); //turn the heater off
   }
  }
  else if ((byte)(ADMUX & B00000111) == (byte)7) { //if is endstop
    //endstop is read with 8 bit precision
    if (zEndstopRes == -1) zEndstopRes = 0;
    else zEndstopRes = ADCH; //left adjust enabled, no need to read ADCL
  }
  ADCSRA |= B01000000;  //start another conversion
}
