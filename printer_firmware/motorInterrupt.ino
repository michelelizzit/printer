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


//motorInterrupt.ino

//Expand preprocessor definitions from Config.h
#ifndef ENABLE_MOT1
  #define MOT_1_RUNPWM 0
  #define MOT_1_STEP_SLOW_1_PWM 0
#endif
#ifndef ENABLE_MOT2
  #define MOT_2_RUNPWM 0
  #define MOT_2_STEP_SLOW_1_PWM 0
#endif

void updateMotor1() {
  if (motorOverride) return;
  #ifdef ENABLE_MOT1
  if (aYposTarget < pos - MOT_1_STEP_SLOW_1_VAL)
    motor1(-MOT_1_RUNPWM);
  else if (aYposTarget < pos)
    motor1(-MOT_1_STEP_SLOW_1_PWM);
  else if (aYposTarget > pos + MOT_1_STEP_SLOW_1_VAL)
    motor1(MOT_1_RUNPWM);
  else if (aYposTarget > pos)
    motor1(MOT_1_STEP_SLOW_1_PWM);
  else if (aYposTarget == pos) {
    motor1Off();
  }
  #endif
}

void updateMotor2() {
  if (motorOverride) return;
  #ifdef ENABLE_MOT2
  if (aYposTarget < pos2 - MOT_2_STEP_SLOW_1_VAL)
    motor2(-MOT_2_RUNPWM);
  else if (aYposTarget < pos2 - MOT_2_STEP_SLOW_2_VAL)
    motor2(-MOT_2_STEP_SLOW_1_PWM);
  else if (aYposTarget < pos2)
    motor2(-MOT_2_STEP_SLOW_2_PWM);
  else if (aYposTarget > pos2 + MOT_2_STEP_SLOW_1_VAL)
    motor2(MOT_2_RUNPWM);
  else if (aYposTarget > pos2 + MOT_2_STEP_SLOW_2_VAL)
    motor2(MOT_2_STEP_SLOW_1_PWM);
  else if (aYposTarget > pos2)
    motor2(MOT_2_STEP_SLOW_2_PWM);
  else if (aYposTarget == pos2) {
    motor2Off();
  }
  #endif
}

void updateMotor3() {
  if (motorOverride) return;
  #ifdef ENABLE_MOT3
  if (aXposTarget < pos3 - MOT_3_STEP_SLOW_1_VAL)
    motor3(-2);
  else if (aXposTarget < pos3)
    motor3(-1);
  else if (aXposTarget > pos3 + MOT_3_STEP_SLOW_1_VAL) {
    motor3(2);
  }
  else if (aXposTarget > pos3)
    motor3(1);
  else if (aXposTarget == pos3) {
    motor3Off();
  }
  #endif
}

void updateMotor4() {
  if (motorOverride) return;
  #ifdef ENABLE_MOT4
  if (aZposTarget > motor4_state)
    motor4Step();
  else if (aZposTarget < motor4_state)
    motor4StepB();
  //if motor has not yet reached its final target position
  if (aZposTarget != motor4_state)
    runUpdateMotor4 = true; //request another motor update
  #endif
}

void updateMotor5() {
  if (motorOverride) return;
  #ifdef ENABLE_MOT5
  if (aEposTarget > motor5_state) {
    motor5Step();
  }
  else if (aEposTarget < motor5_state) {
    #ifdef ENABLE_RETRACTION
    motor5StepB();
    #endif
    #ifndef ENABLE_RETRACTION
    motor5_state--;
    #endif
  }
  //if motor has not yet reached its final target position
  if (aEposTarget != motor5_state)
    runUpdateMotor5 = true; //request another motor update
  #endif
}

//!! function called inside an ISR !!
void updatePosISR() {
  currentlyMoving = false; //if is still true at the end of the function, each motor has reached its final position
  //move each motor target one step nearer its final target position
  if (aXposTarget != aXpos + aXposOffset) {
    if (aXpos + aXposOffset < aXposTarget) aXposTarget--;
    else aXposTarget++;
    runUpdateMotor3 = true; //request a motor update
    currentlyMoving = true;
  }
  if (aYposTarget != aYpos + aYposOffset) {
    if (aYpos + aYposOffset < aYposTarget) aYposTarget--;
    else aYposTarget++;
    runUpdateMotor1 = true; //request a motor update
    runUpdateMotor2 = true; //request a motor update
    currentlyMoving = true;
  }
  if (aZposTarget != aZpos + aZposOffset) {
    if (aZpos + aZposOffset < aZposTarget) aZposTarget--;
    else aZposTarget++;
    runUpdateMotor4 = true; //request a motor update
    currentlyMoving = true;
  }
  if (aEposTarget != aEpos + aEposOffset) {
    if (aEpos + aEposOffset < aEposTarget) aEposTarget--;
    else aEposTarget++;
    runUpdateMotor5 = true; //request a motor update
    currentlyMoving = true;
  }
  //this function gets called at regular intervals
  //it is used to decrement waitTime
  if (waitTime) {
    waitTime--;
  }
}
