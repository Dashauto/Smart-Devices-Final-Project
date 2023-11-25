
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

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SilkRoads";
char pass[] = "protocol250";


// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

BLYNK_WRITE(V0) {
  int up = param.asInt();
  digitalWrite(LED_BUILTIN, up);
}

BLYNK_WRITE(V1) {
  int pinValue = param.asInt(); // Get value as integer
  digitalWrite(A11, pinValue); // Write to pin 2
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Blynk.begin(auth, ssid, pass);

  pinMode(A11, OUTPUT);

}

void loop()
{
  Blynk.run();
}

