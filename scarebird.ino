#include <SD.h>
#include <Thread.h>
#include <ThreadController.h>
#include <TMRpcm.h>


#define MISO 11
#define MOSI 12
#define SCK 13
#define CS 10

#define SPEAKER 9

#define ON_OFF_BTN 4
#define INC_INTERVAL_BTN 5
#define DEC_INTERVAL_BTN 6
#define VPLUS_BTN 7
#define VMINUS_BTN 8

void incrementInterval();
void decrementInterval();
void button_services();
void play_sound();
void increaseVolume();
void decreaseVolume();

TMRpcm tmrpcm;
unsigned long interval=1000;
bool isOn = false;
unsigned int vol=5;
Thread sound = Thread();
Thread buttons = Thread();
ThreadController controller = ThreadController();

void setup() {
  sound.onRun(play_sound);
  sound.setInterval(10);
  sound.enabled = true;
  buttons.onRun(button_services);
  buttons.setInterval(100);
  buttons.enabled = true;
  controller.add(&sound);
  controller.add(&buttons);
  
  pinMode(ON_OFF_BTN,INPUT);
  pinMode(INC_INTERVAL_BTN,INPUT);
  pinMode(DEC_INTERVAL_BTN,INPUT);
  pinMode(VPLUS_BTN,INPUT);
  pinMode(VMINUS_BTN,INPUT);
  if (!SD.begin(4)) 
    return; // initialization failed HALT
  if(!SD.exists("hawk_screaming.wav"))
    return; // there is no correct sound file HALT
  tmrpcm.speakerPin = SPEAKER;
}

void loop() {
  controller.run();
}

void incrementInterval(){
  if(interval+1000>0xFFFFFFFF)//if it's takes more then 4 bytes
    return;
  interval+=1000;
}
void decrementInterval(){
  if(interval-1000>0)//if it's takes more then 4 bytes
    return;
  interval-=1000;
}
void troggleState(){
  isOn=!isOn;
}
void increaseVolume(){
  ++vol;
  tmrpcm.setVolume(vol);
}
void decreaseVolume(){
  --vol;
  tmrpcm.setVolume(vol);
}

void button_services(){
  if(ON_OFF_BTN==0){
    isOn=!isOn;
  }
  if(INC_INTERVAL_BTN==0){
    incrementInterval();
  }
  if(DEC_INTERVAL_BTN==0){
    decrementInterval();
  }
  if(VPLUS_BTN==0){
    increaseVolume();
  }
  if(VMINUS_BTN==0){
    decreaseVolume();
  }
}

void play_sound(){
  if(isOn){
  tmrpcm.play("hawk_screaming.wav",2);
  delay(interval*60);
  }
}

