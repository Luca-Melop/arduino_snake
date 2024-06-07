#include <LedMatrix.h>   //Library for game
#include "LedControl.h"  //import Library for scrolling text



LedControl lc = LedControl(12, 11, 10, 8);  // initialize for scrolling text

int mc[] = { //the order of the matrices (for Library game)
  3, 2, 1, 0,
  7, 6, 5, 4
};

bool md[] = { //If the matrices should be rotated by 180°, more on the documentation of the Library
  false, false, false, false,
  false, false, false, false
};

const int iNumMatrixRows = 4; //how many rows do I have?
const int iNumMatrixColumns = 2; //and how many Columns

LedMatrix lm = LedMatrix(12,                  //Data pin
                         11,                  //CLK pin
                         10,                  //CS pin
                         8,                   //intensity
                         mc,                  //matrix configuration
                         md,                  //matrix directions (are the matrices rotated by 180°)
                         iNumMatrixRows,      //the number of rows
                         iNumMatrixColumns);  //the number of columns



//Letters for scrolling text
byte A[8] = { 0x0, 0x18, 0x24, 0x42, 0x7e, 0x42, 0x42, 0x0 };  //alle Buchstaben von binär zu hexadezimal umgeändert, wegen Platz
byte B[8] = { 0x0, 0x7c, 0x42, 0x42, 0x7c, 0x42, 0x42, 0x7c };
byte E[8] = { 0x0, 0x7e, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x7e };
byte K[8] = { 0x0, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44 };
byte L[8] = { 0x0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7c };
byte M[8] = { 0x0, 0x42, 0x66, 0x5a, 0x42, 0x42, 0x42, 0x42 };
byte N[8] = { 0x0, 0x42, 0x62, 0x72, 0x5a, 0x4e, 0x46, 0x42 };
byte S[8] = { 0x0, 0x3e, 0x40, 0x40, 0x3c, 0x2, 0x2, 0x7c };
byte Y[8] = { 0x0, 0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10 };
byte null[8] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
int pushButton = A2;

byte first_start_text_0to3[32] = {}; //Der erste Teil (Snake), die Matrixen 0 bis 3
byte second_start_text_0to3[32] = {}; //Der erste Teil (Snake), die Matrixen 4 bis 7
byte first_start_text_4to7[32] = {};//Der zweite Teil (By LM), die Matrixen 0 bis 3
byte second_start_text_4to7[32] = {};//Der zweite Teil (By LM), die Matrixen 4 bis 7


//Game Over Animation
int gameOverSmiley[16][2] = { { 11, 3 }, { 20, 3 }, { 13, 11 }, { 14, 11 }, { 15, 11 }, { 16, 11 }, { 17, 11 }, { 18, 11 }, { 11, 12 }, { 12, 12 }, { 19, 12 }, { 20, 12 }, { 10, 13 }, { 10, 14 }, { 21, 13 }, { 21, 14 } };  //sad smiley
int digital_mic = 5; //Pin of Mic --> Rage quit sensor

void setup() {
  //clears the display and set all Leds to be turned off
  lm.ClearDisplay();

pinMode(A5, INPUT); //Value of brightness
pinMode(8, OUTPUT); //LED for ragequit sensor

  pinMode(digital_mic, INPUT); //0 or 1, screaming = 0

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(pushButton, INPUT_PULLUP);  //pushbutton = change scrolling texts
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  Serial.begin(9600);

  



  //Set the letters to the array
  for (int i = 0; i < 8; i++) {
    first_start_text_0to3[i] = S[i];
    first_start_text_0to3[i + 8] = A[i];
    first_start_text_0to3[i + 16] = E[i];
    first_start_text_0to3[i + 24] = null[i];
  }

  for (int i = 0; i < 8; i++) {
    second_start_text_0to3[i] = B[i];
    second_start_text_0to3[i + 8] = null[i];
    second_start_text_0to3[i + 16] = L[i];
    second_start_text_0to3[i + 24] = null[i];
  }

  //Matrixes 4 to 7
  for (int i = 0; i < 8; i++) {
    first_start_text_4to7[i] = N[i];
    first_start_text_4to7[i + 8] = K[i];
    first_start_text_4to7[i + 16] = null[i];
    first_start_text_4to7[i + 24] = null[i];
  }

  for (int i = 0; i < 8; i++) {
    second_start_text_4to7[i] = Y[i];
    second_start_text_4to7[i + 8] = null[i];
    second_start_text_4to7[i + 16] = M[i];
    second_start_text_4to7[i + 24] = null[i];
  }

  start();
  //generate initial food
  generateFood();
}

