#define dataPin 2
#define latchPin 3
#define clockPin 4

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
BLEService ledService = BLEService("FF10");

// create number
BLEIntCharacteristic ledStripCharacteristic = BLEIntCharacteristic("FF12", BLERead | BLEWrite);
BLEDescriptor ledStripDescriptor = BLEDescriptor("2901", "Number");

void setup() {
  Serial.begin(9600);

  // set LED pin to output mode
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  // set advertised local name and service UUID
  blePeripheral.setLocalName("LEDStrip");
  blePeripheral.setDeviceName("LEDStrip");
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

  // add service and characteristics
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(ledStripCharacteristic);
  blePeripheral.addAttribute(ledStripDescriptor);

  // assign event handlers for characteristic
  ledStripCharacteristic.setEventHandler(BLEWritten, ledStripCharacteristicWritten);

  // begin initialization
  blePeripheral.begin();
  Serial.println(F("Bluetooth LED"));
  shiftOut(dataPin, clockPin, MSBFIRST, 255);
  shiftOut(dataPin, clockPin, MSBFIRST, 255);
  shiftOut(dataPin, clockPin, MSBFIRST, 255);
  shiftOut(dataPin, clockPin, MSBFIRST, 255);

}

void loop() {
  // Tell the bluetooth radio to do whatever it should be working on
  //shiftOut(dataPin, clockPin, MSBFIRST, 255);
  //shiftOut(dataPin, clockPin, MSBFIRST, 255);
  blePeripheral.poll();

}

void ledStripCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  Serial.println("in here");
  digitalWrite(latchPin, LOW);

  //int x = ledStripCharacteristic.value();
  Serial.println(ledStripCharacteristic.value());
  int x = ledStripCharacteristic.value();
  int data1 = x & 0xff;
  int data2 = x >> 8;
  int data3 = x >> 16;
  int data4 = x >> 24;

  shiftOut(dataPin, clockPin, MSBFIRST, data4);
  shiftOut(dataPin, clockPin, MSBFIRST, data3);
  shiftOut(dataPin, clockPin, MSBFIRST, data2);
  shiftOut(dataPin, clockPin, MSBFIRST, data1);
  digitalWrite(latchPin, HIGH);
  //delay(1000);
}
