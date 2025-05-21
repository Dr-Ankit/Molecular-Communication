#include <LiquidCrystal.h>

// Initialize the LCD with the pins used by the LCD shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int relayPin = 2;  // Relay control pin
char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 "; // Characters supported
char displayText[32] = "                                "; // Display buffer for 16x2 LCD (32 characters total)

int cursorPosition = 0;  // Current cursor position on LCD

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Ensure relay is OFF initially

  lcd.begin(16, 2);       // Initialize the LCD with 16 columns and 2 rows
  lcd.clear();            // Clear LCD screen

  // Clear both rows on the display
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");

  Serial.begin(9600);     // Initialize serial communication
}

void loop() {
  int x = analogRead(A0); // Read analog input from A0 (button input from keypad)

  // Determine which button is pressed based on analog value
  if (x < 50) {              // Right button
    moveCursorRight();
  } else if (x < 220) {      // Up button
    incrementLetter();
  } else if (x < 420) {      // Down button
    decrementLetter();
  } else if (x < 630) {      // Left button
    moveCursorLeft();
  } else if (x < 850) {      // Select button
    finalizeInput();         // Finalize input and transmit via relay
  }

  delay(200); // Debounce delay for button press
}

// Move the cursor to the right on the LCD
void moveCursorRight() {
  cursorPosition++;
  if (cursorPosition > 31) {
    cursorPosition = 0; // Wrap around to start
  }
  lcd.setCursor(cursorPosition % 16, cursorPosition / 16); // Set cursor position
}

// Move the cursor to the left on the LCD
void moveCursorLeft() {
  cursorPosition--;
  if (cursorPosition < 0) {
    cursorPosition = 31; // Wrap around to end
  }
  lcd.setCursor(cursorPosition % 16, cursorPosition / 16); // Set cursor position
}

// Increment the character at the current cursor position
void incrementLetter() {
  char currentChar = displayText[cursorPosition];
  int index = getLetterIndex(currentChar);
  index = (index + 1) % 37; // Wrap around after last character
  displayText[cursorPosition] = letters[index];
  lcd.setCursor(cursorPosition % 16, cursorPosition / 16);
  lcd.print(letters[index]);
}

// Decrement the character at the current cursor position
void decrementLetter() {
  char currentChar = displayText[cursorPosition];
  int index = getLetterIndex(currentChar);
  index = (index - 1 < 0) ? 36 : index - 1; // Wrap around before first character
  displayText[cursorPosition] = letters[index];
  lcd.setCursor(cursorPosition % 16, cursorPosition / 16);
  lcd.print(letters[index]);
}

// Return index of character in the `letters` array
int getLetterIndex(char letter) {
  for (int i = 0; i < 37; i++) {
    if (letters[i] == letter) {
      return i;
    }
  }
  return 0; // Default to 'A' if not found
}

// Finalize input, send it over serial and transmit using relay
void finalizeInput() {
  Serial.print("Current Text: ");

  // Find the index of the last non-space character
  int stringEnd = -1;
  for (int i = 31; i >= 0; i--) {
    if (displayText[i] != ' ') {
      stringEnd = i;
      break;
    }
  }

  // Proceed only if there's valid text
  if (stringEnd != -1) {
    // Initial relay signal to denote start
    digitalWrite(relayPin, HIGH);
    delay(1000);
    digitalWrite(relayPin, LOW);
    delay(8000); // Wait before sending data

    // Copy meaningful characters into a temporary buffer
    char tempStr[33]; // 32 characters + null terminator
    strncpy(tempStr, displayText, stringEnd + 1); 
    tempStr[stringEnd + 1] = '\0'; // Terminate the string

    Serial.println(tempStr); // Print final string

    // Transmit each character in binary
    for (int pt = 0; pt <= stringEnd; pt++) {
      int binaryValue = 0;

      // Convert character to numerical value
      if (displayText[pt] == ' ') {
        binaryValue = 0;
      } else if (displayText[pt] >= 'A' && displayText[pt] <= 'Z') {
        binaryValue = displayText[pt] - 'A' + 1; // A=1, B=2, ..., Z=26
      } else {
        binaryValue = displayText[pt] - '0' + 27; // 0=27, ..., 9=36
      }

      // Convert to 6-bit binary string
      String binaryString = padBinaryString(String(binaryValue, BIN));

      // Print character and its binary code
      Serial.print(displayText[pt]);
      Serial.print(" -> ");
      Serial.println(binaryString);

      // Send each bit using the relay
      for (int i = 0; i < 6; i++) {
        digitalWrite(relayPin, binaryString[i] == '1' ? HIGH : LOW);
        delay(700); // Pulse duration for bit
        digitalWrite(relayPin, LOW);
        delay(8000); // Gap between bits
      }

      Serial.println(); // Add line break between characters
    }

    // Send a special marker (e.g., 6 HIGH pulses) to mark end of message
    for (int i = 0; i < 6; i++) {
      digitalWrite(relayPin, HIGH);
      delay(700);
      digitalWrite(relayPin, LOW);
      delay(8000);
    }
  }
}

// Pad binary string to 6 bits by adding leading 0s
String padBinaryString(String binary) {
  while (binary.length() < 6) {
    binary = "0" + binary;
  }
  return binary;
}
