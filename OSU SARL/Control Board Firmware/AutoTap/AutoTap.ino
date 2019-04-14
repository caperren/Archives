////////// Includes //////////
#include <LiquidCrystal.h>
#include <EEPROM.h>

////////// Pin Definitions //////////
// LCD
#define LCD_RS_PIN 12
#define LCD_EN_PIN 11
#define LCD_D4_PIN 10
#define LCD_D5_PIN 9
#define LCD_D6_PIN 8
#define LCD_D7_PIN 7
#define LCD_VO_PIN 6

// Front Panel Buttons
#define STOP_BUTTON_PIN 5
#define START_BUTTON_PIN 4

// Rotary Encoder
#define CHANNEL_A_PIN 3
#define CHANNEL_B_PIN 2
#define ENCODER_BUTTON_PIN A0

// Relay
#define LATCH_SIDE_1_PIN A4 // This is the side that connects the relay
#define LATCH_SIDE_2_PIN A5 // This one disconnects it

////////// Class Instantiations /////////
LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

////////// Global Variables //////////
// #define DEBUG
#define PROG_VERSION 1.0

// Timeouts and timing variables
#define DEBOUNCE_TIMEOUT 350

unsigned int start_time = 0;
unsigned int prev_time = 0;

// Encoder Variables
#define ENCODER_SCALE_VAL 8
volatile  int encoder0Pos = 0;
volatile  int encoder0PosScaled = 0;
int prev_encoder_scaled_pos = 0;

// System State Variables and Enums
enum system_states {
	system_init,
	wait_for_start,
	settings_configuration,
	run_tap_program
};

system_states current_system_state = wait_for_start;
system_states previous_system_state = system_init;

// Tap Test Variables and Enums
enum tap_test_states {
	settle_init,
	settle_period,
	normal_run
};

tap_test_states current_tap_state = settle_period;
tap_test_states previous_tap_state = settle_init;

int settle_seconds = 180;
int tap_period_seconds = 30;
unsigned int cycle_length_seconds = 3600;

char has_tapped = 0;

// Settings Configuration Variables and Enums
enum setting_scroller_states{
	settings_init_pg,
	settle_duration_pg,
	tap_period_pg,
	test_duration_pg
};

setting_scroller_states current_editing_state = settle_duration_pg;
setting_scroller_states previous_editing_state = settings_init_pg;

void setup() {
	// Sets the relay to not be engaging the relay
	pinMode(LATCH_SIDE_1_PIN, OUTPUT);
	digitalWrite(LATCH_SIDE_1_PIN, LOW);
	pinMode(LATCH_SIDE_2_PIN, OUTPUT);
	digitalWrite(LATCH_SIDE_2_PIN, HIGH);
	delay(50);
	digitalWrite(LATCH_SIDE_2_PIN, LOW);

	// Sets up front panel buttons
	pinMode(STOP_BUTTON_PIN, INPUT);
	digitalWrite(STOP_BUTTON_PIN, HIGH);

	pinMode(START_BUTTON_PIN, INPUT);
	digitalWrite(START_BUTTON_PIN, HIGH);

	// Sets up the rotary encoder
	pinMode(CHANNEL_A_PIN, INPUT);
	digitalWrite(CHANNEL_A_PIN, HIGH);

	pinMode(CHANNEL_B_PIN, INPUT);
	digitalWrite(CHANNEL_B_PIN, HIGH);

	pinMode(ENCODER_BUTTON_PIN, INPUT);
	digitalWrite(ENCODER_BUTTON_PIN, HIGH);

	attachInterrupt(0, doEncoderA, CHANGE);

	attachInterrupt(1, doEncoderB, CHANGE);

	// Sets up serial debug if needed
#ifdef DEBUG
	Serial.begin(9600);
	Serial.print("------    Auto Tap V");  Serial.print(PROG_VERSION);  Serial.println("     ------");
	Serial.println("------   www.caperren.com    ------");
	Serial.println("------CURRENTLY IN DEBUG MODE------");
#endif

	// Sets lcd contrast
	pinMode(LCD_VO_PIN, OUTPUT);
	analogWrite(LCD_VO_PIN, 128);

	// Sets up and displays intro message on lcd
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print(" Auto Tap V");
	lcd.print(PROG_VERSION);
	lcd.setCursor(0, 1);
	lcd.print("www.caperren.com");

	delay(3000);
	
	// Imports settings from EEPROM
	if(!digitalRead(STOP_BUTTON_PIN)){
		lcd.setCursor(0, 0);
		lcd.print("   Restoring    ");
		lcd.setCursor(0, 1);
		lcd.print("    Settings    ");
		
		EEPROMWritelong(0, settle_seconds);
		EEPROMWritelong(4, tap_period_seconds);
		EEPROMWritelong(8, cycle_length_seconds);
		
		delay(3000);
	}
	
	settle_seconds = int(EEPROMReadlong(0));
	tap_period_seconds = int(EEPROMReadlong(4));
	cycle_length_seconds = int(EEPROMReadlong(8));
	
	

}

