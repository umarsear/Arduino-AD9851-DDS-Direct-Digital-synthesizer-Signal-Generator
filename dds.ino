// Acknowledgement
// Most, if not all of the code in this file has been taken from examples I have seen online, I could not find the right information 
// to add a link or name to give proper credit to the original author. 

void setup_dds()
{
  // DDS pins for data, clock and load
  pinMode (dds_DATA, OUTPUT);   // DDS pins as output
  pinMode (dds_CLOCK, OUTPUT);  
  pinMode (dds_LOAD, OUTPUT);   
  pinMode (dds_RESET, OUTPUT);

  digitalWrite(dds_DATA, LOW);  // internal pull-down
  digitalWrite(dds_CLOCK, LOW);
  digitalWrite(dds_LOAD, LOW);
  digitalWrite(dds_RESET, LOW);
  
// Wait 2 seconds for the AD9851 during power up
  delay (2000);
  init_dds();
  reset_dds();
}

void init_dds()
{
  digitalWrite(dds_RESET, LOW);
  digitalWrite(dds_CLOCK, LOW);
  digitalWrite(dds_LOAD, LOW);
  digitalWrite(dds_DATA, LOW);
}

void reset_dds()
{
  //reset sequence is:
  // CLOCK & LOAD = LOW
  //  Pulse RESET high for a few uS (use 5 uS here)
  //  Pulse CLOCK high for a few uS (use 5 uS here)
  //  Set DATA to ZERO and pulse LOAD for a few uS (use 5 uS here)
  
  // data sheet diagrams show only RESET and CLOCK being used to reset the device, but I see no output unless I also
  // toggle the LOAD line here.
  
    digitalWrite(dds_CLOCK, LOW);
    digitalWrite(dds_LOAD, LOW);
    
     digitalWrite(dds_RESET, LOW);
     delay(5);
     digitalWrite(dds_RESET, HIGH);  //pulse RESET
     delay(5);
     digitalWrite(dds_RESET, LOW);
     delay(5);
     
     digitalWrite(dds_CLOCK, LOW);
     delay(5);
     digitalWrite(dds_CLOCK, HIGH);  //pulse CLOCK
     delay(5);
     digitalWrite(dds_CLOCK, LOW);
     delay(5);
     digitalWrite(dds_DATA, LOW);    //make sure DATA pin is LOW
     
     digitalWrite(dds_LOAD, LOW);
     delay(5);
     digitalWrite(dds_LOAD, HIGH);  //pulse LOAD
     delay(5);
     digitalWrite(dds_LOAD, LOW);
  // Chip is RESET now
}

// dds instruction write - takes unsigned long frequency in Hz
void dds(unsigned long freq)
{
  int last8;
  unsigned long DDSLong;
  unsigned long Bitmask32 = 1; // 32 bit bit mask '0000 0000 0000 0000 0000 0000 0000 0001'
  byte Bitmask8 = 1;           // 8 bit bit mask '0000 0001'
                             // we shift these bitmasks left 1 bit at a time and AND them bitwise with a value to simply
                             // determine if an unknown individual bit within a data structure is a 1 or a 0
  byte FirstBit = 1;

  float clock_frequency = 180000000;  // this is the on-board clock frequency of my AD9851 board
  float twoE32 = pow (2,32);          // this is 2 to the power of 32 (which is quite a lot)

  DDSLong = ((twoE32 * (freq))/ clock_frequency); // this calculates the first 32 bits of the 40 bit DDS instruction
  
  // now we itterate through the first 32 bits one at a time, determine if the individual bits are 1 or 0 and write a HIGH or LOW as appropriate

  for (Bitmask32 = 1; Bitmask32 > 0; Bitmask32 <<= 1) 
  {                                             // iterate through 32 bits of DDSLong
    if (DDSLong & Bitmask32)                    // if bitwise AND resolves to true
      digitalWrite(dds_DATA,HIGH);
    else                                        // if bitwise AND resolves to false
      digitalWrite(dds_DATA,LOW);
    
    // after every single bit we toggle the clock pin for the DDS to receive the data bit
    
    digitalWrite(dds_CLOCK,HIGH);                   // Clock data in by setting clock pin high then low
    delayMicroseconds(1);
    digitalWrite(dds_CLOCK,LOW);

  }

  // now send the final 8 bits to complete the 40 bit instruction
  // the AD9851 datasheet explains this well, but we need 1000 0000 because we want
  // to use the clock multiplier
  // so here we are going to look 8 times and send a 1 the first time round then 7 0s
  // 10 out of 10 for niftyness but 0 out of 10 for readability:

  for (Bitmask8 = 1; Bitmask8 > 0; Bitmask8 <<= 1) 
  {                                             // iterate through last 8 bits of 40 bit instruction to DDS
    if (Bitmask8 & FirstBit)                    // 1st bit of remaining 8 needs to be 1 to enable clock multiplier
      digitalWrite(dds_DATA,HIGH); 
    else
      digitalWrite(dds_DATA,LOW);  

    // after every single bit we toggle the clock pin for the DDS to receive the data bit

    digitalWrite(dds_CLOCK,HIGH);                   // Clock data in by setting clock pin high then low
    delayMicroseconds(1);
    digitalWrite(dds_CLOCK,LOW);
  } 
    
  // and once all 40 bits have been sent
  // finally we toggle the load bit to say we are done 
  // and let the AD9851 do its stuff
    
  digitalWrite (dds_LOAD, HIGH);                    // Pulse DDS update 
  delayMicroseconds(1);
  digitalWrite (dds_LOAD, LOW);                     // to execute previous instruction set
  
  return;
}
