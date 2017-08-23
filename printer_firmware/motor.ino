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


//motor.ino

void initMotors() {
  //set motor pins as outputs
  pinMode(MOTOR_1_1, OUTPUT);
  pinMode(MOTOR_1_2, OUTPUT);
  pinMode(MOTOR_2_1, OUTPUT);
  pinMode(MOTOR_2_2, OUTPUT);
  pinMode(MOTOR_3_1, OUTPUT);
  pinMode(MOTOR_3_2, OUTPUT);
  pinMode(MOTOR_4_1, OUTPUT);
  pinMode(MOTOR_4_2, OUTPUT);
  pinMode(MOTOR_5_1, OUTPUT);
  pinMode(MOTOR_5_2, OUTPUT);
}

void motor1(double val) {
  if (val == 0) motor1Off();
  if (val > 0) motor1CounterClockwise((int)val);
  if (val < 0) motor1Clockwise(-1 * (int)val);
}

void motorOff() {
  motor1Off();
  motor2Off();
  motor3Off();
}

#define RUNPWM 255

void motor1Clockwise(int pwm) {
  MOTOR_1_1_DISABLE_PWM;
  if (pwm == 255) {
    MOTOR_1_1_HIGH();
  }
  else {
    MOTOR_1_1_PWM_VAL = pwm;
    MOTOR_1_1_ENABLE_PWM;
  }
  
  MOTOR_1_2_LOW();
}

void motor1CounterClockwise(int pwm) {
  pwm = 255 - pwm;
  MOTOR_1_1_DISABLE_PWM;
  if (pwm == 0) {
    MOTOR_1_1_LOW();
  }
  else {
    MOTOR_1_1_PWM_VAL = pwm;
    MOTOR_1_1_ENABLE_PWM;
  }
  
  MOTOR_1_2_HIGH();
}

void motor1Off() {
  MOTOR_1_1_DISABLE_PWM;
  MOTOR_1_1_LOW();
  MOTOR_1_2_LOW();
}

void motor2(double val) {
  if (val == 0) motor2Off();
  if (val > 0) motor2CounterClockwise((int)val);
  if (val < 0) motor2Clockwise(-1 * (int)val);
}

void motor2Clockwise(int pwm) {
  MOTOR_2_1_DISABLE_PWM;
  if (pwm == 255) {
    MOTOR_2_1_HIGH();
  }
  else {
    MOTOR_2_1_PWM_VAL = pwm;
    MOTOR_2_1_ENABLE_PWM;
  }
  MOTOR_2_2_LOW();
}

void motor2CounterClockwise(int pwm) {
  pwm = 255 - pwm;
  MOTOR_2_1_DISABLE_PWM;
  if (pwm == 0) {
    MOTOR_2_1_LOW();
  }
  else {
    MOTOR_2_1_PWM_VAL = pwm;
    MOTOR_2_1_ENABLE_PWM;
  }
  MOTOR_2_2_HIGH();
}

void motor2Off() {
  MOTOR_2_1_DISABLE_PWM;
  MOTOR_2_1_LOW();
  MOTOR_2_2_LOW();
}

void motor3goToStep(int targetStep) {
  int diff = (A4988_STEPN + targetStep - motor3_A4988_step) % A4988_STEPN;
  int diffM = (A4988_STEPN - targetStep + motor3_A4988_step) % A4988_STEPN;
  bool reverseDir = false;
  if (diffM < diff) {
    reverseDir = true;
    diff = diffM;
  }
  if (reverseDir) {
    for (int i = 0; i < diff; i++) { 
      motor3stepB();
    }
  }
  else {
    for (int i = 0; i < diff; i++) { 
      motor3step();
    }
  }
  motor3_A4988_step = targetStep;
}

//TODO: use Timer2 instead of wasting CPU cycles
void motor3step() {
  MOTOR_3_2_LOW();
  MOTOR_3_1_HIGH();
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  MOTOR_3_1_LOW();
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
}

//TODO: use Timer2 instead of wasting CPU cycles
void motor3stepB() {
  MOTOR_3_2_HIGH();
  MOTOR_3_1_HIGH();
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  MOTOR_3_1_LOW();
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
}

void motor3Off() {
  motor3goToStep(A4988_OFF);
}

void motor3(int spdVal) {
  switch (spdVal) {
    case 0:
      motor3Off();
      break;
    case 1:
      motor3goToStep(A4988_SPD1);
      break;
    case 2:
      motor3goToStep(A4988_SPD2);
      break;
    case -1:
      motor3goToStep(A4988_SPDB1);
      break;
    case -2:
      motor3goToStep(A4988_SPDB2);
      break;
    default:
      break;
  }
}

//TODO: use Timer2 instead of wasting CPU cycles
void motor4Step() {
  MOTOR_4_2_LOW();
  MOTOR_4_1_HIGH();
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  MOTOR_4_1_LOW();
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  motor4_state++;
}

//TODO: use Timer2 instead of wasting CPU cycles
void motor4StepB() {
  MOTOR_4_2_HIGH();
  MOTOR_4_1_HIGH();
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  MOTOR_4_1_LOW();
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  motor4_state--;
}

//TODO: use Timer2 instead of wasting CPU cycles
void motor5Step() {
  MOTOR_5_2_LOW();
  MOTOR_5_1_HIGH();
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  MOTOR_5_1_LOW();
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  motor5_state++;
}

//TODO: use Timer2 instead of wasting CPU cycles
void motor5StepB() {
  MOTOR_5_2_HIGH();
  MOTOR_5_1_HIGH();
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  MOTOR_5_1_LOW();
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //4*62nS delay
  motor5_state--;
}

