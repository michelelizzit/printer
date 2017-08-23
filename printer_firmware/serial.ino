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


//serial.ino

//check if there are incoming bytes on the Arduino serial buffer and transfer them in the gcode serial buffer
void checkSerial() {
  if ((Serial.available() > 0) && (!gcodeInBuf)) {
    serialBuf[serialBufIndex] = Serial.read();
    #ifdef ENABLE_SERIAL_ECHO
      Serial.write(serialBuf[serialBufIndex]);
    #endif
    if (serialBuf[serialBufIndex] == '\n') {
      gcodeInBuf = true; //if a newline character is received a complete command is present in the buffer
    }
    if (serialBuf[serialBufIndex] == '*') {
      serialBuf[serialBufIndex] = '\n'; //trick to ignore the checksum
    }
    incrSerialBufIndex();
  }
}

//check if there is a command in the serial buffer and process it
void checkGcode() {
  if (gcodeInBuf) {
    //Copy GCODE ID from buffer to gcodeID
    char gcodeID[5];
    byte gcodeIDlen = 0;
    emptyArr((byte*)gcodeID, 5);
    gcodeIDlen = readParam(gcodeID, 0);
    
    if (gcodeID[0] == 'N') { //ignore line numbers
      Serial.println("Line number ignored");
      gcodeIDlen += readParam(gcodeID, gcodeIDlen + 1) + 1;
    }
    
    //Check GCODE ID
    handleGcodeId(gcodeID, gcodeIDlen);

    gcodeInBuf = false;
    serialBufIndex = 0;
  }
}

void incrSerialBufIndex() {
  serialBufIndex++;
  if (serialBufIndex >= SERIALBUF_LEN) {
    serialBufIndex = 0;
  }
}

void emptySerialBuf() {
  emptyArr(serialBuf, SERIALBUF_LEN);
}

//Utilities
void emptyArr(byte arr[], byte len) {
  for (int i = 0; i < len; i++) {
    arr[i] = 0;
  }
}

bool arrCmpNum(byte arr1[], byte arr2[], byte len) {
  for (int i = 0; i < len; i++) {
    if (arr1[i] != arr2[i]) return false;
  }
  return true;
}