int snakeX = 16;  //start the snake **HEAD** in the middle -->iNumMatrixRows * 8 / 2;
int snakeY = 8;   //Y position of snakes head
int preX;         //to add this LED if the snake has grown by one LED
int preY;
int snake[200][2] = { { snakeX, snakeY } };  //200 is the max length for the snake --> theoretically would be 16*32 = 512 possible
char direction = 'l';
int foodX;    //FoodPosition
int foodY;
int score = 0;          //score & is also equal to the snake length (score + 1 = length)
bool foodState = true;  //if Food should be turned on or off --> for flickering effect

bool startBool = true; //to check if start sequence is on or off

const int flickerFrequencyFood = 50;                    // flicker frequency in Hz
const int delayTimeFood = 1000 / flickerFrequencyFood;  // delay time in ms
unsigned long lastFlickerTimeFood = 0;                  // variable to track the last time the Food LED was flicked

int updateFrequencySnake = 2;                      // flicker frequency in Hz
int delayTimeSnake = 1000 / updateFrequencySnake;  // delay time in ms
unsigned long lastUpdateTimeSnake = 0;             // variable to track the last time the Snake position was updated


void start() {
  digitalWrite(8, LOW); //Set the ragequit LED off

  //scrollling Text --> do only once
  while (startBool) { //Wenn der Start aktiv ist. Also wenn die Startsequenz abgespielt werden soll
    while (analogRead(pushButton) > 300) { //solange wie der PushButton nicht gedrückt wird
      first_start_text(); //führe die oben definierte Funktion aus
      brightness(); //check and adjust brightness
    }
    delay(100); //kurzer Delay

    while (analogRead(pushButton) > 300) {//solange wie der PushButton nicht gedrückt wird
      second_start_text(); //führe die oben definierte Funktion aus
      brightness(); //check and adjust brightness
      startBool = false; //Die Start Sequenz ist vorbei, darum bis der Boolean auf false gesetzt.
    }
    delay(100); //kurzer Delay
  }
}


