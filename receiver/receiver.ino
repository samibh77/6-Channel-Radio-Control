//  6 Channel Receiver 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

int ch_width_1 = 0;
int ch_width_2 = 0;
int ch_width_3 = 0;
int ch_width_4 = 0;
int ch_width_5 = 0;
int ch_width_6 = 0;


Servo ch1;
Servo ch2;
Servo ch3;
Servo ch4;
Servo ch5;
Servo ch6;

struct Signal {
byte throttle;
byte pitch;  
byte roll;
byte yaw;
byte aux1;
byte aux2;
    
};
Signal data;
const uint64_t pipeIn = 0xABCDABCD71LL;
RF24 radio(9, 10); 
void ResetData()
{
data.throttle = 0;                                         // Define the inicial value of each data input. 
data.roll = 127;
data.pitch = 127;
data.yaw = 127;
data.aux1 = 0;                                              
data.aux2 = 0;
                                                    
}
void setup()
{
 // Set the pins for each PWM signal 
  ch1.attach(2);
  ch2.attach(3);
  ch3.attach(4);
  ch4.attach(5);
  ch5.attach(6);
  ch6.attach(7);
                                                           
  ResetData();                                             // Configure the NRF24 module  
  radio.openReadingPipe(1,pipeIn);
  radio.setChannel(100);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);                         // The lowest data rate value for more stable communication 
  radio.setPALevel(RF24_PA_MAX);                           // Output power is set for maximum
  radio.startListening();                                  // Start the radio comunication for receiver 
}
unsigned long lastRecvTime = 0;
void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(Signal));
lastRecvTime = millis();                                    // Receive the data 
}
}
void loop()
{
recvData();
unsigned long now = millis();
if ( now - lastRecvTime > 1000 ) {
ResetData();                                                // Signal lost.. Reset data 
}
ch_width_1 = map(data.roll, 0, 255, 1000, 2000);
ch_width_2 = map(data.pitch, 0, 255, 1000, 2000); 
ch_width_3 = map(data.throttle, 0, 255, 1000, 2000); 
ch_width_4 = map(data.yaw, 0, 255, 1000, 2000); 
ch_width_5 = map(data.aux1, 0, 1, 1000, 2000); 
ch_width_6 = map(data.aux2, 0, 1, 1000, 2000); 
ch1.writeMicroseconds(ch_width_1);                          // Write the PWM signal 
ch3.writeMicroseconds(ch_width_3);
ch4.writeMicroseconds(ch_width_4);
ch5.writeMicroseconds(ch_width_5);
ch6.writeMicroseconds(ch_width_6); 
}