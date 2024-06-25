uint8_t buf[8] = {0};  // Keyboard Report Buffer: 8 bytes

#define PIN_PUSH_TO_TALK 2
#define PIN_MUTE_GAME 3
#define PIN_DEAFEN 4

bool currState_PushToTalk = HIGH;
bool currState_MuteGame = HIGH;
bool currState_Deafen = HIGH;
          
bool prevState_PushToTalk = HIGH; 
bool prevState_MuteGame = HIGH; 
bool prevState_Deafen = HIGH; 

unsigned long prevTime_PushToTalk = 0;
unsigned long prevTime_MuteGame = 0;
unsigned long prevTime_Deafen = 0;

unsigned long waitTime_PushToTalk = 50;
unsigned long waitTime_MuteGame = 50;
unsigned long waitTime_Deafen = 50;

void setup() 
{
  Serial.begin(9600);

  pinMode(PIN_PUSH_TO_TALK, INPUT_PULLUP);
  pinMode(PIN_MUTE_GAME, INPUT_PULLUP);
  pinMode(PIN_DEAFEN, INPUT_PULLUP);
  
  delay(200);
}

void loop() 
{
  checkButton();
}

void checkButton() {
  bool currRead_PushToTalk = digitalRead(PIN_PUSH_TO_TALK);
  bool currRead_MuteGame = digitalRead(PIN_MUTE_GAME);
  bool currRead_Deafen = digitalRead(PIN_DEAFEN);

  if (currRead_PushToTalk != prevState_PushToTalk) {
    prevTime_PushToTalk = millis();
  }
  if (currRead_MuteGame != prevState_MuteGame) {
    prevTime_MuteGame = millis();
  }
  if (currRead_Deafen != prevState_Deafen) {
    prevTime_Deafen = millis();
  }

  if ((millis() - prevTime_PushToTalk) > waitTime_PushToTalk) {
    if (currRead_PushToTalk != currState_PushToTalk) {
      currState_PushToTalk = currRead_PushToTalk;
      if (currState_PushToTalk == LOW) {
        // Send the code
        buf[2] = 29;    // HID: z key
#ifdef SERIAL_DEBUG
        buf[2] = 'z';     // Serial: z key
#endif
        Serial.write(buf, 8); // Send keypress
      } else {
        // Release the keyboard
        releaseKey();
      }
    }
  }
  if ((millis() - prevTime_MuteGame) > waitTime_MuteGame) {
    if (currRead_MuteGame != currState_MuteGame) {
      currState_MuteGame = currRead_MuteGame;
      if (currState_MuteGame == LOW) {
        // Send the code
        buf[0] = 0x01;  // Modifier byte: 0x01 for Ctrl
        buf[2] = 0x4B;  // HID: Page Up key
#ifdef SERIAL_DEBUG
        buf[0] = 0;
        buf[2] = 'P';  // Serial: Page Up key
#endif
        Serial.write(buf, 8); // Send keypress
      } else {
        // Release the keyboard
        releaseKey();
      }
    }
  }
  if ((millis() - prevTime_Deafen) > waitTime_Deafen) {
    if (currRead_Deafen != currState_Deafen) {
      currState_Deafen = currRead_Deafen;
      if (currState_Deafen == LOW) {
        // Send the code
        buf[0] = 0x03;  // Modifier byte: 0x03 for Ctrl + Shift
        buf[2] = 7;    // HID: d key
#ifdef SERIAL_DEBUG
        buf[0] = 0;
        buf[2] = 'd';  // Serial: d key
#endif
        Serial.write(buf, 8); // Send keypress
      } else {
        // Release the keyboard
        releaseKey();
      }
    }
  }

  prevState_PushToTalk = currRead_PushToTalk;
  prevState_MuteGame = currRead_MuteGame;
  prevState_Deafen = currRead_Deafen;
}

void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8); // Release key  
}
