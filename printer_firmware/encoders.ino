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


//encoders.ino

//variables to hold current encoder states
volatile boolean enc1_state = false;
volatile boolean enc2_state = false;
volatile boolean enc3_state = false;
volatile boolean enc4_state = false;
volatile boolean enc5_state = false;
volatile boolean enc6_state = false;

//init encoders
void initEnc() {
  //get current encoder states
  enc1_state = digitalRead(ENC_IN_1);
  enc2_state = digitalRead(ENC_IN_2);
  enc3_state = digitalRead(ENC_IN_3);
  enc4_state = digitalRead(ENC_IN_4);
  enc5_state = digitalRead(ENC_IN_5);
  enc6_state = digitalRead(ENC_IN_6);

  //attach an interrupt to each encoder pin
  attachInterrupt(digitalPinToInterrupt(ENC_IN_1), enc_1, CHANGE); //enc1 is attached to external interrupt 0 (pin2)
  attachInterrupt(digitalPinToInterrupt(ENC_IN_2), enc_2, CHANGE); //enc2 is attached to external interrupt 1 (pin3)
  //all other encoders use pin change interrupts
  attachPCINT(digitalPinToPCINT(ENC_IN_3), enc_3, CHANGE);
  attachPCINT(digitalPinToPCINT(ENC_IN_4), enc_4, CHANGE);
  attachPCINT(digitalPinToPCINT(ENC_IN_5), enc_5, CHANGE);
  attachPCINT(digitalPinToPCINT(ENC_IN_6), enc_6, CHANGE);

  //enable internal pullups
  digitalWrite(ENC_IN_1, HIGH);
  digitalWrite(ENC_IN_2, HIGH);
  digitalWrite(ENC_IN_3, HIGH);
  digitalWrite(ENC_IN_4, HIGH);
  digitalWrite(ENC_IN_5, HIGH);
  digitalWrite(ENC_IN_6, HIGH);
}

void resetEnc() {
  //reset encoder pin states
  enc1_state = digitalRead(ENC_IN_1);
  enc2_state = digitalRead(ENC_IN_2);
  enc3_state = digitalRead(ENC_IN_3);
  enc4_state = digitalRead(ENC_IN_4);
  enc5_state = digitalRead(ENC_IN_5);
  enc6_state = digitalRead(ENC_IN_6);
  
  //set all encoder positions to 0
  pos = 0;
  pos2 = 0;
  pos3 = 0;
}

//Encoder ISRs
void enc_1() {
  //update encoder position
  enc1_state = !enc1_state;
  if ((!enc1_state) && (enc2_state)) pos++;
  else if ((enc1_state) && (!enc2_state)) pos++;
  else pos--;
  //request a motor update
  runUpdateMotor1 = true;
}

void enc_2() {
  //update encoder position
  enc2_state = !enc2_state;
  if ((enc1_state) && (!enc2_state)) pos--;
  else if ((!enc1_state) && (enc2_state)) pos--;
  else pos++;
  //request a motor update
  runUpdateMotor1 = true;
}

void enc_3() {
  //update encoder position
  enc3_state = !enc3_state;
  if ((!enc3_state) && (enc4_state)) pos2--;
  else if ((enc3_state) && (!enc4_state)) pos2--;
  else pos2++;
  //request a motor update
  runUpdateMotor2 = true;
}

void enc_4() {
  //update encoder position
  enc4_state = !enc4_state;
  if ((enc3_state) && (!enc4_state)) pos2++;
  else if ((!enc3_state) && (enc4_state)) pos2++;
  else pos2--;
  //request a motor update
  runUpdateMotor2 = true;
}

void enc_5() {
  //update encoder position
  enc5_state = !enc5_state;
  if ((!enc5_state) && (enc6_state)) pos3--;
  else if ((enc5_state) && (!enc6_state)) pos3--;
  else pos3++;
  //request a motor update
  runUpdateMotor3 = true;
}

void enc_6() {
  //update encoder position
  enc6_state = !enc6_state;
  if ((enc5_state) && (!enc6_state)) pos3++;
  else if ((!enc5_state) && (enc6_state)) pos3++;
  else pos3--;
  //request a motor update
  runUpdateMotor3 = true;
}
