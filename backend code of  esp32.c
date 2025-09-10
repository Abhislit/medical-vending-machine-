#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <EEPROM.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Web server on port 80
WebServer server(80);

// Servo motors for dispensing (one per medicine slot)
Servo dispenserServos[10];  // Support up to 10 medicine slots
const int servosPins[10] = {13, 12, 14, 27, 26, 25, 33, 32, 35, 34};

// LED indicators
const int statusLED = 2;        // Built-in LED
const int dispensingLED = 4;    // External LED for dispensing status
const int errorLED = 5;         // Error indicator LED

// Buzzer for notifications
const int buzzerPin = 18;

// IR sensors for detecting dispensed items (optional)
const int irSensorPins[10] = {19, 21, 22, 23, 15, 16, 17, 0, 1, 3};

// Medicine inventory (stored in EEPROM)
struct Medicine {
  int id;
  int stock;
  bool available;
};

Medicine inventory[10];
const int EEPROM_SIZE = 512;
bool dispensingInProgress = false;

// Transaction structure
struct Transaction {
  String transactionId;
  int medicineId;
  int quantity;
  bool completed;
  unsigned long timestamp;
};

Transaction currentTransaction;

void setup() {
  Serial.begin(115200);
  
  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  loadInventory();
  
  // Initialize pins
  pinMode(statusLED, OUTPUT);
  pinMode(dispensingLED, OUTPUT);
  pinMode(errorLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Initialize IR sensors
  for (int i = 0; i < 10; i++) {
    pinMode(irSensorPins[i], INPUT_PULLUP);
  }
  
  // Initialize servos
  for (int i = 0; i < 10; i++) {
    dispenserServos[i].attach(servosPins[i]);
    dispenserServos[i].write(0); // Initial position
  }
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(statusLED, !digitalRead(statusLED)); // Blink while connecting
  }
  
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  digitalWrite(statusLED, HIGH); // Solid on when connected
  
  // Setup web server routes
  setupRoutes();
  
  // Start server
  server.begin();
  Serial.println("HTTP server started");
  
  // Play startup sound
  playStartupSound();
  
  // Initialize inventory if first time
  if (isFirstRun()) {
    initializeDefaultInventory();
    saveInventory();
  }
}

void loop() {
  server.handleClient();
  
  // Check for any pending dispensing operations
  if (dispensingInProgress) {
    handleDispensing();
  }
  
  // Monitor system status
  monitorSystem();
  
  delay(10);
}

void setupRoutes() {
  // Enable CORS for all routes
  server.enableCORS(true);
  
  // Get inventory status
  server.on("/api/inventory", HTTP_GET, handleGetInventory);
  
  // Update inventory (for restocking)
  server.on("/api/inventory", HTTP_POST, handleUpdateInventory);
  
  // Dispense medicine
  server.on("/api/dispense", HTTP_POST, handleDispenseRequest);
  
  // Get dispensing status
  server.on("/api/status", HTTP_GET, handleGetStatus);
  
  // Health check
  server.on("/api/health", HTTP_GET, handleHealthCheck);
  
  // Reset system
  server.on("/api/reset", HTTP_POST, handleReset);
  
  // Handle preflight requests
  server.on("/api/dispense", HTTP_OPTIONS, handleOptions);
  server.on("/api/inventory", HTTP_OPTIONS, handleOptions);
  server.on("/api/status", HTTP_OPTIONS, handleOptions);
  
  // Handle 404
  server.onNotFound(handleNotFound);
}

void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(200);
}

