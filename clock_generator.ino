
#include <SPI.h>
#include <stdint.h>

#define CSB 10
#define SDA 16
#define SCL 17

/*
 * @function: registerWrite()
 * @description: writes the 16bit data to the selected register
 * 
 * Parameters
 *      (address,data) <-- register address, data to be written
 *      
 * Returns
 *      None
 */
void registerWrite(uint8_t address, uint16_t data)
{
     digitalWrite(CSB,LOW);
     SPI.transfer(address);  //We are not masking, because the MSB will be zero and that stisfies the condition to write to a register
     SPI.transfer((data>>8) & 0xFF);
     SPI.transfer(data & 0x00FF); 
     digitalWrite(CSB,HIGH);
}

void registerRead(uint8_t address)
{
   //Read enable 
    registerWrite(0x00,0x2218);
    
    uint16_t data;
    digitalWrite(CSB,LOW);
    SPI.transfer(address | 0x80);
    data = SPI.transfer(0x00);
    data = (data << 8) | (SPI.transfer(0x00));
    digitalWrite(CSB,HIGH);
}


void setup()
{
    Serial.begin(9600);
    pinMode(CSB,OUTPUT);
    pinMode(CE,OUTPUT);
    //pinMode(SCL,OUTPUT);
    //pinMode(SDA,OUTPUT);
}

void loop() 
{

    //Initial Power-on sequence
    //CE should be HIGH
    //digitalWrite(CE,HIGH);
    //Ensure valid Reference clock is applied to OSCin
    delay(1);     //wait for more than 500 microseconds
    digitalWrite(CSB,HIGH);
  
    SPI.begin();
    delay(1000);
      
   //Write RESET=1 in R0
   registerWrite(0x00,0x0002);
   delay(10); 
   //New config begins

   registerWrite(0x7D,0x2288);
   delay(10);
   registerWrite(0x72,0x7802);
   delay(10);
   registerWrite(0x4E,0x0001);
   delay(10);
   registerWrite(0x4B,0x0A40);
   delay(10);
   registerWrite(0x47,0x0081);
   delay(10);
   registerWrite(0x3A,0x9001);
   delay(10);
   registerWrite(0x39,0x0020);
   delay(10);
   registerWrite(0x34,0x0421);
   delay(10);
   registerWrite(0x2D,0xC61F);
   delay(10);
   registerWrite(0x2C,0x1FA3);
   delay(10);
   registerWrite(0x27,0x03E8);
   delay(10);
   registerWrite(0x26,0x0000);
   delay(10);
   registerWrite(0x25,0x0305);
   delay(10);
   registerWrite(0x24,0x00A0);
   delay(10);
   registerWrite(0x1E,0x18A6);
   delay(10);
   registerWrite(0x1D,0x0000);
   delay(10);
   registerWrite(0X14,0x4848);
   delay(10);
   registerWrite(0x0E,0x1820);
   delay(10);
   registerWrite(0x0A,0x1278);
   delay(10);
   registerWrite(0x00,0x211C);
   delay(10);

   //New config ends
    
   SPI.end(); 
   while(1);

}
