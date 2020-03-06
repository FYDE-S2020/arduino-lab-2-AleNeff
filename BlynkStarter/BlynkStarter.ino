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
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define LED 2

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "GeunX86nQ_Q4ab0fR0xT-3hxG18VssDa";
char ssid[32] = "Rita";
char pass[32] = "dogclock12";

int ledState = 0;
int dutyCycle = 1023;
BlynkTimer timer;
int time_count = 0;
String content = "";
//button=V1, slider=V0, display=V2, terminal=V3

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10;

void myTimerEvent()
{
// You can send any value at any time.
// Don't send more than 10 values a second or the Blynk server will block you!
    if (time_count == 100){
        Blynk.virtualWrite(V2, millis() / 1000);
        time_count = 0; // reset time counter
    }
    else {
        // Send serial data to Blynk terminal
        char character;
        while(Serial.available()) { // Check if serial is available every 10 ms
            character = Serial.read();
            content.concat(character);
        }
        if (content != "") {
            Blynk.virtualWrite(V3, content);
            content = ""; // Clear String
        }  
    }
    time_count += 1; // Increment on every tick
}

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LED, ledChannel);
  timer.setInterval(10L, myTimerEvent); //10 ms interval
}

void loop()
{
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V1)
{

    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    ledState = pinValue;
    // Because V1 is a button, pinValue will be a 0 or a 1.
    if (pinValue == 0) {
        ledcWrite(ledChannel, 0);
    }
    else {
        ledcWrite(ledChannel, dutyCycle);
    }
}
BLYNK_WRITE(V0)
{
    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int val = param.asInt(); // assigning incoming value from pin V0 to a variable
    dutyCycle = val;
    if(ledState==1){ledcWrite(ledChannel, dutyCycle);} 
}
