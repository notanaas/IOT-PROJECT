#define BLYNK_TEMPLATE_ID "TMPL6UO0OPJzF"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi credentials
char ssid[] = "M7md's room";
char pass[] = "Musallam123";

// Blynk Auth Token
char auth[] = "Gwpxx9uoxznU8ruTyAu0Cml-eMG492JE";

// DS18B20 Sensor setup
const int oneWireBus = D2; // Connect DS18B20 to D2 pin (or another pin of your choice)
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

BlynkTimer timer;

void sendSensorData() {
  sensors.requestTemperatures(); 
  float waterTemp = sensors.getTempCByIndex(0); // Read temperature in Celsius
  
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    int firstComma = data.indexOf(',');
    int secondComma = data.indexOf(',', firstComma + 1);
    int thirdComma = data.indexOf(',', secondComma + 1);
    int fourthComma = data.indexOf(',', thirdComma + 1);

    int waterLevel = data.substring(0, firstComma).toInt();
    float pH = data.substring(firstComma + 1, secondComma).toFloat() / 1024.0 * 14.0;
    float voltage = data.substring(secondComma + 1, thirdComma).toFloat() * (5.0 / 1023.0);
    float current = data.substring(thirdComma + 1, fourthComma).toFloat() * (5.0 / 1023.0);
    int motion = data.substring(fourthComma + 1).toInt();

    // Send data to Blynk
    Blynk.virtualWrite(V1, waterLevel);
    Blynk.virtualWrite(V2, pH);
    Blynk.virtualWrite(V3, voltage);
    Blynk.virtualWrite(V4, current);
    Blynk.virtualWrite(V5, waterTemp); // Send water temperature to virtual pin V5

    if (motion == HIGH) {
      // Blynk.notify("Motion detected!");
    }
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  sensors.begin();
  timer.setInterval(1000L, sendSensorData); // Adjust as needed
}

void loop() {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
