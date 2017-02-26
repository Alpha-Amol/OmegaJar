#include <ESP8266WiFi.h> 
#include <ArduinoJson.h>

/********************************************WiFi Access**************************************************************
 * Enter the SSID and PASSWORD of your Wi-Fi Router
*********************************************************************************************************************/
const char* _SSID     = "Littlt";           //Wi-Fi SSID 
const char* _PASSWORD = "Autote@111";     // Wi-Fi Password 

//AMAZON DRS parameters
const char* device_model = "OmegaJar"; //Product model ID of your device 
const char* device_identifier = "OmegaJarByESP8266"; // Serial No of device any thing you wish at production end
const char* client_id = ""; // your Amazon developer account client ID
const char* client_secret = ""; // Your Amazon developer account secret code 
const char* slot_id = ""; //Sloat ID for your product
const char* redirect_uri = "https%3A%2F%2Falpha-amol.github.io"; //Encoded Return URL should be encoded and same as used at time of device creation

String authorization_grant_code = "ANXLDHgzmHRZEPokem"; //ex: ANQEgiAOjkQWjhNWIN
String refresh_Token = "Atzr|IwEBIHPFj3XoaOXNHsYhD5Hcz4mjITx9_ayIje_FGhZc148HylHRASgWizpMd8RIEYU_ZlGoTbMNTWSH1lJ1BV7XzV_hGcwZ25QtY7RQSQPXXNJ8NRvn54X2yC4jZNw-EA3LpcCpUc8Ux0r2Qus3UL67gj2q0MfJ1BgLNaDWTKD7UF3W1zMCWvfVzBEvYN-Obv5U4IThHedcRePb4g2NkFOIH-wXJIGl6hmdV5_KugiC4GzKzKnbC1i4raGapi084rJZuGfPyYCgi2kW_PEInHbKRmphQN5P1kCJqwJqP6kP0G-rWbb9h0GXzi6inQLPswKr8aDp9ZofKBe4caxl1euz7OTaXtWTjdIa2Yyl5cIporgSi1-fXHc3JEHlwU2XFn_0";
String access_Token = "";

void wifi_connect() {
  Serial.println(F(""));
  Serial.println(F("******************************WIFI Connecting*******************************************"));
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(F("."));
    delay(1000);
  }

  Serial.println(F(""));
  Serial.println(F("Wi-Fi Connected!!"));
  Serial.println(F("***************************************************************************************"));
}

WiFiClientSecure client;
int c = 1;
 
void setup() 
{
  Serial.begin(9600); //  Initlize UART for Serial debug

  Serial.println(F("\n OmegaJar- Poweredby Amazon DRS"));

  Serial.println(F(""));
  Serial.print(F("Connecting to WiFi"));
  
  // Wifi Setting
  WiFi.begin(_SSID, _PASSWORD);
  wifi_connect();
  
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (c)
  {
  Serial.println(".............Access and Refresh Token.............");
  unsigned int x = obtain_access_and_refresh_token();
  Serial.println(x);
  Serial.println("Connected to AP......and DRS Enabled...");    
  c = 0;
  }

  //only call this on initial setup - once user has authorized
  //LWA for the device just use the refresh_token
  delay(10000); //wait a moment for the authorization code grant to complete the exchange
  
  Serial.println(".............Access and Refresh Token.............");
  unsigned int y = obtain_access_token();
  Serial.println(y);
  
  //only call this on initial setup - once user has authorized
  //LWA for the device just use the refresh_token
  delay(3000); //wait a moment for the authorization code grant to complete the exchange
  
  Serial.println(".............REPLENISHING Jar..............");
  unsigned int z=end_point_request_replenishment(slot_id);
  Serial.println(z);
  if(z==11)
  {
    Serial.println("Standard Order Placed..");
  }
  else if (z==12)
  {
    Serial.println("Test Order Placed..");
  }
  else if (z==13)
  {
    Serial.println("Order In Progress..");
  }
  else 
  {
    Serial.println("Order Failed..");
  }  

  delay(1000*1000);
}

