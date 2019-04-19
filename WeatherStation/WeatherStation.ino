#include <Adafruit_BME280.h>

#include <Adafruit_BME280.h>

#include <HTTPClient.h>

#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <WiFiServer.h>


#include <Wire.h>
#include "Adafruit_BME280.h"

#define I2C_SDA 23
#define I2C_SCL 22
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME280_ADD 0x76

/* Set these to your desired credentials. */
const char *ssid = "";  //ENTER YOUR WIFI SETTINGS
const char *password = "";
 
//Web/Server address to read/write from 
const char *host = "192.168.1.24";   //https://circuits4you.com website or IP address of server
 
//HTTPClient http; 

void getValues(void);

Adafruit_BME280 bme(I2C_SDA, I2C_SCL);

void setup() {
  Serial.begin(115200);
  Serial.println("Program Start");

  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  

  bool status;

  status = bme.begin(BME280_ADD);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  delay(1000);
}

void loop() {
  //getValues();
  
  String temperature, pressure, humidity = "";

  temperature += bme.readTemperature();
  pressure += bme.readPressure() / 100.0F;
  humidity += bme.readHumidity();

  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" ℃");

  Serial.print("Pressure = ");

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();

 HTTPClient http;    //Declare object of class HTTPClient
 
//  String ADCData, station, postData;
//  int adcvalue=analogRead(A0);  //Read Analog value of LDR
//  ADCData = String(adcvalue);   //String to interger conversion
//  station = "A";

  Serial.println("post data");
  String postData = "asdas";

  http.begin("http://192.168.1.24:8080/weather-monitor/temperature/" + temperature + "/humidity/" + humidity + "/pressure/" + pressure);              //Specify request destination
  Serial.println("begin");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  Serial.print("httpCode: ");
   Serial.print(httpCode);
  String payload = http.getString();    //Get the response payload
 
  Serial.println("error code: " + httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection
  
  delay(1000);  //Post Data at every 5 seconds
  
}


void getValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" ℃");

  Serial.print("Pressure = ");

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}



