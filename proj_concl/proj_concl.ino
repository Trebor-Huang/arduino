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
} // namespace echo
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
float lv, mv, rv = 0;
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
inline int grad(int side)
{
  return analogRead(side);
}
int detect() {
  Serial.println(String(lv) + "\t" + String(mv) + "\t" + String(rv));
  if (lv - grad(GrL) > 10) {
    // Left turns black
    return 1; // Turn left
  }
  if (rv - grad(GrR) > 10) {
    // Right turns black
    return 2; // Turn right
  }
  lv = (grad(GrL));
  mv = (grad(GrM));
  rv = (grad(GrR));

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

bool detected_sound = false;
void setup() {
  Serial.begin(9600);
  motor::setup();
  gradient::setup();
  sound::setup();
  echo::setup();
  optic::setup();
  detected_sound = false;
}
int opt;
void loop() {
  if(!detected_sound){
    Serial.println("Waiting");
    detected_sound = sound::getSound(100) > 50;
    return;
  }
  //motor::motorSet(motor::FORWARD);
}
