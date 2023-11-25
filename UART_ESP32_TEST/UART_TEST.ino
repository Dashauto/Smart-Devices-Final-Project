#include <HardwareSerial.h>

//HardwareSerial mySerial(2); // Use the first hardware serial port

void setup() {
  // Start the hardware serial port for debug messages
  Serial.begin(115200);

  // Start the UART communication
  Serial1.begin(9600, SERIAL_8N2, 7, 8); // Using 8 data bits, no parity, and 2 stop bits
}

void loop() {
  // Send a message to Atmega328P
  Serial1.print("Hello from ESP32!\r\n");

  // Check if the Atmega328P has sent a message
  /*
  if (Serial1.available()) {
    int received = Serial1.read();
    Serial.print("Received from Atmega328P: ");
    Serial.println((char) received);
  }
  */

  delay(1000); // Wait for a second
}

