/*  
 * ____                     _      ______ _____    _____
  / __ \                   | |    |  ____|  __ \  |  __ \               
 | |  | |_ __   ___ _ __   | |    | |__  | |  | | | |__) |__ _  ___ ___ 
 | |  | | '_ \ / _ \ '_ \  | |    |  __| | |  | | |  _  // _` |/ __/ _ \
 | |__| | |_) |  __/ | | | | |____| |____| |__| | | | \ \ (_| | (_|  __/
  \____/| .__/ \___|_| |_| |______|______|_____/  |_|  \_\__,_|\___\___|
        | |                                                             
        |_|          
 Open LED Race
 An minimalist cars race for LED track  
  
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 by gbarbarov@singulardevices.com  for Arduino day Seville 2019 

 Code made dirty and fast, next improvements in: 
 https://github.com/gbarbarov/led-race
 https://www.hackster.io/gbarbarov/open-led-race-a0331a
 https://twitter.com/openledrace
*/

                                                            
#include <Adafruit_NeoPixel.h>
#define MAXLED         300 // MAX LEDs actives on track

#define PIN_LED        A0  // R 500 ohms to DI pin for WS2812 and WS2813, for WS2813 BI pin of first LED to GND  ,  CAP 1000 uF to VCC 5v/GND,power supplie 5V 2A  
#define PIN_P1         7   // switch player 1 to PIN and GND
#define PIN_P2         6   // switch player 2 to PIN and GND 
#define PIN_P3         5   // switch player 2 to PIN and GND 
#define PIN_AUDIO      3   // through CAP 2uf to speaker 8 ohms
#define Inacivity_time 30  //time to start disco in s

int NPIXELS=MAXLED; // leds on track

byte  gravity_map[MAXLED];  
struct sColor_my{
  int R;
  int G;
  int B;
};


struct sColor_my Color1={0,0,255};
struct sColor_my Color2={255,255,0};
struct sColor_my Color3={0,255,0};
struct sColor_my Color_hill={127,127,127};

#define CAR1_Color track.Color(Color1.R,Color1.G,Color1.B-i*20)
#define CAR2_Color track.Color(Color2.R-i*20,Color2.G-i*20,Color2.B)
#define CAR3_Color track.Color(Color3.R,Color3.G-i*20,Color3.B)
#define CAR_HILL_Color track.Color(((Color_hill.R-gravity_map[i])/8 ), ((Color_hill.G-gravity_map[i])/8), ((Color_hill.B-gravity_map[i])/8))



int win_music[] = {
  2637, 2637, 0, 2637, 
  0, 2093, 2637, 0,
  3136    
};
      
   

int TBEEP=3; 

float speed1=0;
float speed2=0;
float speed3=0;

float dist1=0;
float dist2=0;
float dist3=0;

byte loop1=0;
byte loop2=0;
byte loop3=0;

byte leader=0;
byte loop_max=5; //total laps race


float ACEL=0.2;
float kf=0.015; //friction constant
float kg=0.003; //gravity constant

byte flag_sw1=0;
byte flag_sw2=0;
byte flag_sw3=0;
byte draworder=0;
 
uint64_t  timestamp=0;
uint64_t Best_timestamp=99999;
uint64_t timestamp_push=0;
uint8_t Block_return=0;

Adafruit_NeoPixel track(MAXLED, PIN_LED, NEO_GRB + NEO_KHZ800);

int tdelay = 5; 

void set_ramp(byte H,byte a,byte b,byte c)
{for(int i=0;i<(b-a);i++){gravity_map[a+i]=127-i*((float)H/(b-a));};
 gravity_map[b]=127; 
 for(int i=0;i<(c-b);i++){gravity_map[b+i+1]=127+H-i*((float)H/(c-b));};
}

void set_loop(byte H,byte a,byte b,byte c)
{for(int i=0;i<(b-a);i++){gravity_map[a+i]=127-i*((float)H/(b-a));};
 gravity_map[b]=255; 
 for(int i=0;i<(c-b);i++){gravity_map[b+i+1]=127+H-i*((float)H/(c-b));};
}


void setup() {
  Serial.begin(9600); 
  
  
  for(int i=0;i<NPIXELS;i++){gravity_map[i]=127;};
  track.begin(); 
  track.setBrightness(255); //max 255
  pinMode(PIN_P1,INPUT_PULLUP); 
  pinMode(PIN_P2,INPUT_PULLUP);  
  pinMode(PIN_P3,INPUT_PULLUP);  

  //if ((digitalRead(PIN_P1)==0)) //push switch 1 on reset for activate physic
  //{
    set_ramp(5,35,45,80);    // ramp centred in LED 100 with 10 led fordward and 10 backguard
    //set_ramp(17,90,130,160);
  set_loop(5,90,135,160);
    
    for(int i=0;i<NPIXELS;i++){track.setPixelColor(i, CAR_HILL_Color );};
    track.show();
  //};
  
  start_race();    
}

