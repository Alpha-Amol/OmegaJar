#include "HX711.h"

HX711 scale(12, 14);    // parameter "gain" is ommited; the default value 128 is used by the library

float calibration_factor = 461.00; //460 worked for my 200gm scale setup
int averageSamples = 100;

void setup()
{
  Serial.begin(9600);
  Serial.println("HX711 Demo");
 
  Serial.println("Before setting up the scale:");
  scale.set_scale(0);         // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
}

void loop()
{
  float weight = 0.0, temp = 0.0;
  for (int i = 0; i < 5; i++)
  {
    temp = scale.getGram();
    if (temp < 0)
    {
      temp = 0;
    }
    weight = weight + temp;
  }
  weight = weight/5;
  

  Serial.print("Reading:- ");
  Serial.print(weight);
  Serial.println(" Gram");
}

