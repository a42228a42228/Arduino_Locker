#include <Stepper.h>
#include "IRremote.h"
#define STEPS 2048 //2048 steps per revolution

int lightPin = 0; //photocell input A0
int receiver = 12; //IR receiver pin 12
int buzzer = 7; // buzzer pin 7
Stepper stepper(STEPS, 8, 10, 9, 11); //stepper control pin8,9,10,11

IRrecv irrecv(receiver); //create receiver instance
decode_results results; // IR input

String default_pw = "0000"; //default password is "0000"
String user_pw = default_pw; //default user password is default password
String input; //record input
boolean set_check = false; //using to check the mode of setting password
boolean open_check = false; //using to check the box is open or not


void detectClose(){
  if(analogRead(lightPin) > 100){ //if the box is close
    delay(500);
    Serial.println("Close");
    stepper.step(512); //stepper motor rotate 90 degrees to lock the box
    open_check = false; //box is closed
    delay(2000); //wait until stepper motor finish the motion 
    digitalWrite(8, LOW); //shutdown stepper motor
    digitalWrite(9, LOW);
    digitalWrite(10, LOW); 
    digitalWrite(11, LOW);
  }
}


void setup() {
  Serial.begin(9600); //connect to serial monitor
  stepper.setSpeed(10); //set speed of stepper motor
  irrecv.enableIRIn(); //turn on IR receiver
  pinMode(buzzer,OUTPUT); //set buzzer
}


void loop() {
  if(!open_check){   //if the box is locked
    if(irrecv.decode(&results)){  //if receive IR signal
      switch(results.value){
        case 0xFF6897:  //"1" button
          if(set_check){  //mode of setting password
            user_pw += "1";
          }else{  //mode of inputting password
            input += "1";
            Serial.println("1");
          }
           delay(20);
           break;
        case 0xFF9867:  //"2" button
          if(set_check){
            user_pw += "2";
          }else{
            input += "2";
            Serial.println("2");
          }
           delay(20);
           break;
        case 0xFFB04F:  //"3" button
          if(set_check){
            user_pw += "3";
          }else{
            input += "3";
            Serial.println("3");
          }
           delay(20);
           break;
        case 0xFF30CF:  //"4" button
          if(set_check){
            user_pw += "4";
          }else{
            input += "4";
            Serial.println("4");
          }
          delay(20);
          break;
        case 0xFF18E7:  //"5" button
          if(set_check){
            user_pw += "5";
          }else{
            input += "5";
            Serial.println("5");
          }
          delay(20);
          break;
        case 0xFF7A85:  //"6" button
          if(set_check){
            user_pw += "6";
          }else{
            input += "6";
            Serial.println("6");
          }
          delay(20);
          break;
        case 0xFF10EF:  //"7" button
          if(set_check){
            user_pw += "7";
          }else{
            input += "7";
            Serial.println("7");
          }
          delay(20);
          break;
        case 0xFF38C7:  //"8" button
          if(set_check){
            user_pw += "8";
          }else{
            input += "8";
            Serial.println("8");
          }
          delay(20);
          break;
        case 0xFF5AA5:  //"9" button
          if(set_check){
            user_pw += "9";
          }else{
            input += "9";
            Serial.println("9");
          }
          delay(20);
          break;
        case 0xFF4AB5:  //"0" button
          if(set_check){
            user_pw += "0";
          }else{
            input += "0";
            Serial.println("0");
          }
          delay(20);
          break;
        case 0xFF42BD: //"*" button
          Serial.println("Setting password");
          digitalWrite(buzzer,HIGH); //buzzer makes sound once
          delay(100);
          digitalWrite(buzzer,LOW);
          user_pw = "";  //initalize the user password
          set_check = true;  //setting mode start
          delay(20);
          break;
        case 0xFF02FD:  //"OK" button
          if(set_check){  //mode of setting password
            Serial.println("Password setting is done");  //finish to set password
            Serial.println("Password is " + user_pw);
            input = "";  //initalize input
            set_check = false;  //close set password mode
            delay(20);
          }else{
            //check password
            Serial.println("Check password");
            if(input.equals(user_pw)){  //correct password
              Serial.println("open");
              stepper.step(-512); //stepper motor rotate 90 degrees to open the box
              open_check = true;  //box is open
              delay(2000); //wait until stepper motor finish the motion 
              digitalWrite(8, LOW); //shutdown stepper motor
              digitalWrite(9, LOW);
              digitalWrite(10, LOW); 
              digitalWrite(11, LOW);
            }else{
              Serial.println("Wrong password");  //wrong password
              for(int i =0; i < 2; i++){  //buzzer makes sound twice
                digitalWrite(buzzer,HIGH);
                delay(300);
                digitalWrite(buzzer,LOW);
                delay(300);
              }
              Serial.println("Please input again");
              delay(20);
            }
            input = "";  //initalize input
          }
          break;
      }
      irrecv.resume();  //receive next IR input
    }
  }else{  //if the box is unlocked
      delay(100);  //wait for user open the box
      detectClose();  //start detect the box is close or not
  }
}
