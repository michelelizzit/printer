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


//gcode_handlers.ino

//execute the correct function based on GCODE
void handleGcodeId(char* gcodeID, byte gcodeIDlen) {
  if (arrCmpNum((byte*)gcodeID, (byte*)"M105", 4)) {
      M105called(gcodeIDlen);
    }
    else if (arrCmpNum((byte*)gcodeID, (byte*)"M104", 4)) {
      M104called(gcodeIDlen);
    }
    else if (arrCmpNum((byte*)gcodeID, (byte*)"M114", 4)) {
      M114called(gcodeIDlen);
    }
    else if (arrCmpNum((byte*)gcodeID, (byte*)"G28", 3)) {
      G28called(gcodeIDlen);
    }
    else if (arrCmpNum((byte*)gcodeID, (byte*)"G90", 3)) {
      G90called(gcodeIDlen);
    }
    else if (arrCmpNum((byte*)gcodeID, (byte*)"G91", 3)) {
      G91called(gcodeIDlen);
    }
    else if (arrCmpNum((byte*)gcodeID, (byte*)"G92", 3)) {
      G92called(gcodeIDlen);
    }
    else if (arrCmpNum((byte*)gcodeID, (byte*)"M82", 3)) {
      M82called(gcodeIDlen);
    }
    else if (arrCmpNum((byte*)gcodeID, (byte*)"M83", 3)) {
      M83called(gcodeIDlen);
    }
    else if (arrCmpNum((byte*)gcodeID, (byte*)"G0", 2)) {
      G0called(gcodeIDlen);
    }
    else if (arrCmpNum((byte*)gcodeID, (byte*)"G1", 2)) {
      G1called(gcodeIDlen);
    }
    else if (arrCmpNum((byte*)gcodeID, (byte*)"G4", 2)) {
      G4called(gcodeIDlen);
    }
    else if (gcodeID[0] == 'G') {
      unsupported(gcodeIDlen);
    }
    else if (gcodeID[0] == 'M') {
      unsupported(gcodeIDlen);
    }
    else if (gcodeID[0] == 'T') {
      unsupported(gcodeIDlen);
    }
}

//function called when an unsupported GCODE command is received
void unsupported(byte gcodeIDlen) {
  Serial.println("Unsupported command");
  Serial.println("ok");
}

//G0 Move
void G0called(byte curBufPos) {
  char param[25];
  while (serialBuf[curBufPos] != '\n') {
    curBufPos++;

    byte paramLen = readParam(param, curBufPos);
    if (paramLen == 0) {
      Serial.println("Gcode err");
    }
    curBufPos += paramLen;
    

    if (!relativePositioning) {
      if (param[0] == 'X') aXpos = ((float)MM_ENC_CONVERSION_FACTOR * parseParam(param));
      else if (param[0] == 'Y') aYpos = ((float)MM_ENC_CONVERSION_FACTOR * parseParam(param));
      else if (param[0] == 'Z') aZpos = ((float)MM_STEPZ_CONVERSION_FACTOR * parseParam(param));
      else if (param[0] == 'E') aEpos = ((float)MM_EXTR_CONVERSION_FACTOR * parseParam(param));
    }
    else {
      if (param[0] == 'X') aXpos = aXpos + ((float)MM_ENC_CONVERSION_FACTOR * parseParam(param));
      else if (param[0] == 'Y') aYpos = aYpos + ((float)MM_ENC_CONVERSION_FACTOR * parseParam(param));
      else if (param[0] == 'Z') aZpos = aZpos + ((float)MM_STEPZ_CONVERSION_FACTOR * parseParam(param));
      else if (param[0] == 'E') aEpos = aEpos + ((float)MM_EXTR_CONVERSION_FACTOR * parseParam(param));
    }
    
  }
  Serial.println("ok");
}

//G1 Move
void G1called(byte gcodeIDlen) {
  G0called(gcodeIDlen); //G1 is treated exactly as a G0 command
}

//G28 Home
void G28called(byte curBufPos) {
  char param[15];
  if (serialBuf[curBufPos] == '\n') {
    runHomeX();
    runHomeY();
    runHomeZ();
  }
  while (serialBuf[curBufPos] != '\n') {
    curBufPos++;

    byte paramLen = readParam(param, curBufPos);

    //Commented because the GCODE specification is unclear here
    /*
    if (paramLen != 0) {
      Serial.println("Gcode err");
    }*/
    
    curBufPos += paramLen;
    
    if (param[0] == 'X') runHomeX();
    else if (param[0] == 'Y') runHomeY();
    else if (param[0] == 'Z') runHomeZ();
  }
  Serial.println("ok");
}