void handleGetInventory() {
  DynamicJsonDocument doc(1024);
  JsonArray medicines = doc.createNestedArray("medicines");
  
  for (int i = 0; i < 10; i++) {
    JsonObject medicine = medicines.createNestedObject();
    medicine["id"] = inventory[i].id;
    medicine["stock"] = inventory[i].stock;
    medicine["available"] = inventory[i].available;
    medicine["slot"] = i + 1;
  }
  
  doc["status"] = "success";
  doc["timestamp"] = millis();
  
  String response;
  serializeJson(doc, response);
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

void handleUpdateInventory() {
  if (server.hasArg("plain")) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, server.arg("plain"));
    
    int medicineId = doc["medicineId"];
    int newStock = doc["stock"];
    
    // Find and update medicine
    for (int i = 0; i < 10; i++) {
      if (inventory[i].id == medicineId) {
        inventory[i].stock = newStock;
        inventory[i].available = newStock > 0;
        saveInventory();
        break;
      }
    }
    
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{\"status\":\"success\"}");
  } else {
    server.send(400, "application/json", "{\"error\":\"Invalid request\"}");
  }
}

void handleDispenseRequest() {
  if (dispensingInProgress) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(409, "application/json", "{\"error\":\"Dispensing in progress\"}");
    return;
  }
  
  if (server.hasArg("plain")) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, server.arg("plain"));
    
    String transactionId = doc["transactionId"];
    JsonArray items = doc["items"];
    
    Serial.println("Dispensing request received:");
    Serial.println("Transaction ID: " + transactionId);
    
    // Validate and start dispensing
    bool canDispense = true;
    String errorMessage = "";
    
    // Check if all items are available
    for (JsonObject item : items) {
      int medicineId = item["id"];
      int quantity = item["quantity"];
      
      bool found = false;
      for (int i = 0; i < 10; i++) {
        if (inventory[i].id == medicineId) {
          found = true;
          if (inventory[i].stock < quantity) {
            canDispense = false;
            errorMessage = "Insufficient stock for medicine ID: " + String(medicineId);
            break;
          }
        }
      }
      
      if (!found) {
        canDispense = false;
        errorMessage = "Medicine ID not found: " + String(medicineId);
        break;
      }
    }
    
    if (canDispense) {
      // Start dispensing process
      currentTransaction.transactionId = transactionId;
      currentTransaction.completed = false;
      currentTransaction.timestamp = millis();
      dispensingInProgress = true;
      
      // Store dispensing queue
      DynamicJsonDocument dispensingQueue(1024);
      dispensingQueue["items"] = items;
      
      // Start dispensing
      startDispensing(dispensingQueue);
      
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "application/json", "{\"status\":\"dispensing_started\",\"transactionId\":\"" + transactionId + "\"}");
    } else {
      digitalWrite(errorLED, HIGH);
      playErrorSound();
      delay(100);
      digitalWrite(errorLED, LOW);
      
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(400, "application/json", "{\"error\":\"" + errorMessage + "\"}");
    }
  } else {
    server.send(400, "application/json", "{\"error\":\"Invalid request body\"}");
  }
}

void handleGetStatus() {
  DynamicJsonDocument doc(512);
  
  doc["dispensingInProgress"] = dispensingInProgress;
  doc["currentTransaction"] = currentTransaction.transactionId;
  doc["systemHealth"] = "OK";
  doc["wifiConnected"] = WiFi.status() == WL_CONNECTED;
  doc["freeHeap"] = ESP.getFreeHeap();
  doc["uptime"] = millis();
  
  String response;
  serializeJson(doc, response);
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

void handleHealthCheck() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"status\":\"healthy\",\"version\":\"1.0\"}");
}

void handleReset() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "{\"status\":\"resetting\"}");
  delay(1000);
  ESP.restart();
}

void handleNotFound() {
  server.send(404, "application/json", "{\"error\":\"Endpoint not found\"}");
}

