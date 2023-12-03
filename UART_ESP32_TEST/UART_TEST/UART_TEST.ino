#include <HardwareSerial.h>

//HardwareSerial mySerial(2); // Use the first hardware serial port
bool newData = false;
char buffer[25];
int buff_ptr = 0;
unsigned int count = 0;
int hour = 13;
int minute = 25;

void setup() {
  // Start the hardware serial port for debug messages
  Serial.begin(115200);

  // Start the UART communication
  Serial1.begin(9600, SERIAL_8N2, 7, 8); // Using 8 data bits, no parity, and 2 stop bits

}

void loop() {
  // Send a message to Atmega328P
  sprintf(buffer,"E%c%c\n", (char) (32 + hour), (char) (32 + minute));
  Serial1.print(buffer);
  delay(50);/*
  Serial1.write("a07\n");
  delay(50);
  Serial1.write("a10\n");
  delay(50);*/
  // Check if the Atmega328P has sent a message
  
  while (Serial1.available() > 0) {
    
    
    char incomingChar = Serial1.read(); // Read the incoming character

    if (incomingChar == '\n') {
      // End of the line, print the buffer
      buffer[buff_ptr] = '\0'; // Null-terminate the string
      Serial.print("Received: ");
      Serial.println(buffer);
      buff_ptr = 0; // Reset buffer position
    } else {
      // Add character to buffer
      if (buff_ptr < 24) {
        buffer[buff_ptr++] = incomingChar;
      }
    }
  }
  

  //delay(1000); // Wait for a second
}



