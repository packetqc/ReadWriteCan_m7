/*
  CAN Read Example

  This sketch shows how to use the CAN transceiver on the Machine
  Control and how to receive data from the RX CAN channel.

  Circuit:
   - Portenta H7
   - Machine Control

*/
#include <Arduino_MachineControl.h>
#include <CAN.h>

using namespace machinecontrol;

#define DATARATE_2MB     2000000
#define DATARATE_1_5MB   1500000
#define DATARATE_1MB     1000000
#define DATARATE_500KB   500000


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }

  Serial.println("Start CAN initialization");
  comm_protocols.enableCAN();
  comm_protocols.can.frequency(DATARATE_1MB);
  Serial.println("Initialization done");
}

unsigned long previousMillis = 0; 
long interval = 2000;

int counter = 0;
unsigned char payload = 0x49;
int payload_size = 1;

void loop() {
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;  
     
    mbed::CANMessage msgToSend = mbed::CANMessage(13ul, &payload, payload_size);
    
    if (comm_protocols.can.write(msgToSend)) {
      counter = counter + 1;
      Serial.print("ID: ");
      Serial.print(msgToSend.id);
      Serial.print(",\tOUT:\t");
      for( int i=0; i<msgToSend.len; i++ ) {
        Serial.print(msgToSend.data[i], HEX);
        Serial.print("\t");
      }
      Serial.println();
    } else {
      Serial.print("ID: ");
      Serial.print(msgToSend.id);
      Serial.println(", Transmission Error: ");
      Serial.println(comm_protocols.can.tderror());
      comm_protocols.can.reset();
    }
  }

  mbed::CANMessage msg;
  if (comm_protocols.can.read(msg)) {
    // Print the sender ID
    Serial.print("ID: ");
    Serial.print(msg.id);

    // Print the first Payload Byte
    Serial.print(",\tIN :\t");
    for( int i=0; i<msg.len; i++ ) {
      Serial.print(msg.data[i], HEX);
      Serial.print("\t");
    }
    Serial.println();
  }

  delay(100);
}