void loop() {
	if (current_system_state == wait_for_start) {
		if (previous_system_state != wait_for_start) {
			lcd.setCursor(0, 0);
			lcd.print("    Auto Tap    ");
			lcd.setCursor(0, 1);
			lcd.print("STOP|V");
			lcd.print(PROG_VERSION);
			lcd.print("|START");
			previous_system_state = wait_for_start;
		}

		if (!digitalRead(ENCODER_BUTTON_PIN)) {
			delay(DEBOUNCE_TIMEOUT);
			current_system_state = settings_configuration;
		} else if (!digitalRead(START_BUTTON_PIN)) {
			delay(DEBOUNCE_TIMEOUT);
			current_system_state = run_tap_program;
		}

	} else if (current_system_state == settings_configuration) {
		if (previous_system_state != settings_configuration) {
			prev_encoder_scaled_pos = encoder0PosScaled;
			current_editing_state = settle_duration_pg;
			previous_system_state = settings_configuration;
		}
		
		if (current_editing_state == settle_duration_pg){
			lcd.setCursor(0, 0);
			lcd.print("  Edit Setting  ");
			lcd.setCursor(0, 1);
			lcd.print("Settle Duration ");
			
			if(!digitalRead(ENCODER_BUTTON_PIN)){
				delay(DEBOUNCE_TIMEOUT);
				lcd.setCursor(0, 0);
				lcd.print("Settle Duration ");
				prev_encoder_scaled_pos = encoder0PosScaled;
				while(digitalRead(ENCODER_BUTTON_PIN)){
					lcd.setCursor(0, 1);
					lcd.print(settle_seconds);
					lcd.print("               ");
					
					if(encoder0PosScaled != prev_encoder_scaled_pos){
						settle_seconds += (encoder0PosScaled - prev_encoder_scaled_pos);
						settle_seconds = constrain(settle_seconds, 0 , 32767);
						prev_encoder_scaled_pos = encoder0PosScaled;
					}
				}
				delay(DEBOUNCE_TIMEOUT);
				prev_encoder_scaled_pos = encoder0PosScaled = 0;
			}
			
			check_if_page_should_change();
			
		}else if (current_editing_state == tap_period_pg){
			lcd.setCursor(0, 0);
			lcd.print("  Edit Setting  ");
			lcd.setCursor(0, 1);
			lcd.print("   Tap Period   ");
			
			if(!digitalRead(ENCODER_BUTTON_PIN)){
				delay(DEBOUNCE_TIMEOUT);
				lcd.setCursor(0, 0);
				lcd.print("   Tap Period   ");
				prev_encoder_scaled_pos = encoder0PosScaled;
				while(digitalRead(ENCODER_BUTTON_PIN)){
					lcd.setCursor(0, 1);
					lcd.print(tap_period_seconds);
					lcd.print("               ");
					
					if(encoder0PosScaled != prev_encoder_scaled_pos){
						tap_period_seconds += (encoder0PosScaled - prev_encoder_scaled_pos);
						tap_period_seconds = constrain(tap_period_seconds, 1 , cycle_length_seconds);
						prev_encoder_scaled_pos = encoder0PosScaled;
					}
				}
				delay(DEBOUNCE_TIMEOUT);
				prev_encoder_scaled_pos = encoder0PosScaled = 0;
			}
			
			check_if_page_should_change();
		}else if (current_editing_state == test_duration_pg){
			lcd.setCursor(0, 0);
			lcd.print("  Edit Setting  ");
			lcd.setCursor(0, 1);
			lcd.print(" Test Duration  ");
			
			if(!digitalRead(ENCODER_BUTTON_PIN)){
				delay(DEBOUNCE_TIMEOUT);
				lcd.setCursor(0, 0);
				lcd.print(" Test Duration  ");
				prev_encoder_scaled_pos = encoder0PosScaled;
				while(digitalRead(ENCODER_BUTTON_PIN)){
					lcd.setCursor(0, 1);
					lcd.print(cycle_length_seconds);
					lcd.print("               ");
					
					if(encoder0PosScaled != prev_encoder_scaled_pos){
						cycle_length_seconds += (encoder0PosScaled - prev_encoder_scaled_pos);
						cycle_length_seconds = constrain(cycle_length_seconds, 0 , 32767);
						prev_encoder_scaled_pos = encoder0PosScaled;
					}
				}
				delay(DEBOUNCE_TIMEOUT);
				prev_encoder_scaled_pos = encoder0PosScaled = 0;
			}
			
			check_if_page_should_change();
		}
		
		if(!digitalRead(START_BUTTON_PIN)){
			lcd.setCursor(0, 0);
			lcd.print("Saving Settings ");
			lcd.setCursor(0, 1);
			lcd.print("  Please Wait   ");
			delay(2000);
			
			EEPROMWritelong(0, settle_seconds);
			EEPROMWritelong(4, tap_period_seconds);
			EEPROMWritelong(8, cycle_length_seconds);
			
			current_system_state = wait_for_start;
		} else if(!digitalRead(STOP_BUTTON_PIN)){
			current_system_state = wait_for_start;
		}

	} else if (current_system_state == run_tap_program) {
		if (previous_system_state != run_tap_program) {
			lcd.setCursor(0, 0);
			lcd.print(" Start Pressed  ");
			lcd.setCursor(0, 1);
			lcd.print(" Beginning Test ");
			delay(2000);

			current_tap_state = settle_period;
			previous_tap_state = settle_init;
			previous_system_state = run_tap_program;
		}

		if (current_tap_state == settle_period) {
			if (previous_tap_state == settle_init) {
				lcd.setCursor(0, 0);
				lcd.print(" Settle Remain  ");
				start_time = get_system_seconds();
				previous_tap_state = settle_period;
			}
			lcd.setCursor(0, 1);
			lcd.print("       ");
			lcd.print(settle_seconds - (get_system_seconds() - start_time));
			lcd.print("               ");

			if ((get_system_seconds() - start_time) > settle_seconds) {
				current_tap_state = normal_run;
			} else if (!digitalRead(STOP_BUTTON_PIN)) {
				delay(DEBOUNCE_TIMEOUT);
				current_tap_state = normal_run;
			}

		} else if (current_tap_state == normal_run) {
			if (previous_tap_state == settle_period) {
				lcd.setCursor(0, 0);
				lcd.clear();
				lcd.print("Cycle  |Tap     ");
				start_time = get_system_seconds();
				has_tapped = 0;
				previous_tap_state = normal_run;
			}

			int cycle_left = cycle_length_seconds - (get_system_seconds() - start_time);
			int tap_left = tap_period_seconds - ((get_system_seconds() - start_time) % tap_period_seconds);

			lcd.setCursor(0, 1);
			lcd.print(cycle_left);
			print_spaces_for_number(7 - get_digits_in_number(cycle_left));
			lcd.setCursor(7 , 1);
			lcd.print("|");
			lcd.print(tap_left - 1);
			lcd.print("       ");



			if ((tap_left  == 1) && !has_tapped) {
				do_tap();
				has_tapped = 1;
			} else if (tap_left  == 5) {
				has_tapped = 0;
			}

			if ((get_system_seconds() - start_time) > cycle_length_seconds) {
				lcd.setCursor(0, 0);
				lcd.print(" Test Finished! ");
				lcd.setCursor(0, 1);
				lcd.print("Duration: ");
				lcd.print(cycle_length_seconds);
				delay(2000);
				current_system_state = wait_for_start;
			} else if (!digitalRead(STOP_BUTTON_PIN)) {
				delay(DEBOUNCE_TIMEOUT);
				current_system_state = wait_for_start;
			}
		}
	}

}

