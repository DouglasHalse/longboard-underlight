#include <Adafruit_NeoPixel.h>
#include <CircularBuffer.h>

#define NUMPIXELS 106
#define dataPin 2
#define hallEffectPin 3


CircularBuffer<unsigned long, 2> hallEffectTimes;
double oscillatingTimer = 0;
double oscillatingHue = 0;

double idlePixelOffsets[NUMPIXELS];
double spinningFlashingTimer = 0;


double flashingTimer = 0;

bool previousHallEffectState = HIGH;
bool currentHallEffectState = LOW;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, dataPin, NEO_GRB + NEO_KHZ800);


void setup() {
  pixels.begin();
  pinMode(hallEffectPin, INPUT);
  for(int i=0;i<NUMPIXELS;i++){
    idlePixelOffsets[i] = ((2*PI)/(NUMPIXELS/2))*i;
  }
}

void loop() {
  //Longboard is moving
  if(hallEffectTimes.isFull()){
    setOscillatingColor();
  }
  //Longboard is NOT moving
  else{
    setFlashingColor();
  }

  checkHallEffectSensor();
  itterateTimers();
  emptyBufferIfOld();
  pixels.show();
}

void emptyBufferIfOld(){
  if((millis() - hallEffectTimes[0])>500){
    hallEffectTimes.clear();
  }
}

void itterateTimers(){
  if(hallEffectTimes.isFull()){
    double averageDelay = calculateAverageHallEffectDelay();
    double estimatedHertz = ((double)1/averageDelay);
    oscillatingTimer+=estimatedHertz*2;
    oscillatingHue+=estimatedHertz*5000;
  }
  spinningFlashingTimer+=0.10;
  flashingTimer+=0.08;

  
}

void setFlashingColor(){
//  for(int i=0;i<NUMPIXELS;i++){
//      double controlValue = (int)(((1+sin((double)flashingTimer))/2)*100);
//      pixels.setPixelColor(i, pixels.ColorHSV(0, 0, controlValue));
//  }
  for(int i=0;i<NUMPIXELS;i++){
    double pixelOffset = idlePixelOffsets[i];
    double spinningPixelBrightness = pow(((1+sin((double)spinningFlashingTimer + pixelOffset))/2), 2)*255;
    pixels.setPixelColor(i, pixels.ColorHSV(0, 0, spinningPixelBrightness));
  }
}

void setOscillatingColor(){
  for(int i=0;i<NUMPIXELS/2;i++){
    double pixelOffset = ((2*PI)/(NUMPIXELS/2))*i;
    int hueOffset = (int)(65535/(NUMPIXELS/2))*i;
    double controlValue = (int)(((1+sin((double)oscillatingTimer+pixelOffset))/2)*255);
    //pixels.setPixelColor(i, pixels.ColorHSV((int)oscillatingHue + hueOffset, 255, controlValue));
    //pixels.setPixelColor(NUMPIXELS-i-1, pixels.ColorHSV((int)oscillatingHue + hueOffset, 255, controlValue));
    pixels.setPixelColor(i, pixels.ColorHSV((int)oscillatingHue - hueOffset, 255, 255));
    pixels.setPixelColor(NUMPIXELS-i-1, pixels.ColorHSV((int)oscillatingHue - hueOffset, 255, 255));
  }
}

double calculateAverageHallEffectDelay(){
  int nrOfElements = hallEffectTimes.size();
  unsigned long delayAcc = 0;
  for(int i = 1; i<nrOfElements; i++)
  {
    delayAcc += hallEffectTimes[i-1] - hallEffectTimes[i];
  }
  return (double)delayAcc/(double)nrOfElements-1;
}

void checkHallEffectSensor(){
  currentHallEffectState = digitalRead(hallEffectPin);
  if (currentHallEffectState == LOW && previousHallEffectState == HIGH){
    hallEffectTimes.unshift(millis());
  }
  previousHallEffectState = currentHallEffectState;
}
