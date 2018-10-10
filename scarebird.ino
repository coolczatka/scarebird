/*
 *  Bluetooth orders
 *  -1 - do nothing
 *  0 - switch off/on
 *  1 - increment interval
 *  2 - decrement interval
 *  3 - volume+
 *  4 - volume-
 */
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

#define DURATION 3 //sec
void incrementInterval();
void decrementInterval();
void button_services();
void play_sound();
void increaseVolume();
void decreaseVolume();

TMRpcm tmrpcm;
unsigned long interval = 1000;
bool isOn = false;
unsigned int vol = 5;
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

  pinMode(ON_OFF_BTN, INPUT);
  pinMode(INC_INTERVAL_BTN, INPUT);
  pinMode(DEC_INTERVAL_BTN, INPUT);
  pinMode(VPLUS_BTN, INPUT);
  pinMode(VMINUS_BTN, INPUT);

  if (!SD.begin(4))
    return; // initialization failed HALT
  if (!SD.exists("hawk_screaming.wav"))
    return; // there is no correct sound file HALT
  tmrpcm.speakerPin = SPEAKER;

  Serial.begin(9600);
  while (!Serial.available());

}

void loop() {
  controller.run();
}

void incrementInterval() {
  if (interval + 1000 > 0xFFFFFFFF) //if it's takes more then 4 bytes
    return;
  interval += 1000;
}
<<<<<<< HEAD
void decrementInterval() {
  if (interval - 1000 > 0) //if it's takes more then 4 bytes
=======
void decrementInterval(){
  if(interval-1000>0)//if it's takes less then 0
>>>>>>> c9f18251a50335ee39d50d6d7384848fe1be8e5a
    return;
  interval -= 1000;
}
void troggleState() {
  isOn = !isOn;
}
void increaseVolume() {
  ++vol;
  tmrpcm.setVolume(vol);
}
void decreaseVolume() {
  --vol;
  tmrpcm.setVolume(vol);
}

void button_services() {
  int bt_order;
  bt_order = Serial.read();
  if (ON_OFF_BTN == 0 || bt_order == 0) {
    isOn = !isOn;
  }
  if (INC_INTERVAL_BTN == 0 || bt_order == 1) {
    incrementInterval();
  }
  if (DEC_INTERVAL_BTN == 0 || bt_order == 2) {
    decrementInterval();
  }
  if (VPLUS_BTN == 0 || bt_order == 3) {
    increaseVolume();
  }
  if (VMINUS_BTN == 0 || bt_order == 4) {
    decreaseVolume();
  }
}

void play_sound() {
  if (isOn) {
    tmrpcm.play("hawk_screaming.wav", DURATION);
    delay(interval * 60);
  }
}

