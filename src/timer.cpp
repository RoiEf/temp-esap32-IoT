#include "timer.h"

timer::timer(){
  delta = 1000;   // 1 sec delta
  startMillis = millis();       //initial start time for DwD
}
timer::timer(int t, int x){
  delta = long(x*t);   // ?min delta
  startMillis = millis();       //initial start time for DwD
}

void timer::setDelta(int t, int x){
  delta = long(x*t);   // ?min delta
}

int timer::getDelta(){
  return int(delta/6000);
}

void timer::resetTimer(){
  startMillis = millis();
}

bool timer::checkInterval(){
  currentMillis = millis();
  if (currentMillis - startMillis >= delta)  //test whether the period has elapsed
  {
    return 1;
  } else {
    return 0;
  }
}
