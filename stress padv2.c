‎#include <WiFi.h>
‎#include <HTTPClient.h>
‎#include <Wire.h>
‎#include <Adafruit_GFX.h>
‎#include <Adafruit_SSD1306.h>
‎
‎const char* ssid = "Wokwi-GUEST";
‎const char* password = "";
‎const char* blynkToken = "KKgmfyR-Q4A6XbSqh0-PPBg2kC1b5yN2";
‎const char* blynkServer = "sgp1.blynk.cloud";
‎
‎const int ledPin = 2;    // Onboard LED
‎const int buttonPin = 4; // Push button pin
‎int consecutiveHighStress = 0; // counter for 3 consecutive high stress readings
‎
‎bool lastButtonState = HIGH; // For one-click detection
‎bool systemRunning = false;  // Flag to start continuous operation
‎
‎// ---- OLED setup ----
‎#define SCREEN_WIDTH 128
‎#define SCREEN_HEIGHT 64
‎#define OLED_RESET    -1
‎Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
‎
‎void setup() {
‎  Serial.begin(115200);
‎  pinMode(ledPin, OUTPUT);
‎  pinMode(buttonPin, INPUT_PULLUP);
‎
‎  // ---- OLED INIT ----
‎  Wire.begin(21, 22); // SDA = 21, SCL = 22
‎  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
‎    Serial.println("❌ OLED not found!");
‎    for (;;);
‎  }
‎  display.clearDisplay();
‎  display.setTextSize(1);
‎  display.setTextColor(SSD1306_WHITE);
‎  display.setCursor(0, 20);
‎  display.println("Connecting WiFi...");
‎  display.display();
‎
‎  // ---- WiFi connection ----
‎  WiFi.begin(ssid, password);
‎  Serial.print("Connecting");
‎  while (WiFi.status() != WL_CONNECTED) {
‎    delay(500);
‎    Serial.print(".");
‎  }
‎  Serial.println("\n✅ WiFi Connected!");
‎
‎  display.clearDisplay();
‎  display.setCursor(0, 20);
‎  display.println("WiFi Connected!");
‎  display.display();
‎  delay(1000);
‎}
‎
‎void loop() {
‎  int buttonState = digitalRead(buttonPin);
‎
‎  // Detect single press (HIGH → LOW transition)
‎  if (buttonState == LOW && lastButtonState == HIGH) {
‎    delay(50);
‎    if (digitalRead(buttonPin) == LOW) {
‎      Serial.println("🔘 Button clicked — starting continuous operation...");
‎      systemRunning = true;
‎
‎      // Update OLED on start
‎      display.clearDisplay();
‎      display.setCursor(0, 20);
‎      display.println("System Running...");
‎      display.display();
‎      delay(1000);
‎    }
‎  }
‎  lastButtonState = buttonState;
‎
‎  if (systemRunning) {
‎    if (WiFi.status() == WL_CONNECTED) {
‎      HTTPClient http;
‎
‎      // ---- Simulate sensor reading ----
‎      int sensorValue;
‎      if (random(0, 100) < 60) { 
‎        sensorValue = random(75, 101); // 60% chance for higher stress
‎      } else {
‎        sensorValue = random(0, 75);
‎      }
‎
‎      float data = sensorValue / 100.0; // normalize 0–1
‎
‎      // ---- Send V1 (raw sensor value) ----
‎      String urlV1 = String("http://") + blynkServer +
‎                     "/external/api/update?token=" + blynkToken +
‎                     "&V1=" + String(sensorValue);
‎      http.begin(urlV1);
‎      int code1 = http.GET();
‎      Serial.printf("📤 Sent V1=%d | Response: %d\n", sensorValue, code1);
‎      http.end();
‎
‎      // ---- Determine stress level ----
‎      String stressText;
‎
‎      // Track which LED should be ON
‎      int ledBlue = 0;
‎      int ledYellow = 0;
‎      int ledRed = 0;
‎
‎      if (data < 0.5) {
‎        stressText = "NORMAL";
‎        ledBlue = 1;  // BLUE ON
‎        ledYellow = 0;
‎        ledRed = 0;
‎        consecutiveHighStress = 0;
‎      } 
‎      else if (data >= 0.5 && data <= 0.75) {
‎        stressText = "MILD TENSION";
‎        ledBlue = 0;
‎        ledYellow = 1;  // YELLOW ON
‎        ledRed = 0;
‎        consecutiveHighStress = 0;
‎      } 
‎      else {
‎        stressText = "HIGH STRESS DETECTED";
‎        ledBlue = 0;
‎        ledYellow = 0;
‎        ledRed = 1;  // RED ON
‎        consecutiveHighStress++;
‎      }
‎
‎      // ---- OLED DISPLAY ----
‎      display.clearDisplay();
‎      display.setTextSize(2);
‎      display.setTextColor(SSD1306_WHITE);
‎      display.setCursor(0, 10);
‎      display.println(stressText);
‎      display.setTextSize(1);
‎      display.setCursor(0, 45);
‎      display.printf("Value: %.2f", data);
‎      display.display();
‎
‎      // ---- LED control (3 consecutive ≥ 0.75) ----
‎      bool ledOn = (consecutiveHighStress >= 3);
‎      digitalWrite(ledPin, ledOn ? HIGH : LOW);
‎
‎      // ---- Send LED (V0) ----
‎      String urlV0 = String("http://") + blynkServer +
‎                     "/external/api/update?token=" + blynkToken +
‎                     "&V0=" + (ledOn ? "1" : "0");
‎      http.begin(urlV0);
‎      int code2 = http.GET();
‎      Serial.printf("💡 LED (V0) = %s | Response: %d | Consecutive High = %d\n", 
‎                    ledOn ? "ON" : "OFF", code2, consecutiveHighStress);
‎      http.end();
‎
‎      // ---- Send LED Colors (V4 BLUE, V5 YELLOW, V6 RED) ----
‎      String urlV4 = String("http://") + blynkServer +
‎                     "/external/api/update?token=" + blynkToken +
‎                     "&V4=" + String(ledBlue);
‎      http.begin(urlV4);
‎      int codeV4 = http.GET();
‎      Serial.printf("🔵 V4 (Blue) = %d | Response: %d\n", ledBlue, codeV4);
‎      http.end();
‎
‎      String urlV5 = String("http://") + blynkServer +
‎                     "/external/api/update?token=" + blynkToken +
‎                     "&V5=" + String(ledYellow);
‎      http.begin(urlV5);
‎      int codeV5 = http.GET();
‎      Serial.printf("🟡 V5 (Yellow) = %d | Response: %d\n", ledYellow, codeV5);
‎      http.end();
‎
‎      String urlV6 = String("http://") + blynkServer +
‎                     "/external/api/update?token=" + blynkToken +
‎                     "&V6=" + String(ledRed);
‎      http.begin(urlV6);
‎      int codeV6 = http.GET();
‎      Serial.printf("🔴 V6 (Red) = %d | Response: %d\n", ledRed, codeV6);
‎      http.end();
‎
‎      // ---- Send notification ----
‎      if (consecutiveHighStress == 3) {
‎        String notifyURL = String("http://") + blynkServer +
‎                           "/external/api/logEvent?token=" + blynkToken +
‎                           "&code=stress_detected";
‎        http.begin(notifyURL);
‎        int notifyCode = http.GET();
‎        Serial.printf("📱 Notification sent | Response: %d\n", notifyCode);
‎        http.end();
‎      }
‎
‎      // ---- Update Blynk display text (V2) ----
‎      String safeText = stressText;
‎      safeText.replace(" ", "%20");
‎
‎      String urlV2 = String("http://") + blynkServer +
‎                     "/external/api/update?token=" + blynkToken +
‎                     "&V2=" + safeText;
‎      http.begin(urlV2);
‎      int code3 = http.GET();
‎      Serial.printf("📝 Display (V2) = %s | Response: %d\n", stressText.c_str(), code3);
‎      http.end();
‎
‎      // ---- Send normalized value (0–1) to Gradient Ramp (V3) ----
‎      String urlV3 = String("http://") + blynkServer +
‎                     "/external/api/update?token=" + blynkToken +
‎                     "&V3=" + String(data, 2);
‎      http.begin(urlV3);
‎      int code4 = http.GET();
‎      if (code4 > 0) {
‎        Serial.printf("🎨 Gradient Ramp (V3) updated: %.2f | Response: %d\n", data, code4);
‎      } else {
‎        Serial.println("⚠️ Failed to update Gradient Ramp");
‎      }
‎      http.end();
‎
‎    } else {
‎      Serial.println("⚠️ Reconnecting WiFi...");
‎      WiFi.begin(ssid, password);
‎    }
‎
‎    delay(5000); // update every 5 seconds
‎  }
‎}
‎