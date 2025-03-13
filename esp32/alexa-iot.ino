#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <WiFiClientSecure.h>
#include <SinricPro.h>
#include <SinricProFan.h>
#include <OneWire.h>
#include <DallasTemperature.h>

Preferences preferences;
String currentSsid;
String currentPassword;

const char* APP_KEY = "60fdc786-f852-4ab6-9c61-8902900bbaa7";
const char* APP_SECRET = "cd26ccb1-7642-4a22-92ba-294a5d5d33d3-fc021500-77b6-4245-bae1-c73e63a7b478";
const char* DEVICE_ID = "6791be6a113ff42c99b406c2";
const char* backendURL = "https://ota-esp32-production.up.railway.app/wifi";

#define RELAY_PIN 5
#define SENSOR_PIN 4
#define TEMPERATURE_ON 25.0
#define TEMPERATURE_OFF 24.0
#define TEMPERATURE_THRESHOLD_TIME 10000  

OneWire oneWire(SENSOR_PIN);
DallasTemperature sensors(&oneWire);

bool relayState = false;
bool manualControl = false;
unsigned long lastTempCheck = 0;
unsigned long lastWifiCheck = 0;
unsigned long lastCheckCredentials = 0;
unsigned long temperatureAboveThresholdTime = 0;

void checkCredentialsUpdate() {
  if (WiFi.status() != WL_CONNECTED) return;

  WiFiClientSecure client;
  client.setInsecure();  

  HTTPClient http;
  http.begin(client, backendURL); 
  
  if (http.GET() == HTTP_CODE_OK) {
    String payload = http.getString();
    StaticJsonDocument<256> doc;
    
    if (deserializeJson(doc, payload) == DeserializationError::Ok) {
      String newSsid = doc["ssid"].as<String>();
      String newPassword = doc["password"].as<String>();

      if (newSsid != currentSsid || newPassword != currentPassword) {
        preferences.putString("ssid", newSsid);
        preferences.putString("password", newPassword);
        Serial.println("Novas credenciais gravadas na EEPROM. Reiniciando...");
        delay(1000);
        ESP.restart();
      }
    }
  }
  http.end();
}

bool onPowerState(const String &deviceId, bool &state) {
  relayState = state;
  manualControl = true;
  digitalWrite(RELAY_PIN, relayState ? LOW : HIGH);
  Serial.printf("Ventilador %s (comando Alexa)\n", relayState ? "Ligado" : "Desligado");
  return true;
}

void checkTemperature() {
  if (millis() - lastTempCheck >= 5000) {
    lastTempCheck = millis();
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);

    if (temperatureC == DEVICE_DISCONNECTED_C || temperatureC < -50 || temperatureC > 125) {
      Serial.println("Erro ao ler o sensor de temperatura! Verifique a conexão.");
      return;
    }

    Serial.printf("Temperatura: %.2f°C | Estado do Cooler: %s\n", temperatureC, relayState ? "LIGADO" : "DESLIGADO");

    if (temperatureC >= TEMPERATURE_ON) {
      if (!relayState && millis() - temperatureAboveThresholdTime >= TEMPERATURE_THRESHOLD_TIME) {
        relayState = true;
        digitalWrite(RELAY_PIN, LOW);
        Serial.println("✅ Cooler LIGADO automaticamente após 10 segundos de alta temperatura!");
      } else {
        temperatureAboveThresholdTime = millis();
      }
    } else {
      temperatureAboveThresholdTime = 0;
      if (!manualControl) {
        relayState = false;
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println("🔴 Cooler DESLIGADO automaticamente pela temperatura!");
      }
    }
  }
}

void connectToWiFi() {
  preferences.begin("wifi", false);
  currentSsid = preferences.getString("ssid", "iPhone de joão");
  currentPassword = preferences.getString("password", "jvssenha");

  Serial.print("Conectando a ");
  Serial.println(currentSsid);
  WiFi.begin(currentSsid.c_str(), currentPassword.c_str());

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 20000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado!");
  } else {
    Serial.println("\nFalha ao conectar no WiFi!");
  }
}

void reconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado! Tentando reconectar...");
    WiFi.disconnect();
    WiFi.begin(currentSsid.c_str(), currentPassword.c_str());

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      delay(500);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi reconectado!");
    } else {
      Serial.println("\nFalha ao reconectar.");
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  sensors.begin();
  connectToWiFi();

  Serial.println("Configurando Sinric Pro...");
  SinricPro.begin(APP_KEY, APP_SECRET);
  SinricPro.restoreDeviceStates(true);

  SinricProFan& fan = SinricPro[DEVICE_ID];
  fan.onPowerState(onPowerState);

  Serial.println("Sinric Pro inicializado!");
}

void loop() {
  if (millis() - lastWifiCheck >= 10000) {
    lastWifiCheck = millis();
    reconnectWiFi();
  }

  if (millis() - lastCheckCredentials >= 30000) { 
    lastCheckCredentials = millis();
    checkCredentialsUpdate();
  }

  SinricPro.handle();
  checkTemperature();
}
