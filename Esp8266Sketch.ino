#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

char* ssid = "GPPune";
char* password ="47174717";

ESP8266WebServer server(80);
#define SignalPin D3
#define motor D4  
#define WATER_PIN D5        
#define FIRE_PIN D6        
#define WIFI D7
#define Led_Alarm_Red D2
#define Buzzer D1

String Water_Status;
String Fire_Status;
String BridgeStatus;
String History= "clear";


char SendHTML_LogPage[] PROGMEM =R"===(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>IOT Based Bridge</title>
<style>
body{
  margin: 0;
  padding: 0;
  background: #262626;
  background-size: cover;
}
.container{
  width: 280px;
  position: absolute;
  top: 50%;
  left: 50%;
  padding: 20px;
  transform:translate(-50%,-50%);
  box-shadow: 0 10px 30px rgba(0,0,0,1);
  color: white;
  border-radius: 5px;
  background: #000;
}
.container h1{
  float: ;
  font-size: 40px;
  border-bottom: 6px solid #4caf50;
  margin-bottom: 50px;
  padding: 13px 0;
}
.login-box{
  width: 100%;
  overflow: hidden;
  font-size: 20px;
  padding: 8px 0;
  margin: 8px 0;
  border-bottom: 1px solid #4caf50;
}
.login-box input{
  border: none;
  outline: none;
  background: none;
  color: white;
  font-size: 18px;
  width: 80%;
  float: left;
  margin: 0 10px;
}
.btn{
  width: 100%;
  background: none;
  border: 2px solid #4caf50;
  color: white;
  padding: 5px;
  font-size: 18px;
  cursor: pointer;
  margin: 12px 0;
}
p{
  color: white;
  font-size:10px;
}
</style>

</head>
<body>
  <div class="container">
    <h1>IOT BRIDGE</h1>
    <form action="/Authority_Check" method="POST">
        <div class="login-box">
          <input  type="text" name="UserName" placeholder="Username">
          </div> 

          <div class="login-box"> 
            <input  type="password" name="PassWord" placeholder="Password">
        </div>

        <input  type="submit" class="btn" value="Login">
    </form> 
    <p>Try  'Khalisi'  and 'Pratham'</p>
  </div>

</body>
</html>
)===";

char SendHTML_Logged[] PROGMEM = R"===(
 <!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>IOT Based Bridge</title>
<style>
  body{
  margin: 0;
  padding: 0;
  background: #262626;
  align-content: center;
}
.box{
  position:absolute;
  top: 50%;
  left: 50%;
  transform:translate(-50%,-50%);
  width: 200px;
  padding:20px;
  background: #000;
  color: #fff; 
  text-align: center;
  box-shadow: 0 10px 30px rgba(0,0,0,1);
  border-radius: 5px;
  overflow: hidden;
}
.box:before{
  content: '';
  position: absolute;
  top: 0;
  left: -50%;
  width: 100%;
  height: 100%;
  background: rgba(255,255,255,0.05);
  transform: skew(-50deg);
  pointer-events: none;
}
.box p{
  margin: 0;
  padding: 0;
  color: #fff;
  font-size: 24px;
}
.box label
{
  position: relative;
  cursor: pointer;
}
.box label input{
  display: none;
}
.box label span{
  position:relative;
  display: inline-block;
  margin: 20px 10px;
  width: 50px;
  padding: 5px;
  background: #000;
  border: 1px solid #444;
  color: #444;
  border-radius: 4px;
}
.box label input:checked ~ span{
  color: #fff;
  border: 1px solid #008eff;
}
.box label input:checked~span:before{
  content: '';
  width: 100%;
  height: 100%;
  position: absolute;
  top: 0;
  left: 0;
  background: #008eff;
  z-index: -1;
  filter: blur(10px);
}
.box label input:checked ~ span:after{
  content: '';
  width: 100%;
  height: 100%;
  position: absolute;
  top: 0;
  left: 0;
  background: #008eff;
  z-index: -1;
  filter: blur(15px);
}
.box label input:checked ~ span.yes{
  color: #62ff00;
  border-color: #62ff00; 
  box-shadow: inset 0 0 10px #62ff00;
}
.box label input:checked ~ span.yes:after,
.box label input:checked ~ span.yes:before{
  background: #62ff00;
}
.box label input:checked ~ span.no{
  color: #008eff;
  border-color: #008eff; 
  box-shadow: inset 0 0 10px #008eff;
}
.box label input:checked ~ span.no:after,
.box label input:checked ~ span.no:before{
  background: ##008eff;
}
.kk{
  background: none;
  border: 2px solid #4caf50;
  border-radius: 10px;
  color: white;
  padding: 5px;
  font-size: 14px;
  margin: 12px 0;

}
.Confirm{
  border: none;
  outline: none;
  background: none;
  color: white;
  font-size: 15px;
  width: 70%;
  margin: 0 10px;
  
}
</style>

