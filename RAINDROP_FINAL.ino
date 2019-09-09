//Declaring which pins are used for anode, and which pins for cathode
const byte ANODE_PINS[8] = {13, 12, 11, 10, 9, 8, 7, 6};
const byte CATHODE_PINS[8] = {A3, A2, A1, A0, 2, 3, 4, 5};

//Declaring some of the other variables used during the program
const  int  TIME_DELAY = 400;
const int BUTTON = A4;
const int BUTTON2 = A5;
const int DEBOUNCE_DELAY = 50;
int counter = 0;  
int counter2 = 0;  


void setup() {
  
  for (int i = 0; i < 8; i++) {
    
    //Setting each of the pins to output, and turning all the LED's off
    pinMode(ANODE_PINS[i], OUTPUT);
    pinMode(CATHODE_PINS[i], OUTPUT);
    digitalWrite(ANODE_PINS[i], HIGH);
    digitalWrite(CATHODE_PINS[i], HIGH);
  }
  
  //Setting up all the Serial Settings
  Serial.begin(115200);
  Serial.setTimeout(100);
  pinMode(BUTTON, INPUT_PULLUP);
}

//This part of the code checks the light above each LED, and if on turns on the required light
void movePatternDown(byte pattern[8][8]) {
  for (int i = 6; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      pattern[i+1][j] = pattern[i][j];
      // Check the light
      if (i == 0) { // If the LED is off
        pattern[i][j] = 0;
        // If there are no other options, the LED is turned off
      }
    }
  }
}

//This part chooses which cathode gets turned on, and thus which row of LED turns on
void chooseRandomInTopRow(byte pattern[8][8]) {
  byte ran = random(0,8);
  pattern[0][ran] = 1;
}

//Set up two different cases and switch between them. The first case is when the raindrop pattern is activated, while the second case displays a still image
void displayh(byte pattern[8][8],int counter) {
  switch (counter%2) {
  case  0 : {for (int a = 0; a < 8; a++) {  //The raindrop code is activated
    for (int c = 0; c < 8; c++) {
      if (pattern[a][c] == 1) {
        digitalWrite(CATHODE_PINS[c], LOW); //Activating cathode wire
      } 
      else {
        digitalWrite(CATHODE_PINS[c], HIGH); //Deactivating cathode wire
      }
    }
    digitalWrite(ANODE_PINS[a], LOW); //Activating anode wire
    delayMicroseconds(1000);//Keep each light on for this long
    digitalWrite(ANODE_PINS[a], HIGH); //Deactivating anode wire
  }  
  break;
  }
    
  case  1 :
  {const byte newpattern[8][8] ={
                    0,0,0,1,1,1,0,0,
                    0,0,1,0,0,0,1,0,
                    0,0,1,0,0,0,1,0,
                    0,0,0,0,0,0,1,0,
                    0,0,0,0,1,1,0,0,
                    0,0,0,0,1,0,0,0,
                    0,0,0,0,0,0,0,0,
                    0,0,0,0,1,0,0,0};

                  
    for (int a = 0; a < 8; a++) {
    for (int c = 0; c < 8; c++) {
      if (newpattern[a][c] == 1) {
        digitalWrite(CATHODE_PINS[c], LOW); //Activating cathode wire
      } 
      else {
        digitalWrite(CATHODE_PINS[c], HIGH); //Deactivating cathode wire
      }
    }
    digitalWrite(ANODE_PINS[a], LOW); //Activating anode wire
    delayMicroseconds(1000);
    digitalWrite(ANODE_PINS[a], HIGH); //Deactivating anode wire
  }
    break;
  }
}

  
}



void loop() {
  // count (debounced) button presses
  static byte button_state = HIGH;
  static byte last_reading = HIGH;
  static long last_reading_change = 0;
  static byte button2_state = HIGH;
  static byte last_reading2 = HIGH;
  static long last_reading_change2 = 0;
  static char message[50];             
  // buffer for sprintfs

  byte reading = digitalRead(BUTTON);
  byte reading2 = digitalRead(BUTTON2);
  unsigned long now = millis();

  // Ignore button_state changes within DEBOUNCE_DELAY milliseconds of the last
  // reading change, otherwise accept.
  if (now - last_reading_change > DEBOUNCE_DELAY) {
    if (reading == LOW && button_state == HIGH) { // button pressed down (HIGH to LOW)
      sprintf(message, "accepted [%d], %ld ms since last\n", ++counter,
      now - last_reading_change);
      Serial.print(message);
      
    }
    button_state = reading;
    //Ignore button_state changes within DEBOUNCE_DELAY milliseconds of the last
    // reading change, otherwise accept.
  }
    if (now - last_reading_change2 > DEBOUNCE_DELAY) {
    if (reading2 == LOW && button2_state == HIGH) { // button pressed down (HIGH to LOW)
      sprintf(message, "accepted [%d], %ld ms since last\n", ++counter2,
      now - last_reading_change2);
      Serial.print(message);
      
    }
    button2_state = reading2;
  }
  
//Makes the code loop over and over again
  static byte pattern[8][8];

  static long nextUpdateTime = millis();
  displayh(pattern,counter);
  if (millis() >= nextUpdateTime) {
    Serial.println("Update");
    movePatternDown(pattern);
    chooseRandomInTopRow(pattern);
   
    displayh(pattern,counter);
    nextUpdateTime = nextUpdateTime + TIME_DELAY;


  }

}


