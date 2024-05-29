
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
  
  // Keep reading from HM-10 and send to Arduino Serial Monitor  
  while(BTSerial.available() > 0) {

    // Read from the bluetooth buffer
    c = BTSerial.read();

    // Write the received bluetooth data to the Serial Monitor
    Serial.print(c);
  }
  
  // Print a newline once all data is read for this cycle
  if (BTSerial.available() == 0) {
    Serial.println();
  }

  delay(5000);
}
