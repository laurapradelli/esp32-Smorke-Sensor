#include <Arduino.h>
#include <Wifi.h>
#include <DFRobotDFPlayerMini.h>
#include <ESP_Mail_Client.h>
#include <time.h>
#include <PubSubClient.h>
#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

//dati per collegamento al wifi
#define SSID ""
#define PASSWORD ""

//dati per collegamento al db
#define API_KEY ""
#define DB_URL "https://provaesp32-63e5e-default-rtdb.europe-west1.firebasedatabase.app/"

#define ntpServer "pool.ntp.org"

#define smtp_server "smtp.gmail.com"
#define smtp_port 465
#define sender "";
#define sender_password "";
#define recipient "";

#define sensor_input 36

//costanti per calcolo ora
const long gmtOffset_sec=3600;
const int daylightOffset_sec=3600;

HardwareSerial mySerial(2);
DFRobotDFPlayerMini myDFPlayer;
WiFiClient esp;
PubSubClient client(esp);
FirebaseData fData;
FirebaseConfig config;
FirebaseAuth auth;
SMTPSession smtp;


int audio[3]={0, 0, 0};
int mail[5]={0, 0, 0, 0, 0};
int i=0, x=0;
int check_audio=0, check_mail=0;   //check_audio serve per controllare se c'è fumo per 3 volte e fa sentire il messaggio
//check_mail controlla se c'è fumo per 5 volte e manda la mail

bool isAuthenticated = false;


void setup(){
  Serial.begin(9600);

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

  //SETUP WIFI
  WiFi.begin(SSID, PASSWORD);
  Serial.print("WiFi - ");
  while(WiFi.status()!=WL_CONNECTED){
    delay(200);
  }
  Serial.println("ok");

  //SETUP DB
  Serial.print("Firebase - ");
  //Mail per accesso al db
  auth.user.email="";
  //Password
  auth.user.password="";
  config.api_key=API_KEY;
  config.database_url=DB_URL;
  if(Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    isAuthenticated=true;
  }
  else{
    Serial.println("failed");
  }
  Firebase.begin(&config, &auth);

  //SETUP ORARIO
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  //SETUP MQTT
  Serial.print("MQTT - ");
  //Da completare con indirizzo del server MQTT e relativa porta
  client.setServer("", 1883);
  client.connect("esp32");
  if(client.connected()){
    Serial.println("ok");
  }
  else{
    Serial.println("failed");
  }


  //SETUP EMAIL
  MailClient.networkReconnect(true);
  smtp.debug(1);
  smtp.callback(smtpCallback);

  Session_Config config;
  config.server.host_name=smtp_server;
  config.server.port=smtp_port;
  config.login.email=email_sender;
  config.login.password=email_password;
  config.login.user_domain="";
  config.time.ntp_server=F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset=3;
  config.time.day_light_offset=0;

  SMTP_Message message;
  message.sender.name=F("SensoreFumo");
  message.sender.email=email_sender;
  message.subject=F("ALLARME FUMO");
  message.addRecipient(F("Dest"), email_recipient);
  String textMess="È stata rilevata un'allerta fumo presso il tuo sensore!";
  message.text.content=textMess.c_str();
  message.text.charSet="us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;
  smtp.connect(&config);
}



void loop(){
  check_audio=0;
  check_mail=0;

  int livello=410; //livello iniziale settato basso

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
    MailClient.sendMail(&smtp, &message);
  }

  if(isAuthenticated && Firebase.ready()){
    if(Firebase.pushInt(fData, "/sensoreFumo", livello)){
      Serial.println("Dato inserito correttamente");
    }
    else{
      Serial.println("Qualcosa è andato storto");
    }
  }

  //invio dei dati al broker mqtt per visualizzazione su app
  client.publish("/SensoreFumo", String(livello).c_str());

  delay(4000);
}