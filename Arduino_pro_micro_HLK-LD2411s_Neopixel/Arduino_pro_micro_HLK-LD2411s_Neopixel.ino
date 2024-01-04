#define INTERVAL 100
#include <Adafruit_NeoPixel.h>
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket

long previousMillis = 0;
byte findheader = 0; 
int PixelsCount = 0;

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 2 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 300 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned int GetDistance(){
  unsigned int distance; 
  
  if(Serial1.available()){
    if (Serial1.read()==0xAA && Serial1.read()==0xAA) {
      findheader=1;
      }
    else {
      return;
      }
    }
    
  if (findheader == 1 && Serial1.available()){
   byte h_data, l_data;
   byte buf[5];
   Serial1.readBytes(buf, 5);
   h_data = buf[2];
   l_data = buf[1];
   distance = (h_data<<8) + l_data; 
   
   if (distance <=500) {
    return distance;  
    }
   else {
    return;  
    } 
   findheader = 0; // в конце сбрасывая флаг найденного заголовка
   } 
}

void setup() {
  Serial.begin(115200); // Init console
  unsigned long previousMillis = millis();
  Serial1.begin(256000); 
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();            // Turn OFF all pixels ASAP
  }

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < INTERVAL)
  return;    
  PixelsCount = GetDistance()*60/100;
  Serial.println(PixelsCount);
  for (int i=0; i<PixelsCount; i++) { // For each pixel...
  pixels.setPixelColor(i, pixels.Color(0, 0, 100));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
  while (Serial1.available()) Serial1.read();
  previousMillis = currentMillis;     
  }
