 //MP3 player
 #include "SoftwareSerial.h"
SoftwareSerial mySerial(9,5);//(10TX, 11RX)*require PWM  
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

# define ACTIVATED LOW
# define PINWelcome 4
boolean isPlaying = false;
 //Neopixel&LED
 #include <Adafruit_NeoPixel.h>
 #define PIN 6 //input of pixels12
 #define PIN16L 3 //input of the left pixels16
 #define PIN16R 9 //input of the right pixels16
 Adafruit_NeoPixel pixels12 = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
 Adafruit_NeoPixel pixels16L = Adafruit_NeoPixel(16, PIN16L, NEO_GRB + NEO_KHZ800);
 Adafruit_NeoPixel pixels16R = Adafruit_NeoPixel(16, PIN16R, NEO_GRB + NEO_KHZ800);
 int delayval=50;
 int redColor=0;
 int greenColor=0;
 int blueColor=0;
 int count=0;
 int echo=12;
 int leftLED=10;
 int rightLED=11;
 int trigger=13;
 int powerPixel12=7;
 int powerPixel16L=8;
 int powerPixel16R=2;
void setup(){
  //Neopixel&LED
  pinMode(PINWelcome,OUTPUT);
  pinMode(echo,INPUT);//Echo of the sensor
  pinMode(leftLED,OUTPUT);//LEFTLED
  pinMode(rightLED,OUTPUT);//RIGHTLED
  pinMode(trigger,OUTPUT);//Trigger of the sensor
  pinMode(powerPixel12,OUTPUT);//Power of neopixel with 12 bulbs
  pinMode(powerPixel16L,OUTPUT);//Power of neopixel with 16 bulbs
  pinMode(powerPixel16R,OUTPUT);//Power of neopixel with 16 bulbs
  Serial.begin(9600);
  pixels12.begin();
  pixels16L.begin();
  pixels16R.begin();
  }
float cal_distance(float microseconds){
  float v=340;
  float distm;
  float distcm;
  distm= v*microseconds/2000000;
  distcm=distm*100;
  return distcm;
  }
void loop(){
  float duration, dist;
  char dist_str[6];
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  duration=pulseIn(echo,HIGH);
  dist=(cal_distance(duration));
  if (dist<170){
    //playFirst();
    digitalWrite(powerPixel12,HIGH);
    digitalWrite(powerPixel16L,HIGH);
    digitalWrite(powerPixel16R,HIGH);
    digitalWrite(PINWelcome,HIGH);
     setColor();
 for (int i=0; i < 16; i++) {
int j;
j=(i*4)/15;
  if(count%2!=0){analogWrite(leftLED,255-(i*17));analogWrite(rightLED,255-(i*17));
    pixels12.setPixelColor(5-j, pixels12.Color(redColor, greenColor, blueColor));
    pixels12.setPixelColor(7+j, pixels12.Color(redColor, greenColor, blueColor));
    pixels16L.setPixelColor(i, pixels16L.Color(redColor, greenColor, blueColor));
    pixels16R.setPixelColor(15-i, pixels16R.Color(redColor, greenColor, blueColor));
    pixels12.show();
    pixels16L.show();
    pixels16R.show();
    pixels12.setPixelColor(5-j, pixels12.Color(0, 0, 0));
    pixels12.setPixelColor(7+j, pixels12.Color(0,0,0));
    pixels16L.setPixelColor(i, pixels16L.Color(0, 0, 0));
    pixels16R.setPixelColor(15-i, pixels16R.Color(0, 0, 0));
    delay(delayval);  }
  else {analogWrite(leftLED,i*17);analogWrite(rightLED,i*17);
    pixels12.setPixelColor(j+1, pixels12.Color(redColor, greenColor, blueColor));
    pixels12.setPixelColor(11-j, pixels12.Color(redColor, greenColor, blueColor));
    pixels16L.setPixelColor(i, pixels16L.Color(redColor, greenColor, blueColor));
    pixels16R.setPixelColor(15-i, pixels16R.Color(redColor, greenColor, blueColor));
    pixels12.show();
    pixels16L.show();
    pixels16R.show();
    pixels12.setPixelColor(j+1, pixels12.Color(0, 0, 0));
    pixels12.setPixelColor(11-j, pixels12.Color(0,0,0));
    pixels16L.setPixelColor(i, pixels16L.Color(0, 0, 0));
    pixels16R.setPixelColor(15-i, pixels16R.Color(0, 0, 0));
    delay(delayval);  }
  
  if(j==4)count=count+1;
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  duration=pulseIn(echo,HIGH);
  dist=(cal_distance(duration));
  if(dist>=170) break;
  }
  

    }
    else {digitalWrite(leftLED,LOW);digitalWrite(rightLED,LOW);digitalWrite(powerPixel12,LOW);digitalWrite(powerPixel16L,LOW);digitalWrite(powerPixel16R,LOW);digitalWrite(6,LOW);digitalWrite(3,LOW);digitalWrite(PINWelcome,LOW);delay(200);}
  dtostrf(dist,5,2,dist_str);
  Serial.println(String("Distance: ")+dist_str+"cm");
  if(count==101) count=0; //To prevent count approachs to inifinity
  }
  void setColor(){
  redColor = random(0, 255);
  greenColor = random(0, 255);
  blueColor = random(0, 255);
}
void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
//Send the command line to the module
for (byte k=0; k<10; k++)
{
mySerial.write( Command_line[k]);
}
}
void playFirst()
{
  /*execute_CMD(0x3F, 0, 0);
  delay(300);*/
  setVolume(30);
  delay(300);
  execute_CMD(0x11,0,1); 
  delay(1750); 
  execute_CMD(0x0E,0,0); 
  delay(300);
}
void setVolume(int volume)
{
  execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
  delay(2000);
}
