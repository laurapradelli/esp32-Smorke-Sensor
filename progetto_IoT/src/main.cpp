#include <Arduino.h>
#include <WiFi.h>
#include <DFRobotDFPlayerMini.h>
#include <esp_system.h>
#include <EMailSender.h>
#include <FirebaseESP32.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define sensor_input 36

//dati per collegamento al wifi
#define SSID "Pradelli WLAN"
#define PASSWORD "PRDLRA05D63F257A"


HardwareSerial mySerial(2);
DFRobotDFPlayerMini myDFPlayer;


int audio[3]={0, 0, 0};
int mail[5]={0, 0, 0, 0, 0};
int i=0, x=0;
int check_audio=0, check_mail=0;   //check_1 serve per controllare se c'è fumo per 3 volte e fa sentire il messaggio
//check_2 controlla se c'è fumo per 5 volte e manda la mail

bool isAuthenticated = false;


void setup(){
  //SETUP SERIALE
  Serial.begin(9600);


  //SETUP WIFI
  WiFi.begin(SSID, PASSWORD);
  Serial.print("WiFi - ");
  while(WiFi.status()!=WL_CONNECTED){
    delay(200);
  }
  Serial.println("ok");

  //SETUP SPEAKER
  Serial.print("Speaker - ");
  mySerial.begin(9600, SERIAL_8N1, 27, 26);
  if(!myDFPlayer.begin(mySerial)){
    Serial.println("failed");
  }
  else{
    Serial.println("ok");
  }
  myDFPlayer.volume(30);


}

void loop(){
  int sensor_aout=analogRead(sensor_input);
  Serial.print("Sensore: ");
  Serial.println(sensor_aout);

  check_audio=0;
  check_mail=0;

  int livello=analogRead(sensor_input);
  Serial.println(livello);
  //int livello=410;

  audio[i]=livello;
  mail[x]=livello;
  i=(i+1)%3;
  x=(x+1)%5;
  for(int j=0;j<3;j++){
    if(audio[j]>400){
      check_audio++;
    }
  }
  for(int k=0;k<5;k++){
    if(mail[k]>400){
      check_mail++;
    }
  }
  if(check_audio==3){
    myDFPlayer.play(1);
  }
  
  if(check_mail==5){
    Serial.println("invio la mail");
  }
  

  delay(1000);
}