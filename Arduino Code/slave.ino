#include <stdio.h>
#include <stdlib.h>
#include <SPI.h>
#include <string.h>
#include <SD.h>
#define SCK_PIN 13
#define MISO_PIN 12
#define CS_PIN 9
#define CS_MCP 10
#define MOSI_PIN 11
File file;
int buzzer=8;


void setup (void) {
   pinMode(buzzer,OUTPUT);
  
   pinMode(MOSI_PIN,OUTPUT);
  pinMode(MISO_PIN,INPUT);  
  pinMode(SCK_PIN,OUTPUT);
  pinMode(CS_MCP,OUTPUT);

digitalWrite(CS_MCP,HIGH);
digitalWrite(CS_PIN,HIGH);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  


  digitalWrite(CS_MCP,LOW);
  SPI.transfer(0x40);
  SPI.transfer(0x0A);   //IOCON
  SPI.transfer(0x0C); 
  delay(200);  
  digitalWrite(CS_MCP,HIGH); 

    digitalWrite(CS_MCP,LOW);
  SPI.transfer(0x40);
  SPI.transfer(0x01);    //IODIR B
  SPI.transfer(0x00); 
   delay(200); 
     digitalWrite(CS_MCP,HIGH); 

 digitalWrite(CS_MCP,LOW);
 SPI.transfer(0x40);
 SPI.transfer(0x00);    //IODIR A
  SPI.transfer(0x00); 
   delay(200); 
   digitalWrite(CS_MCP,HIGH); 



   

 delay(200);
  
  


    

  Serial.begin(9600);

//debut partie ecriture
 initializeSD();
 
  
}

int v1,v2,v3,v4,v5,v6,v7;


void loop (void) {

 

   
    
  while(Serial.available())
      {
v1=Serial.read();
delay(100);
v2=Serial.read();
delay(100);
v3=Serial.read();
delay(100);
v4=Serial.read();
delay(100);
v5=Serial.read();
delay(100);
v6=Serial.read();
delay(100);
v7=Serial.read();
delay(100);



if(v4==1)
 {beep(50);beep(50);beep(50);}

if(v1==1 && v2==1)
   {digitalWrite(CS_MCP,LOW);
 SPI.transfer(0x40);
 SPI.transfer(0x12);
 if(v3==1)
 {SPI.transfer(0xD0);
 //SPI.transfer(0xFF);
 }
 else
 {SPI.transfer(0xC0);
  //SPI.transfer(0xFF);
  }
  
 digitalWrite(CS_MCP,HIGH); 
 delay(30);}
   else if ((v1==0 && v2==1)||(v1==1 && v2==0))
   {digitalWrite(CS_MCP,LOW);
 SPI.transfer(0x40);
 SPI.transfer(0x12);
if(v3==1)
 {SPI.transfer(0x50);}
 else
 {SPI.transfer(0x40);} 
 digitalWrite(CS_MCP,HIGH); 
 delay(30);}
 else
 {digitalWrite(CS_MCP,LOW);
 SPI.transfer(0x40);
 SPI.transfer(0x12);
 if(v3==1)
 {SPI.transfer(0x10);}
 else
 {SPI.transfer(0x00);}
 digitalWrite(CS_MCP,HIGH); 
 delay(30);} 
 //Debut ecriture luminiosite
  createFile("data.txt");
writeToFile("*************");
closeFile();
delay(100);
 createFile("data.txt");
writeToFile("*************");
closeFile();
delay(100);
   createFile("data.txt");
writeToFile("valeur de luminiosite ");
closeFile();
delay(100);
   createFile("data.txt");
writeToFile("*************");
closeFile();
delay(100);
int x=v5;
char res[20];
createFile("data.txt");
itoa(x,res,10);
writeToFile(res);
closeFile();
delay(100);
createFile("data.txt");
writeToFile("*************");
closeFile();
delay(100);
  createFile("data.txt");
writeToFile("*************");
closeFile();
delay(100);
 //fin ecriture luminiosite
 //Debut ecriture temperature
   createFile("data.txt");
writeToFile("valeur de temperature ");
closeFile();
delay(100);
   createFile("data.txt");
writeToFile("*************");
closeFile();
delay(100);
int y=v6;
char res1[20];
createFile("data.txt");
itoa(y,res1,10);
writeToFile(res1);
closeFile();
delay(100);
createFile("data.txt");
writeToFile("*************");
closeFile();
delay(100);
  createFile("data.txt");
writeToFile("*************");
closeFile();
delay(100);
 //fin ecriture temperature
 //Debut ecriture humidite
   createFile("data.txt");
writeToFile("valeur de humidite ");
closeFile();
delay(100);
   createFile("data.txt");
writeToFile("*************");
closeFile();
delay(100);
int z=v7;
char res2[20];
createFile("data.txt");
itoa(z,res2,10);
writeToFile(res2);
closeFile();
delay(100);
createFile("data.txt");
writeToFile("*************");
closeFile();
delay(100);
  createFile("data.txt");
writeToFile("*************");
closeFile();
delay(100);
 //fin ecriture humidite

   
}
}
void initializeSD()
{
  //Serial.println("Initializing SD card...");
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin())
  {
    //Serial.println("SD card is ready to use.");
  } else
  {
   // Serial.println("SD card initialization failed");
    return;
  }
}
int createFile(char filename[])
{
  file = SD.open(filename, FILE_WRITE);

  if (file)
  {
    //Serial.println("File created successfully.");
    return 1;
  } else
  {
    //Serial.println("Error while creating file.");
    return 0;
  }}
int writeToFile(char text[])
{
  if (file)
  { file.println(text);
    //Serial.println("Writing to file: ");
    //Serial.println(text);
    return 1;
  } else
  {
    //Serial.println("Couldn't write to file");
    return 0;
  }
}

void closeFile()
{
  if (file)
  {
    file.close();
   // Serial.println("File closed");
  }
}

int openFile(char filename[])
{
  file = SD.open(filename);
  if (file)
  {
  //  Serial.println("File opened with success!");
    return 1;
  } else
  {
   // Serial.println("Error opening file...");
     //   Serial.println(filename);

    
    return 0;
  }
  
}
void beep(unsigned char delayms){
  digitalWrite(buzzer,HIGH);
  delay(delayms);          // wait for a delayms ms
  digitalWrite(buzzer,LOW);
  delay(delayms);          // wait for a delayms ms   
} 

