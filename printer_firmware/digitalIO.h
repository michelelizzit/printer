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


//digitalIO.h

//Pin definitions
#define MOTOR_1_1_HIGH(); P05_HIGH
#define MOTOR_1_1_LOW(); P05_LOW

#define MOTOR_1_2_HIGH(); P04_HIGH
#define MOTOR_1_2_LOW(); P04_LOW

#define MOTOR_2_1_HIGH(); P06_HIGH
#define MOTOR_2_1_LOW(); P06_LOW

#define MOTOR_2_2_HIGH(); P09_HIGH
#define MOTOR_2_2_LOW(); P09_LOW

#define MOTOR_3_1_HIGH(); P07_HIGH
#define MOTOR_3_1_LOW(); P07_LOW

#define MOTOR_3_2_HIGH(); P08_HIGH
#define MOTOR_3_2_LOW(); P08_LOW

#define MOTOR_4_1_HIGH(); PA0_HIGH
#define MOTOR_4_1_LOW(); PA0_LOW

#define MOTOR_4_2_HIGH(); PA1_HIGH
#define MOTOR_4_2_LOW(); PA1_LOW

#define MOTOR_5_1_HIGH(); PA4_HIGH
#define MOTOR_5_1_LOW(); PA4_LOW

#define MOTOR_5_2_HIGH(); PA5_HIGH
#define MOTOR_5_2_LOW(); PA5_LOW

#define HEATER_HIGH(); P13_HIGH
#define HEATER_LOW(); P13_LOW

//Pin 5 and 6 definition
#define MOTOR_1_1_PWM_VAL P05_PWM_VAL
#define MOTOR_2_1_PWM_VAL P06_PWM_VAL
#define MOTOR_1_1_ENABLE_PWM P05_ENABLE_PWM
#define MOTOR_2_1_ENABLE_PWM P06_ENABLE_PWM
#define MOTOR_1_1_DISABLE_PWM P05_DISABLE_PWM
#define MOTOR_2_1_DISABLE_PWM P06_DISABLE_PWM

#define P06_ENABLE_PWM TCCR0A |= _BV(COM0A1);
#define P05_ENABLE_PWM TCCR0A |= _BV(COM0B1);
#define P06_DISABLE_PWM TCCR0A &= ~_BV(COM0A1);
#define P05_DISABLE_PWM TCCR0A &= ~_BV(COM0B1);
#define P06_PWM_VAL OCR0A
#define P05_PWM_VAL OCR0B

//Fast PWM
void init_P05_P06_pwm() {
  TCCR0A |= _BV(WGM01) | _BV(WGM00);
  #ifdef PWM_PRESCALER_0001
    TCCR0B = _BV(CS00);
  #endif
  #ifdef PWM_PRESCALER_0008
    TCCR0B = _BV(CS01);
  #endif
  #ifdef PWM_PRESCALER_0064
    TCCR0B = _BV(CS01) | _BV(CS00);
  #endif
  #ifdef PWM_PRESCALER_0256
    TCCR0B = _BV(CS02);
  #endif
  #ifdef PWM_PRESCALER_1024
    TCCR0B = _BV(CS02) | _BV(CS00);
  #endif
}


//PORTB

#define PINNAME_READ(); P12_INPUT
#define P12_INPUT PINB = PIND & (1<<PB4);


#define P13_HIGH PORTB |= _BV(PB5);
#define P13_LOW PORTB &= ~_BV(PB5);

#define P09_HIGH PORTB |= _BV(PB1);
#define P09_LOW PORTB &= ~_BV(PB1);

#define P08_HIGH PORTB |= _BV(PB0);
#define P08_LOW PORTB &= ~_BV(PB0);

//PORTD
#define P07_HIGH PORTD |= _BV(PD7);
#define P07_LOW PORTD &= ~_BV(PD7);

#define P06_HIGH PORTD |= _BV(PD6);
#define P06_LOW PORTD &= ~_BV(PD6);

#define P05_HIGH PORTD |= _BV(PD5);
#define P05_LOW PORTD &= ~_BV(PD5);

#define P04_HIGH PORTD |= _BV(PD4);
#define P04_LOW PORTD &= ~_BV(PD4);

#define P03_HIGH PORTD |= _BV(PD3);
#define P03_LOW PORTD &= ~_BV(PD3);

#define P02_HIGH PORTD |= _BV(PD2);
#define P02_LOW PORTD &= ~_BV(PD2);

//PORTC
#define PA0_HIGH PORTC |= _BV(PC0);
#define PA0_LOW PORTC &= ~_BV(PC0);

#define PA1_HIGH PORTC |= _BV(PC1);
#define PA1_LOW PORTC &= ~_BV(PC1);

#define PA2_HIGH PORTC |= _BV(PC2);
#define PA2_LOW PORTC &= ~_BV(PC2);

#define PA3_HIGH PORTC |= _BV(PC3);
#define PA3_LOW PORTC &= ~_BV(PC3);

#define PA4_HIGH PORTC |= _BV(PC4);
#define PA4_LOW PORTC &= ~_BV(PC4);

#define PA5_HIGH PORTC |= _BV(PC5);
#define PA5_LOW PORTC &= ~_BV(PC5);
