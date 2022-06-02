
#include "timer.h"

Timer timer;

const int LM393 = 2;
int counter = 0;
void setup() {
  attachInterrupt(digitalPinToInterrupt(LM393), count, RISING);
  Serial.begin(115200);
  timer.setInterval(1000);
  timer.setCallback(RPM);
  timer.start();
}

void count() {
  counter++;
}

void RPM() {
  Serial.println(counter * 60*0.1885*.0025);
  counter = 0;
}

void loop() {
  timer.update();
}
