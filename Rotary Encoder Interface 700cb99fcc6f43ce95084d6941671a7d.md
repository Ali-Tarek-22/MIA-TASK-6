# Rotary Encoder Interface


In this code we are interfacing with an encoder and calculating the rotation of the encoder.

## Setup function

In the setup, we will connect `PIN_ENCODER_A` to Arduino digital pin 2 and `PIN_ENCODER_B` to pin 3. We will also use the internal pull-up resistors of the Arduino to ensure that the signal pins are always in a defined state when the encoder is not being rotated.

```arduino
// Rotary Encoder Inputs
#define PIN_ENCODER_A 2
#define PIN_ENCODER_B 3

// Counter variable (four per resolution)
int counter = 0;

void setup() {
	// Set encoder pins as inputs with pull-up resistors
	pinMode(PIN_ENCODER_A, INPUT_PULLUP);
	pinMode(PIN_ENCODER_B, INPUT_PULLUP);

	// Setup Serial Monitor
	Serial.begin(9600);

	// Define functions as ISR
	attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), ISR_A, CHANGE);
	attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), ISR_B, CHANGE);
}

```

## Interrupt Service Routines (ISRs)

The `ISR_A` and `ISR_B` functions are attached to the A and B pins of the rotary encoder, respectively. These functions are triggered whenever a change in the state of the corresponding pin is detected. Within the interrupt function, we update the `counter` variable based on the direction of rotation.

The code below shows the Interrupt Service Routines (ISRs) for the rotary encoder interface. These functions are called by the Arduino whenever there is a change in the state of the pins connected to the rotary encoder.

```arduino
// Pin_A ISR
void ISR_A(void) {
	if (digitalRead(PIN_ENCODER_A) != digitalRead(PIN_ENCODER_B)) {
		counter++;
		Serial.println("Direction: CCW | ");
		Serial.println(counter);
	} else {
		counter--;
		Serial.println("Direction: CW | ");
		Serial.println(counter);
	}
}

// Pin_B ISR
void ISR_B(void) {
	if (digitalRead(PIN_ENCODER_B) == digitalRead(PIN_ENCODER_A)) {
		counter++;
		Serial.println("Direction: CCW | ");
		Serial.println(counter);
	} else {
		counter--;
		Serial.println("Direction: CW | ");
		Serial.println(counter);
	}
}

```

The `ISR_A` function is triggered whenever there is a change in the state of the `PIN_ENCODER_A` pin. Within this function, the code checks whether `PIN_ENCODER_A` is different from `PIN_ENCODER_B`. If they are different, it means that the encoder is being rotated counterclockwise (CCW), so the `counter` variable is incremented. The direction of rotation and the current count of the `counter` variable are then printed to the Serial Monitor. If `PIN_ENCODER_A` is the same as `PIN_ENCODER_B`, it means that the encoder is being rotated clockwise (CW), so the `counter` variable is decremented. Again, the direction of rotation and the current count of the `counter` variable are printed to the Serial Monitor.

The same logic applies to the `ISR_B` function, which is triggered whenever there is a change in the state of the `PIN_ENCODER_B` pin. Within this function, the code checks whether `PIN_ENCODER_B` is the same as `PIN_ENCODER_A`. If they are the same, it means that the encoder is being rotated counterclockwise (CCW), so the `counter` variable is incremented. Again, the direction of rotation and the current count of the `counter` variable are printed to the Serial Monitor. If `PIN_ENCODER_B` is different from `PIN_ENCODER_A`, it means that the encoder is being rotated clockwise (CW), so the `counter` variable is decremented. The direction of rotation and the current count of the `counter` variable are printed to the Serial Monitor.

These ISR functions continuously update the `counter` variable with the direction and speed of rotation of the rotary encoder.

## The Loop Function

The `loop()` function is empty, as we don't need to perform any other actions. The `delay(1)` statement simply adds a small delay of 1 millisecond between each loop iteration to avoid overloading the microcontroller with too many interrupts and to ensure that the code runs smoothly.

```arduino
void loop() {
	delay(1);
}

```

##