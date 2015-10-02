#include <AEAT6010.h>




Encoder enc1(28,26,22);//Encoder(int dataPin,int clockPin,int chipSelect);
Encoder enc2(28,26,24);//Encoder(int dataPin,int clockPin,int chipSelect);

void setup() {
	Serial.begin(115200);

}

void loop() {
	Serial.print ("Encoder\t");
  	Serial.print(enc1.getAbsolute());
  	Serial.print ("\t");
  	Serial.println(enc2.getAbsolute());
 }
