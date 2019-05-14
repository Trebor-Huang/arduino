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
const int leftMotor1 = 4;
const int leftMotor2 = 5;
const int rightMotor1 = 6;
const int rightMotor2 = 7;
void motorRun(int);
void setup()
{
  // put your setup code here, to run once:
  //设置控制电机的引脚为输出状态
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
}

void loop()
{
  int cmd;
  for (cmd = 0; cmd < 5; cmd++) //依次执行向前、向后、向左、想有、停止四个运动状态
  {
    motorRun(cmd);
    delay(2000); //每个命令执行2s
  }
}
//运动控制函数
void motorRun(int cmd)
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
  Serial.begin(9600);
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
  Serial.begin(9600);
}
int getSound(int m)
{
  int r = 0;
  for (int i = 0; i < m; i++)
  {
    r += 1 - digitalRead(SOUND);
  }
  return r;
} // threshold: getSound(100)<75
void loop()
{
  Serial.println(String(getSound(100)) + "\t100");
}
} // namespace sound
namespace infrared
{

} // namespace infrared
namespace gradient
{
  float lv, mv, rv = 0;
enum GRAD
{
  Grad_L = 2,
  Grad_R = 3,
  Grad_M = 4
};
void setup(){
  pinMode(Grad_L, INPUT);
  pinMode(Grad_M, INPUT);
  pinMode(Grad_R, INPUT);
}
inline int grad(int side)
{
  return analogRead(side);
}
int detect(){
  Serial.println(String(lv) + "\t" + String(mv) + "\t" + String(rv));
    if(lv - grad(Grad_L) > 10){
      // Left turns black
      return 1; // Turn left
    }
    if(rv - grad(Grad_R) > 10){
      // Right turns black
      return 2; // Turn right
    }
    lv = (grad(Grad_L));
    mv = (grad(Grad_M));
    rv = (grad(Grad_R));
  
}
} // namespace gradient

void setup() {
  motor::setup();
  gradient::setup();
  sound::setup();
  echo::setup();
}
void loop() {
  gradient::detect();
}