void start_race(){for(int i=0;i<NPIXELS;i++){track.setPixelColor(i, track.Color(0,0,0));};
                  loop1=0;loop2=0;loop3=0;dist1=0;dist2=0;dist3=0;speed1=0;speed2=0;speed3=0;timestamp=0;
                  track.show();
                  delay(2000);
                  track.setPixelColor(12, track.Color(255,0,0));
                  track.setPixelColor(11, track.Color(255,0,0));
                  track.show();
                  tone(PIN_AUDIO,400);
                  delay(2000);
                  noTone(PIN_AUDIO);                  
                  track.setPixelColor(12, track.Color(0,0,0));
                  track.setPixelColor(11, track.Color(0,0,0));
                  track.setPixelColor(10, track.Color(255,255,0));
                  track.setPixelColor(9, track.Color(255,255,0));
                  track.show();
                  tone(PIN_AUDIO,600);
                  delay(2000);
                  noTone(PIN_AUDIO);                  
                  track.setPixelColor(9, track.Color(0,0,0));
                  track.setPixelColor(10, track.Color(0,0,0));
                  track.setPixelColor(8, track.Color(0,255,0));
                  track.setPixelColor(7, track.Color(0,255,0));
                  track.show();
                  tone(PIN_AUDIO,1200);
                  delay(2000);
                  noTone(PIN_AUDIO);                               
                  timestamp=millis();
                                
                 };

void winner_fx() {
               int msize = sizeof(win_music) / sizeof(int);
               for (int note = 0; note < msize; note++) {
               tone(PIN_AUDIO, win_music[note],200);
               delay(230);
               noTone(PIN_AUDIO);

}

                                               
              };

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<track.numPixels(); i++) { // For each pixel in track...
    track.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    track.show();                          //  Update track to match
    delay(wait);                           //  Pause for a moment
           if( Block_return==1){return;   }else if((digitalRead(PIN_P1)==0) || (digitalRead(PIN_P2)==0) || (digitalRead(PIN_P3)==0)){start_race();Block_return=1;timestamp_push=millis(); return; }   
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la track.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      track.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of track in steps of 3...
      for(int c=b; c<track.numPixels(); c += 3) {
        track.setPixelColor(c, color); // Set pixel 'c' to value 'color'
               if( Block_return==1){return;   }else if((digitalRead(PIN_P1)==0) || (digitalRead(PIN_P2)==0) || (digitalRead(PIN_P3)==0)){start_race();Block_return=1;timestamp_push=millis(); return; }   
      }
      track.show(); // Update track with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole track. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<track.numPixels(); i++) { // For each pixel in track...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the track
      // (track.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / track.numPixels());
      // track.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through track.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      track.setPixelColor(i, track.gamma32(track.ColorHSV(pixelHue)));
             if( Block_return==1){return;   }else if((digitalRead(PIN_P1)==0) || (digitalRead(PIN_P2)==0) || (digitalRead(PIN_P3)==0)){start_race();Block_return=1;timestamp_push=millis(); return; }   
    }
    track.show(); // Update track with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      track.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of track in increments of 3...
      for(int c=b; c<track.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the track (track.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / track.numPixels();
        uint32_t color = track.gamma32(track.ColorHSV(hue)); // hue -> RGB
        track.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      track.show();                // Update track with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
            if( Block_return==1){return;   }else if((digitalRead(PIN_P1)==0) || (digitalRead(PIN_P2)==0) || (digitalRead(PIN_P3)==0)){start_race();Block_return=1;timestamp_push=millis(); return; }   
    }
  }
}