String assembleAuthCodeBody(String authCode, String clientId, String clientSecret, String redirectUri)
{
   String tempTokenBody = "";
    
   tempTokenBody += "grant_type=authorization_code";
   tempTokenBody += "&";
   tempTokenBody += "code=";
   tempTokenBody += authorization_grant_code;
   tempTokenBody += "&";
   tempTokenBody += "client_id=";
   tempTokenBody += client_id;
   tempTokenBody += "&";
   tempTokenBody += "client_secret=";
   tempTokenBody += client_secret;
   tempTokenBody += "&";
   tempTokenBody += "redirect_uri=";
   tempTokenBody += redirect_uri;

   return tempTokenBody;
}

unsigned int obtain_access_and_refresh_token() 
{
  //assemble the body
  String authCodeBody = assembleAuthCodeBody(authorization_grant_code, client_id, client_secret, redirect_uri);
  // Assemble POST request:
  //----------------------------------------------------------------
  //Request for Bearer and Refresh Tokens
  //----------------------------------------------------------------

  // Use WiFiClientSecure class to create TCP connections
  if (!client.connect("api.amazon.com", 443)) {
    Serial.println("Error 5 --> Connection failed");
    return 5;
  }
  client.flush();
  Serial.println("Connected\r\nSending Request for obtain Tokens");
  // This will send request for refresh of Access token to the server
  client.println("POST /auth/o2/token HTTP/1.1");
  client.println("Host: api.amazon.com"); 
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Cache-Control: no-cache");
  client.print("Content-Length: ");
  client.println(authCodeBody.length());
  client.println();
  client.println(authCodeBody);
  Serial.println("Request Send");
  
  unsigned long timeout = millis();
  
  while (client.available() == 0) 
  {
    if (millis() - timeout > 30000) 
    {
      Serial.println("Error 4 --> Connection Timeout !");
      client.stop();
      return 4;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  String _line = "temp";
  unsigned int _line_no = 1;
  
  while(client.available())
  {
    _line = client.readStringUntil('\r');
    Serial.print(_line);
    if ((_line_no == 1)&& (_line != "HTTP/1.1 200 OK"))
    {
      Serial.println();
      Serial.println("Error 3 --> Invalid Response");
      client.stop();
      return 3;
    }
    _line_no++;
  }
  
  DynamicJsonBuffer jsonBuffer;
  // Responce last line have access token and referesh token json oblect 
  JsonObject& root = jsonBuffer.parseObject(_line);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("Error 2 --> Json parseObject() failed");
    return 2;
  }  
  Serial.println();
  
  const char* access_token = root["access_token"];
  Serial.print("Access token: ");
  Serial.println(access_token);
  access_Token = access_token;
  Serial.println(access_Token);
  Serial.println("SUCCESS");
  
  const char* _refresh_token = root["refresh_token"];
  Serial.print("Refresh token: ");
  Serial.println(_refresh_token);
  refresh_Token = _refresh_token;
  Serial.println(refresh_Token);
  Serial.println("SUCCESS");

  return 1;
}


String assembleRefreshTokenBody(String refresh_token, String clientId, String clientSecret, String redirectUri)
{
   String tempTokenBody = "";
    
   tempTokenBody += "grant_type=refresh_token";
   tempTokenBody += "&";
   tempTokenBody += "refresh_token=";
   tempTokenBody += refresh_token;
   tempTokenBody += "&";
   tempTokenBody += "client_id=";
   tempTokenBody += client_id;
   tempTokenBody += "&";
   tempTokenBody += "client_secret=";
   tempTokenBody += client_secret;
   tempTokenBody += "&";
   tempTokenBody += "redirect_uri=";
   tempTokenBody += redirect_uri;

   return tempTokenBody;
}

unsigned int obtain_access_token() 
{
  //assemble the body
  String authCodeBody = assembleRefreshTokenBody(refresh_Token, client_id, client_secret, redirect_uri);
  // Assemble POST request:
  //----------------------------------------------------------------
  //Request for Bearer and Refresh Tokens
  //----------------------------------------------------------------

  // Use WiFiClientSecure class to create TCP connections
  if (!client.connect("api.amazon.com", 443)) {
    Serial.println("Error 5 --> Connection failed");
    return 5;
  }
  client.flush();
  Serial.println("Connected\r\nSending Request for obtain Tokens");
  // This will send request for refresh of Access token to the server
  client.println("POST /auth/o2/token HTTP/1.1");
  client.println("Host: api.amazon.com"); 
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Cache-Control: no-cache");
  client.print("Content-Length: ");
  client.println(authCodeBody.length());
  client.println();
  client.println(authCodeBody);
  Serial.println("Request Send");
  
  unsigned long timeout = millis();
  
  while (client.available() == 0) 
  {
    if (millis() - timeout > 30000) 
    {
      Serial.println("Error 4 --> Connection Timeout !");
      client.stop();
      return 4;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  String _line = "temp";
  unsigned int _line_no = 1;
  
  while(client.available())
  {
    _line = client.readStringUntil('\r');
    Serial.print(_line);
    if ((_line_no == 1)&& (_line != "HTTP/1.1 200 OK"))
    {
      Serial.println();
      Serial.println("Error 3 --> Invalid Response");
      client.stop();
      return 3;
    }
    _line_no++;
  }
  
  DynamicJsonBuffer jsonBuffer;
  // Responce last line have access token and referesh token json oblect 
  JsonObject& root = jsonBuffer.parseObject(_line);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("Error 2 --> Json parseObject() failed");
    return 2;
  }  
  Serial.println();
  
  const char* access_token = root["access_token"];
  Serial.print("Access token: ");
  Serial.println(access_token);
  access_Token = access_token;
  Serial.println(access_Token);
  Serial.println("SUCCESS");

  return 1;
}

unsigned int end_point_request_replenishment(const char* _slot_id)
{
  Serial.print("Requesting Replenishment for slotId ");
  Serial.println(_slot_id);
  // Assemble POST request:
  //----------------------------------------------------------------
  //Request replenishment for supplied slotId
  //----------------------------------------------------------------
  
    if(!client.connect("dash-replenishment-service-na.amazon.com", 443)) 
  {
    Serial.println("Error 5 --> Connection failed");
    return 5;
  }
  client.flush();
  Serial.println("Connected\r\nSending Request for Replenishment");
  //This will send the replenishment request to the Amazon DRS server
  client.print("POST /replenish/");
  client.print(_slot_id); 
  client.println(" HTTP/1.1");
  client.println("Host: dash-replenishment-service-na.amazon.com");
  client.print("Authorization: Bearer ");
  client.println(access_Token);
  client.println("x-amzn-accept-type: com.amazon.dash.replenishment.DrsReplenishResult@1.0");
  client.println("x-amzn-type-version: com.amazon.dash.replenishment.DrsReplenishInput@1.0");
  client.println("Cache-Control: no-cache");
  client.println();
  Serial.println("Request Send");
  
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 20000) {
      Serial.println("Error 4 --> Connection Timeout !");
      client.stop();
      return 4;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  String _line = "temp";
  unsigned int _line_no = 1;
  while(client.available())
  {
    _line = client.readStringUntil('\r');
    Serial.print(_line);
    if ((_line_no == 1)&& (_line != "HTTP/1.1 200 OK"))
    {
      Serial.println();
      Serial.println("Error 3 --> Invalid Response");
      client.stop();
      return 3;
    }
    _line_no++;
  }
  DynamicJsonBuffer jsonBuffer;
  // Responce last line have access token and referesh token json oblect 
  JsonObject& root = jsonBuffer.parseObject(_line);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("Error 2 --> Json parseObject() failed");
    return 2;
  }
  const char* _detailCode = root["detailCode"];
  // Print values
  Serial.println();
  Serial.print("detailCode: ");
  Serial.println(_detailCode);

  if(strcmp(_detailCode,"STANDARD_ORDER_PLACED")==0)
  {
    Serial.println("SUCCESS 11 --> Standard Order Placed");
    return 11;
  }
  else if(strcmp(_detailCode,"TEST_ORDER_PLACED")==0)
  {
    Serial.println("SUCCESS 12 --> Test Order Placed");
    return 12;
  }
  else if(strcmp(_detailCode,"ORDER_INPROGRESS")==0)
  {
    Serial.println("SUCCESS 13 --> Order In Progress");
    return 13;
  }
  else
  {
    return 0;
  }
}
