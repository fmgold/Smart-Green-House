#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <Wire.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

#define FIREBASE_HOST "" //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "" //Your Firebase Auth Token

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

FirebaseData firebaseData;
FirebaseData relayData;
FirebaseJson json;
String path;

#define DHTPIN 14      // what pin we're connected to
#define DHTTYPE DHT11  // DHT 22  (AM2302)

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4); // Change to (0x27,20,4) for 20x4 LCD.

int State = 0;
int LDR = 34;
int light = 18;
int fan = 15;
int pump = 19;
int soilMoisture = 33;
int m;

int maxHum = 60;
int maxTemp = 40;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(light, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(pump, OUTPUT);
  Serial.begin(9600);
  dht.begin();
  lcd.setCursor(0, 0);  //move cursor to second line, first position
  lcd.print("GREEN HOUSE");
  lcd.setCursor(0, 1);  //move cursor to second line, first position
  lcd.print("MONITORING SYSTEM");
  delay(3000);  // wait 1sec
  lcd.clear();

  lcd.setCursor(0, 0);  //move cursor to second line, first position
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);  //move cursor to second line, first position
  lcd.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    lcd.setCursor(0, 0);  //move cursor to second line, first position
    lcd.print("not wifi available");
    lcd.print(".");
    Serial.print(".");
    delay(300);
  }
  lcd.clear();
  lcd.setCursor(0, 0);  //move cursor to second line, first position
  lcd.print("CONNECTION ESTABLISHED");
  delay(2000);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  lcd.clear();
  lcd.setCursor(0, 0);  //move cursor to second line, first position
  lcd.print("CONNECTED TO DATABASE");
  delay(2000);
  lcd.clear();
  Serial.println("Starting");
  digitalWrite(light, HIGH);
  delay(1000);
  digitalWrite(light, LOW);
  digitalWrite(fan, HIGH);
  delay(1000);
  digitalWrite(fan, LOW);
  digitalWrite(pump, HIGH);
  delay(1000);
  digitalWrite(pump, LOW);
  lcd.setCursor(0, 0);  //move cursor to second line, first position
  lcd.print("SYSTEM READY");
  //  lcd.setCursor(0, 1);  //move cursor to second line, first position
  //  lcd.print("MONITORING SYSTEM");
  delay(3000);  // wait 1sec
  lcd.clear();
  Serial.print("GREEN HOUSE");
  Serial.print("MONITORING SYSTEM");
}

