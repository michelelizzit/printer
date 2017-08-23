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


//Config.h
//Firmware configuration


//Motor 1 and 2 PWM prescaler configuration
//Selecting a higher prescaler results in a lower pwm frequency
//An excessively high PWM frequency may result in the DC motor controller not working correctly, see the DC motor controller datasheet
//An excessively low PWM frequency may result in the DC motors not working correctly
//Default: PWM_PRESCALER_0064

//#define PWM_PRESCALER_0001
//#define PWM_PRESCALER_0008
#define PWM_PRESCALER_0064
//#define PWM_PRESCALER_0256
//#define PWM_PRESCALER_1024


//Serial baud rate
//Default: 250000
#define SERIAL_SPEED 250000


//mm/encUnits conversion factors
//For most linear encoders found in inkjet printers: 33
//Default: 33
#define MM_ENC_CONVERSION_FACTOR 33


//mm/step conversion factor for Z axis
//Default: -70
#define MM_STEPZ_CONVERSION_FACTOR -70


//mm/step conversion factor for extruder
//Value can not be negative
//Default: 600
#define MM_EXTR_CONVERSION_FACTOR 600


//Uncomment to enable serial echo
//Useful for debugging
//Default: enabled
#define ENABLE_SERIAL_ECHO


//Axis length, used while homing
//Keep all the values higher than the real axis length (about 1.25 times the real axis length)
#define X_AXIS_LENGTH 2000 //value in encoder units
#define Y_AXIS_LENGTH 2000 //value in encoder units
#define Z_AXIS_LENGTH 4500 //value in steps


//Threshold value for Z endstop (max ADC value is 255)
//Z endstop is read using the ADC due to lack of digital pins available
//The printer will consider the endstop to be triggered if the value is higher than the threshold value 
//Default: 128
#define Z_ENDSTOP_THRESHOLD 128


//Uncomment to enable Z endstop debugging
//If enabled sends via serial readings of the Z endstop from the ADC
//Default: disabled
//#define DEBUG_Z_ENDSTOP


//How many stepper steps to rise once endstop is hit
//Default: 3800
#define Z_HEIGHT 3800


//Uncomment to continuously output position to serial
//WARNING: significantly affects printer precision if enabled
//Default: disabled
//#define DEBUG_POS


//Uncomment to enable retraction
//Default: disabled
//#define ENABLE_RETRACTION


//Comment to disable motor1 (Y axis)
//Default: enabled
#define ENABLE_MOT1

//Comment to disable motor2 (Y axis)
//Default: enabled
#define ENABLE_MOT2

//Comment to disable motor3 (X axis)
//Default: enabled
#define ENABLE_MOT3

//Comment to disable motor4 (Z axis)
//Default: enabled
#define ENABLE_MOT4

//Comment to disable motor5 (extruder)
//Default: enabled
#define ENABLE_MOT5


//Motor calibration parameters
//If an axis keeps oscillating around its target position edit these settings

//MOTOR_1 calibration parameters (Y axis)
#define MOT_1_STEP_SLOW_1_VAL 2 //zone 1 width
#define MOT_1_STEP_SLOW_1_PWM 120 //PWM value while in Zone 1
#define MOT_1_RUNPWM 210 //max PWM value

//MOTOR_2 calibration parameters (Y axis)
#define MOT_2_STEP_SLOW_2_VAL 2 //zone 2 width
#define MOT_2_STEP_SLOW_2_PWM 0 //PWM value while in Zone 2
#define MOT_2_STEP_SLOW_1_VAL 35 //zone 1 width
#define MOT_2_STEP_SLOW_1_PWM 91 //PWM value while in Zone 1
#define MOT_2_RUNPWM 120 //max PWM value

//MOTOR_3 calibration parameters (X axis)
#define MOT_3_STEP_SLOW_1_VAL 15 //zone 1 width

//A4988 calibration parameters (used for MOTOR_3)
#define A4988_STEPN 128 //number of microsteps enabled on the A4988
#define A4988_SPD1 58 //microstep number for motor at speed 1
#define A4988_SPD2 70 //microstep number for motor at speed 2
#define A4988_SPDB1 39 //microstep number for motor at speed -1
#define A4988_SPDB2 33 //microstep number for motor at speed -2
#define A4988_OFF 48 //microstep number for motor off
