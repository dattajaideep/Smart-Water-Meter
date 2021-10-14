#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
int sensorInterrupt = 0;  // interrupt 0
int sensorPin       = 2; //Digital Pin 2
int solenoidValve = 4; // Digital pin 5
//int c=0;
int b=0;

/*The hall-effect flow sensor outputs pulses per second per litre/minute of flow.*/
float calibrationFactor = 4.5; //You can change according to your datasheet

volatile byte pulseCount =0;  

float flowRate = 0.0;
unsigned int flowMilliLitres =0;
unsigned long totalMilliLitres = 0;

unsigned long oldTime = 0;
MFRC522 mfrc522(SS_PIN, RST_PIN);
void setup()
{

  // Initialize a serial connection for reporting values to the host
  pinMode(solenoidValve , OUTPUT);
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  Serial.begin(9600);
   SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();
  Serial.println("Put the card over or somewhere near the RF id Reader");
  Serial.println();

  /*The Hall-effect sensor is connected to pin 2 which uses interrupt 0. Configured to trigger on a FALLING state change (transition from HIGH
  (state to LOW state)*/
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING); //you can use Rising or Falling
 
}

void loop()
{
   //The below while loop gets inistiated only once
//----------------------------------------------------
  //Counting of taps

for(int j=0;j<ar.length();j++)
{
  
  for(int i=0;b<4;i++)
  {
   if(b!=i)
   {
    break;     
   }
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  String s="";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     s=s+mfrc522.uid.uidByte[i];
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == ar[j]) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Auterized access");
    b++;
    Serial.print("Number of times accesed : ");
    Serial.println(b);
  delay(10000);
  }
  else
  {
    Serial.println("UN-Autherized acess");
  }
  }
  if(b==3)
  {
  Serial.println("Acces limit crossed");
  }
}  
//hvdhksfbjksabfksbdfukaslkdfdshvilsdhvljkdhxvkhdxbdvoshoufvhsudviusgdvuishvousdhvuisxgvludsgvblkdbxviudsvui
  digitalWrite(4,HIGH);
  Serial.println("Opened the valve");
  while(digitalRead(4)==HIGH)
    {      
   if((millis() - oldTime) > 1000)    
  { 
    Serial.println("Stsrted readind the flow");
    detachInterrupt(sensorInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();

    flowMilliLitres = (flowRate / 60) * 1000;

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;

    unsigned int frac;
    Serial.print("Flow rate: ");
    Serial.print(flowMilliLitres, DEC);  
    Serial.print("mL/Second");
    Serial.print("\t");           

    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");        
    Serial.print(totalMilliLitres,DEC);
    Serial.println("mL"); 
    Serial.print("\t");         
    if (totalMilliLitres > b*10)
    {
      SetSolinoidValve();
    }
// Reset the pulse counter so we can start incrementing again
    pulseCount = 0;

    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}
}
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}

void SetSolinoidValve()
{
  digitalWrite(solenoidValve, LOW);
  Serial.println("Closed the valve");
}
