#include <SPI.h>  //SPI Biblio
#include <Wire.h> // I2C Biblio
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <BMP180I2C.h>
#include "DHT.h"
//Defning addresses and values of the tram of data
#define A0_DS1621  0
#define A1_DS1621  0
#define A2_DS1621  0
#define ADDRESS_DS1621  (0x48 | A2_DS1621<<2 | A1_DS1621<<1 | A0_DS1621)
#define ONESHOT 1   
#define POL     0   
#define NVB     0   
#define TLF     0   
#define THF     0   
#define REGISTER_CONFIG   (THF<<6 | TLF<<5 | NVB<<4 | POL<<1 | ONESHOT)
#define DONE_MASK   0x80   // Masque pour bit DONE
#define READ_TEMPERATURE  0xAA
#define ACCESS_CONFIG     0xAC
#define START_CONVERT     0xEE
#define STOP_CONVERT      0x22
#define Addr 0x39
#define DHTPIN 2 
#define DHTTYPE DHT22
#define I2C_ADDRESS 0x77

byte endConversion = 0;
byte temperatureMSB = 0;
byte temperatureLSB = 0;
float temperature;
DHT dht(DHTPIN, DHTTYPE);
BMP180I2C bmp180(I2C_ADDRESS); 
LiquidCrystal_I2C lcd(0x27, 16, 2); 

//Declaration pins of the obstacle sensor
const int trigPin = 5;
const int echoPin = 4;
long duration;
int distanceCm, distanceInch;

void setup(){
  Wire.begin();
  Serial.begin(9600);
    dht.begin();
   //Sending the tram of data I2C   
  Wire.beginTransmission(ADDRESS_DS1621);
  Wire.write(STOP_CONVERT);
  Wire.endTransmission();

  Wire.beginTransmission(ADDRESS_DS1621);
  Wire.write(ACCESS_CONFIG);   
  Wire.write(REGISTER_CONFIG); 
  Wire.endTransmission();

    if (!bmp180.begin())
  {
    while (1);
  }
  bmp180.resetToDefaults();
  bmp180.setSamplingMode(BMP180MI::MODE_UHR);


lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
  // Initialise I2C communication as MASTER


  // Starts I2C communication
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x00 | 0x80);
  // Power ON mode
  Wire.write(0x03);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Starts I2C communication
  Wire.beginTransmission(Addr);
  // Select timing register
  Wire.write(0x01 | 0x80);
  // Nominal integration time = 402ms
  Wire.write(0x02);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(30);
 
}
 int action[7]; //pour les actions qu'on va transmettre vers l'esclave
