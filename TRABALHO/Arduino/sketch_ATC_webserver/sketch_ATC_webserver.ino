#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>

const char *apSSID = "ESP8266";
const char *apPassword = "123456789";

Adafruit_SHT31 sht31 = Adafruit_SHT31();
ESP8266WebServer server(80);

// Data structure to store timestamped temperature and humidity readings
struct SensorData {
  float temperature;
  float humidity;
  unsigned long timestamp;
};

// Circular buffer to store the last 5 minutes of data (adjust as needed)
const int bufferSize = 60 * 5; // 5 minutes at 2 readings per second (updated every 500ms)
SensorData dataBuffer[bufferSize];
int bufferIndex = 0;

void setup() {
  Serial.begin(115200);
  WiFi.softAP(apSSID, apPassword);
  Serial.println("Access Point created");
  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(apIP);

  if (!sht31.begin(0x44)) {
    Serial.println("Couldn't find SHT31");
    while (1);
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/data.csv", HTTP_GET, handleDataCSV);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    // Store data in the buffer
    dataBuffer[bufferIndex].temperature = t;
    dataBuffer[bufferIndex].humidity = h;
    dataBuffer[bufferIndex].timestamp = millis();
    
    // Move to the next position in the circular buffer
    bufferIndex = (bufferIndex + 1) % bufferSize;
  } else {
    Serial.println("Sensor reading error");
  }

  // Print sensor data to Serial Monitor
  if (!isnan(t)) {
    Serial.print("Temp *C = "); Serial.print(t); Serial.print(" ");
  } else { 
    Serial.println("TEMP ERROR      ");
  }
  
  if (!isnan(h)) {
    Serial.print("Hum     = "); Serial.print(h); Serial.println("%");
  } else { 
    Serial.println("HUM ERROR       ");
  }

  delay(500); // Read data every 500 milliseconds
}

void handleRoot() {
  String html = "<html><head><meta http-equiv='refresh' content='0.5'></head><body>";
  html += "<h1>SHT31 Sensor Data</h1>";
  html += "<p>Temperature: " + String(dataBuffer[bufferIndex].temperature) + " &deg;C</p>";
  html += "<p>Humidity: " + String(dataBuffer[bufferIndex].humidity) + " %</p>";
  html += "<a href='/data.csv' download='sensor_data.csv'>Download CSV</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleDataCSV() {
  // Calculate elapsed time since the start of data collection
  unsigned long elapsedTime = millis() - dataBuffer[bufferIndex].timestamp;

  String csv = "Relative Time (s),Temperature (C),Humidity (%)\n";
  
  // Generate CSV data for the elapsed time (up to a maximum of 5 minutes)
  for (int i = 0; i < bufferSize; i++) {
    int index = (bufferIndex + i) % bufferSize;
    unsigned long timestamp = dataBuffer[index].timestamp;
    float temperature = dataBuffer[index].temperature;
    float humidity = dataBuffer[index].humidity;

    // Calculate relative time in seconds
    unsigned long relativeTime = (timestamp - dataBuffer[bufferIndex].timestamp) / 1000;

    // Check if the elapsed time is greater than the relative time
    if (elapsedTime > relativeTime) {
      csv += String(relativeTime) + "," + String(temperature) + "," + String(humidity) + "\n";
    }
  }

  server.send(200, "text/csv", csv);
}
