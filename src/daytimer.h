#ifndef DAYTIMER_H
#define DAYTIMER_H

class DayTimer{
 private:
  float time{400};
  float increment{0.01};
  float daylength{2400};
 public:
  DayTimer(){};
  DayTimer(float Incre, float Length){
    increment = Incre;
    daylength = Length;
  }
  
  float GetTime(){return time;};
  void SetTime(int newtime){time = newtime;};
  void SetIncrement(int newincre){increment = newincre;};
  void SetDayLength(int length){daylength = length;};
  void IncrementTime(){
    time = time + increment;
    if(time > daylength)
      time = 0;
  };
};

extern DayTimer DayTime;

#endif //DAYTIMER_H
