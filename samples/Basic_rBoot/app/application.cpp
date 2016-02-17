
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include <Libraries/Alex/Alex.h>


#define UP_PIN 14 // D5   // GPIO0
#define DOWN_PIN 12 // D6
#define PWM_PIN 15

#include <HardwarePWM.h>

uint8_t pins[1] = { PWM_PIN }; // List of pins that you want to connect to pwm  4erd
HardwarePWM HW_pwm(pins, 1);

static int speed;

void changeSpeed(bool faster) {
		
	faster ? speed++ : speed--;
	speed = max(0, speed);
	speed = min(22, speed);
	Serial.printf("New speed on pin %i: %i.\r\n", PWM_PIN, speed);
	HW_pwm.analogWrite(PWM_PIN, speed * 1000);
}
void IRAM_ATTR upHandler() {  changeSpeed(true); }
void IRAM_ATTR downHandler() { changeSpeed(false); }

void init(){

	alex_init();
	attachInterrupt(UP_PIN, upHandler, CHANGE);
	attachInterrupt(DOWN_PIN, downHandler, CHANGE);
}
