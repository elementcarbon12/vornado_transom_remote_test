/*
/// Send a raw IRremote message.
///
/// @param[in] buf An array of uint16_t's that has microseconds elements.
/// @param[in] len Nr. of elements in the buf[] array.
/// @param[in] hz Frequency to send the message at. (kHz < 1000; Hz >= 1000)
/// @note Even elements are Mark times (On), Odd elements are Space times (Off).
/// Ref:
///   examples/IRrecvDumpV2/IRrecvDumpV2.ino (or later)
void IRsend::sendRaw(const uint16_t buf[], const uint16_t len,
                     const uint16_t hz)
*/

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>


#define BIT_ONE 1300, 400 //long mark, short space
#define BIT_ZERO 400, 1300 //short mark, long space
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

IRsend irsend(kIrLedPin);


void sendMyData (const uint16_t *data, int data_size, int repeat,int gap){
  const uint16_t warm_up [] = {2000, 2000};
  Serial.printf("sending %d entries of data %d times. Gap of %d ms\n",data_size, repeat, gap);
  Serial.println("to send: ");
  for (int i=0; i<data_size; i++){
    Serial.printf("%d ",data[i]);
  }
  //irsend.sendRaw(warm_up,2,38000);
  Serial.println();
  for (int i=0; i<repeat; i++){
    irsend.sendRaw(data,data_size,kFrequency);
    delay(8);
  }
}





void setup() {
  irsend.begin();
  Serial.begin(115200);
}

void loop() {
  int msg_gap = 8;
  if (Serial.available() > 0) {
    int incomingByte = Serial.read(); // read the incoming byte:
    Serial.print(" I received: ");
    Serial.println(incomingByte);

    switch (incomingByte) {
      case 'p':
      //power button
        Serial.println("sending power_button");
        sendMyData(power_button,sizeof(power_button)/sizeof(uint16_t),7,msg_gap);
        break;
      case 'u':
      //up arrow
        Serial.println("sending up_arrow");
        sendMyData(up_arrow,sizeof(up_arrow)/sizeof(uint16_t),6,msg_gap);
        break;
      case 'd':
      //down arrow
        Serial.println("sending down_arrow");
        sendMyData(down_arrow,sizeof(down_arrow)/sizeof(uint16_t),6,msg_gap);
        break;
      case 'f':
      //airflow direction in/out
        Serial.println("sending airflow_dir");
        sendMyData(airflow_dir,sizeof(airflow_dir)/sizeof(uint16_t),3,msg_gap);
        break;
      case 't':
      //thermometer button
        Serial.println("sending temp_ctrl");
        sendMyData(temp_ctrl,sizeof(temp_ctrl)/sizeof(uint16_t),3,msg_gap);
        break;
      default:
        break;
    }

  }

}
