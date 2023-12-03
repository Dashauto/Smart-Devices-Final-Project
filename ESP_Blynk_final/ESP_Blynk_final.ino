
/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.
    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This example runs directly on NodeMCU.
  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino
  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!
  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino
  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/**
 * FILL OUT THE FOLLOWING BASED ON YOUR TEMPLATE INFORMATION
 * 1. BLYNK_TEMPLATE_ID (line 48)
 * 2. BLYNK_DEVICE_NAME (line 49)
 * 3. BLYNK_AUTH_TOKEN (line 50)
 */

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) 
#define BLYNK_TEMPLATE_ID           "XXXXXX"  // FILL THIS OUT
#define BLYNK_DEVICE_NAME           "XXXXXX" // FILL THIS OUT
#define BLYNK_AUTH_TOKEN            "XXXXXX" // FILL THIS OUT*/
#define BLYNK_TEMPLATE_ID "TMPL2kYl5UbB0"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "9tMeoFE6qhPCArprKNFQO5gvYeMs-MW5"



#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <HardwareSerial.h>


// This function creates the timer object. It's part of Blynk library 
BlynkTimer timer; 

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "Domus611";
//char pass[] = "GTY19991031";
char ssid[] = "SilkRoads";
char pass[] = "protocol250";

// Declaring a global variabl for sensor data
int timeVal;
char buffer[4];
char string[25];
char string_out[25];
int buff_ptr = 0;
int buff_receive = 0;
int temp = 0;
int humidity = 0;
int currentHour = 0;
int currentMinute = 0;


// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// V0 is open curtain button
BLYNK_WRITE(V0) {
  if (param.asInt() == 1){
    // communication code to open the curtains
    Serial1.print("A11\n");
    //digitalWrite(LED_BUILTIN, 1);
  };
}

// V1 is close curtain button
BLYNK_WRITE(V1) {
  if (param.asInt() == 1){
    // communication code to close the curtains
    Serial1.print("B11\n");
    //digitalWrite(A11, 1); // Write to pin 2
  };
}

// V2 is open AC button
BLYNK_WRITE(V2) {
  if (param.asInt() == 1){
    // communication code to open AC
    Serial1.print("C11\n");
  }
  else Serial1.print("C00\n");
}

// V3 is close AC button
/*BLYNK_WRITE(V3) {
  if (param.asInt() == 1){
    // communication code to close AC
    Serial1.print("D11\n");
  };
}*/

// V4 is set time to open curtain input
BLYNK_WRITE(V4) {

  int T; // the total seconds
  int hours, minutes;

  // param[0] is the user time value selected in seconds.
  T = param[0].asInt();

  hours = T / 3600;
  minutes = (T % 3600) / 60;
  sprintf(buffer, "%c%c%c\n", 'E',(char) hours + 32, (char) minutes + 32);
  Serial1.print(buffer);
}

// V5 is set time to close curtain input
BLYNK_WRITE(V5) {

  int T; // the total seconds
  int hours, minutes;

  // param[0] is the user time value selected in seconds.
  T = param[0].asInt();

  hours = T / 3600;
  minutes = (T % 3600) / 60;
  sprintf(buffer, "%c%c%c\n", 'F',(char) hours + 32, (char) minutes + 32);
  Serial1.print(buffer);
}

// V6 is set temperature to open A/C input
BLYNK_WRITE(V6) {
  switch (param.asInt()) {
    case 0: { // Item 0
      Serial1.print("G00\n");
      break;
    }
    case 1: { // Item 1
      Serial1.print("G05\n");
      break;
    } 
    case 2: { // Item 2
      Serial1.print("G10\n");
      break;
    }
    case 3: { // Item 3
      Serial1.print("G15\n");
      break;
    }
    case 4: { // Item 4
      Serial1.print("G20\n");
      break;
    } 
    case 5: { // Item 4
      Serial1.print("G25\n");
      break;
    }  
    case 6: { // Item 4
      Serial1.print("G30\n");
      break;
    }  
    case 7: { // Item 4
      Serial1.print("G35\n");
      break;
    }  
    case 8: { // Item 4
      Serial1.print("G40\n");
      break;
    } 
  }
}

