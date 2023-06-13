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

int position = 0;

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

const int width = 6;
const int height = 7;

int helpMatrix[width][height] = {
  {0,0,0,0,0,0, 0},
  {0,0,0,0,0,0, 0},
  {0,0,0,0,0,0, 0},
  {0,0,0,0,0,0, 0},
  {0,0,0,0,0,0, 0},
  {0,0,0,0,0,0, 0},
  
};

int currentTopLane = -1;
int player = 1; // 1 = Player 1, 2 = Player 2
int joyX_1 = 0;
int joyX_2 = 0;

void border(int start, int end);
void fullBorder();
void resetMatrix();
void showTopLine(int i, int player);
void dropPeace(int i, int player);
void endScreen(int player);
void Line(int anfang, int ende);
int translatePosition(int position);
bool checkWin(int row, int col, int dx, int dy);
bool checkWinner();
bool checkZeros();

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

  resetMatrix();
}

void loop()
{
    if (player == 1) {
      while (digitalRead(25) != HIGH || translatePosition(position) >= 6) {
        joyX_1 = analogRead(32);
        if (joyX_1 > 2500 && position < 6) {
          position++;
        } else if (joyX_1 < 500 && position > 0)  {
          position--;
        }
        showTopLine(position, 1);        
        delay(250);
      }
      helpMatrix[translatePosition(position)][position] = player;
      dropPeace(position, 1);
      if (checkWinner()) {
        endScreen(player);
      }
      player = 2;
    }
    
    if (player == 2) {
      while (digitalRead(26) != HIGH || translatePosition(position) >= 6) {
        joyX_2 = analogRead(33);
        if (joyX_2 > 2500 && position > 0) {
          position--;
        } else if (joyX_2 < 500 && position < 6)  {
          position++;
        }
        delay(250);
        showTopLine(position, 2);
      }
      helpMatrix[translatePosition(position)][position] = player;
      dropPeace(position, 2);
      if (checkWinner()) {
        endScreen(player);
      }
      player = 1;
    }

    if (checkZeros()) {
      endScreen(0);
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

  border(224, 230);
  border(250, 256);
  border(272, 278);
  border(298, 304);
  border(320, 326);
  border(346, 352);
  border(368, 384); // oben links

  border(22, 26);
  border(40, 42);
  border(54, 56);
  border(70, 74);
  border(88, 90);
  border(102, 104);
  border(118, 122);
  border(136, 138);
  border(150, 152);
  border(166, 170); // unten rechts
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

void dropPeaceHelper(int bottom, int row[], int oppositeRow[], int player) {
  if (player == 1) {
    for (int i = 27; i >= bottom; i--) {
      if (i > 3 + bottom) {
        matrix_1.setPixelColor(row[i], 0, 0, 0);
        matrix_1.setPixelColor(row[i - 4], 0, 250, 0);
        matrix_2.setPixelColor(oppositeRow[i], 0, 0, 0);
        matrix_2.setPixelColor(oppositeRow[i - 4], 0, 250, 0);
      }
      matrix_1.show();
      matrix_2.show();
      delay(50);
    }
  } else if (player == 2) {
    for (int i = 27; i >= bottom; i--) {
      if (i > 3 + bottom) {
        matrix_1.setPixelColor(row[i], 0, 0, 0);
        matrix_1.setPixelColor(row[i - 4], 250, 0, 0);
        matrix_2.setPixelColor(oppositeRow[i], 0, 0, 0);
        matrix_2.setPixelColor(oppositeRow[i - 4], 250, 0, 0);
      }
      matrix_1.show();
      matrix_2.show();
      delay(50);
    }
  }
}

void dropPeace(int row, int player) {
  if (player == 1) {
    if (row == 0) {
      dropPeaceHelper(firstRowBottom, firstRow, seventhRow, player);
      firstRowBottom = firstRowBottom + 4;
    }  
    if (row == 1) {
      dropPeaceHelper(secondRowBottom, secondRow, sixthRow, player);
      secondRowBottom = secondRowBottom + 4;
    }  
    if (row == 2) {
      dropPeaceHelper(thirdRowBottom, thirdRow, fifthRow, player);
      thirdRowBottom = thirdRowBottom + 4;
    }
    if (row == 3) {
      dropPeaceHelper(fourthRowBottom, fourthRow, fourthRow, player);
      fourthRowBottom = fourthRowBottom + 4;
    }
    if (row == 4) {
      dropPeaceHelper(fifthRowBottom, fifthRow, thirdRow, player);
      fifthRowBottom = fifthRowBottom + 4;
    }
    if (row == 5) {
      dropPeaceHelper(sixthRowBottom, sixthRow, secondRow, player);
      sixthRowBottom = sixthRowBottom + 4;
    }
    if (row == 6) {
      dropPeaceHelper(seventhRowBottom, seventhRow, firstRow, player);
      seventhRowBottom = seventhRowBottom + 4;
    }
  } else if (player == 2) {
    if (row == 0) {
      dropPeaceHelper(firstRowBottom, firstRow, seventhRow, player);  
      firstRowBottom = firstRowBottom + 4;
    }
    if (row == 1) {
      dropPeaceHelper(secondRowBottom, secondRow, sixthRow, player);
      secondRowBottom = secondRowBottom + 4;
    }
    if (row == 2) {
      dropPeaceHelper(thirdRowBottom, thirdRow, fifthRow, player);
      thirdRowBottom = thirdRowBottom + 4;
    }
    if (row == 3) {
      dropPeaceHelper(fourthRowBottom, fourthRow, fourthRow, player);
      fourthRowBottom = fourthRowBottom + 4;
    }
    if (row == 4) {
      dropPeaceHelper(fifthRowBottom, fifthRow, thirdRow, player);
      fifthRowBottom = fifthRowBottom + 4;
    }
    if (row == 5) {
      dropPeaceHelper(sixthRowBottom, sixthRow, secondRow, player);
      sixthRowBottom = sixthRowBottom + 4;
    }
    if (row == 6) {
      dropPeaceHelper(seventhRowBottom, seventhRow, firstRow, player);
      seventhRowBottom = seventhRowBottom + 4;
    }
  }
    matrix_1.show();
    matrix_2.show();
}

void Line(int anfang, int ende) {
  for (int start =anfang; start < ende; start++) {
    matrix_1.setPixelColor(start, 100, 100, 100);
    matrix_2.setPixelColor(start, 100, 100, 100);
  }
}

void endScreen(int player) {
  matrix_1.clear();
  matrix_2.clear();
  // O 
  Line(9, 15);
  Line(17, 18);
  Line(22, 23);
  Line(25, 26);
  Line(30, 31);
  Line(33, 39);

  // V
  Line(49, 53);
  Line(58, 59);
  Line(70, 71);
  Line(74, 75);
  Line(81, 85);

  // E
  Line(97, 103);
  Line(105, 106);
  Line(107, 108);
  Line(110, 111);
  Line(113, 114);
  Line(116, 117);
  Line(118, 119);
  Line(121, 122);
  Line(126, 127);

  // R
  Line(137, 143);
  Line(145, 146);
  Line(147, 148);
  Line(155, 157);
  Line(158, 159);
  Line(161, 164);
  Line(165, 167);

  // G
  Line(369, 375);
  Line(366, 367);
  Line(361, 362);
  Line(358, 359);
  Line(355, 356);
  Line(353, 354);
  Line(348, 351);
  Line(345, 346);

  // A
  Line(329, 335);
  Line(326, 327);
  Line(323, 324);
  Line(316, 317);
  Line(313, 314);
  Line(305,311);

  // M
  Line(289, 295);
  Line(282, 283);
  Line(276, 277);
  Line(266, 267);
  Line(257,263);

  // E
  Line(241, 247);
  Line(238, 239);
  Line(236, 237);
  Line(233, 234);
  Line(230, 231);
  Line(227, 228);
  Line(225, 226);
  Line(222, 223);
  Line(217, 218);

  matrix_1.show();
  matrix_2.show();
  delay(10000);

  if (player == 1) {
    matrix_1.clear();
    matrix_2.clear();

    // P
    Line(336, 343);
    Line(332, 333);
    Line(329, 330);
    Line(326, 327);
    Line(323, 324);
    Line(313, 317);

    // 1
    Line(256, 263);
    Line(266, 267);
    Line(276, 277);

    // W
    Line(10, 15);
    Line(22, 23);
    Line(26, 27);
    Line(38, 39);
    Line(42, 47);

    // I
    Line(57, 63);

    // N
    Line(73, 79);
    Line(82, 83);
    Line(92, 93);
    Line(100, 101);
    Line(105, 111);

    // S
    Line(121, 122);
    Line(123, 127);
    Line(129, 130);
    Line(132, 133);
    Line(134, 135);
    Line(137, 138);
    Line(139, 140);
    Line(142, 143);
    Line(145, 146);
    Line(148, 149);
    Line(150, 151);
    Line(153, 156);
    Line(158, 159);

    matrix_1.show();
    matrix_2.show();
    delay(10000);

  } else if (player == 2) {
    matrix_1.clear();
    matrix_2.clear();

    // P
    Line(336, 343);
    Line(332, 333);
    Line(329, 330);
    Line(326, 327);
    Line(323, 324);
    Line(313, 317);

    // 2
    Line(262, 263);
    Line(258, 259);
    Line(255, 257);
    Line(252, 253);
    Line(249, 250);
    Line(244, 246);
    Line(240, 241);
    Line(265, 266);
    Line(277, 278);
    Line(270, 273);


    // W
    Line(10, 15);
    Line(22, 23);
    Line(26, 27);
    Line(38, 39);
    Line(42, 47);

    // I
    Line(57, 63);

    // N
    Line(73, 79);
    Line(82, 83);
    Line(92, 93);
    Line(100, 101);
    Line(105, 111);

    // S
    Line(121, 122);
    Line(123, 127);
    Line(129, 130);
    Line(132, 133);
    Line(134, 135);
    Line(137, 138);
    Line(139, 140);
    Line(142, 143);
    Line(145, 146);
    Line(148, 149);
    Line(150, 151);
    Line(153, 156);
    Line(158, 159);

    matrix_1.show();
    matrix_2.show();
    delay(10000);

  }
  ESP.restart();
}

int translatePosition(int position) {

  if (position == 0) {
    if (firstRowBottom == 0) {
      return 0;
    }
    return firstRowBottom / 4;
  }
  if (position == 1) {
    if (secondRowBottom == 0) {
      return 0;
    }
    return secondRowBottom / 4;
  }
  if (position == 2) {
    if (thirdRowBottom == 0) {
      return 0;
    }
    return thirdRowBottom / 4;
  }
  if (position == 3) {
    if (fourthRowBottom == 0) {
      return 0;
    }
    return fourthRowBottom / 4;
  }
  if (position == 4) {
    if (fifthRowBottom == 0) {
      return 0;
    }
    return fifthRowBottom / 4;
  }
  if (position == 5) {
    if (sixthRowBottom == 0) {
      return 0;
    }
    return sixthRowBottom / 4;
  }
  if (position == 6) {
    if (seventhRowBottom == 0) {
      return 0;
    }
    return seventhRowBottom / 4;
  }
  return 0;
}

bool checkWin(int row, int col, int dx, int dy) {
    int count = 0;
    int value = helpMatrix[row][col];

    while (row >= 0 && row < width && col >= 0 && col < height && helpMatrix[row][col] == value && value != 0) {
        row += dx;
        col += dy;
        count++;
    }

    return count >= 4;
}

bool checkWinner() {
    // Überprüfen der horizontalen Reihen
    for (int i = 0; i < width; i++) {
        for (int j = 0; j <= height - 4; j++) {
            if (checkWin(i, j, 0, 1))
                return true;
        }
    }

    // Überprüfen der vertikalen Spalten
    for (int i = 0; i <= width - 4; i++) {
        for (int j = 0; j < height; j++) {
            if (checkWin(i, j, 1, 0))
                return true;
        }
    }

    // Überprüfen der Diagonalen von links oben nach rechts unten
    for (int i = 0; i <= width - 4; i++) {
        for (int j = 0; j <= height - 4; j++) {
            if (checkWin(i, j, 1, 1))
                return true;
        }
    }

    // Überprüfen der Diagonalen von rechts oben nach links unten
    for (int i = 4 - 1; i < width; i++) {
        for (int j = 0; j <= COLS - height; j++) {
            if (checkWin(i, j, -1, 1))
                return true;
        }
    }
    return false;
}

bool checkZeros() {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (helpMatrix[i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}