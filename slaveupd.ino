// Library to make a Software UART
#include <SoftwareSerial.h>

#define RX 3
#define TX 2

#define BAUDRATE 9600

char c = ' ';
 
SoftwareSerial BTSerial(RX, TX); // (RX, TX)
 
void setup() {

  // Start Serial Monitor for feedback
  Serial.begin(BAUDRATE);

  // HM-10 default speed in AT command mode
  BTSerial.begin(BAUDRATE);
}

void loop() {
  bool dataReceived = false;

  // Keep reading from HM-10 and send to Arduino Serial Monitor  
  while(BTSerial.available() > 0) {
    c = BTSerial.read();
    Serial.print(c);
    dataReceived = true;
  }
  
  // Print a newline only if data was received
  if (dataReceived) {
    Serial.println();
  }

  delay(100);
}
