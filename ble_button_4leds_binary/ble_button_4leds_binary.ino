// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>

// define pins (varies per shield/board)
// https://github.com/sandeepmistry/arduino-BLEPeripheral#pinouts
// Blend
#define BLE_REQ     9
#define BLE_RDY     8
#define BLE_RST     5

// create peripheral instance, see pinouts above
BLEPeripheral blePeripheral = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

// create service
BLEService buttonService = BLEService("FBE0");

// create characteristic
BLEUnsignedIntCharacteristic buttonCharacteristic = BLEUnsignedIntCharacteristic("FBE1", BLERead);
BLEDescriptor buttonDescriptor = BLEDescriptor("2901", "Button State");

// #define BUTTON_PIN 7 // RedBear Blend
// #define BUTTON_PIN D2 // RedBear Nano
#define BUTTON_PIN 6  // RFduino

#define LED_PIN1 2
#define LED_PIN2 3
#define LED_PIN3 4
#define LED_PIN4 5

int maxLED_count = 15;
int LED_count = 0;
int count = 0;
bool pressed = false;
int buttonState = 0;

unsigned long lastReadTime = 0;
int readInterval = 200;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  pinMode(LED_PIN4, OUTPUT);

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
      LED_count++;
      ledDisplay();
      pressed = false;
      Serial.print(count);
      Serial.print(",");
      Serial.println(LED_count);
      buttonCharacteristic.setValue(count);
    }
  }
}

void ledDisplay() {

  switch (LED_count) {
    case 1:
      // 0001
      digitalWrite(LED_PIN1, HIGH);
      break;
    case 2:
      // 0010
      digitalWrite(LED_PIN2, HIGH);
      break;
    case 3:
      // 0011
      digitalWrite(LED_PIN1, HIGH);
      digitalWrite(LED_PIN2, HIGH);
      break;
    case 4:
      // 0100
      digitalWrite(LED_PIN3, HIGH);
      break;
    case 5:
      // 0101
      digitalWrite(LED_PIN1, HIGH);
      digitalWrite(LED_PIN3, HIGH);
      break;
    case 6:
      // 0110
      digitalWrite(LED_PIN2, HIGH);
      digitalWrite(LED_PIN3, HIGH);
      break;
    case 7:
      // 0111
      digitalWrite(LED_PIN1, HIGH);
      digitalWrite(LED_PIN2, HIGH);
      digitalWrite(LED_PIN3, HIGH);
      break;
    case 8:
      // 1000
      digitalWrite(LED_PIN4, HIGH);
      break;
    case 9:
      // 1001
      digitalWrite(LED_PIN1, HIGH);
      digitalWrite(LED_PIN4, HIGH);
      break;
    case 10:
      // 1010
      digitalWrite(LED_PIN2, HIGH);
      digitalWrite(LED_PIN4, HIGH);
      break;
    case 11:
      // 1011
      digitalWrite(LED_PIN1, HIGH);
      digitalWrite(LED_PIN2, HIGH);
      digitalWrite(LED_PIN4, HIGH);
      break;
    case 12:
      // 1100
      digitalWrite(LED_PIN3, HIGH);
      digitalWrite(LED_PIN4, HIGH);
      break;
    case 13:
      // 1101
      digitalWrite(LED_PIN1, HIGH);
      digitalWrite(LED_PIN3, HIGH);
      digitalWrite(LED_PIN4, HIGH);
      break;
    case 14:
      // 1110
      digitalWrite(LED_PIN2, HIGH);
      digitalWrite(LED_PIN3, HIGH);
      digitalWrite(LED_PIN4, HIGH);
      break;
    case 15:
      // 1111
      digitalWrite(LED_PIN1, HIGH);
      digitalWrite(LED_PIN2, HIGH);
      digitalWrite(LED_PIN3, HIGH);
      digitalWrite(LED_PIN4, HIGH);
      break;
  }

  delay(200);
  //turn off all LEDs
  digitalWrite(LED_PIN1, LOW);
  digitalWrite(LED_PIN2, LOW);
  digitalWrite(LED_PIN3, LOW);
  digitalWrite(LED_PIN4, LOW);

  // reset LED counter when max is readched
  if (LED_count == maxLED_count) {
    LED_count = 0;
  }
}

