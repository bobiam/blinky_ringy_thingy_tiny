/*
Blinky Ringy Thingy is an art project by Bob Eells for Flipside 2016, mostly 3d printed, with a strand of WS2811 LEDs
It borrows heavily from the Adafruit_Neopixel demo code and uses their library.
It runs on an ATTINY85 at 8MHz

LICENSE - 
This code makes lights blink, and should not be used to run heart monitors or keep planes in the air.
If you actually need a real software license for this, I'm curious what you're doing and will happily 
release it under the very open license of your choosing if you reach out and ask for it.  
You can pretty much assume MIT, GPL, CC style terms.  
I'm not gonna sue you over blinky light code I wrote for fun.
*/

#include <Adafruit_NeoPixel.h>

#define pir 2
#define numLEDs 36

//strip data pin
#define PIN 1

int rs[numLEDs];
int gs[numLEDs];
int bs[numLEDs];

//doyoubelieveinmagic?
const uint32_t dybim = Color(255,255,255)+1;
//doyoubelieveinrandom?
const uint32_t dybir = dybim+1;

bool debug = false;

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLEDs, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
    
  strip.begin();
//  Serial.begin(9600);
  pinMode(pir, INPUT);

  // Update LED contents, to start they are all 'off'
  strip.show();
  if(!debug)
  {
    all(Color(255,255,255));
    delay(2000);
  }  
}

void loop() {
  if(debug)
  {
    bob_debug();
  }else{
    int polled = poll();
    if(polled == 999 && !debug)
    {
      saw_motion();
    }else{
      no_one_close();
    }
  }
}

int no_one_close()
{
  int polled;

  polled =  cross_the_streams(randomColor(),randomColor(),randomColor(),10,20);
  if(polled == 999)
    saw_motion();    

  polled = fract(randomColor(),randomColor(),350);  
  if(polled == 999)
    saw_motion();  
        
  polled = nchase(randomColor(),randomColor(),5,random(0,100));
  if(polled == 999)
    saw_motion();      

  polled = randommy();
  if(polled == 999)
    saw_motion();     

  polled = rainbowCycle(20);
  if(polled == 999)
    saw_motion();
  
  polled =  cross_the_streams(Color(0,0,0),Color(255,69,0),Color(255,0,0),10,10);
  if(polled == 999)
    saw_motion();

  polled = fract(Color(0,0,255),Color(255,255,0),500);  
  if(polled == 999)
    saw_motion();      

  polled = earthRand(10);
  if(polled == 999)
    saw_motion();

  polled = airRand(10);
  if(polled == 999)
    saw_motion();

  polled = fireRand(10);
  if(polled == 999)
    saw_motion();
    
  polled = waterRand(10);
  if(polled == 999)
    saw_motion();
       
  polled = nchase(Color(255,0,0),Color(0,255,0),5,50);
  if(polled == 999)
    saw_motion();

  polled = nchase(Color(0,255,0),Color(0,0,255),4,25);
  if(polled == 999)
    saw_motion();

  polled = nchase(Color(0,0,255),Color(255,0,0),5,12);
  if(polled == 999)
    saw_motion();  

       
  return 1;
}

void bob_debug() {
  
}

void saw_motion(){
  all(Color(255,255,255));
  delay(4000);
}

//BE - air color palette
int airRand(int loops)
{
  uint32_t colors[] = {Color(115,138,189),Color(255,255,255),Color(0,11,76),Color(118,149,200)};
  int maxWait = 150;
  int poll = paletteRand(colors,sizeof(colors)/sizeof(uint32_t),maxWait,loops);
  if(poll == 999)
    return 999;    
  return 1;
}

//BE - earth color palette
int earthRand(int loops)
{
  uint32_t colors[] = {Color(85,255,47),Color(47,255,79),Color(0,255,0),Color(0,0,0)};
  int maxWait = 150;
  int poll = paletteRand(colors,sizeof(colors)/sizeof(uint32_t),maxWait,loops);
  if(poll == 999)
    return 999;  
  return 1;
}

//BE - fire color palette
int fireRand(int loops)
{
  uint32_t colors[] = {Color(0,0,0),Color(255,0,0),Color(255,69,0),Color(255,255,0)};
  int maxWait = 150;
  int poll = paletteRand(colors,sizeof(colors)/sizeof(uint32_t),maxWait,loops);
  if(poll == 999)
    return 999;  
  return 1;
}

