
#include <SPI.h>
#include <stdint.h>

#define CSB 10
#define CE  11
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


  //Read enable 
  registerWrite(0x00,0x2218);
  

  
    //R75: Channel divider, should be divided by 32
    //Reset value of R75 is 0x0800
    //So we need to write 0x09C0
    registerWrite(0x4B,0x09C0);
    registerRead(0x4B);

   //R44: MASH_ORDER should be 3, By default it's 2
   //Reset value is 0x08A2
   registerWrite(0x2C,0x08A3);

   //R34,R36: Divider value should be 80, By default 28
   //R34 is 3 bit MSB we don't need that, R36 is 16 bit so we should write 0x00A0 to get 80
   registerWrite(0x24,0x00A0);

   //R10: Multuplier should be 4x
   //Reset value of R10 is 0x10F8
   registerWrite(0x0A,0x1478);

    //R0: FCAL_HPFD_ADJ should be 2, be default it's 1
    //reset value is 0x221C
    registerWrite(0x00,0x211C); //modified from 2310c to 211c
       
   //R9: Doubler we want in X1, so it should be disabled. By default it's disabled

   //R12: PreR should be 1, By default it's 1

   //R11: PreR should be 1, By default it's 1

   //R14: CPG- Charge Pump Gain, We need 2500micro Amps. By default its 2500

   //R42,R43: Fraction numerator should be 0,By default iit's zero

   //R38,R39: Do not care, because numerator is 0

   //R44: OUTA_PWR should be 8, By default it's 8

   //R37: PFD_DLY_SEL should be 2, By default it's 2

   //R40,R41: MESH seed should be 0x0000, by defult it's 0

  //VCO calibration

  //R0: FCAL_LPFD_ADJ should be 0, by default it's 0

 //R1: CAL_CLK_DIV should be 0,  by default it's 0

  
   SPI.end(); 
   while(1);

}