void Disco_effect(){    //neopixel demo
  // Fill along the length of the track in various colors...
  colorWipe(track.Color(255,   0,   0), 10); // Red
        if( Block_return==1){return;   }else if((digitalRead(PIN_P1)==0) || (digitalRead(PIN_P2)==0) || (digitalRead(PIN_P3)==0)){start_race();Block_return=1;timestamp_push=millis(); return; }   
  
  colorWipe(track.Color(  0, 255,   0), 10); // Green
         if( Block_return==1){return;   }else if((digitalRead(PIN_P1)==0) || (digitalRead(PIN_P2)==0) || (digitalRead(PIN_P3)==0)){start_race();Block_return=1;timestamp_push=millis(); return; }   
  colorWipe(track.Color(  0,   0, 255), 10); // Blue
         if( Block_return==1){return;   }else if((digitalRead(PIN_P1)==0) || (digitalRead(PIN_P2)==0) || (digitalRead(PIN_P3)==0)){start_race();Block_return=1;timestamp_push=millis(); return; }   

  // Do a theater marquee effect in various colors...
  theaterChase(track.Color(127, 127, 127), 50); // White, half brightness
         if( Block_return==1){return;   }else if((digitalRead(PIN_P1)==0) || (digitalRead(PIN_P2)==0) || (digitalRead(PIN_P3)==0)){start_race();Block_return=1;timestamp_push=millis(); return; }   
  theaterChase(track.Color(127,   0,   0), 50); // Red, half brightness
         if( Block_return==1){return;   }else if((digitalRead(PIN_P1)==0) || (digitalRead(PIN_P2)==0) || (digitalRead(PIN_P3)==0)){start_race();Block_return=1;timestamp_push=millis(); return; }   
  theaterChase(track.Color(  0,   0, 127), 50); // Blue, half brightness

  rainbow(10);             // Flowing rainbow cycle along the whole track
         if( Block_return==1){return;   }else if((digitalRead(PIN_P1)==0) || (digitalRead(PIN_P2)==0) || (digitalRead(PIN_P3)==0)){start_race();Block_return=1;timestamp_push=millis(); return; }   
  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
  
  if( Block_return==1){return;
  }else if((digitalRead(PIN_P1)==0) || (digitalRead(PIN_P2)==0) || (digitalRead(PIN_P3)==0)){start_race();Block_return=1;timestamp_push=millis(); return; }
  
}

void burning1(){
//to do
 }

void burning2(){
//to do
 }

void track_rain_fx(){
//to do
 }

void track_oil_fx(){
//to do
 }

void track_snow_fx(){
//to do
 }


void fuel_empty(){
//to do
 }

void fill_fuel_fx(){
//to do
 }

void in_track_boxs_fx(){
//to do
 }

void pause_track_boxs_fx(){
//to do
 }
 
void flag_boxs_stop(){
//to do
 }

void flag_boxs_ready(){
//to do
 }

void draw_safety_car(){
//to do
 }

void telemetry_rx(){
  //to do
 }
 
void telemetry_tx(){
  //to do
 }

void telemetry_lap_time_car1(){
//to do
 }

void telemetry_lap_time_car2(){
//to do
 }

void telemetry_record_lap(){
//to do
 }

void telemetry_total_time(){
//to do
 }

int read_sensor(byte player){
//to do
}

int calibration_sensor(byte player){
  //to do  
}

int display_lcd_laps(){
  //to do  
}

int display_lcd_time(){
  //to do  
}



void draw_car1(void){for(int i=0;i<=loop1;i++){track.setPixelColor(((word)dist1 % NPIXELS)+i, CAR1_Color );};                   
  }

void draw_car2(void){for(int i=0;i<=loop2;i++){track.setPixelColor(((word)dist2 % NPIXELS)+i, CAR2_Color );};            
 }

void draw_car3(void){for(int i=0;i<=loop3;i++){track.setPixelColor(((word)dist3 % NPIXELS)+i, CAR3_Color );};            
 }
  