void check_if_page_should_change(){
	int rot_amount = encoder0PosScaled - prev_encoder_scaled_pos;
	
	// #ifdef DEBUG
	// Serial.print("Rotation Amount: ");
	// Serial.println(rot_amount);
	// #endif
	
	if(abs(rot_amount) > 2){
		if(rot_amount < 0){
			
			if(current_editing_state == settle_duration_pg){
			}else if(current_editing_state == tap_period_pg){
				current_editing_state = settle_duration_pg;
			}else if(current_editing_state == test_duration_pg){
				current_editing_state = tap_period_pg;
			}
			
		}else if(rot_amount > 0){
			
			if(current_editing_state == settle_duration_pg){
				current_editing_state = tap_period_pg;
			}else if(current_editing_state == tap_period_pg){
				current_editing_state = test_duration_pg;
			}else if(current_editing_state == test_duration_pg){
			}
		}
		prev_encoder_scaled_pos = encoder0PosScaled;
	}
	
}

void print_spaces_for_number(int number) {
	for (int i = 0 ; i < number ; i++) {
		lcd.print(" ");
	}
}

int get_digits_in_number(unsigned int number) {
	return number > 0 ? (int) log10 ((double) number) + 1 : 1;
}

unsigned long get_system_seconds() {
	return millis() / 1000;
}

