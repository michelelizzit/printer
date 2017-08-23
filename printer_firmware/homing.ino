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


//homing.ino

//request a homing for the X axis
void runHomeX() {
  currentlyMoving = true;
  aXpos = -1 * X_AXIS_LENGTH;
  aXposOffset = 0;
  aXposTarget = 0;
  pos3 = 0;
  homeX = true;
}

//set current X axis position as origin (homing completed)
void resetXaxis() {
  aXpos = 0;
  aXposOffset = 0;
  aXposTarget = 0;
  pos3 = 0;
}

//request a homing for the Y axis
void runHomeY() {
  currentlyMoving = true;
  aYpos = -1 * Y_AXIS_LENGTH;
  aYposOffset = 0;
  aYposTarget = 0;
  pos = 0;
  pos2 = 0;
  homeY = true;
}

//set current Y axis position as origin (homing completed)
void resetYaxis() {
  aYpos = 0;
  aYposOffset = 0;
  aYposTarget = 0;
  pos = 0;
  pos2 = 0;
}

//request a homing for the Z axis
void runHomeZ() {
  currentlyMoving = true;
  aZpos = -1 * Z_AXIS_LENGTH;
  aZposOffset = 0;
  aZposTarget = 0;
  motor4_state = 0;
  //use the ADC to read the endstop
  SET_ADC_Z_ENDSTOP();
  FORCE_START_CONVERSION();
  homeZ = true;
  zEndstopRes = -1;
}

//set current Z axis position as origin (homing completed)
void resetZaxis() {
  SET_ADC_THERMISTOR(); //switch the ADC mux back to the thermistor
  FORCE_START_CONVERSION(); //force start a new conversion
  //set current position as zero
  aZpos = 0;
  aZposOffset = 0;
  aZposTarget = 0;
  motor4_state = 0;
  zEndstopRes = 0;
}