void loop() {

  //delay(2000);
  int lightIntensity = analogRead(LDR);
  int soil = analogRead(soilMoisture);
  m = map(soil, 4023, 0, 0, 100);
  int lightInt = map(lightIntensity, 0, 4023, 0, 100);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    lcd.print("dht not connected");
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (Firebase.getString(relayData, "/automatic")) {
    Serial.println("pump ");
    Serial.println(relayData.stringData());
    if (relayData.stringData() == "1") {
      Serial.println("pump on ");
      State = 1;
    }
    else if (relayData.stringData() == "0") {
      Serial.println("pump on ");
      State = 0;
    }
    //delay(1000);
  }

  if (State == 0) {
    if (t > 31 && lightInt < 30 && m < 40) {
      lcd.clear();
      lcd.setCursor(0, 0);  //move cursor to second line, first position
      lcd.print("LIGHT ON");
      lcd.setCursor(0, 1);  //move cursor to second line, first position
      lcd.print("FAN ON");
      lcd.setCursor(0, 2);  //move cursor to second line, first position
      lcd.print("PUMP ON");
      Serial.println("Soil on ");
      Serial.println("Temperarue ");
      Serial.println("Light on ");
      digitalWrite(light, HIGH);
      digitalWrite(pump, HIGH);
      digitalWrite(fan, HIGH);
      //delay(500);
    }

    else if (t > 31 && lightInt < 30) {
      lcd.clear();
      lcd.setCursor(0, 0);  //move cursor to second line, first position
      lcd.print("LIGHT ON");
      lcd.setCursor(0, 1);  //move cursor to second line, first position
      lcd.print("FAN ON");
      lcd.setCursor(0, 2);  //move cursor to second line, first position
      lcd.print("TEMPERATURE=");
      lcd.print(t);
      lcd.setCursor(0, 3);  //move cursor to second line, first position
      lcd.print("LIGHT INTENSITY=");
      Serial.println("temp and light ");
      lcd.print(lightInt);
      digitalWrite(light, HIGH);
      digitalWrite(pump, LOW);
      digitalWrite(fan, HIGH);
    }

    else if (t > 31 && m < 40) {
      lcd.clear();
      lcd.setCursor(0, 0);  //move cursor to second line, first position
      lcd.print("FAN ON");
      lcd.setCursor(0, 1);  //move cursor to second line, first position
      lcd.print("PUMP ON");
      lcd.setCursor(0, 2);  //move cursor to second line, first position
      lcd.print("TEMPERATURE=");
      lcd.print(t);
      lcd.setCursor(0, 3);  //move cursor to second line, first position
      lcd.print("SOIL MOISTURE=");
      lcd.print(soil);
      Serial.println("soil and temperature on ");
      digitalWrite(light, LOW);
      digitalWrite(pump, HIGH);
      digitalWrite(fan, HIGH);
    }


    else if (lightInt < 30 && m < 40) {
      lcd.clear();
      lcd.setCursor(0, 0);  //move cursor to second line, first position
      lcd.print("LIGHT ON");
      lcd.setCursor(0, 1);  //move cursor to second line, first position
      lcd.print("PUMP ON");
      lcd.setCursor(0, 2);  //move cursor to second line, first position
      lcd.print("LIGHT INTENSITY=");
      lcd.print(lightIntensity);
      lcd.setCursor(0, 3);  //move cursor to second line, first position
      lcd.print("SOIL MOISTURE=");
      lcd.print(soil);
      Serial.println("soil and light ");
      Serial.println("pump and light on");
      Serial.println(" *C ");
      digitalWrite(light, HIGH);
      digitalWrite(pump, HIGH);
      digitalWrite(fan, LOW);
    }
    // //check for temperature
    else if (t > 31) {
      lcd.clear();
      lcd.setCursor(0, 0);  //move cursor to second line, first position
      lcd.print("HUMIDITY = ");
      lcd.print(h);
      lcd.setCursor(0, 1);  //move cursor to second line, first position
      lcd.print("TEMP =");
      lcd.print(t);
      lcd.setCursor(0, 2);  //move cursor to second line, first position
      lcd.print("FAN ON");
      //lcd.print(lightInt);
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(" *C ");
      Serial.println("FAN ON");
      digitalWrite(fan, HIGH);
      digitalWrite(light, LOW);
      digitalWrite(pump, LOW);
    } else if (lightInt < 30) {
      lcd.clear();
      lcd.setCursor(0, 0);  //move cursor to second line, first position
      lcd.print("LIGHT INTENSITY= ");
      lcd.print(lightInt);
      lcd.setCursor(0, 1);  //move cursor to second line, first position
      lcd.print("LIGHT ON");
      Serial.print("light intensity = ");
      Serial.println(lightInt);
      Serial.print("light on");

      digitalWrite(light, HIGH);
      digitalWrite(pump, LOW);
      digitalWrite(fan, LOW);

    }
    else if (m < 40) {
      lcd.clear();
      lcd.setCursor(0, 0);  //move cursor to second line, first position
      lcd.print("SOIL MOISTURE= ");
      lcd.print(soil);
      lcd.setCursor(0, 1);  //move cursor to second line, first position
      lcd.print("PUMP ON");
      Serial.print("Soil Moisture = ");
      Serial.println(m);
      Serial.println("pump on ");

      digitalWrite(light, LOW);
      digitalWrite(pump, HIGH);
      digitalWrite(fan, LOW);
    }
    else {
      lcd.setCursor(0, 0);  //move cursor to second line, first position
      lcd.print("HUMIDITY = ");
      lcd.print(h);
      lcd.setCursor(0, 1);  //move cursor to second line, first position
      lcd.print("TEMP =");
      lcd.print(t);
      lcd.setCursor(0, 2);  //move cursor to second line, first position
      lcd.print("LIGHT INT =");
      lcd.print(lightInt);
      lcd.setCursor(0, 3);  //move cursor to second line, first position
      lcd.print("SOIL MOIST=");
      lcd.print(m);
      Serial.print("light intensity = ");
      Serial.println(lightInt);
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(" *C ");
      Serial.print("light intensity = ");
      Serial.println(lightInt);
      Serial.print("Soil=");
      Serial.println(m);
      digitalWrite(light, LOW);
      digitalWrite(pump, LOW);
      digitalWrite(fan, LOW);
    }
  }
  if (State == 1) {
    lcd.clear();
    lcd.setCursor(3, 1);  //move cursor to second line, first position
    lcd.print("MANUAL SWITCH");
    
    if (Firebase.getString(relayData, "/relay")) {
      Serial.println("pump ");
      Serial.println(relayData.stringData());
      if (relayData.stringData() == "1") {
        lcd.clear();
        lcd.setCursor(0, 1);  //move cursor to second line, first position
        lcd.print("PUMP ON");
        Serial.println("pump on ");
        digitalWrite(pump, HIGH);
        delay(500);
      }
      else if (relayData.stringData() == "0") {
        lcd.clear();
        Serial.println("pump on ");
        lcd.setCursor(0, 1);  //move cursor to second line, first position
        lcd.print("PUMP OFF");
        digitalWrite(pump, LOW);
        delay(500);
      }
      //delay(1000);
    }
    if (Firebase.getString(relayData, "/fan")) {
      Serial.println("light ");
      Serial.println(relayData.stringData());
      if (relayData.stringData() == "1") {
        lcd.setCursor(0, 2);  //move cursor to second line, first position
        lcd.print("LIGHT ON");
        Serial.println("light on ");
        digitalWrite(light, HIGH);
        delay(500);
      }
      else if (relayData.stringData() == "0") {
        Serial.println("light oFF ");
        lcd.setCursor(0, 2);  //move cursor to second line, first position
        lcd.print("LIGHT OFF");
        digitalWrite(light, LOW);
        delay(500);
      }
      //delay(1000);
    }
    if (Firebase.getString(relayData, "/pump")) {
      Serial.println("fan ");
      Serial.println(relayData.stringData());
      if (relayData.stringData() == "1") {
        lcd.setCursor(0, 3);  //move cursor to second line, first position
        lcd.print("FAN ON");
        Serial.println("fan on ");
        digitalWrite(fan, HIGH);
        delay(500);
      }
      else if (relayData.stringData() == "0") {
        Serial.println("fan on ");
        lcd.setCursor(0, 3);  //move cursor to second line, first position
        lcd.print("FAN OFF");
        digitalWrite(fan, LOW);
        delay(500);
      }
      //delay(1000);
    }
  }


  Firebase.setFloat(firebaseData, "/Temperature", t);
  Firebase.setFloat(firebaseData, "/Humidity", h);
  Firebase.setFloat(firebaseData, "/Moisture", m);
  Firebase.setFloat(firebaseData, "/Light", lightInt);


  delay(1000);
}
