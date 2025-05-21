// Define the digital input pin for the sensor
const int sensor3Pin = 3;

void setup() {
  // Start the serial communication at 9600 baud rate
  Serial.begin(9600);
}

void loop() {
  String message = "";       // Variable to store the received message
  int receivedValue = 0;     // Integer to store the decoded 6-bit value

  // Wait for the start of the transmission (signal goes LOW)
  while (digitalRead(sensor3Pin) == 1) {
    int sensor3Value = digitalRead(sensor3Pin);
    Serial.print(" | Sensor 2: ");
    Serial.println(sensor3Value);
  };

  Serial.println("Message receiving:");
  
  // Wait before beginning to receive bits (transmission sync delay)
  delay(8900); // 8.9 seconds

  char letter; // Character to store each decoded letter

  // Start receiving bits continuously
  while (true) {
    receivedValue = 0; // Reset for each character

    // Read 6 bits (150ms per bit, with additional inter-bit delay)
    for (int i = 0; i < 6; i++) {
      delay(150); // Delay to let the signal settle

      int sensor3Value = digitalRead(sensor3Pin); // Read the sensor value

      Serial.print("  Sensor : ");
      Serial.println(sensor3Value);

      // Invert the logic: LOW (0) = 1, HIGH (1) = 0
      int currentBit = (sensor3Value == 0) ? 1 : 0;

      // Shift bits to form a 6-bit binary number
      receivedValue = (receivedValue << 1) | currentBit;

      delay(8890); // Delay before reading the next bit (8.89 seconds)
    }

    // Check for end-of-transmission marker (binary 111111 = decimal 63)
    if (receivedValue == 63) break;

    // Convert the 6-bit value to corresponding character
    if (receivedValue >= 1 && receivedValue <= 26) {
      letter = 'A' + receivedValue - 1;  // Map 1–26 to 'A'–'Z'
    } else if (receivedValue >= 27 && receivedValue <= 36) {
      letter = '0' + receivedValue - 27; // Map 27–36 to '0'–'9'
    } else {
      letter = ' '; // Use space for undefined values
    }

    Serial.print("letter: ");
    Serial.println(letter);

    message += letter; // Append the decoded character to the message
  }

  // Display the full received message
  Serial.print("Message received: ");
  Serial.println(message);

  // Wait before restarting to avoid serial flooding
  delay(8000);
}