void updateSnake() {

  // calculate the current time --> damit es nicht immer läuft und damit man die Geschwindigkeit steuern kann
  unsigned long currentTimeSnake = millis();

  // check if it's time to update the Snake
  if (currentTimeSnake - lastUpdateTimeSnake >= delayTimeSnake) {


    //preX/Y ist die LED die gerade ausgemacht wurde, also eine LED hinter der Schlange. Diese LED brauchen wir, wenn die SChlange wachsen will oder eins nach vorne gehen will
    preX = snake[score][0]; //preX/Y ist immer so weit hinter dem Schlangekopf wie der Score also der Score + 1 und länge sind gleich
    preY = snake[score][1];
    lm.SetLed(snake[score][0], snake[score][1], false); //setze diese LED aus um Schlange nach vore zu bewegen
    lm.UpdateMatrix();  //this is needed to send the changes to the matrices

    //push everytime snake one Led forward Verschiebe jedes Array im Array um Schlange zu bewegen
    for (int i = score; i > 0; i--) {
      snake[i][0] = snake[i - 1][0];
      snake[i][1] = snake[i - 1][1];
    }


    if (analogRead(A0) > 900 && direction != 'l') { //Wenn Joystick nach rechts und Richtung nicht nach links(sonst würde die Schlange in sich selber laufen) mache Direction nach rechts
      direction = 'r';
    } else if (analogRead(A0) < 100 && direction != 'r') {//Wenn Joystick nach links und Richtung nicht nach reichts(sonst würde die Schlange in sich selber laufen) mache Direction nach links
      direction = 'l';
    }

    if (analogRead(A1) > 900 && direction != 'u') {//Wenn Joystick nach oben und Richtung nicht nach unten(sonst würde die Schlange in sich selber laufen) mache Direction nach oben
      direction = 'd';
    } else if (analogRead(A1) < 100 && direction != 'd') {//Wenn Joystick nach unten und Richtung nicht nach oben(sonst würde die Schlange in sich selber laufen) mache Direction nach unten
      direction = 'u';
    }


  //Hier wird ja nach der Richtung der Schlangenkopf bewegt
    if (direction == 'r') {
      //Serial.println("right");
      snake[0][0]++;
    } else if (direction == 'l') {
      //Serial.println("left");
      snake[0][0]--;
    } else if (direction == 'u') {
      //Serial.println("up");
      snake[0][1]++;
    } else if (direction == 'd') {
      //Serial.println("down");
      snake[0][1]--;
    }


    //Wall detection --> walls = death --> Wànde sind bei X0, X15, Y0 und Y31
    if (snake[0][1] < 0 || snake[0][1] > 15 || snake[0][0] < 0 || snake[0][0] > 31) {
      gameOver();
    }

    //snake detection --> snake in snake = death
    for (int i = 1; i <= score; i++) { //Mit dieser Loop wird überprüft ob der Schlangenkopf mit der Schlange kollidiert ist
      if (snake[0][0] == snake[i][0] && snake[0][1] == snake[i][1]) {
        gameOver();
      }
    }

    //check if snake has eaten an apple --> if yes, the snake should grow
    if (snake[0][0] == foodX && snake[0][1] == foodY) {
      grow();
      //Serial.println("snake grown");
    }




  //hier wird dann die Schlange angeschalten, damit man sie auf der Matrix sieht
    for (int i = 0; i <= score; i++) {
      lm.SetLed(snake[i][0], snake[i][1], true);
      lm.UpdateMatrix();  //this is needed to send the changes to the matrices
    }


    // update the last update time, for the time managing
    lastUpdateTimeSnake = currentTimeSnake;
  }
}
void generateFood() {
  foodX = random(0, 31);
  foodY = random(0, 15);
}
void grow() {
  score++;
  delay(10);
  Serial.println(score);
  Serial.write(score); // Send Score to Arduino ESP2886 Chip, to push Highscore to Website and say the score (Talkie Library not compatible with LedControl)
  
  updateFrequencySnake += 1;                     //make the game faster every time a food gets eaten
  delayTimeSnake = 1000 / updateFrequencySnake;  //also update the delay, otherwise it will have no effect
  generateFood();                                //to get new food
  tone(3, 1500, 100);

  snake[score][0] = preX;
  snake[score][1] = preY;
}
void(* resetFunc) (void) = 0; //declare reset function @ address 0
void gameOver() {
  tone(3, 1500, 115);
  delay(100);
  tone(3, 1200, 200);
  delay(150);
  tone(3, 800, 300);

  lm.ClearDisplay();
  lm.UpdateMatrix();

  //I know it would be possible to do it with a for loop (for(int i = 0; i<16; i++)) --> but strangly if I do this I can't upload my code 9 out of 10 times, and if I can upload it, the LED's show some bullshit
  //If you wanna know why, I have no Idea, maybe my Arduino is broken :(
  //To try on a other Arduino just uncomment this code below:
  //for(int i=0; i<16; i++){
    //lm.SetLed(gameOverSmiley[i][0],gameOverSmiley[i][1], true);
  //}
  lm.SetLed(gameOverSmiley[0][0], gameOverSmiley[0][1], true);
  lm.SetLed(gameOverSmiley[1][0], gameOverSmiley[1][1], true);
  lm.UpdateMatrix();
  delay(400);
  lm.SetLed(gameOverSmiley[2][0], gameOverSmiley[2][1], true);
  lm.SetLed(gameOverSmiley[3][0], gameOverSmiley[3][1], true);
  lm.SetLed(gameOverSmiley[4][0], gameOverSmiley[4][1], true);
  lm.SetLed(gameOverSmiley[5][0], gameOverSmiley[5][1], true);
  lm.SetLed(gameOverSmiley[6][0], gameOverSmiley[6][1], true);
  lm.SetLed(gameOverSmiley[7][0], gameOverSmiley[7][1], true);
  lm.UpdateMatrix();
  delay(400);
  lm.SetLed(gameOverSmiley[8][0], gameOverSmiley[8][1], true);
  lm.SetLed(gameOverSmiley[9][0], gameOverSmiley[9][1], true);
  lm.SetLed(gameOverSmiley[10][0], gameOverSmiley[10][1], true);
  lm.SetLed(gameOverSmiley[11][0], gameOverSmiley[11][1], true);
  lm.UpdateMatrix();
  delay(400);
  lm.SetLed(gameOverSmiley[12][0], gameOverSmiley[12][1], true);
  lm.SetLed(gameOverSmiley[13][0], gameOverSmiley[13][1], true);
  lm.SetLed(gameOverSmiley[14][0], gameOverSmiley[14][1], true);
  lm.SetLed(gameOverSmiley[15][0], gameOverSmiley[15][1], true);
  lm.UpdateMatrix();

  delay(4000);
  resetFunc(); 
}
void brightness(){
  for(int i = 0; i<8; i++){
      lm.SetIntensity(16 - (analogRead(A5)/64), i); //divide by 64 because sensor has values from 0 to 1024 but Matrix has only 0 to 15 intensities
      Serial.println(16 - (analogRead(A5)/64));
  }                                               //then subtract this value from 16 to get lower intensity if darker, not the other way around
}

