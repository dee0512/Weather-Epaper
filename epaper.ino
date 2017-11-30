/*********************************************************************************************************
*
* File                : Arduino-epd
* Hardware Environment:
* Build Environment   : Arduino
* Version             : V1.6.1
* By                  : WaveShare
*
*                                  (c) Copyright 2005-2015, WaveShare
*                                       http://www.waveshare.net
*                                       http://www.waveshare.com
*                                          All Rights Reserved
*
*********************************************************************************************************/
#include <epd.h>
const int led =  D7;                           //user led

const int offset[10] = {0,20,0,0,0,0,0,0,0,0};
const int width[10] = {120,100,120,120,130,120,120,120,120,120};

void setup(void)
{
  /*
  user led init
  */
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  epd_init();
  epd_screen_rotation(EPD_INVERSION);
  epd_wakeup();
  epd_set_memory(MEM_TF);
  epd_set_color(BLACK, WHITE);
  epd_clear();
  epd_set_ch_font(GBK32);
  epd_set_en_font(ASCII32);
  epd_disp_bitmap("amherst.BMP", 240, 100);
  epd_udpate();
  Time.zone(-5);
  Particle.subscribe("eWeather_today_dee0512", updateTemperature);
  // updateTemp(9);

  // epd_disp_string("MHERST", 230,100);


}

void updateTemperature(const char *event, const char *data){
  int temp;
  if(sscanf(data, "%d", &temp) == 1){
    String tempString = String(temp);
    epd_clear();
    epd_disp_bitmap("amherst.BMP", 240, 100);
    time_t time = Time.now();
    epd_disp_string(Time.format(time, "%I:%M %p"), 250,140);
    epd_disp_string(Time.format(time, "%a %d %b"), 235,170);
    int toalWidth = 0;
    for(int i = 0; i< tempString.length(); i++){
      int number = tempString.charAt(i) - '0';
      toalWidth += width[number];
    }
    int currentWidth = 0;
    for(int i = 0; i< tempString.length(); i++){
      int number = tempString.charAt(i) - '0';
      int currentOffset = (-1 * (toalWidth)/2)  + currentWidth;
      currentWidth += width[number];
      drawTemp(tempString.charAt(i),currentOffset);
    }
    epd_disp_bitmap("degree.BMP", 320 + (toalWidth/2), 380);
    epd_disp_bitmap("celcius.BMP", 350 + (toalWidth/2), 380);
    drawAccent(1);
    epd_udpate();
  }
}

void drawTemp (char digit, int characterOffset){
  int number = digit - '0';
  String imageString(digit);
  imageString = imageString + ".BMP";
  epd_disp_bitmap(imageString, 320 + offset[number] + characterOffset, 250);
}

void updateTemp(int temp){
  String tempString = String(temp);
  epd_clear();
  epd_disp_bitmap("amherst.BMP", 240, 100);
  time_t time = Time.now();
  epd_disp_string(Time.format(time, "%I:%M %p"), 250,140);
  epd_disp_string(Time.format(time, "%a %d %b"), 235,170);
  int toalWidth = 0;
  for(int i = 0; i< tempString.length(); i++){
    int number = tempString.charAt(i) - '0';
    toalWidth += width[number];
  }
  int currentWidth = 0;
  for(int i = 0; i< tempString.length(); i++){
    int number = tempString.charAt(i) - '0';
    int currentOffset = (-1 * (toalWidth)/2)  + currentWidth;
    currentWidth += width[number];
    drawTemp(tempString.charAt(i),currentOffset);
  }
  epd_disp_bitmap("degree.BMP", 320 + (toalWidth/2), 380);
  epd_disp_bitmap("celcius.BMP", 350 + (toalWidth/2), 380);
  drawAccent(1);
  epd_udpate();
}

void drawAccent(int option){
    switch (option) {
      case 1:
        epd_disp_bitmap("web.BMP", 0, 0);
        epd_disp_bitmap("SPIDER.BMP", 5, 130);
        epd_disp_bitmap("web.BMP", 475, 0);
        epd_disp_bitmap("SPIDER.BMP", 480, 130);
    }
}

void loop(void)
{
}