void loop() {
    Block_return=0;
    //for(int i=0;i<NPIXELS;i++){track.setPixelColor(i, track.Color(0,0,0));};
    for(int i=0;i<NPIXELS;i++){track.setPixelColor(i, CAR_HILL_Color );};
    
    if ( (flag_sw1==1) && (digitalRead(PIN_P1)==0) ) {flag_sw1=0;speed1+=ACEL;timestamp_push=millis();};
    if ( (flag_sw1==0) && (digitalRead(PIN_P1)==1) ) {flag_sw1=1;};

    if ((gravity_map[(word)dist1 % NPIXELS])<127) speed1-=kg*(127-(gravity_map[(word)dist1 % NPIXELS]));
    if ((gravity_map[(word)dist1 % NPIXELS])>127) speed1+=kg*((gravity_map[(word)dist1 % NPIXELS])-127);
    
    
    speed1-=speed1*kf; 
    
    if ( (flag_sw2==1) && (digitalRead(PIN_P2)==0) ) {flag_sw2=0;speed2+=ACEL;timestamp_push=millis();};
    if ( (flag_sw2==0) && (digitalRead(PIN_P2)==1) ) {flag_sw2=1;};

    if ((gravity_map[(word)dist2 % NPIXELS])<127) speed2-=kg*(127-(gravity_map[(word)dist2 % NPIXELS]));
    if ((gravity_map[(word)dist2 % NPIXELS])>127) speed2+=kg*((gravity_map[(word)dist2 % NPIXELS])-127);
        
    speed2-=speed2*kf; 

    
    if ( (flag_sw3==1) && (digitalRead(PIN_P3)==0) ) {flag_sw3=0;speed3+=ACEL;timestamp_push=millis();};
    if ( (flag_sw3==0) && (digitalRead(PIN_P3)==1) ) {flag_sw3=1;};

    if ((gravity_map[(word)dist3 % NPIXELS])<127) speed3-=kg*(127-(gravity_map[(word)dist3 % NPIXELS]));
    if ((gravity_map[(word)dist3 % NPIXELS])>127) speed3+=kg*((gravity_map[(word)dist3 % NPIXELS])-127);
        
    speed3-=speed3*kf; 

        
    dist1+=speed1;
    dist2+=speed2;
    dist3+=speed3;

    if (dist1>dist2 && dist2>dist3) {leader=1;} 
    if (dist2>dist1 && dist2>dist3) {leader=2;};
    if (dist3>dist1 && dist3>dist2) {leader=3;};
      
    if (dist1>NPIXELS*loop1) {loop1++;tone(PIN_AUDIO,600);TBEEP=2;};
    if (dist2>NPIXELS*loop2) {loop2++;tone(PIN_AUDIO,700);TBEEP=2;};
    if (dist3>NPIXELS*loop3) {loop3++;tone(PIN_AUDIO,700);TBEEP=2;};

    
    if(timestamp_push+Inacivity_time*1000<millis())
    {
      Disco_effect();
    }

    unsigned long timestamp_end=0;
    if (loop1>loop_max) //win 1
    {
      for(int i=0;i<NPIXELS;i++)
      {
        track.setPixelColor(i, track.Color(Color1.R,Color1.G,Color1.B));
      }; 
      track.show();
      
      timestamp_end=millis();
      uint64_t czas_przejazdu=timestamp_end-timestamp;
      Serial.println("Gracz 1(niebieski). Najlepszy wynik: " + String((uint32_t)Best_timestamp) + "[ms] Czas osiagniety: " + String((uint32_t)czas_przejazdu) +" [ms] \n");
     
      if(czas_przejazdu < Best_timestamp)
      {
        Serial.println("Pobiles rekord, twoj czas: "+ String((uint32_t)czas_przejazdu) +"[ms] \n");
        Best_timestamp=czas_przejazdu;                                                    
      }

      
      winner_fx();
      start_race();
    }
    
    if (loop2>loop_max) //win 2
    {
      for(int i=0;i<NPIXELS;i++)
      {
        track.setPixelColor(i, track.Color(Color2.R,Color2.G,Color2.B) );
      }
      track.show();
      
      timestamp_end=millis();
      uint64_t czas_przejazdu=timestamp_end-timestamp;
      Serial.println("Gracz 2(zolty). Najlepszy wynik: " + String((uint32_t)Best_timestamp) +"[ms] Czas osiagniety: " + String((uint32_t)czas_przejazdu) +"[ms] \n");
     
      if(czas_przejazdu < Best_timestamp)
      {
        Serial.println("Pobiles rekord, twoj czas: "+ String((uint32_t)czas_przejazdu) +"[ms] \n");
        Best_timestamp=czas_przejazdu;                                                    
      }

      
      winner_fx();
      start_race();
    }

    if (loop3>loop_max) //win 2
    {
      for(int i=0;i<NPIXELS;i++)
      {
        track.setPixelColor(i, track.Color(Color3.R,Color3.G,Color3.B) );
      }
      track.show();
      
      timestamp_end=millis();
      uint32_t czas_przejazdu=timestamp_end-timestamp;
      Serial.println("Gracz 3(zielony). Najlepszy wynik: " + String((uint32_t)Best_timestamp) +"[ms] Czas osiagniety: " + String((uint32_t)czas_przejazdu) +"[ms] \n");
     
      if(czas_przejazdu < Best_timestamp)
      {
        Serial.println("Pobiles rekord, twoj czas: "+ String((uint32_t)czas_przejazdu) +"[ms] \n");
        Best_timestamp=czas_przejazdu;                                                    
      }

      
      winner_fx();
      start_race();
    }
    
    if ((millis() & 512)==(512*draworder)) {if (draworder==0) {draworder=1;}
                          else {draworder=0;}   
                         }; 

    if (draworder==0) {draw_car1();draw_car2();draw_car3();}
        else {draw_car3(); draw_car2();draw_car1();}   
                 
    track.show(); 
    delay(tdelay);
    
    if (TBEEP>0) {TBEEP-=1; 
                  if (TBEEP==0) {noTone(PIN_AUDIO);}; // lib conflict !!!! interruption off by neopixel
                 };   
}
