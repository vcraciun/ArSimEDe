#include "Header.h"

void setup()
{
  pinMode(13, TIP_OUTPUT);
  pinMode(12, TIP_OUTPUT);
  pinMode(11, TIP_OUTPUT);
  pinMode(10, TIP_OUTPUT);
  pinMode(9,  TIP_OUTPUT);
  pinMode(8,  TIP_OUTPUT);
}

void mapBitsToPins(int nr, int* pins, int count)
{
  for (int i=0;i<count;i++)   
    digitalWrite(pins[i], (nr >> (count - i - 1)) & 1);
}

void fadeout(int* leds, int count, int total_time)
{
  static int T = total_time / 10;
  
  int interval = (millis() / (total_time / T)) % T;
  
  for (int i = 0; i < T - interval; i++)
  {
      for (int j = 0; j < count; j++)
        digitalWrite(leds[j], 1);
  }

  for (int i = 0; i < interval; i++)
  {
      for (int j = 0; j < count; j++)
        digitalWrite(leds[j], 0);
  }
}

void fadein(int* leds, int count, int total_time)
{
  static int T = total_time / 10;
  
  int interval = (millis() / (total_time / T)) % T;
  
  for (int i = 0; i < T - interval; i++)
  {
      for (int j = 0; j < count; j++)
        digitalWrite(leds[j], 0);
  }

  for (int i = 0; i < interval; i++)
  {
      for (int j = 0; j < count; j++)
        digitalWrite(leds[j], 1);
  }
}

void fadeinout(int* leds, int count, int total_time)
{
  int mode = (millis() / (total_time / 2)) % 2;
  void (*fade_modes[2])(int*,int,int) = {fadein, fadeout};
  fade_modes[mode](leds, count, total_time / 2);
}

void blink(int* leds, int count, int total_time)
{
  int value = (millis() / (total_time / 2)) % 2;
  for (int i=0;i<count;i++)
  	digitalWrite(leds[i], value);
}

void pingpong(int* leds, int count, int total_time)
{
  int caz = (millis() / (total_time / (count * 2 - 2))) % (count * 2 - 2);
  if (caz < count)
  	mapBitsToPins(1 << (count - 1 - caz), leds, count);
  else
    mapBitsToPins(1 << (caz % count + 1), leds, count);
}

void reverse_pingpong(int* leds, int count, int total_time)
{
  int caz = (millis() / (total_time / (count * 2 - 2))) % (count * 2 - 2);
  int leds_on = pow(2, count);
  if (caz < count)
  	mapBitsToPins((1 << (count - 1 - caz)) ^ leds_on, leds, count);
  else
    mapBitsToPins((1 << (caz % count + 1)) ^ leds_on, leds, count);
}

void cycle(int* leds, int count, int total_time)
{
  int caz = (millis() / (total_time / count)) % count;
  mapBitsToPins(1 << (count - 1 - caz), leds, count);
}

void negative_cycle(int* leds, int count, int total_time)
{
  int caz = (millis() / (total_time / count)) % count;
  int leds_on = pow(2, count);
  mapBitsToPins((1 << (count - 1 - caz)) ^ leds_on, leds, count);
}

void lower(int* leds, int count, int total_time)
{
  int caz = (millis() / (total_time / (count + 1))) % (count + 1);
  int bits_on = (1 << (count - caz)) - 1;
  mapBitsToPins(bits_on, leds, count);  
}

void raise(int* leds, int count, int total_time)
{
  int caz = (millis() / (total_time / (count + 1))) % (count + 1);  
  int bits_on = ~((1 << (count - caz)) - 1);
  mapBitsToPins(bits_on, leds, count);    
}

void count(int* leds, int pin_count, int total_time)
{
  int caz = (millis() / (total_time / (1 << pin_count))) % (1 << pin_count);  
  mapBitsToPins(caz, leds, pin_count);    
}

void reverse_count(int* leds, int pin_count, int total_time)
{
  int caz = (1<<pin_count) - 1 - (millis() / (total_time / (1 << pin_count))) % (1 << pin_count);  
  mapBitsToPins(caz, leds, pin_count);    
}

void to_center(int* leds, int count, int total_time)
{
  int value = (millis() / (total_time / (count / 2))) % (count / 2);
  for (int i=0; i < count / 2; i++)
  {
    int nr = 1<<(count - 1 - value) | (1<<value);
  	mapBitsToPins(nr, leds, count);    
  }
}

void negative_to_center(int* leds, int count, int total_time)
{
  int value = (millis() / (total_time / (count / 2))) % (count / 2);
  for (int i=0; i < count / 2; i++)
  {
    int nr = ((1 << count) - 1) ^ (1<<(count - 1 - value) | (1<<value));
  	mapBitsToPins(nr, leds, count);    
  }  
}

void cycle_2(int* leds, int count, int total_time)
{
  int nr;
  int caz = (millis() / (total_time / (count + 1))) % (count + 1);
  if (caz < count)  
  	nr = 1 << (count - 1 - caz) | 1 << (count - 2 - caz);
  else
    nr = 0;
  mapBitsToPins(nr, leds, count);  
}

void cycle_3(int* leds, int count, int total_time)
{
  int nr;
  int caz = (millis() / (total_time / (count + 1))) % (count + 1);
  if (caz < count)  
  	nr = 1 << (count - 1 - caz) | 1 << (count - 2 - caz) | 1 << (count - 3 - caz);
  else
    nr = 0;
  mapBitsToPins(nr, leds, count);   
}

void negative_cycle_2(int* leds, int count, int total_time)
{
  int nr;
  int caz = (millis() / (total_time / (count + 1))) % (count + 1);
  if (caz < count)  
  	nr = ~(1 << (count - 1 - caz) | 1 << (count - 2 - caz));
  else
    nr = (1 << count) - 1;
  mapBitsToPins(nr, leds, count);    
}

void negative_cycle_3(int* leds, int count, int total_time)
{
  int nr;
  int caz = (millis() / (total_time / (count + 1))) % (count + 1);
  if (caz < count)  
  	nr = ~(1 << (count - 1 - caz) | 1 << (count - 2 - caz) | 1 << (count - 3 - caz));
  else
    nr = (1 << count) - 1;
  mapBitsToPins(nr, leds, count);    
}

void loop()
{
  static int leds[6] = {13,12,11,10,9,8};
  //fadein(leds, 2, 5000);
  //blink(13, 100);  	
  //pingpong(leds, 6, 500);  
  //reverse_pingpong(leds, 6, 1000); 
  //cycle(leds, 6, 1000);
  //negative_cycle(leds, 6, 1000);
  //lower(leds, 6, 5000);
  //raise(leds, 6, 5000);
  //reverse_count(leds, 6, 5000);
  //cycle_2(leds, 6, 5000);
  //negative_to_center(leds, 6, 1000);
}