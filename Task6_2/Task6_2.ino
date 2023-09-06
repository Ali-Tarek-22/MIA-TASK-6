// Rotary Encoder Inputs
#define PIN_ENCODER_A 2
#define PIN_ENCODER_B 3

//Counter variable (four per resolution)
int counter = 0;

void setup() {
        
	//Set encoder pins as inputs
	pinMode(PIN_ENCODER_A,INPUT_PULLUP);
	pinMode(PIN_ENCODER_B,INPUT_PULLUP);

	// Setup Serial Monitor
	Serial.begin(9600);

  //Define functions as ISR
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), ISR_A,CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), ISR_B,CHANGE);
}

//Pin_A ISR
void ISR_A(void)
{
  if(digitalRead(PIN_ENCODER_A) != digitalRead(PIN_ENCODER_B))
    {
      counter++;
      Serial.println("Direction: CCW | ");
      Serial.println(counter);
    }
  else
    {
      counter--;
      Serial.println("Direction: CW | ");
      Serial.println(counter);
    }
}

//PIN_B ISR
void ISR_B(void)
{
  if(digitalRead(PIN_ENCODER_B) == digitalRead(PIN_ENCODER_A))
    {
      counter++;
      Serial.println("Direction: CCW | ");
      Serial.println(counter);
    }
  else
    {
      counter--;
      Serial.println("Direction: CW | ");
      Serial.println(counter);
    }
}


void loop() 
{
	delay(1);
}
