// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>

// define pins (varies per shield/board)
// https://github.com/sandeepmistry/arduino-BLEPeripheral#pinouts
// Blend
#define BLE_REQ     9
#define BLE_RDY     8
#define BLE_RST     5

int count = 0;
bool pressed = false;
int buttonState = 0;

// create peripheral instance, see pinouts above
BLEPeripheral blePeripheral = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

// create service
BLEService buttonService = BLEService("FBE0");

// create characteristic
BLEUnsignedIntCharacteristic buttonCharacteristic = BLEUnsignedIntCharacteristic("FBE1", BLERead);
BLEDescriptor buttonDescriptor = BLEDescriptor("2901", "Button State");

// #define BUTTON_PIN 7 // RedBear Blend
// #define BUTTON_PIN D2 // RedBear Nano
#define BUTTON_PIN 5  // RFduino

unsigned long lastReadTime = 0;
int readInterval = 200;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);

  // set advertised local name and service UUID
  blePeripheral.setLocalName("Counter");
  blePeripheral.setDeviceName("Counter");
  blePeripheral.setAdvertisedServiceUuid(buttonService.uuid());

  // add service and characteristics
  blePeripheral.addAttribute(buttonService);
  blePeripheral.addAttribute(buttonCharacteristic);
  blePeripheral.addAttribute(buttonDescriptor);

  // begin initialization
  blePeripheral.begin();
  Serial.println(F("Bluetooth Button"));
}

void loop() {
  // Tell the bluetooth radio to do whatever it should be working on
  blePeripheral.poll();

  // limit how often we read the button
  if (millis() - lastReadTime > readInterval) {
    readButton();
    lastReadTime = millis();
  }
}

void readButton() {
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH) {
    pressed = true;
  } else {
    //checks if realeased
    if (pressed) {
      count++;
      pressed = false;
      Serial.println(count);
      buttonCharacteristic.setValue(count);
    }
  }
}
