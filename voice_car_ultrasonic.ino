#include <AFMotor.h>
#include<Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial BT(A0, A1);
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
#define trigPin1 A3                       //Define the HC-SE04 triger 
#define echoPin1 A2                       //Define the HC-SE04 echo 
#define trigPin2 A5                       //Define the HC-SE04 triger 
#define echoPin2 A4                       //Define the HC-SE04 echo 
String command; //Int to store app command state.
int speedCar = 100; // Initial car speed set 0 to 255.
//boolean lightFront = false;
//boolean lightBack = false;
//boolean horn = false;

Servo myservo;

void setup()
{
     pinMode(trigPin1, OUTPUT);           //set the trigpin to output
    pinMode(echoPin1, INPUT);            //set the echopin to input 
  pinMode(trigPin2, OUTPUT);           //set the trigpin to output
    pinMode(echoPin2, INPUT);            //set the echopin to input 

    Serial.begin(9600);
    BT.begin(9600);

  //Stop();
}

void forward()
{
 

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
    motor1.setSpeed(speedCar);
  motor2.setSpeed(speedCar);
  motor3.setSpeed(speedCar);
  motor4.setSpeed(speedCar);
  

}

void backward()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  motor1.setSpeed(speedCar);
  motor2.setSpeed(speedCar);
  motor3.setSpeed(speedCar);
  motor4.setSpeed(speedCar);
}
void left()
{
  myservo.write(180);
  delay(500);
  myservo.write(90);
  delay(500);
  motor1.run(FORWARD);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(FORWARD);
  motor1.setSpeed(speedCar);
  motor4.setSpeed(speedCar);
  delay(1000);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
void right()
{
  myservo.write(180);
  delay(500);
  myservo.write(90);
  delay(500);
  motor1.run(RELEASE);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(RELEASE);
  motor2.setSpeed(speedCar);
  motor3.setSpeed(speedCar);
  delay(1000);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void keep_watch() {
  digitalWrite (3, HIGH);
  digitalWrite (4, LOW);
  digitalWrite (5, LOW);
  digitalWrite (6, LOW);
  delay (100);
}

void Stop()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void loop(){

int duration1, distance1;             //Define two intregers duration and distance to be used to save data
    digitalWrite(trigPin1, HIGH);        //write a digital high to the trigpin to send out the pulse
    delayMicroseconds(50);             //wait half a millisecond
    digitalWrite(trigPin1, LOW);         //turn off the trigpin
    duration1 = pulseIn(echoPin1, HIGH);  //measure the time using pulsein when the echo receives a signal set it to high
    distance1 = (duration1/2) / 29.1;     //distance is the duration divided by 2 becasue the signal traveled from the trigpin then back to the echo pin, then devide by 29.1 to convert to centimeters
    Serial.println(distance1);             //Dispaly the distance on the serial monitor 
    //Serial.println(" CM");              //in centimeters
if (distance1<10)
{
  Stop();
  backward();
  delay(100);
   Stop();
  Serial.println(" STOP..._obstacle1");
  command == "NA";
} 

int duration2, distance2;             //Define two intregers duration and distance to be used to save data
    digitalWrite(trigPin2, HIGH);        //write a digital high to the trigpin to send out the pulse
    delayMicroseconds(50);             //wait half a millisecond
    digitalWrite(trigPin2, LOW);         //turn off the trigpin
    duration2 = pulseIn(echoPin2, HIGH);  //measure the time using pulsein when the echo receives a signal set it to high
    distance2 = (duration2/2) / 29.1;     //distance is the duration divided by 2 becasue the signal traveled from the trigpin then back to the echo pin, then devide by 29.1 to convert to centimeters
    //Serial.print(distance2);             //Dispaly the distance on the serial monitor 
    //Serial.println(" CM");              //in centimeters


if (distance2<20)
{
  Stop();
 forward();
  delay(100);
   Stop();
  Serial.println(" STOP..._obstacle2");
}
while (BT.available())
{
delay(3);
char c = BT.read();
command += c;
Serial.println(command);
}
//Serial.println(command);
 if (command.length() > 0){
  //Serial.println("match");
    if(command == "go ahead"){
      Serial.println("forward");
      forward();
      }
    else if(command == "go back"){
      backward();
      }
      else if (command == "stop")
      {
        Stop();
      }
      else if (command == "left")
      {
        left();
      }
        else if (command == "right")
      {
        Serial.println("go right");
        right();
      }
    
    command=""; //Reset the variable after initiating
}
}
