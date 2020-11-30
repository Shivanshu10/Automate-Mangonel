#include<LiquidCrystal.h>
#include<Servo.h>

#define RS 8
#define EN 9
#define D4 10
#define D5 11
#define D6 12
#define D7 13
#define TRIGGER 7
#define ECHO 6
#define SWITCH 5
#define TIME_TO_CM 0.01723
#define G 9.8
#define DEGREE_TO_RADIANS 0.01745329
#define MOTOR 3
#define TIME_TO_M 0.0001723
#define CM_TO_M 100
#define CAL 5
#define MAX_RANGE 250

LiquidCrystal lcd(RS,EN,D4,D5,D6,D7);
Servo motor;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

float angle(float dist, float cal)
{
  float launch=(dist / cal);
  if (dist>MAX_RANGE)
  {
    launch=-1;
  }
  return (launch);
}

void motorRotation(float angle, int pin)
{
  motor.write(angle);
}

float measureVelocity(float dist, float ang, float g)
{
  dist=dist/CM_TO_M;
  ang=ang*DEGREE_TO_RADIANS;
  return(sqrt(g*dist/sin(2*ang)));
}

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(SWITCH, INPUT);
  motor.attach(3);
}

void loop()
{
  if (digitalRead(SWITCH))
  {
  	lcd.setCursor(0,0);
  	double cm = TIME_TO_CM * readUltrasonicDistance(TRIGGER, ECHO);
  	// Serial.println(cm);
  	lcd.print(cm);
  	lcd.print(" cm");
  	float theta = angle(cm, CAL);
  	// Serial.println(theta);
  	lcd.setCursor(0,1);
  	lcd.print(theta);
  	lcd.print(" deg");
  	motorRotation(theta, MOTOR);
  	float vel = measureVelocity(cm, theta, G);
    Serial.println(vel);
    lcd.setCursor(0,0);
    delay(2000);
    lcd.print(vel);
    lcd.print(" m/s");
    delay(50);
    motor.write(0);
  }
}
