#include <epd.h>
const int led =  D7;                           //user led

const int offset[11] = {0,20,0,0,0,0,0,0,0,0,0};
const int width[11] = {120,100,120,120,130,120,120,120,120,120,120};
const bool test = false;

void setup(void)
{
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  epd_init();
  epd_wakeup();
  epd_screen_rotation(EPD_INVERSION);
  epd_set_memory(MEM_TF);
  epd_set_color(BLACK, WHITE);
  epd_set_ch_font(GBK32);
  epd_set_en_font(ASCII32);
  Time.zone(-5);

  epd_clear();
  epd_disp_bitmap("amherst.BMP", 240, 100);
  epd_udpate();

  if(test){
    drawTop(-9);
  }else{
    Particle.subscribe("eWeather_today_dee0512", updateTemperature);
  }


  // epd_disp_string("MHERST", 230,100);


}

void updateTemperature(const char *event, const char *data){
  int temp;
  if(sscanf(data, "%d", &temp) == 1){
    drawTop(temp);
  }
}

void drawTemp (char digit, int characterOffset){
  int number = digit - '0';
  String imageString(digit);
  imageString = imageString + ".BMP";
  int numberOffset,yOffset;
  if(imageString == "-.BMP"){
    numberOffset =offset[10];
    yOffset = 75;
  }else{
    numberOffset =offset[number];
    yOffset = 0;
  }
  epd_disp_bitmap(imageString, 320 + numberOffset + characterOffset, 250 + yOffset);
}

void drawTop(int temp){
  String tempString = String(temp);
  epd_clear();

  epd_disp_bitmap("amherst.BMP", 240, 100);

  time_t time = Time.now();
  epd_disp_string(Time.format(time, "%I:%M %p"), 250,140);
  epd_disp_string(Time.format(time, "%a %d %b"), 245,170);

  int toalWidth = 0;
  for(int i = 0; i< tempString.length(); i++){
    int number = tempString.charAt(i) - '0';
    if(tempString.charAt(i) == '-'){
      toalWidth += width[10];
    }else{
      toalWidth += width[number];
    }
  }

  int currentWidth = 0;
  for(int i = 0; i< tempString.length(); i++){
    int number = tempString.charAt(i) - '0';
    int currentOffset = (-1 * (toalWidth)/2)  + currentWidth;
    if(tempString.charAt(i) == '-'){
        currentWidth += width[10];
    }else{
        currentWidth += width[number];
    }
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
