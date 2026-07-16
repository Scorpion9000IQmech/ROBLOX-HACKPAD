#include <Wire.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define JOY_X A8
#define JOY_Y A7
#define JOY_SW A6

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

const int ROWS = 3;
const int COLS = 3;
bool keyState[ROWS][COLS];
bool lastJoyButton = HIGH;

const int rowPins[3] = {
  3,
  4,
  5
};

const int colPins[3] = {
  0,
  1,
  2
};

char keys[ROWS][COLS] =
{
 {'D','W','Q'},
 {'X','E','A'},
 {'C','Z','S'}
};

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

void setup() {
  // put your setup code here, to run once:
  Keyboard.begin();
  Mouse.begin();

  pinMode(JOY_SW, INPUT_PULLUP);


  for(int i=0;i<ROWS;i++)
  {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }


  for(int i=0;i<COLS;i++)
  {
    pinMode(colPins[i], INPUT_PULLUP);
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    while(true);
  }

}
void loop() {
  // put your main code here, to run repeatedly:
  scanMatrix();
  updateJoystick();
}

void scanMatrix()
{

  for(int row=0; row<ROWS; row++)
  {

    digitalWrite(rowPins[row], LOW);


    for(int col=0; col<COLS; col++)
    {

      bool pressed = digitalRead(colPins[col]) == LOW;


      if(pressed && !keyState[row][col])
      {
        Keyboard.press(keys[row][col]);
        keyState[row][col] = true;
      }


      if(!pressed && keyState[row][col])
      {
        Keyboard.release(keys[row][col]);
        keyState[row][col] = false;
      }

    }


    digitalWrite(rowPins[row], HIGH);

  }

}

void updateJoystick()
{
    // Mouse code

  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);


  int moveX = 0;
  int moveY = 0;

  if(x < 300)
  {
    moveX=-5;
  }

  if(x > 700)
  {
    moveX=5;
  }


  if(y < 300)
  {
    moveY = -5;
  }

  if(y > 700)
  {
    moveY = 5;
  }


  Mouse.move(moveX/2, moveY/2);


  bool currentButton = digitalRead(JOY_SW);


  if(currentButton == LOW && lastJoyButton == HIGH)
  {
    Mouse.click();
  }


  lastJoyButton = currentButton;

}
