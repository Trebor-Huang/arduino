#include <Servo.h>
namespace motor
{
//定义五中运动状态
const int STOP = 0;
const int FORWARD = 1;
const int BACKWARD = 2;
const int TURNLEFT = 3;
const int TURNRIGHT = 4;
//定义需要用到的引脚
const int leftMotor1 = 8;
const int leftMotor2 = 9;
const int rightMotor1 = 10;
const int rightMotor2 = 11;
void motorSet(int);
void setup()
{
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
}

void loop()
{
  int cmd;
  for (cmd = 0; cmd < 5; cmd++)
  {
    motorSet(cmd);
    delay(2000);
  }
}

void motorSet(int cmd)
{
  switch (cmd)
  {
    case FORWARD:
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
    case BACKWARD:
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
    case TURNLEFT:
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
    case TURNRIGHT:
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
    default:
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
  }
}
} // namespace motor
/*
namespace echo
{
const int Echo = 3;
const int Trig = 4;
float cm;
void setup()
{
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(5, OUTPUT);
  
}

float echo()
{
  digitalWrite(Trig, LOW);
  delay(2);
  digitalWrite(Trig, HIGH);
  delay(10);
  digitalWrite(Trig, LOW);
  return pulseIn(Echo, HIGH) / 58.0;
}

void loop()
{
  //Serial.println(echo());
  if (echo() < 10)
  {
    digitalWrite(5, LOW);
  }
  else
  {
    digitalWrite(5, HIGH);
  }
}
} // namespace echo*/
namespace sound
{
const int SOUND = 6;

void setup()
{
  pinMode(SOUND, INPUT);
  
}
int getSound(int m)
{
  int r = 0;
  for (int i = 0; i < m; i++)
  {
    r += 1 - digitalRead(SOUND);
  }
  //Serial.println(r);
  return r;
} // threshold: getSound(100)<75
void loop()
{
  Serial.println(String(getSound(100)) + "\t100");
}
} // namespace sound
namespace gradient
{
enum GRAD
{
  GrL = 2,
  GrR = 3,
  GrM = 4
};
void setup() {
  pinMode(GrL, INPUT);
  pinMode(GrM, INPUT);
  pinMode(GrR, INPUT);
}
int l[10],m[10],r[10];
void init(){
  // initializes gradient data
  for(int i=0;i<10;i++){
  l[i] = analogRead(GrL);
  m[i] = analogRead(GrM);
  r[i] = analogRead(GrR);
  }
}
inline int grad(int side)
{
  return analogRead(side);
}
int detect() {
  // -1 0 +1
  // LL M RR
  int M = grad(GrM);
  int L = grad(GrL);
  int R = grad(GrR);
  Serial.println(String(l[0]) + "\t" + String(m[0]) + "\t" + String(r[0])+"\t"+String(L) + "\t" + String(M) + "\t" + String(R));
  if(M-m[0]>10){
    //check left
    if(l[0]-L>10){
      return -1;
    }else if(r[0]-R>10){
      return 1;
    }else{
      return 0;
    }
  }else{
    for(int i=0;i<9;i++){
      l[i] = l[i+1];
      r[i] = r[i+1];
      m[i] = m[i+1];
    }
    l[9] = L;
    m[9] = M;
    r[9] = R;
    return 0;
  }
}
} // namespace gradient
namespace optic{
  const int L = 2;
  const int R = 12;
  void setup(){
    pinMode(L, INPUT);
    pinMode(R, INPUT);
  }
  int detect(){
    return ((1-digitalRead(L))<<1) | (1-digitalRead(R));
  }
  void loop(){
    Serial.println(String(digitalRead(L)) + "\t" + String(digitalRead(R)));
  }
} //namespace optic

namespace fan{
  const int FAN = 1;
  void setup(){
    pinMode(FAN, OUTPUT);
  }
  void set(bool istoon){
    digitalWrite(FAN, int(istoon));
  }
} //namespace fan

bool detected_sound = false;
void setup() {
  Serial.begin(9600);
  motor::setup();
  gradient::setup();
  sound::setup();
  optic::setup();
  fan::setup();
  detected_sound = false;
}
void loop() {
  if(!detected_sound){
    Serial.println("Waiting");
    detected_sound = sound::getSound(100) > 50;
    if(detected_sound){
      
  gradient::init();
    }
    return;
  }
  switch(gradient::detect()){
    case -1: //left
    {
      motor::motorSet(motor::TURNLEFT);
      while(gradient::detect()!=0){
        Serial.println("Waiting left");
      }
      break;
    }
    case 1: //right
    {
      motor::motorSet(motor::TURNRIGHT);
      while(gradient::detect()!=0){
        Serial.println("Waiting right");
      }
      break;
    }
    case 0:
    {
      motor::motorSet(motor::FORWARD);
      //Serial.println("FOrward");
      break;
    }
  }
}
