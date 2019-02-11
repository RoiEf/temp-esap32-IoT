#ifndef tim
#define tim


#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


class timer {
  unsigned long currentMillis;
  unsigned long startMillis;
  unsigned long delta;
  
  public:
  timer();
  timer(int, int);
  void setDelta(int, int);
  int getDelta();
  void resetTimer();
  bool checkInterval();
};

#endif