</head>
<body> 

<form action="/Authorised" method="POST">  
  <div class="box">  
    <P>Brigde action</P>
    <label>
    <input type="radio" name= "Bridge_Status" value="CLOSE">
    <span class="yes">CLOSE</span>
    </label>
    <label>
    <input type="radio" name= "Bridge_Status" value="OPEN">
    <span class="no">OPEN</span>
    </label>  
    <input type="text" class="Confirm" name="confirm" placeholder="type confirm" >
  <input  type="submit" class ="kk" value="Confirm">
  </div>
</form> 
</body>
</html>
)===";

void setup()
{
  pinMode( Buzzer, OUTPUT);
  pinMode( Led_Alarm_Red, OUTPUT);
  pinMode( SignalPin, OUTPUT);
  pinMode( WIFI, OUTPUT);
  pinMode( WATER_PIN, INPUT);
  pinMode( FIRE_PIN, INPUT);
  pinMode( motor, INPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  while(WiFi.status()!= WL_CONNECTED )
  {
    Serial.print(".");
    delay(500);
     }
  Serial.print(" ");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.print(" ");

server.on("/", handleRoot);
server.on("/LoginPage", handlePage);
server.on("/Authority_Check",handleLogin);
server.on("/Authorised",handleLogged);
server.onNotFound(handleNotFound);
server.begin();

}

void loop() {
  server.handleClient(); 
  if (digitalRead(WATER_PIN) ==  LOW && digitalRead(FIRE_PIN) == LOW )
  {
    Water_Status="Detected";
    Fire_Status="Detected";
    BridgeStatus="Open";
    digitalWrite(Buzzer,HIGH);
    digitalWrite(Led_Alarm_Red,HIGH);
  }
  else if (digitalRead(WATER_PIN) ==  LOW)
  {
    Water_Status="Detected";
    Fire_Status="Not_Detected";
    BridgeStatus="Open";
    digitalWrite(Buzzer,HIGH);
    digitalWrite(Led_Alarm_Red,HIGH);
  }
  else if(digitalRead(FIRE_PIN) == LOW)
  {
    Fire_Status="Detected";
    BridgeStatus="close";
    Water_Status="Not_Detected";
    digitalWrite(Buzzer,HIGH);
    digitalWrite(Led_Alarm_Red,HIGH);

  }
 else if(digitalRead(motor) == HIGH && History == "clear")
  {
    BridgeStatus="open";
    Fire_Status="Not_Detected";
    Water_Status="Not_Detected";   
    History="set";
    digitalWrite(Buzzer,LOW);
    digitalWrite(Led_Alarm_Red,LOW);
 
  } else if(digitalRead(motor) == LOW && History == "set"){
    
    Fire_Status="Not_Detected";
    Water_Status="Not_Detected";
    BridgeStatus="close";
    History = "clear";
    digitalWrite(Buzzer,LOW);
    digitalWrite(Led_Alarm_Red,LOW);
  }
}



void handleRoot()
  {
  server.send(200,"text/html",SendHTML_Initial(Fire_Status,Water_Status,BridgeStatus));
  }
 
 void handlePage(){
  server.send_P(200,"text/html",SendHTML_LogPage);
 }

void handleLogin()
  { 
    if(!server.hasArg("UserName") || !server.hasArg("PassWord") || server.arg("UserName")==NULL ||server.arg("PassWord")==NULL)
      {
        server.send(400,"text/plain","400:Invalid Request");
         return;
      }
     else{
      if(server.arg("UserName")=="Khalisi" && server.arg("PassWord")=="Pratham")
     
       {
       
        server.send(303,"text/html",SendHTML_Logged);
       }
     else
       {
        server.send(401,"text/plain","401:Unauthorized");
         
      }
  }
}

void handleLogged(){
  if(!server.hasArg("confirm") || !server.hasArg("Bridge_Status")|| server.arg("Confirm")==NULL)
  {
    server.send(400,"text/plain","400:Invalid Request");
      digitalWrite(SignalPin,LOW);
      return;
 
  }else{
    if (server.arg("confirm")=="Confirm" && server.arg("Bridge_Status")=="OPEN")
    {
      digitalWrite(WIFI,LOW);
      digitalWrite(SignalPin,HIGH);
      delay(2000);
      digitalWrite(SignalPin,LOW);
      
      Serial.print("OPEN");

      server.sendHeader("Location","/");
      server.send(303);

    }else if(server.arg("confirm")=="Confirm" && server.arg("Bridge_Status")=="CLOSE")
    {
      digitalWrite(WIFI,HIGH);
      digitalWrite(SignalPin,HIGH);
      delay(2000);
      digitalWrite(SignalPin,LOW);

      Serial.print("CLOSE");

      server.sendHeader("Location","/");
      server.send(303);
    }else{
      digitalWrite(SignalPin,LOW);
      Serial.println("error Button value");
    }
  }
}


void handleNotFound()
  {
    server.send(404,"text/plain","404: Not Found");
  }
  

String SendHTML_Initial(String FierStat,String WaterStat,String BridgeStat)
  {
  String ptr ="<!DOCTYPE html>\n";
 ptr +="<html><head><meta name=\"viewport\"content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"refresh\" content=\"5\"><style>\n";
 ptr +="body{margin: 0;padding: 0;background: #262626;background-size: cover;}\n";
 ptr +=".container{width: 280px;position: absolute;top: 50%;left: 50%;padding: 20px;transform:translate(-50%,-50%);box-shadow: 0 10px 30px rgba(0,0,0,1);color: white;border-radius: 5px;background: #000;}\n";
 ptr +=".container h1{font-size: 40px;border-bottom: 6px solid #4caf50;margin-bottom: 50px;padding: 13px 0;}\n";
 ptr +=".btn{width: 100%;background: none;border: 2px solid #4caf50;color: white;padding: 5px;font-size: 18px;cursor: pointer;margin: 12px 0;}\n";
 ptr +=".Athority-use{border-bottom: 4px solid #4caf50;margin-bottom: 12px;padding: 0px 65px 13px;}\n";
 ptr +=".Status-box{border-bottom: 4px solid #4caf50;padding: 0px 100px 13px;}\n";
 ptr +=".Status-boxx{padding: 0px 70px;}\n";
 ptr +="</style><title>IOT Based Bridge</title></head><body>\n";
 ptr +="<div class=\"container\">\n";
 ptr +="<h3 class=\"Status-box\">STATUS</h3>\n";
 ptr +="<div class=\"Status-boxx\">\n";
 ptr +="<h4>Fire:";
 ptr +=FierStat;
 ptr +="</h4><h4>Water:";
 ptr +=WaterStat;
 ptr +="</h4><h4>Bridge:";
 ptr +=BridgeStat;
 ptr +="</h4></div><br>\n";
 ptr +="<h3 class=\"Athority-use\">Athority Use Only</h3>\n";
 ptr +="<form\taction=\"/LoginPage\"\tmethod=\"POST\">\n";
 ptr +="<input\ttype=\"submit\" class=\" btn\" value=\"Login\"></form> </div></body></html>:";
 return ptr;
   }
