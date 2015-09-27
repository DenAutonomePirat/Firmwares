/*
  EncoderLib.h Library for sensor AEAT 6010
*/
#ifndef EncoderLib_h
#define EncoderLib_h


class Encoder
{
  public:
    Encoder(int dataPin,int clockPin,int chipSelect);
    int getEncoder();
    int getAbsolute();
  private:
	int _Absolute;
    int _dataPin;
	int _clockPin;
	int _chipSelect;
	int _lastPos;
	int _metaPos;
	char _incoming;
	int _sensorValue;
	int _pos;
	int _pos1;
};

#endif
