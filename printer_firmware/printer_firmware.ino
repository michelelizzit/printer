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

#include <avr/io.h>
#include <avr/wdt.h>
#include <TimerOne.h>
#include <PinChangeInterrupt.h>

#include "Config.h"

#include "digitalIO.h"

#include "adc.h"

//Pin definitions

//Be careful when changing pins, most pins declared here are just ignored
//you may have to change also digitalIO.h and adc.h
//pin 2 and 3 have hardware interrupts attached
//pin 5 and 6 are attached to Timer0 and are used for controlling DC motors using Fast PWM
//pin A6 and A7 are just ADC channels and cannot be used as digital pins

//Heater pin
#define HEATER 13

//Thermistor input
#define THERMISTOR A6

//Feature disabled, see comment below
//#define A4988_RESET -1

#define ENC_LED_OUT 11

//ENC_1 and ENC_2 are connected to the encoder on motor1 (Y axis)
#define ENC_IN_1 2
#define ENC_IN_2 3
//ENC_3 and ENC_4 are connected to the encoder on motor2 (also Y axis)
#define ENC_IN_3 A3
#define ENC_IN_4 A2
//ENC_5 and ENC_6 are connected to the encoder on motor3 (X axis)
#define ENC_IN_5 10
#define ENC_IN_6 12

//PWM DC motors
//MOTOR_1 is a DC motor controlled via PWM
#define MOTOR_1_1 5
#define MOTOR_1_2 4
//MOTOR_2 is a DC motor controlled via PWM
#define MOTOR_2_1 6
#define MOTOR_2_2 9

//Driven with A4988
//MOTOR_3 is a DC motor controlled with an A4988
#define MOTOR_3_1 7 //STEP
#define MOTOR_3_2 8 //DIR
//MOTOR_4 is a stepper motor controlled with an A4988
#define MOTOR_4_1 A0 //STEP
#define MOTOR_4_2 A1 //DIR
//MOTOR_5 is a stepper motor controlled with an A4988
#define MOTOR_5_1 A4 //STEP
#define MOTOR_5_2 A5 //DIR

//Endstop on Z axis
#define MOTOR_4_ENDSTOP A7

//current step number of motor4
volatile long motor4_state = 0;
//current step number of motor5
volatile long motor5_state = 0;

//variables to hold current encoder positions
volatile int pos = 0;
int prev_pos = 0;
volatile int pos2 = 0;
int prev_pos2 = 0;
volatile int pos3 = 0;
int prev_pos3 = 0;

//if set to true closed-loop control is disabled
boolean motorOverride = true;

//variable to hold current A4988 step for motor3
volatile int motor3_A4988_step = 0;

//flags to require a motor update
volatile boolean runUpdateMotor1 = false;
volatile boolean runUpdateMotor2 = false;
volatile boolean runUpdateMotor3 = false;
volatile boolean runUpdateMotor4 = false;
volatile boolean runUpdateMotor5 = false;

//false if motors have reached their target position
volatile boolean currentlyMoving = true;

//length of the serial buffer in bytes
#define SERIALBUF_LEN 150
byte serialBuf[SERIALBUF_LEN];
byte serialBufIndex;
//if true a full gcode command is present in the serial buffer
bool gcodeInBuf;

//if true use relative positioning for axis
bool relativePositioning;
//if true use relative positioning for extruder
bool relativeExtruder;

//final target position
volatile int aXpos;
volatile int aYpos;
volatile int aZpos;
volatile long aEpos;
//offset
volatile int aXposOffset;
volatile int aYposOffset;
volatile int aZposOffset;
volatile long aEposOffset;
//current target position, follows final target position without exceeding SPEED
volatile int aXposTarget;
volatile int aYposTarget;
volatile int aZposTarget;
volatile long aEposTarget;

//if != 0 printer waits waitTime milliseconds before accepting another command
//used primarily by the G4 command
volatile unsigned int waitTime = 0;

//true while homing
boolean homeX = false;
boolean homeY = false;
boolean homeZ = false;

//speed is microseconds/encoderUnit
#define SPEED 2000
//#define SPEED 200