// V7 is set temperature to close A/C input
BLYNK_WRITE(V7) {
  switch (param.asInt()) {
    case 0: { // Item 0
      Serial1.print("H00\n");
      break;
    }
    case 1: { // Item 1
      Serial1.print("H05\n");
      break;
    } 
    case 2: { // Item 2
      Serial1.print("H10\n");
      break;
    }
    case 3: { // Item 3
      Serial1.print("H15\n");
      break;
    }
    case 4: { // Item 4
      Serial1.print("H20\n");
      break;
    } 
    case 5: { // Item 4
      Serial1.print("H25\n");
      break;
    }  
    case 6: { // Item 4
      Serial1.print("H30\n");
      break;
    }  
    case 7: { // Item 4
      Serial1.print("H35\n");
      break;
    }  
    case 8: { // Item 4
      Serial1.print("H40\n");
      break;
    } 
  }
}

// V11 is set current time
BLYNK_WRITE(V11) {

  int T; // the total seconds
  int hours, minutes;

  // param[0] is the user time value selected in seconds.
  T = param[0].asInt();

  hours = T / 3600;
  minutes = (T % 3600) / 60;
  sprintf(buffer, "%c%c%c\n", 'I',(char) hours + 32, (char) minutes + 32);
  Serial1.print(buffer);
}

// V12 is open/close curtain button based on smart decision
BLYNK_WRITE(V12) {
  if (param.asInt() == 1){
    // communication code to open/close curtain button based on smart decision
    Serial1.print("J11\n");
  }
  else Serial1.print("J00\n");
}

// V13 is open/close A/C button based on smart decision
BLYNK_WRITE(V13) {
  if (param.asInt() == 1){
    // communication code to open/close A/C button based on smart decision
    Serial1.print("K11\n");
  }
  else Serial1.print("K00\n");
}


void myTimer() 
{
  // This function describes what will happen with each timer tick
  // e.g. writing sensor value to datastream V5
  //Blynk.virtualWrite(V2, timeVal);
  if(timeVal == 100){
    timeVal = 0;
  }
  timeVal ++;
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  // Start the UART communication
  Serial1.begin(9600, SERIAL_8N2, 7, 8);

  pinMode(LED_BUILTIN, OUTPUT);
  Blynk.begin(auth, ssid, pass);

  pinMode(A11, OUTPUT);

  timeVal = 0;

  // Setting interval to send data to Blynk Cloud to 1000ms. 
  // It means that data will be sent every second
  timer.setInterval(1000L, myTimer);
}

void loop()
{
  // Reading sensor from hardware analog pin A0
  // sensorVal = analogRead(A0); 
  
  // Runs all Blynk stuff
  Blynk.run(); 
  
  // runs BlynkTimer
  timer.run(); 

  while (Serial1.available() > 0) {
    char incomingChar = Serial1.read(); // Read the incoming character

    if (incomingChar == '\n') {
      // End of the line, print the buffer
      string[buff_ptr] = '\0'; // Null-terminate the string
      Serial.print("Received: ");
      Serial.println(string);
      buff_ptr = 0; // Reset buffer position
      buff_receive = 1;
    } else {
      // Add character to buffer
      if (buff_ptr < 24) {
        string[buff_ptr++] = incomingChar;
      }
    }
    if(buff_receive){
      if(string[0] == 'Y'){
        humidity = (int) string[1] - 32;
        temp = (int) string[2] -32;
        Blynk.virtualWrite(V8, temp);
        Blynk.virtualWrite(V9, humidity);


        // currentHour = (int) string[1] - 32;
        // currentMinute = (int) string[2] -32;
        // sprintf(string_out, "%d : %d", currentHour, currentMinute);
        // Blynk.virtualWrite(V10, string_out); 
      }
      else if (string[0] == 'X'){
        currentHour = (int) string[1] - 32;
        currentMinute = (int) string[2] -32;
        sprintf(string_out, "%d : %d", currentHour, currentMinute);
        Blynk.virtualWrite(V10, string_out); 
      }
    }
   

    
  }
}


