#include "KTMS1201.h"
#include <EEPROM.h>

const unsigned long max_frequency_step = 1000000; //Max Frequency step
const unsigned long max_frequency = 50000000; //Max Frequency
const int min_frequency=25; // Minimum Frequency

unsigned long last_frequency = 5000;
unsigned long frequency_step = 1;

// Rotary encoder

const int EncoderPinCLK = 2; 
const int EncoderPinDT = 3;  
const int EncoderPinSW = 4;  

// KTMS1201 Pin definitions
byte CD =    9;
byte RESET = 10;
byte CS =    11;
byte N_SCK = 12;
byte SI =    13;

//byte dds_RESET = 11;
//byte dds_DATA  = 10;
//byte dds_CLOCK = 12;
//byte dds_LOAD  = 13;

byte dds_RESET = 5;
byte dds_DATA  = 6;
byte dds_LOAD  = 7;
byte dds_CLOCK = 8;


KTMS1201 lcd(N_SCK, SI, CD, RESET, CS);

// Updated by the ISR (Interrupt Service Routine)
unsigned volatile long frequency = 5000;

void isr ()  {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  if (interruptTime - lastInterruptTime > 5) {
    if (digitalRead(EncoderPinDT) == LOW)
    {
      frequency=frequency-frequency_step ; // Could be -5 or -10
    }
    else {
      frequency=frequency+frequency_step ; // Could be +5 or +10
    }

    frequency = min(max_frequency, max(min_frequency, frequency));

    lastInterruptTime = interruptTime;
  }
}

void show_frequency()
{
  lcd.clear();
  float display_frequency=frequency;
  String frequency_string=String(frequency);
  if (frequency<1000)
  {
    lcd.setCursor(0);
    lcd.print(frequency);
  }
  
  if (frequency>=1000)
  {
    lcd.setCursor(0);
    lcd.print(display_frequency/1000,3);
  }
}

void setup() {
  Serial.begin(9600);
  
  // Rotary pulses are INPUTs
  pinMode(EncoderPinCLK, INPUT);
  pinMode(EncoderPinDT, INPUT);

  // Switch is floating so use the in-built PULLUP so we don't need a resistor
  pinMode(EncoderPinSW, INPUT_PULLUP);

  // Attach the routine to service the interrupts
  attachInterrupt(digitalPinToInterrupt(EncoderPinCLK), isr, LOW);
  lcd.begin();
  setup_dds();

  show_frequency();
  dds(frequency);
  Serial.println("Start");
}

void loop() {
  // Is someone pressing the rotary switch?
  if ((!digitalRead(EncoderPinSW))) {
    while (!digitalRead(EncoderPinSW))
      delay(10);
    Serial.println("Reset");
    if (frequency_step==max_frequency_step)
    {
      frequency_step=1;
    }
    else
    {
      frequency_step=frequency_step*10;  
    }
    Serial.print("multiplier:");
    Serial.println(frequency_step);
    lcd.clear();
    lcd.setCursor(0);
    lcd.print("STEP:");
    lcd.setCursor(5);
    lcd.print(frequency_step);
  }
 
  if (frequency != last_frequency) {
    Serial.print(frequency > last_frequency ? "Up  :" : "Down:");
    Serial.println(frequency);
    show_frequency();
    dds(frequency);
    last_frequency = frequency ;
  }
}
