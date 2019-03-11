#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

#define CE_PIN 10
#define CSN_PIN 9

#define WHICH_NODE 2     // must be a number from 1 - 6 identifying the PTX node

const uint64_t rAddress[] = {0x7878787878LL, 0xB3B4B5B6F1LL};
const uint64_t PTXpipe = rAddress[ WHICH_NODE - 1 ];

RF24 radio(CE_PIN, CSN_PIN); // Create a  Radio

MPU6050 accelgyro;

int data[8];
bool SendingStatus;

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
    Wire.begin();
    radio.begin();
    radio.setRetries(3,5);
    radio.setChannel(108); 
    radio.setDataRate(RF24_250KBPS);//set data rate to 250kbps
    radio.openReadingPipe(WHICH_NODE-1, rAddress[WHICH_NODE-1]);
    radio.setPALevel(RF24_PA_LOW);
    radio.enableAckPayload();
    radio.startListening();
    
    accelgyro.initialize();
    accelgyro.setFullScaleAccelRange(3);
    accelgyro.setFullScaleGyroRange(3);
}

void loop() {
    if(radio.available()){
      bool tmp;
      radio.read(&tmp, sizeof(tmp));
      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      data[0] = ax;
      data[1] = ay;
      data[2] = az;
      data[3] = gx;
      data[4] = gy;
      data[5] = gz;
      data[6] = 0510;
      radio.writeAckPayload(WHICH_NODE-1, &data, sizeof(data));
    }
    
}