//G4 Wait
void G4called(byte curBufPos) {
  char param[15];
  if (serialBuf[curBufPos] != '\n') {
    curBufPos++;
    byte paramLen = readParam(param, curBufPos);
    if (paramLen == 0) {
      Serial.println("Gcode err");
    }
    if (param[0] == 'S') {
      waitTime = (parseParam(param) * 1000 * 1000) / SPEED;
    }
    else if (param[0] == 'P') {
      waitTime = (parseParam(param) * 1000) / SPEED;
    }
    else {
      Serial.println("Gcode err");
    }
  }
  else {
    Serial.println("Gcode err");
  }
  
  Serial.println("ok");
}

//M105 Get temperature
void M105called(byte gcodeIDlen) {
  Serial.print("ok T:");
  //Serial.print(curTemp);
  Serial.print(ADCtoTemp(thermistorRes));
  Serial.print(" /");
  //Serial.print(thermistorRes);
  //Serial.print("  ");
  //Serial.print(targetTempADCval);
  //Serial.print("  ");
  Serial.println(targetTemp);
}

//M104 Set temperature
void M104called(byte curBufPos) {
  char param[15];
  if (serialBuf[curBufPos] != '\n') {
    curBufPos++;
    byte paramLen = readParam(param, curBufPos);
    if (paramLen == 0) {
      Serial.println("Gcode err");
    }
    if (param[0] == 'S') {
      targetTemp = parseParam(param);
      targetTempADCval = tempToADC(targetTemp);
    }
    else {
      Serial.println("Gcode err");
    }
  }
  else {
    Serial.println("Gcode err");
  }
  
  Serial.println("ok");
}

//M114 Get current position
void M114called(byte gcodeIDlen) {
  Serial.print("ok C: X:");
  Serial.print(((float)aXposTarget / (float)MM_ENC_CONVERSION_FACTOR));
  Serial.print(" Y:");
  Serial.print(((float)aYposTarget / (float)MM_ENC_CONVERSION_FACTOR));
  Serial.print(" Z:");
  Serial.print(((float)aZposTarget / (float)MM_STEPZ_CONVERSION_FACTOR));
  Serial.print(" E:");
  Serial.println(((float)aEposTarget / (float)MM_EXTR_CONVERSION_FACTOR));
}

//G90 Use absolute positioning for axis
void G90called(byte gcodeIDlen) {
  relativePositioning = false;
  Serial.println("ok");
}

//G91 Use relative positioning for axis
void G91called(byte gcodeIDlen) {
  relativePositioning = true;
  Serial.println("ok");
}

//M82 Use absolute positioning for extruder
void M82called(byte gcodeIDlen) {
  relativeExtruder = false;
  Serial.println("ok");
}

//M83 Use relative positioning for extruder
void M83called(byte gcodeIDlen) {
  relativeExtruder = true;
  Serial.println("ok");
}

//G92 Set current position as zero
void G92called(byte curBufPos) {
  aXpos = 0;
  aYpos = 0;
  aZpos = 0;
  aEpos = 0;
  char param[10];
  while (serialBuf[curBufPos] != '\n') {
    curBufPos++;

    byte paramLen = readParam(param, curBufPos);
    if (paramLen == 0) {
      Serial.println("Gcode err");
    }
    curBufPos += paramLen;
    
    if (param[0] == 'X') aXposOffset = aXpos + ((float)MM_ENC_CONVERSION_FACTOR * parseParam(param));
      else if (param[0] == 'Y') aYposOffset = aYpos + ((float)MM_ENC_CONVERSION_FACTOR * parseParam(param));
      else if (param[0] == 'Z') aZposOffset = aZpos + ((float)MM_ENC_CONVERSION_FACTOR * parseParam(param));
      else if (param[0] == 'E') aEposOffset = aEpos + ((float)MM_ENC_CONVERSION_FACTOR * parseParam(param));
  }
  Serial.println("ok");
}


//Utilities

//returns param value if successful, 30000 otherwise
float parseParam(char* param) {
  float res = 0;
  bool isNegative = false;
  byte paramLen = 0;
  for (; param[paramLen] != '\0'; paramLen++) {}
  
  if (paramLen == 1) return 30000; //30000 is an error code
  
  if (param[1] == '-') {
      isNegative = true;
  }

  short decimals = 0;
  for (byte i = 1 + isNegative; i < paramLen; i++) {
    if (param[i] == '.') {
      decimals = paramLen - i - 1;
      continue;
    }
    res *= 10;
    res += param[i] - 48;
  }
  if (isNegative) {
    res *= -1;
  }
  res /= pow(10, decimals);
  return res;
}

//returns param length if successful, 255 otherwise
byte readParam(char* param, byte pos) {
  byte paramLen = 255;
  for (int i = pos; i < SERIALBUF_LEN; i++) {
    if ((serialBuf[i] == ' ') || (serialBuf[i] == '\n')) {
      paramLen = i - pos;
      break;
    }
  }
  if (paramLen == 255) return paramLen; //use 255 as error code
  
  for (int i = 0; i < paramLen; i++) {
    param[i] = serialBuf[pos + i];
  }
  param[paramLen] = '\0';

  return paramLen;
}

