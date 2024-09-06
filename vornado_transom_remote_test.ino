#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>


#define BIT_ONE 1260, 400 //long mark, short space
#define BIT_ZERO 420, 1260 //short mark, long space
#define HEX_D BIT_ONE, BIT_ONE, BIT_ZERO, BIT_ONE
#define HEX_C BIT_ONE, BIT_ONE, BIT_ZERO, BIT_ZERO
#define HEX_8 BIT_ONE, BIT_ZERO, BIT_ZERO, BIT_ZERO
#define HEX_6 BIT_ZERO, BIT_ONE, BIT_ONE, BIT_ZERO
#define HEX_4 BIT_ZERO, BIT_ONE, BIT_ZERO, BIT_ZERO
#define HEX_3 BIT_ZERO, BIT_ZERO, BIT_ONE, BIT_ONE
#define HEX_2 BIT_ZERO, BIT_ZERO, BIT_ONE, BIT_ZERO
#define HEX_1 BIT_ZERO, BIT_ZERO, BIT_ZERO, BIT_ONE

const uint16_t power_button[24] = {HEX_D,HEX_8,HEX_4}; //d85 is also power
const uint16_t airflow_dir[24] = {HEX_D,HEX_8,HEX_1};
const uint16_t temp_ctrl[24] = {HEX_D,HEX_C,HEX_3};
const uint16_t up_arrow[24] = {HEX_D, HEX_C, HEX_6};
const uint16_t down_arrow[24] = {HEX_D, HEX_8, HEX_2}; //d83 also works
const uint16_t dummy[24] = {HEX_D, HEX_D, HEX_D}; 

// GPIO to use to control the IR LED circuit. Recommended: 4 (D2).
const uint16_t kIrLedPin = 4;

// kFrequency is the modulation frequency all messages will be replayed at.
const uint16_t kFrequency = 38000; 

IRsend irsend(kIrLedPin, false);

void sendMyData (const uint16_t *data, int data_size, int repeat){
  Serial.printf("sending %d entries of data %d times. Gap of %d ms\n",data_size, repeat);
  Serial.println("to send: ");
  for (int i=0; i<data_size; i++){
    Serial.printf("%d ",data[i]);
  }
  Serial.println();
  // The fan can 'sleep' the control pannel and need to be worken up.
  // We don't want to use any actual commands in case the control panel isn't asleep.
  // It turns out that an invalid, but 'properly formatted' pulse train will wake the panel.
  // If the panel isn't sleeping, this is a no-op.
  // If the panel is sleeping, this will wake it.
  // The command needs to be sent twice. The first looks like it's used to 
  // get the fan's 'attention' and the second will register as an actual command attempt. 
  irsend.sendRaw(dummy,24,kFrequency);
  delay(7);
  irsend.sendRaw(dummy,24,kFrequency);
  delay(9); //a gap in transmitting longer than 8ms tells the fan that a button has been released and pressed again.
  for (int i=0; i<repeat; i++){
    irsend.sendRaw(data,data_size,kFrequency);
    delay(7); //a delay of 7ms seems to work for all commands. Anything lower than 6 will cause the command to be ignored.
  }
}

void setup() {
  irsend.begin();
  Serial.begin(115200);
}

void loop() {
  int msg_gap = 9;
  if (Serial.available() > 0) {
    int incomingByte = Serial.read(); // read the incoming byte:

    switch (incomingByte) {
      case 'p':
        //power button
        Serial.println("sending power_button");
        sendMyData(power_button,sizeof(power_button)/sizeof(uint16_t),3);
        break;
      case 'u':
        //up arrow
        Serial.println("sending up_arrow");
        sendMyData(up_arrow,sizeof(up_arrow)/sizeof(uint16_t),3);
        break;
      case 'd':
      //down arrow
        Serial.println("sending down_arrow");
        sendMyData(down_arrow,sizeof(down_arrow)/sizeof(uint16_t),3);
        break;
      case 'f':
      //airflow direction in/out
        Serial.println("sending airflow_dir");
        sendMyData(airflow_dir,sizeof(airflow_dir)/sizeof(uint16_t),3);
        break;
      case 't':
      //thermometer button
        Serial.println("sending temp_ctrl");
        sendMyData(temp_ctrl,sizeof(temp_ctrl)/sizeof(uint16_t),3);
        break;
      default:
        break;
    }

  }

}