void startDispensing(DynamicJsonDocument& items) {
  digitalWrite(dispensingLED, HIGH);
  playDispenseStartSound();
  
  JsonArray itemsArray = items["items"];
  
  for (JsonObject item : itemsArray) {
    int medicineId = item["id"];
    int quantity = item["quantity"];
    
    Serial.println("Dispensing Medicine ID: " + String(medicineId) + ", Quantity: " + String(quantity));
    
    // Find the slot for this medicine
    int slot = findMedicineSlot(medicineId);
    if (slot != -1) {
      dispenseFromSlot(slot, quantity);
      
      // Update inventory
      inventory[slot].stock -= quantity;
      if (inventory[slot].stock <= 0) {
        inventory[slot].available = false;
      }
    }
    
    delay(1000); // Delay between different medicines
  }
  
  // Save updated inventory
  saveInventory();
  
  // Complete transaction
  currentTransaction.completed = true;
  dispensingInProgress = false;
  digitalWrite(dispensingLED, LOW);
  
  playDispenseCompleteSound();
  
  Serial.println("Dispensing completed for transaction: " + currentTransaction.transactionId);
}

void dispenseFromSlot(int slot, int quantity) {
  Serial.println("Dispensing " + String(quantity) + " items from slot " + String(slot + 1));
  
  for (int i = 0; i < quantity; i++) {
    // Rotate servo to dispense one item
    dispenserServos[slot].write(90);  // Rotate to dispense position
    delay(500);                       // Wait for mechanical action
    dispenserServos[slot].write(0);   // Return to home position
    delay(1000);                      // Wait between items
    
    // Optional: Check IR sensor to confirm dispensing
    if (digitalRead(irSensorPins[slot]) == LOW) {
      Serial.println("Item detected - dispensed successfully");
      playItemDispensedSound();
    }
    
    // Brief pause between items
    delay(500);
  }
}

int findMedicineSlot(int medicineId) {
  for (int i = 0; i < 10; i++) {
    if (inventory[i].id == medicineId) {
      return i;
    }
  }
  return -1; // Not found
}

void handleDispensing() {
  // Monitor dispensing process
  // This function runs in the main loop during dispensing
  
  // Check for timeout (5 minutes max per transaction)
  if (millis() - currentTransaction.timestamp > 300000) {
    Serial.println("Dispensing timeout - aborting");
    dispensingInProgress = false;
    digitalWrite(dispensingLED, LOW);
    digitalWrite(errorLED, HIGH);
    playErrorSound();
    delay(100);
    digitalWrite(errorLED, LOW);
  }
}

void monitorSystem() {
  static unsigned long lastCheck = 0;
  
  if (millis() - lastCheck > 10000) { // Check every 10 seconds
    lastCheck = millis();
    
    // Check WiFi connection
    if (WiFi.status() != WL_CONNECTED) {
      digitalWrite(statusLED, LOW);
      Serial.println("WiFi disconnected - attempting reconnection");
      WiFi.begin(ssid, password);
    } else {
      digitalWrite(statusLED, HIGH);
    }
    
    // Check memory
    if (ESP.getFreeHeap() < 10000) {
      Serial.println("Low memory warning: " + String(ESP.getFreeHeap()));
    }
  }
}

// Sound functions
void playStartupSound() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
}

void playDispenseStartSound() {
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(buzzerPin, LOW);
}

void playItemDispensedSound() {
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
}

void playDispenseCompleteSound() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(300);
    digitalWrite(buzzerPin, LOW);
    delay(200);
  }
}

void playErrorSound() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
}

// EEPROM functions
void loadInventory() {
  for (int i = 0; i < 10; i++) {
    EEPROM.get(i * sizeof(Medicine), inventory[i]);
  }
}

void saveInventory() {
  for (int i = 0; i < 10; i++) {
    EEPROM.put(i * sizeof(Medicine), inventory[i]);
  }
  EEPROM.commit();
}

bool isFirstRun() {
  // Check if EEPROM has been initialized
  return inventory[0].id == 0 || inventory[0].id == 255;
}

void initializeDefaultInventory() {
  // Initialize with default medicine IDs and stock levels
  int defaultMedicines[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int defaultStock[10] = {15, 12, 8, 20, 18, 25, 16, 14, 10, 7};
  
  for (int i = 0; i < 10; i++) {
    inventory[i].id = defaultMedicines[i];
    inventory[i].stock = defaultStock[i];
    inventory[i].available = defaultStock[i] > 0;
  }
  
  Serial.println("Default inventory initialized");
}
