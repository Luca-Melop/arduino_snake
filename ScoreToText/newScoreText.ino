#include "Talkie.h" //to let the speaker talk
#include "Vocab_US_Large.h" //all the words used in my Program
#include "Vocab_Special.h"

Talkie voice; //Talkie bereit machen
int highscore; //die variable Highscore inittialiseren

void displayScore() {
  //Say the score via the speaker --> only till 29, but easily scalable
  if (highscore == 1) { //checkt wie hoch der Score ist
  voice.say(sp3_ONE); //sagt den score über Speaker
} else if (highscore == 2) {
  voice.say(sp3_TWO);
} else if (highscore == 3) {
  voice.say(sp3_THREE);
} else if (highscore == 4) {
  voice.say(sp3_FOUR);
} else if (highscore == 5) {
  voice.say(sp3_FIVE);
} else if (highscore == 6) {
  voice.say(sp3_SIX);
} else if (highscore == 7) {
  voice.say(sp3_SEVEN);
} else if (highscore == 8) {
  voice.say(sp3_EIGHT);
} else if (highscore == 9) {
  voice.say(sp3_NINE);
} else if (highscore == 10) {
  voice.say(sp3_TEN);
} else if (highscore == 11) {
  voice.say(sp3_ELEVEN);
} else if (highscore == 12) {
  voice.say(sp3_TWELVE);
} else if (highscore == 13) {
  voice.say(sp3_THIRTEEN);
} else if (highscore == 14) {
  voice.say(sp3_FOURTEEN);
} else if (highscore == 15) {
  voice.say(sp3_FIFTEEN);
} else if (highscore == 16) {
  voice.say(sp3_SIXTEEN);
} else if (highscore == 17) {
  voice.say(sp3_SEVENTEEN);
} else if (highscore == 18) {
  voice.say(sp3_EIGHTEEN);
} else if (highscore == 19) {
  voice.say(sp3_NINETEEN);
} else if (highscore == 20) {
  voice.say(sp3_TWENTY);
} else if (highscore == 21) {
  voice.say(sp3_TWENTY);
  voice.say(sp3_ONE);
} else if (highscore == 22) {
  voice.say(sp3_TWENTY);
  voice.say(sp3_TWO);
} else if (highscore == 23) {
  voice.say(sp3_TWENTY);
  voice.say(sp3_THREE);
} else if (highscore == 24) {
  voice.say(sp3_TWENTY);
  voice.say(sp3_FOUR);
} else if (highscore == 25) {
  voice.say(sp3_TWENTY);
  voice.say(sp3_FIVE);
} else if (highscore == 26) {
  voice.say(sp3_TWENTY);
  voice.say(sp3_SIX);
} else if (highscore == 27) {
  voice.say(sp3_TWENTY);
  voice.say(sp3_SEVEN);
} else if (highscore == 28) {
  voice.say(sp3_TWENTY);
  voice.say(sp3_EIGHT);
}else if (highscore == 29) {
  voice.say(sp3_TWENTY);
  voice.say(sp3_NINE);
}
}

void setup(){
  Serial.begin(9600); //set baud to 9600
}
int previousScore = 0; //to check that the score only went up by 1 (to eliminate errors)
void loop(){
  int value = Serial.read(); //read the Value from the RX pin

  if(value > 0){ //If value is correct
    highscore = value; 
    if(highscore == previousScore+1 || highscore == 1){ //if the value went up by one (to eliminte errors)
        displayScore(); //say the highscore
        previousScore = highscore; //update previousScore
        Serial.println(highscore);
    }
  }
  
}