//BE - water color palette
int waterRand(int loops)
{
  uint32_t colors[] = {Color(0,0,0),Color(0,0,255),Color(84,255,159)};
  int maxWait = 150;
  int poll = paletteRand(colors,sizeof(colors)/sizeof(uint32_t),maxWait,loops);
  if(poll == 999)
    return 999;  
  return 1;
}

int paletteRand(uint32_t colors[], int colorCount,int maxWait, int loops)
{
  for(int j = 0; j < loops; j++)
  {
    for(int i =0; i < numLEDs;i++)
    {
      strip.setPixelColor(i,colors[random(0,colorCount)]);
      if(poll() == 999)
        return 999;
    }
    strip.show();
    delay(maxWait);    
  }
  return 1;
}

//cross the streams
//sets ring to bc, runs a pod from each end (fgc1, fgc2), they overwrite each other as they cross.
//duration = loops * wait * numLEDs
int cross_the_streams(uint32_t bc, uint32_t fgc1, uint32_t fgc2, int wait, int loops)
{
  all(bc);
  for(int j=0;j<loops;j++)
  {
    for(int i=0;i<numLEDs+1;i++)
    {
      strip.setPixelColor(i,fgc1);
      strip.setPixelColor(numLEDs-i,fgc2);
      strip.show();
      delay(wait);
      if(poll() == 999)
        return 999;        
    }
  }
  return 1;
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
int rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < numLEDs; i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / numLEDs part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / numLEDs) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
    if(poll() == 999)
      return 999;      
  }
  return 1;  
}

int nchase(uint32_t bg,uint32_t fg,int n,int wait)
{
  int p;
  all(bg);
  for(int j=0;j<(numLEDs/n);j++)
  {
    for(int i=0;i<numLEDs/n;i++)
    {
      p = (n*i)+j;
      if(p>numLEDs)
        p = p-numLEDs;
      strip.setPixelColor(p,fg);
      strip.show();
      delay(wait);
      if(poll()==999)
        return 999;
    }
  }
  return 1;
}

int randommy()
{
  for(int i=0;i<random(150,500);i++)
  {
    strip.setPixelColor(random(0,numLEDs),randomColor());
    delay(random(0,200));  
    strip.show();
    if(poll()==999)
      return 999;    
  }
  return 1;
}

int omgp()
{
  int i,j,k;
  int pos; //cheap/easy rollover

  for(k=0;k<numLEDs;k++)
  {
    for(i=0;i<(numLEDs/3);i++)
    {
      for(j=0;j<3;j++)
      {
        pos = i * 3 * k;
        while(pos > 255)
        {
          pos = pos - 255;
        }
        strip.setPixelColor(i*3+j,Wheel(pos));
      }
    }
    strip.show();
    if(poll()==999)
      return 999;    
    delay(500);
  }
  return 1;
} 

int chase(uint32_t fg, uint32_t bg, int wait)
{
  int i,j;
  for(j=0;j<100;j++)
  {
    all(bg);
    for(i=0;i<numLEDs;i++)
    {
      strip.setPixelColor(i,fg);
      strip.setPixelColor(i-1,bg);
      strip.show();
      delay(wait);        
      if(poll()==999)
        return 999;      
    }
    for(i=numLEDs;i>-1;i--)
    {
      all(bg);
      strip.setPixelColor(i,fg);
      strip.show();
      delay(wait);  
      if(poll()==999)
        return 999;      
    }
  }    
  return 1;
}

int accel_chase(uint32_t fg, uint32_t bg, int init_wait)
{
  int i,j, wait;
  wait = init_wait;
  for(j=0;j<22;j++)
  {
    all(bg);
    for(i=0;i<numLEDs;i++)
    {
      strip.setPixelColor(i,fg);
      strip.setPixelColor(i-1,bg);
      strip.show();
      delay(wait);        
      if(poll()==999)
        return 999;      
    }
    for(i=numLEDs;i>-1;i--)
    {
      strip.setPixelColor(i,fg);
      strip.setPixelColor(i+1,bg);
      strip.show();
      delay(wait);        
      if(poll()==999)
        return 999;      
    }
    wait = .90 * wait;
  }    
  return 1;
}

