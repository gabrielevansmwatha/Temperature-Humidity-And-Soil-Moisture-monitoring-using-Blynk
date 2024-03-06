//Put your Blynk Auth Token here
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "";  // type your WiFi name
char pass[] = "";  // type your WiFi password

BlynkTimer timer;

// DHT Sensor setup
#define DHTPIN 2       // D4 on ESP8266 - change according to your physical setup
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);

int WET = 16; // Wet Indicator at Digital PIN D0
int DRY = 2;  // Dry Indicator at Digital PIN D4
int sense_Pin = A0; // Soil Sensor input at Analog PIN A0
int value = 0;

void sendSensorData() {
    value = analogRead(sense_Pin);
    int moisture_percentage = map(value, 0, 1023, 100, 0); 
    Serial.print("MOISTURE LEVEL : ");
    Serial.println(moisture_percentage);
    
    // Reading temperature or humidity takes about 250 milliseconds!
    float h = dht.readHumidity();
    float t = dht.readTemperature(); // Read temperature as Celsius (the default)
    // You can also read temperature as Fahrenheit (isFahrenheit = true)
    // float t = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
    } else {
        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: ");
        Serial.print(t);
        Serial.println(" *C ");
    }

    Blynk.virtualWrite(V2, moisture_percentage); // Sending soil moisture value to Virtual Pin V2
    Blynk.virtualWrite(V0, t); // Sending temperature to Virtual Pin V3
    Blynk.virtualWrite(V1, h); // Sending humidity to Virtual Pin V4

    if(moisture_percentage < 50) {
        digitalWrite(WET, HIGH);
        digitalWrite(DRY, LOW);
    } else {
        digitalWrite(WET, LOW);
        digitalWrite(DRY, HIGH);
    }
}

void setup() {
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass);
    dht.begin(); // Initialize DHT sensor
    pinMode(WET, OUTPUT);
    pinMode(DRY, OUTPUT);
    delay(2000); // Ensure stable conditions before starting

    timer.setInterval(2000L, sendSensorData); // Set the interval to 2 seconds (2000 milliseconds) to ensure DHT11 reading stability
}

void loop() {
    Blynk.run(); // Initiates Blynk
    timer.run(); // Initiates the BlynkTimer
}