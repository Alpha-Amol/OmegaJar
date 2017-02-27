Use ArduinoJson Library to Generate and Parse the Json Objects:
https://github.com/bblanchon/ArduinoJson

Encoding / Generating -->
StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
root["sensor"] = "gps";
root["time"] = 1351824120;
JsonArray& data = root.createNestedArray("data");
data.add(48.756080, 6);  // 6 is the number of decimals to print
data.add(2.302038, 6);   // if not specified, 2 digits are printed
root.printTo(Serial);
// This prints:
// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}


Decoding / Parsing -->
char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.parseObject(json);
const char* sensor = root["sensor"];
long time          = root["time"];
double latitude    = root["data"][0];
double longitude   = root["data"][1];


HX711 Library:-
An Arduino library to interface the Avia Semiconductor HX711 24-Bit Analog-to-Digital Converter (ADC) for Weight Scales.
The library is changed to measure the weight in grams and examples are provided in the ESP8266 code section under HX711 Interfacing

WiFi Manager Library
https://github.com/tzapu/WiFiManager
Whenever someone presses the Config button after the device boots up then the device creates webserver from where you can enter Wi-Fi credentials and Authorization Grant Code