void loop()
{
     action[2]=1;

  
  Wire.beginTransmission(ADDRESS_DS1621);
  Wire.write(START_CONVERT);
  Wire.endTransmission();
 do {
    Wire.beginTransmission(ADDRESS_DS1621);
    Wire.write(ACCESS_CONFIG);
    Wire.endTransmission(false); 
    Wire.requestFrom(ADDRESS_DS1621, 1); 
    if (1 <= Wire.available()) endConversion = Wire.read() & DONE_MASK;
  } while (!endConversion);
   Wire.beginTransmission(ADDRESS_DS1621);
  Wire.write(READ_TEMPERATURE);
  Wire.endTransmission(false);  
  Wire.requestFrom(ADDRESS_DS1621, 2); 
  if (2 <= Wire.available()) {
    temperatureMSB = Wire.read(); 
    temperatureLSB = Wire.read(); 
  }
   temperature = (float) temperatureMSB;
  if (temperatureLSB & 0x80) temperature += 0.5; 
  if (temperatureMSB & 0x80) temperature -= 256;  
   

  delay(100);  
  if(temperature > 10) { 
  action[3]=1;
  }
  else
  {action[3]=0;}//test buzzer
float h = dht.readHumidity();
float t = dht.readTemperature();
float f = dht.readTemperature(true);
if (isnan(h) || isnan(t) || isnan(f)) {
   
    return;
}
 float hi = dht.computeHeatIndex(f, h);
 if (!bmp180.measureTemperature())
 {
    
    return;
  }
  do
  {
    delay(100);
  } while (!bmp180.hasValue());


  float tempBMP180 =bmp180.getTemperature() ;
  if (!bmp180.measurePressure())
  {
 
    return;
  }

  do
  {
    delay(100);
  } while (!bmp180.hasValue());

 
  float tempTOT=(tempBMP180+t)/2;

  float lux = 0, c0 = 0, c1 = 1, c3=1,test = 1;
   char c;
   
   int i=0;
 



  unsigned int data[4];
  for (int i = 0; i < 4; i++)
  {
    // Starts I2C communication
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write((140 + i));
    // Stop I2C Transmission
    Wire.endTransmission();

    
    Wire.requestFrom(Addr, 1);

    
    if (Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
    delay(20);
  }

  
  
  double ch0 = ((data[1] & 0xFF) * 256) + (data[0] & 0xFF);
  double ch1 = ((data[3] & 0xFF) * 256) + (data[2] & 0xFF);
  
  test = ch1 / ch0;
  if (test > 0 && test <= 0.52)
  { c0 =0.0304;
         c1 =0.062;
         c3=pow(test,1.4);
  }
  else if (test <=0.61 )
  { c0 =0.0224;
    c1 =0.031;
    c3=1;
  }
  else if (test <= 0.80)
  { c0 =0.0128;
    c1 =0.0153;
    c3=1;
  }
  else if (test <=1.3 )
  { c0 =0.00146;
    c1 =0.00112;
    c3=1;
  }
  else
  { c0 = 0;
    c1 = 0;
    c3=0;
  }
  // Output data to serial monitor
  /*Serial.print("Full Spectrum(IR + Visible) :");
  Serial.println(ch0);
  Serial.print("Infrared Value :");
  Serial.println(ch1);
  Serial.print("Visible Value :");
  Serial.println(ch0 - ch1);
  Serial.print("ch1/ch0 ");
  Serial.println(test);
  Serial.print("c0= ");
  Serial.print(c0);
  Serial.print(" c1= ");
  Serial.println(c1);*/
  lux = c0 * ch0 - c1 * ch1*c3;
  //Serial.print("valeur en lux ");
  //Serial.println(lux);
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distanceCm= duration*0.034/2;

                       // Initialize I2C LCD module
 if (distanceCm<35)
 {
  lcd.init(); 
    lcd.backlight(); 
      
           lcd.print("Bonjour           ");
     delay(500);
   lcd.init(); 
      
           lcd.print("Lum en LUX ");
     lcd.print(lux);
     delay(500);
     lcd.init(); 
   lcd.print("TempAmb en C ");
     lcd.print(tempTOT);
     delay(500);
      lcd.init(); 
   lcd.print("TempBoit en C ");
     lcd.print(temperature);
     delay(500);
     lcd.init(); 
        lcd.print("Humidite   ");
     lcd.print(h);
     delay(500);
 }

else lcd.noBacklight();
/**********Debut affectation des donnees************/
 
   if(lux<10)

  { 
  
    action[0]=1;
    action[1]=1;
   
    }
  else if(lux<100)
  {
    action[0]=1;
    action[1]=0;}
  else
  {
    action[0]=0;
    action[1]=0;
    }


/********** Fin affectation des donnees************/
/*******debut partie communication************/

   action[4]=(int)lux;
   action[5]=(int)tempTOT;
   action[6]=(int)h;
    Serial.write(action[0]);
    delay(100);
    Serial.write(action[1]);
    delay(100);
    Serial.write(action[2]);
    delay(100);
     Serial.write(action[3]);
    delay(100);
    Serial.write(action[4]);
    delay(100);
    Serial.write(action[5]);
    delay(100);
    Serial.write(action[6]);
    delay(100);
   // Serial.write((int)lux);
   


   
   
   
/*********fin partie communication*************/
}

  
