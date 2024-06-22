/* Arduino USB HID Keyboard Demo
 * Type 'Push to Talk' (z key), 'Mute Game' (Ctrl + Page Up), or 'Deafen' (Ctrl + Shift + D) When Buttons are Pressed
 */

uint8_t buf[8] = {0}; 	/* Keyboard report buffer */
const int buttonPushToTalkPin = 2;  // Push button for 'Push to Talk' (z key) connected to digital pin 2
const int buttonMuteGamePin = 3;  // Push button for 'Mute Game' (Ctrl + Page Up) connected to digital pin 3
const int buttonDeafenPin = 4;  // Push button for 'Deafen' (Ctrl + Shift + D) connected to digital pin 4

void setup() 
{
  pinMode(buttonPushToTalkPin, INPUT_PULLUP);  // Set pin mode with internal pull-up resistor
  pinMode(buttonMuteGamePin, INPUT_PULLUP);  // Set pin mode with internal pull-up resistor
  pinMode(buttonDeafenPin, INPUT_PULLUP);  // Set pin mode with internal pull-up resistor
  Serial.begin(9600);
  delay(200);
}

void loop() 
{
  if (digitalRead(buttonPushToTalkPin) == LOW)  // Check if the 'Push to Talk' button is pressed
  {
    buf[2] = 29;  // Keycode for 'z'
    Serial.write(buf, 8);  // Send keypress
    delay(100);  // Debounce delay
    releaseKey();
  }
  else if (digitalRead(buttonMuteGamePin) == LOW)  // Check if the 'Mute Game' button is pressed
  {
    buf[0] = 0x01;  // Modifier byte: 0x01 for Ctrl
    buf[2] = 0x4B;  // Keycode for 'Page Up'
    Serial.write(buf, 8);  // Send keypress
    delay(100);  // Debounce delay
    releaseKey();
    delay(1000);  // delay before it can be pressed again
  }
  else if (digitalRead(buttonDeafenPin) == LOW)  // Check if the 'Deafen' button is pressed
  {
    buf[0] = 0x03;  // Modifier byte: 0x03 for Ctrl + Shift
    buf[2] = 7;  // Keycode for 'd'
    Serial.write(buf, 8);  // Send keypress
    delay(100);  // Debounce delay
    releaseKey();
    delay(1000);  // delay before it can be pressed again
  }
  else
  {
    releaseKey();  // Release any pressed key if no button is pressed
    delay(100);  // Debounce delay
  }
}

void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8);  // Release key  
}
