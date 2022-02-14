#include <LiquidCrystal.h>
#include <Servo.h>


/*--LCD Data Pins--*/
const int rs=1,en=2,d4=3,d5=4,d6=5,d7=6;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

/*--ESP8266 Data Pins--*/
const int SignalPin=7;
const int Motor_pin=8;
const int Water_pin=9;
const int Fire_Pin=12;
const int WiFi_pin=13;

/*Anlog Read Pins*/
const int FireSensorPin=A0;
const int IR_Sensor1=A1;
const int IR_Sensor2=A2;
const int IR_Sensor3=A3;
const int WaterDetector=A4;

String History="Clear";
String Massage="";
String Stat="";
String Bridge_STAT ; 
String History1="";

Servo Servo_1,Servo_2;
/*Initialization*/
void setup(){

  Serial.begin(9600);

	pinMode(FireSensorPin,INPUT);
	pinMode(IR_Sensor1,INPUT);
	pinMode(IR_Sensor2,INPUT);
	pinMode(IR_Sensor3,INPUT);
	pinMode(WaterDetector,INPUT);

	pinMode(Motor_pin,OUTPUT);
	pinMode(Water_pin,OUTPUT);
	pinMode(Fire_Pin,OUTPUT);
	pinMode(WiFi_pin,INPUT);
  pinMode(SignalPin,INPUT);
	Servo_2.attach(10);
	Servo_1.attach(11);

	lcd.begin(16,2);

}
/*Body of Program*/
void loop() {
  
if (analogRead(FireSensorPin) <=500 && History != "Clear"){
		
		digitalWrite(Fire_Pin,LOW);
		digitalWrite(Water_pin,HIGH);
		digitalWrite(Motor_pin,HIGH);

   	Serial.println("Fire");

		Massage="Fire_Detected";
		lcd.clear();
		LCD_Display(Massage);
		Stat="Bridge_is_Close";
		lcd.clear();
		LCD_Status(Stat);
   		MotorAntiClockWise();
   		History="Clear";

	}

	else if (analogRead(FireSensorPin) <=500 )
	{
		
		digitalWrite(Fire_Pin,LOW);
		digitalWrite(Water_pin,HIGH);
		digitalWrite(Motor_pin,LOW);

   		Serial.println("Fire");

		Massage="Fire_Detected";
		lcd.clear();
   		History ="Clear";
		LCD_Display(Massage);
		Stat="Bridge_is_Close";
		lcd.clear();
		LCD_Status(Stat);
   	
   	}

   	else if (analogRead(WaterDetector) <= 500 )
   	{

		digitalWrite(Fire_Pin,HIGH);
		digitalWrite(Water_pin,LOW);
		digitalWrite(Motor_pin,HIGH);

   	Serial.println("Water");

		Massage="Flood_Detected";
		LCD_Display(Massage);
		Stat="Bridge_is_Close";
		LCD_Status(Stat);
		MotorAntiClockWise();

	}
	else if(analogRead(IR_Sensor1) <= 500 && History=="Clear")
	{
		
		Serial.println("Ship Arived");
		History="Set";
		Stat="Ship_Is_Arived";
		LCD_Status(Stat);

	}
	
	else if(analogRead(IR_Sensor2) <=500 && History =="Set")
	{
		digitalWrite(Motor_pin,HIGH);
		digitalWrite(Fire_Pin,HIGH);
		digitalWrite(Water_pin,HIGH);

  	 	Serial.println("Bridge open");
   		
   		lcd.clear();
		Massage="Bridge_Opening";
		LCD_Display(Massage);
  		lcd.clear();
		Stat="Bridge_is_open";
		LCD_Status(Stat);

		if (History1 != "Clear")
		{
			MotorClockWise();	
		}
		
		History1=="Clear";
		History="pass";

	}

	else if (analogRead(IR_Sensor3) <=500 && History =="pass")
	{
		digitalWrite(Motor_pin,LOW);
		digitalWrite(Fire_Pin,HIGH);
		digitalWrite(Water_pin,HIGH);
  		
  		Serial.println("Bridge close");

		Massage="Bridge_Closing";
		lcd.clear();
		LCD_Display(Massage);
   		lcd.clear();
		Stat="Bridge_is_close";
		LCD_Status(Stat);

		if (History1 != "set")
		{
			MotorAntiClockWise();
		}

		History1=="set";
		History="Clear";
	}

	else if(digitalRead(WiFi_pin) == HIGH && History != "Clear" && digitalRead(SignalPin)==HIGH)
	{
    					
			digitalWrite(Motor_pin,LOW);
			digitalWrite(Fire_Pin,HIGH);
			digitalWrite(Water_pin,HIGH);
			
			Serial.println("Close");
		
			Massage="Bridge_Closing";
			lcd.clear();
			LCD_Display(Massage);
			Stat="Bridge_is_Close";
			lcd.clear();
			LCD_Status(Stat);
      	
      		if (History1 != "set")
      		{
      			MotorAntiClockWise();
      			History1="set";
      		}
			
			History="Clear";
    
	}
	
	else if(digitalRead(WiFi_pin)== LOW && History == "Clear" && digitalRead(SignalPin)== LOW)
	{

			digitalWrite(Motor_pin,HIGH);
			digitalWrite(Fire_Pin,HIGH);
			digitalWrite(Water_pin,HIGH);

			Serial.println("_OPEN_"); 
		
			Massage="Bridge_Opening";
			lcd.clear();
			LCD_Display(Massage);
     		lcd.clear();
			Stat="Bridge_is_open";
			LCD_Status(Stat);
			if (History1 != "Clear")
			{
				MotorClockWise();
				History1 ="Clear";
			}
			
			History = "pass";
	}
}

void MotorClockWise(){
  int j = 180;
	for (int i = 0; i <= 180; ++i)
	{
   		 j-=1;
		Servo_1.write(i);
		Servo_2.write(j);
		delay(15);
	}
}

void MotorAntiClockWise(){
  int j =0;
	for (int i = 180; i >= 0; --i)
	{
   		 j+=1;
		Servo_1.write(i);
		Servo_2.write(j);
		delay(15);
	}
}

void LCD_Display(String msg){
	
	 	lcd.setCursor(0,0);
	 	lcd.print(msg);
  		int Count=9;
 	
 	while(Count>0)
 	{
     	lcd.setCursor(0,1);
		  delay(1000);
	  	lcd.print(" ");
    	lcd.print(Count);
    	Count -= 1;

	}
}

void LCD_Status(String Status){

	lcd.setCursor(0,0);
	lcd.print(Status);
}
