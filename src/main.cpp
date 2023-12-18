#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

#define LED_RED 5
#define LED_GREEN 4

const char * SSID = "ZTE-59ba94";
const char * PASSWORD = "44ffba59homeELABBASSI";

const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Lampe</title>
    <script src="https://kit.fontawesome.com/fee0429066.js" crossorigin="anonymous"></script>

    <style>
        body{
            background-color: black;
        }
        div{
            display: flex;
            justify-content: center;
            align-items: center;
            background-color: black;
            width: 100%;
            height: 220px;
            
        }
        button{
            font-size: 100px;
            width: 200px;
            height: 200px;
            border-radius: 50%;
            cursor: pointer;
        }
        .on{
            background-color: cornflowerblue;
            box-shadow: 0px 0px 200px 50px cornflowerblue ;
            border: none;
            outline: none;
        }
        h1{
            color: cornflowerblue;
            display: block;
        }
    </style>
</head>
<body>
    <div>
        <h1 id="etatLed"></h1>
        <br>
    </div>
    <div>
        <button id="btn" class="on" onclick="switchLed()" ><i class="fa-sharp fa-solid fa-power-off"></i></button>    
    </div>
    <script>
        function switchLed() {
            var element = document.getElementById("btn");
            element.classList.toggle("on");
            appelServeur("/switchLed",traiteReponse);
        }
        function appelServeur(url, cFonction){
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function(){
                if(this.readyState == 4 && this.status == 200){
                    cFonction(this);
                }
            }
            xhttp.open("GET",url,true);
            xhttp.send();
        }
        function traiteReponse(xhttp){
          document.getElementById("etatLed").innerHTML = "Lamp IS " + xhttp.responseText;
        }
    </script>
</body>
</html>
)=====";

ESP8266WebServer WebServer(80);

void onConnected(const WiFiEventStationModeConnected& event);
void onGotIp(const WiFiEventStationModeGotIP& event);

void handleRoot(){
  WebServer.send(200, "text/html", index_html);
}
void SwitchLed(){
  if(digitalRead(LED_GREEN)){
    digitalWrite(LED_GREEN,LOW);
    WebServer.send(200, "text/html", "ON");
    handleRoot();

  }
  else{
    digitalWrite(LED_GREEN,HIGH);
    WebServer.send(200, "text/html", "OFF");
    handleRoot();

  }
  
}

void setup() {
  Serial.begin(115200L);
  Serial.println("");

  pinMode(LED_RED,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID,PASSWORD);
  static WiFiEventHandler onConnectedHandler = WiFi.onStationModeConnected(onConnected);
  static WiFiEventHandler onGotIpHandler = WiFi.onStationModeGotIP(onGotIp);

  WebServer.on("/switchLed",SwitchLed);
  WebServer.on("/",handleRoot);
  WebServer.on("/index.html",handleRoot);
  WebServer.begin();
}

void loop() {
  if(WiFi.isConnected()){
    digitalWrite(LED_RED,HIGH);
    WebServer.handleClient();
  }
  else{
    digitalWrite(LED_RED,LOW);

  }


}


void onConnected(const WiFiEventStationModeConnected& event){
  Serial.println("Wifi connected");
}
void onGotIp(const WiFiEventStationModeGotIP& event){
  Serial.println("Adresse Ip : " + WiFi.localIP().toString());
  Serial.println("Passerelle Ip : " + WiFi.gatewayIP().toString());
  Serial.println("DNS Ip : " + WiFi.dnsIP().toString());
  Serial.print("Puissance de réception : ");
  Serial.println(WiFi.RSSI());
}
