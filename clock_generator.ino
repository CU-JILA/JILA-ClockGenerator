
#include <SPI.h>
#include <stdint.h>

#define CSB     (10)
#define SDA     (16)
#define SCL     (17)

#define rst1    (11)
#define lockIn  (12)
#define lockOut (13)


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
     SPI.transfer(address);  //We are not masking, because the MSB will be zero and that satisfies the condition to write to a register
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
    delay(1);
    SPI.transfer(address | 0x80);
    data = SPI.transfer(0x00);
    data = (data << 8) | (SPI.transfer(0x00));
    delay(1);
    digitalWrite(CSB,HIGH);
}



void clkMulIn(void)
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
   //delay(10); 
     
   //New config begins
     
   //R124: Reset value is 0x0820  
   registerWrite(0x7D,0x2288);
   //delay(10);
     
   //R114: Reset value is 0x7800
   //FSK_MODE_SEL = FSK SPI  
   registerWrite(0x72,0x7802);
   //delay(10);
     
   //R78: Reset value is 0x0064  
   registerWrite(0x4E,0x0001);
   //delay(10);
     
   //R75: Reset value is 0x0800
   //Divide by 64 -> CHDIV  
   registerWrite(0x4B,0x0A40);
   //delay(10);
     
   //R71: Reset value is 0x0080  
   registerWrite(0x47,0x0081);
   //delay(10);
     
   //R58: Reset value is 0x8001
   //INPIN_LVL = Vin  
   registerWrite(0x3A,0x9001);
   //delay(10);
     
   //R57: Reset value is 0x0000 
   registerWrite(0x39,0x0020);
   //delay(10);
     
   //R52: Reset value is 0x0420  
   registerWrite(0x34,0x0421);
   //delay(10);
     
   //R45: Reset value is 0xC608
   //OUTB_PWR   
   registerWrite(0x2D,0xC61F);
   //delay(10);
     
   //R44: Reset value is 0x08A2  
   registerWrite(0x2C,0x1FA3);
   //delay(10);
     
   //R39: Reset value is 0xFFFF  
   registerWrite(0x27,0x03E8);
   //delay(10);
     
   //R38: Reset value is 0xFFFF  
   registerWrite(0x26,0x0000);
   //delay(10);
     
   //R37: Reset value is 0x0205  
   registerWrite(0x25,0x0305);
   //delay(10);
     
   //R36: Reset value is 0x0028  
   registerWrite(0x24,0x00A0);
   //delay(10);
     
   //R30: Reset value is 0x18C6  
   registerWrite(0x1E,0x18A6);
   //delay(10);
     
   //R29: Reset value is 0x18C6  
   registerWrite(0x1D,0x0000);
   //delay(10);
     
   //R20: Reset value is 0x3048  
   registerWrite(0X14,0x4848);
   //delay(10);
     
   //R14: Reset value is 0x1840  
   registerWrite(0x0E,0x1820);
   //delay(10);
     
   //R10: Reset value is 0x10F8  
   registerWrite(0x0A,0x1278);
   //delay(10);
     
   //R0: Reset value is 0x221C 
   registerWrite(0x00,0x211C);
   //delay(10);

   //New config ends
    
   SPI.end();
}

void setup()
{
    Serial.begin(9600);
    pinMode(CSB,OUTPUT);
    //pinMode(CE,OUTPUT);
    pinMode(SCL,OUTPUT);
    pinMode(SDA,OUTPUT);
}

void main() 
{
  // Initial clock setup
  clkMulIn();
  while(1){

      // Reads the reset and re-progrms the clock multiplier
      if( !digitalRead(rst1) ){
        clkMulIn();
      }

      // Reads the lock from the clock multiplier and outputs to lock Out
      if( digitalRead(lockIn) ){
        digitalWrite(lockOut,HIGH);
      }else
        digitalWrite(lockOut,LOW);
      
  }
  
  
}
