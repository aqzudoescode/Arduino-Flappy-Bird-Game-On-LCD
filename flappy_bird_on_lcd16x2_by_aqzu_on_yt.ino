// code was written by Aqzu on 02.02.2023
// youtube: https://www.youtube.com/channel/UCLNdeTdHRBPOLwEvKxwLjFg
// about that project: a simple lcd game, based on a basic flappy bird game
// add my discord for any questions: Aqzu#4761
// code, schematics and component list are now also available on TinkerCad under profile name: Aqzu

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define x 13
#define y 14
#define z 15

int a=21;
int b=22;
int c=23;
int spaceGenerator=24;

bool gameOver;

byte birdMoveOne [8] = {
  B00000,
  B01110,
  B01101,
  B00110,
  B11110,
  B01110,
  B10010,
  B00000
};

//down 3 block obstacle
byte obstacleDown[8] = {
  B00000, B11111, B11111, B11111, B11111, B11111, B11111, B11111,
};
byte obstacleLeftDown[8] = {
  B00000, B00111, B00011, B00011, B00011, B00011, B00011, B00011,
};
byte obstacleRightDown[8] = {
  B00000, B11100, B11000, B11000, B11000, B11000, B11000, B11000,
};
//up 3 block obstacle
byte obstacleUp[8] = {
  B11111, B11111, B11111, B11111, B11111, B11111, B11111, B00000,
};
byte obstacleLeftUp[8] = {
  B00011, B00011, B00011, B00011, B00011, B00011, B00111, B00000,
};
byte obstacleRightUp[8] = {
  B11000, B11000, B11000, B11000, B11000, B11000, B11100, B00000,
};

int preGameState=0;
int switchState=0;

int buttonPressed=0;
int buttonPosition=7;

unsigned long birdTime = millis();
unsigned long scoreTime = millis();
unsigned long obstacleLowerTime = millis();
unsigned long obstacleUpperTime = millis();
unsigned long birdCrash = millis();
unsigned long birdCrash2 = millis();
unsigned long scoreDivision = millis();

unsigned int number=16;

int deductionLower=0;
int deductionUpper=0;

long unsigned int terrainGeneration;

int scoreMultiplier;

bool birdUp=0;

void creationPage() {
  lcd.setCursor(0, 0);
  lcd.print("FLAPPY BIRD GAME");
  lcd.setCursor(0, 1);
  lcd.print("BY AQZU");
}
void menu() {
  lcd.home();
  lcd.print("Press to start");
  delay(500);
}

void initializeUpperObstacleGraphics() { //0
  lcd.setCursor(a-deductionUpper, 0);
  lcd.write(5);
  lcd.setCursor(b-deductionUpper, 0);
  lcd.write(6);
  lcd.setCursor(c-deductionUpper, 0);
  lcd.write(4);
  delay(150);
  lcd.setCursor(a-deductionUpper, 0);
  lcd.write(6);
  lcd.setCursor(b-deductionUpper, 0);
  lcd.write(6);
  lcd.setCursor(c-deductionUpper, 0);
  lcd.write(" ");
}
void initializeLowerObstacleGraphics() { //1
  lcd.setCursor(x-deductionLower, 1);
  lcd.write(8);
  lcd.setCursor(y-deductionLower, 1);
  lcd.write(9);
  lcd.setCursor(z-deductionLower, 1);
  lcd.write(7);
  delay(150);
  lcd.setCursor(x-deductionLower, 1);
  lcd.write(9);
  lcd.setCursor(y-deductionLower, 1);
  lcd.write(9);
  lcd.setCursor(z-deductionLower, 1);
  lcd.write(" ");
}
void setup() {
  lcd.begin(16, 2);
  lcd.createChar(11, birdMoveOne);
  //down
  lcd.createChar(9, obstacleDown);
  lcd.createChar(8, obstacleLeftDown);
  lcd.createChar(7, obstacleRightDown);
  //up
  lcd.createChar(6, obstacleUp);
  lcd.createChar(5, obstacleLeftUp);
  lcd.createChar(4, obstacleRightUp);
  
  int j;
  j=0;
  while (j == 0) {
  creationPage();
  
  preGameState = digitalRead(buttonPosition);
    if (preGameState == HIGH && buttonPressed == 0) {
        lcd.clear();
    j=1;
        buttonPressed=1;
        goto clickToStartMenu;
    }
  }
clickToStartMenu:
  while(j == 1) {
    menu();
    preGameState = digitalRead(buttonPosition);
      if (preGameState == HIGH && buttonPressed == 1) {
          lcd.clear();
          j=2;
          buttonPressed=0;
          break;
      }
  }
}
void loop() {
  if ((millis()- scoreTime)>=200) {
    scoreTime=millis();
    scoreMultiplier=scoreMultiplier+1;
  }
  byte digits = (scoreMultiplier > 9999) ? 5 : (scoreMultiplier > 999) ? 4 : (scoreMultiplier > 99) ? 3 : (scoreMultiplier > 9) ? 2 : 1;
  lcd.setCursor(number-digits, 0);
  lcd.print(scoreMultiplier);
  terrainGeneration = random(2);
  switch (terrainGeneration) {
  case 0:
      if ((millis() - obstacleLowerTime) >= 200) {
        obstacleLowerTime=millis();
        initializeLowerObstacleGraphics();
        deductionLower=deductionLower+1;
        if(deductionLower == 16) {
          deductionLower=0;
        }
      }
  case 1:
      if ((millis() - obstacleUpperTime) >= 200) {
        obstacleUpperTime=millis();
        initializeUpperObstacleGraphics();
        deductionUpper=deductionUpper+1;
        if(deductionUpper == spaceGenerator) {
          deductionUpper=0;
          //digital reset
          a=13;
          b=14;
      c=15;
          spaceGenerator=16;
        }
      }
    if((millis()- birdCrash) >= 50) {
      birdCrash=millis();
      switchState = digitalRead(buttonPosition);
      if (switchState == HIGH && birdUp == false) {
        lcd.setCursor(3, 0);
        lcd.write(11);
        lcd.setCursor(3, 1);
        lcd.print(" ");
        lcd.clear();
        birdUp = true;
        birdCrash2=millis();
      }
      else if (birdUp == true) {
        lcd.setCursor(3, 0);
        lcd.write(11);
      }
      else if (birdUp == false) {
        lcd.setCursor(3, 0);
        lcd.print(" ");
    lcd. setCursor(3, 1);
        lcd.write(11);
      }    
    }
    if ((millis()-birdCrash2)>=1000 && birdUp == true) {
      birdCrash2=millis();
      lcd.setCursor(3, 1);
      lcd.write(11);
      lcd.setCursor(3, 0);
      lcd.print(" ");
      birdUp = false;
  }
    if (birdUp == false && (y-deductionLower == 3)) {
      gameOver = true;
      lcd.clear();
    }
    if (birdUp == true && (b-deductionUpper == 3)) {
      gameOver = true;
      lcd.clear();
    }
    while (gameOver == true) {
      lcd.home();
      lcd.print("Game Over");
      lcd.setCursor(0, 1);
    lcd.print("Tap to retry");
      scoreMultiplier=0;
      deductionUpper=0;
      deductionLower=0;
      switchState = digitalRead(buttonPosition);
      if (switchState == HIGH) {
        lcd.clear();
        gameOver=0;
        break;
      }
    }
  }
}
