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
#define END_GAP 8000

const uint16_t power_button[24] = {HEX_D,HEX_8,HEX_4};
const uint16_t airflow_dir[24] = {HEX_D,HEX_8,HEX_1};
const uint16_t temp_ctrl[24] = {HEX_D,HEX_C,HEX_3};
const uint16_t up_arrow[24] = {HEX_D, HEX_C, HEX_6};
const uint16_t down_arrow[24] = {HEX_D, HEX_8, HEX_2};

// GPIO to use to control the IR LED circuit. Recommended: 4 (D2).
const uint16_t kIrLedPin = 4;

// kFrequency is the modulation frequency all messages will be replayed at.
const uint16_t kFrequency = 38000; 

IRsend irsend(kIrLedPin, false);

void sendPower(){
  irsend.sendRaw(power_button,sizeof(power_button)/sizeof(uint16_t),kFrequency);
  delay(8);
  irsend.sendRaw(power_button,sizeof(power_button)/sizeof(uint16_t),kFrequency);
  delay(8);
  irsend.sendRaw(power_button,sizeof(power_button)/sizeof(uint16_t),kFrequency);
  delay(8);
  irsend.sendRaw(power_button,sizeof(power_button)/sizeof(uint16_t),kFrequency);

}

void sendMyData (const uint16_t *data, int data_size, int repeat,int gap){
  Serial.printf("sending %d entries of data %d times. Gap of %d ms\n",data_size, repeat, gap);
  Serial.println("to send: ");
  for (int i=0; i<data_size; i++){
    Serial.printf("%d ",data[i]);
  }
  //irsend.sendRaw(warm_up,2,38000);
  Serial.println();
  for (int i=0; i<repeat; i++){
    irsend.sendRaw(data,data_size,kFrequency);
    delay(gap);
  }
}

void setup() {
  irsend.begin();
  Serial.begin(74880);
}

void loop() {
  int msg_gap = 9;
  if (Serial.available() > 0) {
    int incomingByte = Serial.read(); // read the incoming byte:
    Serial.print(" I received: ");
    Serial.println(incomingByte);

    switch (incomingByte) {
      case 'p':
      //power button
        Serial.println("sending power_button");
        sendMyData(power_button,sizeof(power_button)/sizeof(uint16_t),6,7);
        //sendPower();
        break;
      case 'u':
      //up arrow
        Serial.println("sending up_arrow");
        sendMyData(up_arrow,sizeof(up_arrow)/sizeof(uint16_t),6,7);
        break;
      case 'd':
      //down arrow
        Serial.println("sending down_arrow");
        sendMyData(down_arrow,sizeof(down_arrow)/sizeof(uint16_t),6,7);
        break;
      case 'f':
      //airflow direction in/out
        Serial.println("sending airflow_dir");
        sendMyData(airflow_dir,sizeof(airflow_dir)/sizeof(uint16_t),3,8);
        break;
      case 't':
      //thermometer button
        Serial.println("sending temp_ctrl");
        sendMyData(temp_ctrl,sizeof(temp_ctrl)/sizeof(uint16_t),3,8);
        break;
      default:
        break;
    }

  }

}