void loop() {
  updateSnake();
  brightness();

    if  (digitalRead(digital_mic)==0) 
  {
    digitalWrite(8, HIGH); //Set LED on
      gameOver();;
      
  }

  // calculate the current time (for Food)
  unsigned long currentTimeFood = millis();
  // check if it's time to flicker the Food LED
  if (currentTimeFood - lastFlickerTimeFood >= delayTimeFood) {
    // toggle the state of the LED
    lm.SetLed(foodX, foodY, foodState);
    lm.UpdateMatrix();
    foodState = !foodState;

    // update the last flicker time
    lastFlickerTimeFood = currentTimeFood;
  }

}




//Scrolling Text------------------------------------------------------------
void first_start_text() {
  byte col_0;
  byte col_01;
  for (int y = 0; y < 32; y++) {
    lc.setColumn(y / 8, 7 - (y % 8), first_start_text_0to3[y]);  // Bsp: y=10; y/8=1; y 8= y MOD 8
    lc.setColumn(y / 8 + 4, 7 - (y % 8), first_start_text_4to7[y]);
  }

  // Laufschrift nach oben schieben
  col_0 = first_start_text_0to3[0];  // Die erste Kolonne merken, da die am Ende ganz rechts
  col_01 = first_start_text_4to7[0];
  for (int y = 0; y < 31; y++) {  // nur bis 30 resp. 31, da unten die letzte Kolonne neu zugeordnet wird
    first_start_text_0to3[y] = first_start_text_0to3[y + 1];
    first_start_text_4to7[y] = first_start_text_4to7[y + 1];
  }
  first_start_text_0to3[31] = col_0;   // Hier die erste Kolonne am Ende wieder anhängen
  first_start_text_4to7[31] = col_01;  // Hier die erste Kolonne am Ende wieder anhängen
}
void second_start_text() {
  byte col_0;
  byte col_01;
  for (int y = 0; y < 32; y++) {
    lc.setColumn(y / 8, 7 - (y % 8), second_start_text_0to3[y]);      // Bsp: y=10; y/8=1; y 8= y MOD 8
    lc.setColumn(y / 8 + 4, 7 - (y % 8), second_start_text_4to7[y]);  // Bsp: y=10; y/8=1; y 8= y MOD 8
  }

  // Laufschrift nach oben schieben
  col_0 = second_start_text_0to3[0];  // Die erste Kolonne merken, da die am Ende ganz rechts
  col_01 = second_start_text_4to7[0];
  for (int y = 0; y < 31; y++) {  // nur bis 30 resp. 31, da unten die letzte Kolonne neu zugeordnet wird
    second_start_text_0to3[y] = second_start_text_0to3[y + 1];
    second_start_text_4to7[y] = second_start_text_4to7[y + 1];
  }
  second_start_text_0to3[31] = col_0;   // Hier die erste Kolonne am Ende wieder anhängen
  second_start_text_4to7[31] = col_01;  // Hier die erste Kolonne am Ende wieder anhängen
}