void setup() {
  //disable the watchdog
  //prevents the watchdog from triggering another reset
  wdt_disable();

  //init serial at SERIAL_SPEED baud
  Serial.begin(SERIAL_SPEED);
  //empty serial buffer
  emptySerialBuf();
  Serial.println("Starting...");

  //use absolute positioning for axis
  relativePositioning = false;
  //use absolute positioning for extruder
  relativeExtruder = false;
  //buffer is currently empty and no command is present
  gcodeInBuf = false;
  serialBufIndex = 0;
  
  //current target position is 0
  aXpos = 0;
  aYpos = 0;
  aZpos = 0;
  aEpos = 0;
  aXposOffset = 0;
  aYposOffset = 0;
  aZposOffset = 0;
  aEposOffset = 0;
  aXposTarget = 0;
  aYposTarget = 0;
  aZposTarget = 0;
  aEposTarget = 0;

  //init pins
  pinMode(ENC_LED_OUT, OUTPUT);
  digitalWrite(ENC_LED_OUT, HIGH);
  
  pinMode(HEATER, OUTPUT);

  pinMode(ENC_IN_1, INPUT_PULLUP);
  pinMode(ENC_IN_2, INPUT_PULLUP);
  pinMode(ENC_IN_3, INPUT_PULLUP);
  pinMode(ENC_IN_4, INPUT_PULLUP);
  pinMode(ENC_IN_5, INPUT_PULLUP);
  pinMode(ENC_IN_6, INPUT_PULLUP);
  
  //init motors
  initMotors();

  //init Timer0 in Fast PWM mode
  init_P05_P06_pwm();

  //Attach pin5 to Timer0
  P05_ENABLE_PWM;

  //Attach pin6 to Timer0
  P06_ENABLE_PWM;

  //wait a few milliseconds (millis() relays on Timer0 and cannot be used)
  delayMicroseconds(2 * 1000);

  //init encoders
  initEnc();

  //init the ADC
  initADC();
  //set the ADC channel to thermistor (A6)
  SET_ADC_THERMISTOR();
  //start an ADC conversion
  FORCE_START_CONVERSION();
  
  //There are no pins left to support this feature
  //A4988 reset has to be connected to the AVR reset pin
  /*
  Serial.print("Resetting A4988\t");
  pinMode(A4988_RESET, OUTPUT);
  digitalWrite(A4988_RESET, HIGH);
  delayMicroseconds(500 * 1000);
  digitalWrite(A4988_RESET, LOW);
  */
  motor3_A4988_step = 0;
  /*Serial.println("DONE");*/

  //stop all motors
  motorOff();
  
  delayMicroseconds(250 * 1000);

  //init encoders
  resetEnc();
  
  //enable closed loop control
  motorOverride = false; //when set to false closed loop control is enabled

  //update motor target position every SPEED microseconds
  Timer1.initialize(SPEED);
  Timer1.attachInterrupt(updatePosISR);

  //init sequence completed
  Serial.println("DONE");
  
  Serial.println("Init sequence completed");
  Serial.println("Printer ready");
}

void loop() {
  //if DEBUG_POS is set output current position to serial
  #ifdef DEBUG_POS
  if ((pos != prev_pos) || (pos2 != prev_pos2) || (pos3 != prev_pos3) || (targetX != prev_targetX)) {
    prev_pos = pos;
    prev_pos2 = pos2;
    prev_pos3 = pos3;
    prev_targetX = targetX;
    Serial.print("POS: ");
    Serial.print(pos);
    Serial.print("\t");
    Serial.print(pos2);
    Serial.print("\t");
    Serial.print(pos3);
    Serial.print("\t T: ");
    Serial.println(aXposTarget);
  }
  #endif

  //load new bytes in the serial buffer
  checkSerial();

  //if axis are not moving
  if ((!currentlyMoving) && (!waitTime) && (!homeX) && (!homeY) && (!homeZ)) {
    //check if a gcode command is present in the serial buffer
    checkGcode();
  }

  //if currently homing X axis
  if (homeX) {
    if (aXposTarget == aXpos + aXposOffset) { //if target position reached
      resetXaxis(); //set current position as 0
      homeX = false; //no longer homing
    }
  }
  //if currently homing Y axis
  if (homeY) {
    if (aYposTarget == aYpos + aYposOffset) { //if target position reached
      resetYaxis(); //set current position as 0
      homeY = false; //no longer homing
    }
  }
  //if currently homing Z axis
  if (homeZ) {
    if (aZposTarget == aZpos + aZposOffset) { //if target position reached
      resetZaxis(); //set current position as 0
      homeZ = false; //no longer homing
    }
    //if endstop is triggered
    if ((zEndstopRes > Z_ENDSTOP_THRESHOLD) && (aZpos != Z_HEIGHT)) {
      aZpos = Z_HEIGHT;
      aZposOffset = 0;
      aZposTarget = 0;
      motor4_state = 0;
      zEndstopRes = 0;
    }
    #ifdef DEBUG_Z_ENDSTOP
    Serial.println("ENDZ: " + (String)zEndstopRes);
    #endif
  }

  //check flags and eventually update motors
  if (runUpdateMotor1) {
    runUpdateMotor1 = false;
    updateMotor1();
  }
  if (runUpdateMotor2) {
    runUpdateMotor2 = false;
    updateMotor2();
  }
  if (runUpdateMotor3) {
    runUpdateMotor3 = false;
    updateMotor3();
  }
  if (runUpdateMotor4) {
    runUpdateMotor4 = false;
    updateMotor4();
  }
  if (runUpdateMotor5) {
    runUpdateMotor5 = false;
    updateMotor5();
  }
}