//does random stuff.  
int rand(int loops, int max_wait, int modus)
{
  int p;
  uint32_t c;
  c = Color(0,0,0);
  for(int i=0;i<loops;i++)
  {
    p = random(0,numLEDs);
    switch(modus){
      case 0:
        c = Color(random(0,255),random(0,255),random(0,255));
        break;
      case 1:
        c = Color(random(0,255),0,0);
        break;
      case 2:
        c = Color(0,random(0,255),0);
        break;
      case 3:
        c = Color(0,0,random(0,255));        
        break;
    }
    strip.setPixelColor(p,c);
    strip.show();
    delay(random(0,max_wait));
    if(poll()==999)
      return 999;    
  }
  return 1;
}

int fract(uint32_t c1, uint32_t c2, int wait)
{
  if(fract_segments(c1,c2,18,wait) == 999)
    return 999;  
  if(fract_segments(c1,c2,9,wait) == 999)
    return 999;  
  if(fract_segments(c1,c2,6,wait) == 999)
    return 999;  
  if(fract_segments(c1,c2,3,wait) == 999)
    return 999;  
  if(fract_segments(c1,c2,1,wait) == 999)
    return 999;  
  return 1;
}

int fract_segments(uint32_t c1,uint32_t c2,int segment_size, int wait)
{
  for(int j=1;j<150;j++)
  {
    uint32_t c;
    int switcher_count = 0;
    for(int i=0;i<numLEDs;i++)
    {
      if(j%2)
      {
        strip.setPixelColor(i,c1);
      }else{
        strip.setPixelColor(i,c2);
      }
      switcher_count++;
      if(switcher_count == segment_size)
      {
        switcher_count = 0;
        c = c1;
        c1 = c2;
        c2 = c;
      }      
    }
    strip.show();
    delay(wait);  
    wait = wait * .95;        
    if(poll()==999)
      return 999;    
  }
  return 1;
}

//BE - alternate pods between two colors
int ants(uint32_t c1, uint32_t c2, uint8_t wait, int loops){
  int i,j;
  for(j=0;j<loops;j++)
  {
    for(i=0; i< numLEDs; i++)
    {
      if(j%2)
      {     
        if(i % 2)
        {
          strip.setPixelColor(i,c1);
        }else{
          strip.setPixelColor(i,c2);
        }
      }else{
        if((i+1) % 2)
        {
          strip.setPixelColor(i,c1);
        }else{
          strip.setPixelColor(i,c2);
        }
      }
    }
    strip.show();
    delay(wait);
    if(poll() == 999)
      return 999;      
  }
  return 1;
}

// BE
// fill the dots one after the other with said color
// good for testing purposes and color wipes inbetween patterns.
int colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < numLEDs; i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
      if(poll() == 999)
        return 999;        
  }
  return 1;
}




/* Helper functions */

//BE - sometimes it's useful to get the RGB values back when passing colors to and fro.
void decodeColor(uint32_t c, byte ret[])
{
  int r,g,b;
  
  b = c & 255;
  g = c >>= 8;
  r = c >> 8;
  
  ret[0] = r;
  ret[1] = g;
  ret[2] = b;  
}

//BE
uint32_t randomColor()
{
  //generate a random color
  int r = random(0,256);
  int g = random(0,256);
  int b = random(0,256);
  return Color(r,g,b);
}

//BE 
void array_spin(byte arr[], int count)
{
  byte tmp = arr[0];
  for(int i=0;i<count-1;i++)
  {
    arr[i] = arr[i+1];
  }
  arr[count-1] = tmp;
}

void all(uint32_t c)
{
  for(int i=0;i<numLEDs;i++)
  {
    strip.setPixelColor(i,c);
  }
  strip.show();  
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

//Theatre-style crawling lights with rainbow effect
int theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < numLEDs; i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
        if(poll() == 999)
          return 999;
        delay(wait);
       
        for (int i=0; i < numLEDs; i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }  
  return 1;
}

uint32_t desaturate(uint32_t c, float multiplier)
{
  byte ret[3];
  ret[0] = 0;
  ret[1] = 0;
  ret[2] = 0;

  decodeColor(c,ret);
  
  byte ret_c_comp[3];
  ret_c_comp[0] = multiplier * ret[0];
  ret_c_comp[1] = multiplier * ret[1];
  ret_c_comp[2] = multiplier * ret[2];  
  
  uint32_t ret_c = Color(ret_c_comp[0],ret_c_comp[1],ret_c_comp[2]);
  
  return ret_c;
}

//poll the motion sensors, kick out a 999 if one trips, which kicks us to  motion (saw_motion())
int poll()
{
  int r = digitalRead(pir);
  if(r)
  {
    return 999;
  }
  return 1;
}
