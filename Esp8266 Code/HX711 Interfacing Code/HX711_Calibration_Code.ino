#include "HX711.h"

HX711 scale(12, 14);    // parameter "gain" is ommited; the default value 128 is used by the library

float calibration_factor = 462.00; //460 worked for my 200gm scale setup
int averageSamples = 100;

void setup()
{
  Serial.begin(9600);
  Serial.println("HX711 Demo");
  
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  
  Serial.println("Before setting up the scale:");
  scale.set_scale(0);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  long zero_factor = scale.read_average(averageSamples); //Get a baseline reading
  Serial.print("read average: \t\t");
  Serial.println(zero_factor);    // print the average of 20 readings from the ADC
  
}

void loop()
{
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  
  float weight = scale.getGram();

  Serial.print("Reading:- ");
  Serial.println(weight);
  
  Serial.print("calibration_factor:- ");
  Serial.println(calibration_factor); 
  
  if (Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
       calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
        calibration_factor -= 10;
  }
  delay(2000);
}