void doEncoderA() {

	// look for a low-to-high on channel A
	if (digitalRead(CHANNEL_A_PIN) == LOW) {
		// check channel B to see which way encoder is turning
		if (digitalRead(CHANNEL_B_PIN) == HIGH) {
			encoder0Pos = encoder0Pos + 1;         // CW
		}
		else {
			encoder0Pos = encoder0Pos - 1;         // CCW
		}
	}
	else   // must be a high-to-low edge on channel A
	{
		// check channel B to see which way encoder is turning
		if (digitalRead(CHANNEL_B_PIN) == LOW) {
			encoder0Pos = encoder0Pos + 1;          // CW
		}
		else {
			encoder0Pos = encoder0Pos - 1;          // CCW
		}
	}

	encoder0PosScaled = encoder0Pos / ENCODER_SCALE_VAL;
}

void doEncoderB() {

	// look for a low-to-high on channel B
	if (digitalRead(CHANNEL_B_PIN) == LOW) {
		// check channel A to see which way encoder is turning
		if (digitalRead(CHANNEL_A_PIN) == LOW) {
			encoder0Pos = encoder0Pos + 1;         // CW
		}
		else {
			encoder0Pos = encoder0Pos - 1;         // CCW
		}
	}
	// Look for a high-to-low on channel B
	else {
		// check channel B to see which way encoder is turning
		if (digitalRead(CHANNEL_A_PIN) == HIGH) {
			encoder0Pos = encoder0Pos + 1;          // CW
		}
		else {
			encoder0Pos = encoder0Pos - 1;          // CCW
		}
	}
	encoder0PosScaled = encoder0Pos / ENCODER_SCALE_VAL;
}

void EEPROMWritelong(int address, long value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
      }

long EEPROMReadlong(long address)
      {
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
      }

void do_tap() {
	digitalWrite(LATCH_SIDE_1_PIN, HIGH);
	delay(50);
	digitalWrite(LATCH_SIDE_1_PIN, LOW);

	delay(150);

	digitalWrite(LATCH_SIDE_2_PIN, HIGH);
	delay(50);
	digitalWrite(LATCH_SIDE_2_PIN, LOW);
}
