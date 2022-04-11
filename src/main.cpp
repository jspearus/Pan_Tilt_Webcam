#include <Arduino.h>
#include <Servo.h>

Servo panServo;  // create servo object to control a servo
Servo tiltServo; // twelve servo objects can be created on most boards

//################# PIN DEFINITIONS ########################
const int panPin = 9;
const int tiltPin = 10;

//############################ VARIABLES ########################################
float panPos = 1400.0;          // variable to store the servo position
float tiltPos = 1500.0;         // variable to store the servo position
float panPosSmoothed = 1400.0;  // variable to store the servo position
float tiltPosSmoothed = 1500.0; // variable to store the servo position
float panPosPre;                // variable to store the servo position
float tiltPosPre;               // variable to store the servo position

String Data_In = "";
String panIn = "";
String tiltIn = "";

bool isOn = true;

//################### FUNCTIONDECLAREATIONS ###############
void move(float pan, float tilt);

void setup()
{
  Serial.begin(115200);
  panServo.attach(panPin);
  tiltServo.attach(tiltPin);
  delay(500);
  panServo.writeMicroseconds(panPos);
  tiltServo.writeMicroseconds(tiltPos);
}

void loop()
{
  if (Serial.available() > 0)
  {
    Data_In = Serial.readStringUntil('#');
    if (Data_In == "whoRu")
    {

      Serial.println("mastCam-#");
    }
    panIn = Data_In.substring(0, Data_In.indexOf("-"));
    tiltIn = Data_In.substring(Data_In.indexOf("-") + 1, Data_In.indexOf("#"));
    if (panIn == "off")
    {
      isOn = false;
      panServo.detach();
      tiltServo.detach();
    }
    else if (panIn == "on")
    {
      isOn = true;
      panServo.attach(panPin);
      tiltServo.attach(tiltPin);
    }
    else
    {
      float convertp = panIn.toFloat();
      float convertt = tiltIn.toFloat();
      if (convertp > 0.00)
      {
        panPos = convertp;
      }
      if (convertt > 0.00)
      {
        tiltPos = convertt;
      }
    }
  }

  panPosSmoothed = (panPos * 0.1) + (panPosPre * 0.9);
  tiltPosSmoothed = (tiltPos * 0.1) + (tiltPosPre * 0.9);

  panPosPre = panPosSmoothed;
  tiltPosPre = tiltPosSmoothed;
  if (isOn == true)
  {
    move(panPosSmoothed, tiltPosPre);
  }
  delay(5);
}

void move(float pan, float tilt)
{
  // Serial.print("pan ");
  // Serial.print(pan);
  // Serial.print("  tilt ");
  // Serial.println(tilt);
  panServo.writeMicroseconds(pan);
  tiltServo.writeMicroseconds(tilt);
}