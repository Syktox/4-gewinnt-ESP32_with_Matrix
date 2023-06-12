#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

const int COLS = 16;
const int ROW = 24;
const int NUM_PIXELS = COLS * ROW;

#define MATRIX_PIN_1 12
#define MATRIX_PIN_2 13
#define BRIGHTNESS 8

Adafruit_NeoPixel matrix_1 = Adafruit_NeoPixel(NUM_PIXELS, MATRIX_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel matrix_2 = Adafruit_NeoPixel(NUM_PIXELS, MATRIX_PIN_2, NEO_GRB + NEO_KHZ800);


// Wird alles von der Matrix 1 beachtet
int firstRowBottom = 0;
int secondRowBottom = 0;
int thirdRowBottom = 0;
int fourthRowBottom = 0;
int fifthRowBottom = 0;
int sixthRowBottom = 0;
int seventhRowBottom = 0;

int i = 0;

int topLine[] = {361, 360, 358, 359, /*|*/ 328, 329, 342, 343, /*|*/ 310, 311, 312, 313, /*|*/ 280, 281, 294, 295, /*|*/ 262, 263, 264, 265, /*|*/ 232, 233, 246, 247, /*|*/ 214, 215, 216, 217};
int topLineSecond[] = {214, 215, 216, 217, 232, 233, 246, 247, 262, 263, 264, 265,280, 281, 294, 295, 310, 311, 312, 313, 328, 329, 342, 343, 361, 360, 358, 359};


int firstRow[] = {26, 21, 20, 27, /*|*/ 18, 28, 19, 29, /*|*/ 16, 17, 30, 31,             /*|*/
                  352, 353, 366, 367, /*|*/ 354, 355, 365, 364, /*|*/ 356, 357, 362, 363, /*|*/
                  361, 360, 358, 359};      // ganz links

int secondRow[] = {42, 43, 52, 53, /*|*/ 44, 45, 51, 50, /*|*/ 46, 47, 48, 49,             /*|*/
                   334, 335, 336, 337, /*|*/ 333, 332, 338, 339, /*|*/ 331, 330, 340, 341, /*|*/
                   328, 329, 342, 343};      // 2. von links

int thirdRow[] = {69, 68, 74, 75, /*|*/ 67, 66, 76, 77, /*|*/ 64, 65, 78, 79,             /*|*/
                  304, 305, 318, 319, 307, 306, 316, 317, 309, 308, 314, 315, 310, 311, 312, 313};      // 3. von links

int fourthRow[] = {90, 91, 100, 101, /*|*/ 92, 93, 98, 99, /*|*/ 94, 95, 97, 96,             /*|*/
                   286, 287, 288, 289, /*|*/ 285, 284, 290, 291, /*|*/ 283, 282, 292, 293, /*|*/
                   280, 281, 294, 295};      // 4. von links

int fifthRow[] = {116, 117, 122, 123, /*|*/ 114, 115, 124, 125, /*|*/ 112, 113, 126, 127,             /*|*/
                   256, 257, 270, 271, 259, 258, 268, 269, 261, 260, 266, 267, 262, 263, 264, 265,};      // 5. von links

int sixthRow[] = {138, 139, 148, 149, /*|*/ 140, 141, 146, 147, /*|*/ 142, 143, 144, 145,             /*|*/
                  238, 239, 240, 241, /*|*/ 237, 236, 242, 243, /*|*/ 235, 234, 244, 245, /*|*/
                  232, 233, 246, 247};      // 6. von links

int seventhRow[] = {164, 165, 170, 171, /*|*/ 162, 163, 172, 173, /*|*/ 160, 161, 174, 175,             /*|*/
                    208, 209, 222, 223, 211, 210, 220, 221, 213, 212, 218, 219, 214, 215, 216, 217,};      // 7. von links

int currentTopLane = -1;
int player = 1; // 1 = Player 1, 2 = Player 2
int joyX_1 = 0;
int joyX_2 = 0;

void border(int start, int end);
void fullBorder();
void resetMatrix();
void showTopLine(int i, int player);
void dropPeace(int i, int player);

void setup()
{
  Serial.begin(9600);
  // Matrix_1 und Matrix_2 Initialisieren
  matrix_1.begin();
  matrix_1.setBrightness(BRIGHTNESS);
  matrix_1.show();

  matrix_2.begin();
  matrix_2.setBrightness(BRIGHTNESS);
  matrix_2.show();

  pinMode(33, INPUT);   // Player 1 Button
  pinMode(32, INPUT);   // Player 2 Button
  pinMode(25, INPUT_PULLDOWN);   // Player 1 JoyStick
  pinMode(26, INPUT_PULLDOWN);   // Player 2 JoyStick

  // resetMatrix();
  resetMatrix();
}

void loop()
{
    if (player == 1) {
      while (digitalRead(25) != HIGH) {
        // Logik
        // pixel oben blinken lassen
        joyX_1 = analogRead(32);
        if (joyX_1 > 2500 && i < 6) {
          i++;
        } else if (joyX_1 < 500 && i > 0)  {
          i--;
        }
        showTopLine(i, 1);
        delay(250);
      }
      if (i == 0 && firstRowBottom >= 25) {
        i++;
      }
      dropPeace(i, 1);
      player = 2;
    }
    
    // Player 2
    if (player == 2) {
      while (digitalRead(26) != HIGH) {
        joyX_2 = analogRead(33);
        if (joyX_2 > 2500 && i > 0) {
          i--;
        } else if (joyX_2 < 500 && i < 6)  {
          i++;
        }
        delay(250);
        showTopLine(i, 2);
      }
      if (i == 0 && firstRowBottom >= 25) {
        i++;
      }
      dropPeace(i, 2);
      player = 1;
    }
}

void border(int start, int end)
{
  for (int i = start; i < end; i++)
  {
    matrix_1.setPixelColor(i, 0, 250, 250);
    matrix_2.setPixelColor(i, 0, 250, 250);
    matrix_1.show();
    matrix_2.show();
  }
}
void fullBorder()
{
  int i = 8;
  border(0, 16);
  while (i < 192)
  {
    border(i, i + 8);
    i = i + 24;
  }
  border(184, 208);
  i = 224;
  while (i < 376)
  {
    border(i, i + 8);
    i = i + 24;
  }
  border(376, 384);
  border(22, 26);
  border(40, 42);
  border(54, 56);
  border(70, 74);
  border(88, 90);
  border(102, 104);
  border(118, 122);
  border(136, 138);
  border(150, 152);
  border(166, 170);
}
void resetMatrix()
{
  matrix_1.clear();
  matrix_2.clear();
  fullBorder();
}
void showTopLine(int i, int player)
{
  if (player == 1) {
  // clearing old TopLine
  if (currentTopLane == 0)
  {
    for (int index = 0; index < 4; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }
  else if (currentTopLane == 1)
  {
    for (int index = 4; index < 8; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }
  else if (currentTopLane == 2)
  {
    for (int index = 8; index < 12; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }
  else if (currentTopLane == 3)
  {
    for (int index = 12; index < 16; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }
  else if (currentTopLane == 4)
  {
    for (int index = 16; index < 20; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }
  else if (currentTopLane == 5)
  {
    for (int index = 20; index < 24; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }
  else if (currentTopLane == 6)
  {
    for (int index = 24; index < 28; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }

  // Setting new TopLine
  if (i == 0)
  {
    for (int index = 0; index < 4; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 255, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 255, 0);
    }
  }
  else if (i == 1)
  {
    for (int index = 4; index < 8; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 255, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 255, 0);
    }
  }
  else if (i == 2)
  {
    for (int index = 8; index < 12; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 255, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 255, 0);
    }
  }
  else if (i == 3)
  {
    for (int index = 12; index < 16; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 255, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 255, 0);
    }
  }
  else if (i == 4)
  {
    for (int index = 16; index < 20; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 255, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 255, 0);
    }
  }
  else if (i == 5)
  {
    for (int index = 20; index < 24; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 255, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 255, 0);
    }
  }
  else if (i == 6)
  {
    for (int index = 24; index < 28; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 255, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 255, 0);
    }
  }
  } else if (player == 2) {
    if (currentTopLane == 0)
  {
    for (int index = 0; index < 4; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }
  else if (currentTopLane == 1)
  {
    for (int index = 4; index < 8; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }
  else if (currentTopLane == 2)
  {
    for (int index = 8; index < 12; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }
  else if (currentTopLane == 3)
  {
    for (int index = 12; index < 16; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }
  else if (currentTopLane == 4)
  {
    for (int index = 16; index < 20; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }
  else if (currentTopLane == 5)
  {
    for (int index = 20; index < 24; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }
  else if (currentTopLane == 6)
  {
    for (int index = 24; index < 28; index++)
    {
      matrix_1.setPixelColor(topLine[index], 0, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 0, 0, 0);
    }
  }

  // Setting new TopLine
  if (i == 0)
  {
    for (int index = 0; index < 4; index++)
    {
      matrix_1.setPixelColor(topLine[index], 255, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 255, 0, 0);
    }
  }
  else if (i == 1)
  {
    for (int index = 4; index < 8; index++)
    {
      matrix_1.setPixelColor(topLine[index], 255, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 255, 0, 0);
    }
  }
  else if (i == 2)
  {
    for (int index = 8; index < 12; index++)
    {
      matrix_1.setPixelColor(topLine[index], 255, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 255, 0, 0);
    }
  }
  else if (i == 3)
  {
    for (int index = 12; index < 16; index++)
    {
      matrix_1.setPixelColor(topLine[index], 255, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 255, 0, 0);
    }
  }
  else if (i == 4)
  {
    for (int index = 16; index < 20; index++)
    {
      matrix_1.setPixelColor(topLine[index], 255, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 255, 0, 0);
    }
  }
  else if (i == 5)
  {
    for (int index = 20; index < 24; index++)
    {
      matrix_1.setPixelColor(topLine[index], 255, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 255, 0, 0);
    }
  }
  else if (i == 6)
  {
    for (int index = 24; index < 28; index++)
    {
      matrix_1.setPixelColor(topLine[index], 255, 0, 0);
      matrix_2.setPixelColor(topLineSecond[index], 255, 0, 0);
    }
  }
  }
  currentTopLane = i;
  matrix_1.show();
  matrix_2.show();
}
void dropPeace(int row, int player) {
  int count = 0;
  if (player == 1) {
    if (row == 0) {
      for (int i = 28; i >= firstRowBottom; i--)  {
        if (count != 3 && i > 3 + firstRowBottom) {
          matrix_1.setPixelColor(firstRow[i], 0, 0, 0);
          matrix_1.setPixelColor(firstRow[i - 4], 0, 250, 0);
          matrix_2.setPixelColor(seventhRow[i], 0, 0, 0);
          matrix_2.setPixelColor(seventhRow[i - 4], 0, 250, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      firstRowBottom = firstRowBottom + 4;
    }  
    if (row == 1) {
      for (int i = 28; i >= secondRowBottom; i--)  {
        if (count != 3 && i > 3 + secondRowBottom) {
          matrix_1.setPixelColor(secondRow[i], 0, 0, 0);
          matrix_1.setPixelColor(secondRow[i - 4], 0, 250, 0);
          matrix_2.setPixelColor(sixthRow[i], 0, 0, 0);
          matrix_2.setPixelColor(sixthRow[i - 4], 0, 250, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      secondRowBottom = secondRowBottom + 4;
    }  
    if (row == 2) {
      for (int i = 28; i >= thirdRowBottom; i--)  {
        if (count != 3 && i > 3 + thirdRowBottom) {
          matrix_1.setPixelColor(thirdRow[i], 0, 0, 0);
          matrix_1.setPixelColor(thirdRow[i - 4], 0, 250, 0);
          matrix_2.setPixelColor(fifthRow[i], 0, 0, 0);
          matrix_2.setPixelColor(fifthRow[i - 4], 0, 250, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      thirdRowBottom = thirdRowBottom + 4;
    }
    if (row == 3) {
      for (int i = 28; i >= fourthRowBottom; i--)  {
        if (count != 3 && i > 3 + fourthRowBottom) {
          matrix_1.setPixelColor(fourthRow[i], 0, 0, 0);
          matrix_1.setPixelColor(fourthRow[i - 4], 0, 250, 0);
          matrix_2.setPixelColor(fourthRow[i], 0, 0, 0);
          matrix_2.setPixelColor(fourthRow[i - 4], 0, 250, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      fourthRowBottom = fourthRowBottom + 4;
    }
    if (row == 4) {
      for (int i = 28; i >= fifthRowBottom; i--)  {
        if (count != 3 && i > 3 + fifthRowBottom) {
          matrix_1.setPixelColor(fifthRow[i], 0, 0, 0);
          matrix_1.setPixelColor(fifthRow[i - 4], 0, 250, 0);
          matrix_2.setPixelColor(thirdRow[i], 0, 0, 0);
          matrix_2.setPixelColor(thirdRow[i - 4], 0, 250, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      fifthRowBottom = fifthRowBottom + 4;
    }
    if (row == 5) {
      for (int i = 28; i >= sixthRowBottom; i--)  {
        if (count != 3 && i > 3 + sixthRowBottom) {
          matrix_1.setPixelColor(sixthRow[i], 0, 0, 0);
          matrix_1.setPixelColor(sixthRow[i - 4], 0, 250, 0);
          matrix_2.setPixelColor(secondRow[i], 0, 0, 0);
          matrix_2.setPixelColor(secondRow[i - 4], 0, 250, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      sixthRowBottom = sixthRowBottom + 4;
    }
    if (row == 6) {
      for (int i = 28; i >= seventhRowBottom; i--)  {
        if (count != 3 && i > 3 + seventhRowBottom) {
          matrix_1.setPixelColor(seventhRow[i], 0, 0, 0);
          matrix_1.setPixelColor(seventhRow[i - 4], 0, 250, 0);
          matrix_2.setPixelColor(firstRow[i], 0, 0, 0);
          matrix_2.setPixelColor(firstRow[i - 4], 0, 250, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      seventhRowBottom = seventhRowBottom + 4;
    }
  } else if (player == 2) {
    if (row == 0) {
      for (int i = 28; i >= firstRowBottom; i--)  {
        if (count != 3 && i > 3 + firstRowBottom) {
          matrix_1.setPixelColor(firstRow[i], 0, 0, 0);
          matrix_1.setPixelColor(firstRow[i - 4], 250, 0, 0);
          matrix_2.setPixelColor(seventhRow[i], 0, 0, 0);
          matrix_2.setPixelColor(seventhRow[i - 4], 250, 0, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      firstRowBottom = firstRowBottom + 4;
    }
    if (row == 1) {
      for (int i = 28; i >= secondRowBottom; i--)  {
        if (count != 3 && i > 3 + secondRowBottom) {
          matrix_1.setPixelColor(secondRow[i], 0, 0, 0);
          matrix_1.setPixelColor(secondRow[i - 4], 250, 0, 0);
          matrix_2.setPixelColor(sixthRow[i], 0, 0, 0);
          matrix_2.setPixelColor(sixthRow[i - 4], 250, 0, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      secondRowBottom = secondRowBottom + 4;
    }
    if (row == 2) {
      for (int i = 28; i >= thirdRowBottom; i--)  {
        if (count != 3 && i > 3 + thirdRowBottom) {
          matrix_1.setPixelColor(thirdRow[i], 0, 0, 0);
          matrix_1.setPixelColor(thirdRow[i - 4], 250, 0, 0);
          matrix_2.setPixelColor(fifthRow[i], 0, 0, 0);
          matrix_2.setPixelColor(fifthRow[i - 4], 250, 0, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      thirdRowBottom = thirdRowBottom + 4;
    }
    if (row == 3) {
      for (int i = 28; i >= fourthRowBottom; i--)  {
        if (count != 3 && i > 3 + fourthRowBottom) {
          matrix_1.setPixelColor(fourthRow[i], 0, 0, 0);
          matrix_1.setPixelColor(fourthRow[i - 4], 250, 0, 0);
          matrix_2.setPixelColor(fourthRow[i], 0, 0, 0);
          matrix_2.setPixelColor(fourthRow[i - 4], 250, 0, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      fourthRowBottom = fourthRowBottom + 4;
    }
    if (row == 4) {
      for (int i = 28; i >= fifthRowBottom; i--)  {
        if (count != 3 && i > 3 + fifthRowBottom) {
          matrix_1.setPixelColor(fifthRow[i], 0, 0, 0);
          matrix_1.setPixelColor(fifthRow[i - 4], 250, 0, 0);
          matrix_2.setPixelColor(thirdRow[i], 0, 0, 0);
          matrix_2.setPixelColor(thirdRow[i - 4], 250, 0, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      fifthRowBottom = fifthRowBottom + 4;
    }
    if (row == 5) {
      for (int i = 28; i >= sixthRowBottom; i--)  {
        if (count != 3 && i > 3 + sixthRowBottom) {
          matrix_1.setPixelColor(sixthRow[i], 0, 0, 0);
          matrix_1.setPixelColor(sixthRow[i - 4], 250, 0, 0);
          matrix_2.setPixelColor(secondRow[i], 0, 0, 0);
          matrix_2.setPixelColor(secondRow[i - 4], 250, 0, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      sixthRowBottom = sixthRowBottom + 4;
    }
    if (row == 6) {
      for (int i = 28; i >= seventhRowBottom; i--)  {
        if (count != 3 && i > 3 + seventhRowBottom) {
          matrix_1.setPixelColor(seventhRow[i], 0, 0, 0);
          matrix_1.setPixelColor(seventhRow[i - 4], 250, 0, 0);
          matrix_2.setPixelColor(firstRow[i], 0, 0, 0);
          matrix_2.setPixelColor(firstRow[i - 4], 250, 0, 0);
          count++;
        }
        matrix_1.show();
        matrix_2.show();
        delay(50);
        count = 0;
      }
      seventhRowBottom = seventhRowBottom + 4;
  }
  }
    matrix_1.show();
    matrix_2.show();
}
