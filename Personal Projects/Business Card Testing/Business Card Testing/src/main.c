/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
//#define _CORTEX_SC 0	//Just for getting rid of a compile error for something that doesn't matter. See if doing this breaks stuff once usb is working

//Pin Definitions
#define BAT_EN1_PIN PIN_PB02
#define BAT_EN2_PIN PIN_PB03
#define BAT_CHRG_PIN PIN_PB00

#define LED_R_PIN PIN_PA08
#define LED_G_PIN PIN_PA09
#define LED_B_PIN PIN_PA10

#define USER_S1_PIN PIN_PB04
#define USER_S2_PIN PIN_PB05
#define USER_S3_PIN PIN_PB06
#define USER_S4_PIN PIN_PB07

#include <asf.h>
#include "EPD_Graphics_Library/epd_graphics_library.h"
#include "config/conf_EPD.h"

//Global variables
struct usart_module usart_instance;

FRESULT res;
FATFS FatFs_obj;
FIL file_obj;
uint32_t total_space, free_space;

//Function prototypes
void epd_system_init(void);
//void configure_usart(void);
void user_io_test(void);
void epd_display_test(void);
//void sd_card_test(void);
//void serial_test(void);

int main (void)
{
	epd_system_init();
	epd_display_test();
}

void epd_system_init(void){
// 	struct port_config config_port_pin;
// 	
// 	
// 	//Battery Control Pin Setup
// 	port_pin_set_config(BAT_EN1_PIN, &config_port_pin);
// 	port_pin_set_config(BAT_EN2_PIN, &config_port_pin);
// 	
// 	port_pin_set_output_level(BAT_EN1_PIN, 0);
// 	port_pin_set_output_level(BAT_EN2_PIN, 0);	
// 	
// 	//LED Pins Setup
// 	port_get_config_defaults(&config_port_pin);
// 	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
// 	
// 	port_pin_set_config(LED_R_PIN, &config_port_pin);
// 	port_pin_set_config(LED_G_PIN, &config_port_pin);
// 	port_pin_set_config(LED_B_PIN, &config_port_pin);
// 	
// 	port_pin_set_output_level(LED_R_PIN, 0);
// 	port_pin_set_output_level(LED_G_PIN, 0);
// 	port_pin_set_output_level(LED_B_PIN, 0);
	
// 	
// 	port_pin_set_config(SPI_CLK_PIN, &config_port_pin);
// 	port_pin_set_config(EPD_BUSY_PIN, &config_port_pin);
// 	port_pin_set_config(PWM_PIN, &config_port_pin);
// 	port_pin_set_config(EPD_RST_PIN, &config_port_pin);
// 	port_pin_set_config(EPD_PANELON_PIN, &config_port_pin);
// 	port_pin_set_config(EPD_DISCHARGE_PIN, &config_port_pin);
// 	port_pin_set_config(EPD_BORDER_PIN, &config_port_pin);
// 	port_pin_set_config(SPI_MISO_PIN, &config_port_pin);
// 	port_pin_set_config(SPI_MOSI_PIN, &config_port_pin);
// 	port_pin_set_config(EPD_CS_PIN, &config_port_pin);
// 	
// 	port_pin_set_output_level(SPI_CLK_PIN, 1);
// 	port_pin_set_output_level(EPD_BUSY_PIN, 0);
// 	port_pin_set_output_level(PWM_PIN, 0);
// 	port_pin_set_output_level(EPD_RST_PIN, 1);
// 	port_pin_set_output_level(EPD_PANELON_PIN, 0);
// 	port_pin_set_output_level(EPD_DISCHARGE_PIN, 0);
// 	port_pin_set_output_level(EPD_BORDER_PIN, 0);
// 	port_pin_set_output_level(SPI_MISO_PIN, 0);
// 	port_pin_set_output_level(SPI_MOSI_PIN, 0);
// 	port_pin_set_output_level(EPD_CS_PIN, 1);
// 	
// 	//User Switch Pin Setup
// 	port_get_config_defaults(&config_port_pin);
// 	config_port_pin.direction = PORT_PIN_DIR_INPUT;
// 	
// 	port_pin_set_config(BAT_CHRG_PIN, &config_port_pin);
// 	
// 	config_port_pin.input_pull = PORT_PIN_PULL_UP;
// 		
// 	port_pin_set_config(USER_S1_PIN, &config_port_pin);
// 	port_pin_set_config(USER_S2_PIN, &config_port_pin);
// 	port_pin_set_config(USER_S3_PIN, &config_port_pin);
// 	port_pin_set_config(USER_S4_PIN, &config_port_pin);
// 	
// 	
// 	
// 	port_pin_set_output_level(LED_R_PIN, 0);
// 	port_pin_set_output_level(LED_G_PIN, 0);
// 	port_pin_set_output_level(LED_B_PIN, 0);
// 	
// 	for(uint32_t i = 0 ; i < 5*10000 ; i++){
// 		
// 	}
// 	port_pin_set_output_level(LED_R_PIN, 1);
// 	port_pin_set_output_level(LED_G_PIN, 1);
// 	port_pin_set_output_level(LED_B_PIN, 1);
// 	
	
		
	system_init();
	delay_init();
	//configure_usart();
	irq_initialize_vectors();
	cpu_irq_enable();
	delay_ms(500);
	
	/* Initialize EPD hardware */
	EPD_display_init();
	system_interrupt_enable_global();
	delay_ms(2000);
}

//void configure_usart(void)
//{
	//struct usart_config config_usart;
	//usart_get_config_defaults(&config_usart);
	//config_usart.baudrate    = 9600;
	//config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	//config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	//config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	//while (usart_init(&usart_instance,
	//EDBG_CDC_MODULE, &config_usart) != STATUS_OK) {
	//}
	//usart_enable(&usart_instance);
//}

void user_io_test(void){
	struct port_config config_port_pin;
	
	//LED Pins Setup
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	
	port_pin_set_config(LED_R_PIN, &config_port_pin);
	port_pin_set_config(LED_G_PIN, &config_port_pin);
	port_pin_set_config(LED_B_PIN, &config_port_pin);
	
	port_pin_set_output_level(LED_R_PIN, 1);
	port_pin_set_output_level(LED_G_PIN, 1);
	port_pin_set_output_level(LED_B_PIN, 1);
	
	
	//Battery Control Pin Setup
	port_pin_set_config(BAT_EN1_PIN, &config_port_pin);
	port_pin_set_config(BAT_EN2_PIN, &config_port_pin);
	
	port_pin_set_output_level(BAT_EN1_PIN, 1);
	port_pin_set_output_level(BAT_EN2_PIN, 1);
	
	
	//User Switch Pin Setup
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_UP;
	
	port_pin_set_config(USER_S1_PIN, &config_port_pin);
	port_pin_set_config(USER_S2_PIN, &config_port_pin);
	port_pin_set_config(USER_S3_PIN, &config_port_pin);
	port_pin_set_config(USER_S4_PIN, &config_port_pin);

	//Test LEDs
	for(int i = 0 ; i < 5 ; i++){
		port_pin_set_output_level(LED_R_PIN, 0);
		delay_ms(750);
		port_pin_set_output_level(LED_R_PIN, 1);
		delay_ms(1500);
			
		port_pin_set_output_level(LED_G_PIN, 0);
		delay_ms(750);
		port_pin_set_output_level(LED_G_PIN, 1);
		delay_ms(1500);
			
		port_pin_set_output_level(LED_B_PIN, 0);
		delay_ms(750);
		port_pin_set_output_level(LED_B_PIN, 1);
	}
	
	//Test Buttons
	char button_test_flag = true;
	while(button_test_flag){
		char s1 = !port_pin_get_input_level(USER_S1_PIN);
		char s2 = !port_pin_get_input_level(USER_S2_PIN);
		char s3 = !port_pin_get_input_level(USER_S3_PIN);
		char s4 = !port_pin_get_input_level(USER_S4_PIN);
		
		if (s1 && s4){
			button_test_flag = false;
		}else if(s1){
			port_pin_set_output_level(LED_R_PIN, 0);
			port_pin_set_output_level(LED_G_PIN, 1);
			port_pin_set_output_level(LED_B_PIN, 1);
		}else if(s2){
			port_pin_set_output_level(LED_R_PIN, 1);
			port_pin_set_output_level(LED_G_PIN, 0);
			port_pin_set_output_level(LED_B_PIN, 1);
		}else if(s3){
			port_pin_set_output_level(LED_R_PIN, 1);
			port_pin_set_output_level(LED_G_PIN, 1);
			port_pin_set_output_level(LED_B_PIN, 0);
		}else if(s4){
			port_pin_set_output_level(LED_R_PIN, 0);
			port_pin_set_output_level(LED_G_PIN, 0);
			port_pin_set_output_level(LED_B_PIN, 0);
		}
	}
	
	//Disable LEDs from test
	port_pin_set_output_level(LED_R_PIN, 1);
	port_pin_set_output_level(LED_G_PIN, 1);
	port_pin_set_output_level(LED_B_PIN, 1);
	
	char standby = 0;
	char slow_charge = 1;
	
	char test_mode = standby;
	
	//Charging Test
	while(1){
		char s1 = !port_pin_get_input_level(USER_S1_PIN);
		char s2 = !port_pin_get_input_level(USER_S2_PIN);
		
		if(s1){
			test_mode = standby;
		}else if(s2){
			test_mode = slow_charge;
		}
		
		if(test_mode == standby){
			port_pin_set_output_level(BAT_EN1_PIN, 1);
			port_pin_set_output_level(BAT_EN2_PIN, 1);
			
			port_pin_set_output_level(LED_R_PIN, 0);
			port_pin_set_output_level(LED_G_PIN, 1);
			port_pin_set_output_level(LED_B_PIN, 1);
		}else if(test_mode == slow_charge){
			port_pin_set_output_level(BAT_EN1_PIN, 0);
			port_pin_set_output_level(BAT_EN2_PIN, 0);			
			
			port_pin_set_output_level(LED_R_PIN, 1);
			port_pin_set_output_level(LED_G_PIN, 1);
			port_pin_set_output_level(LED_B_PIN, 0);
		}
	}
}

void epd_display_test(void){
	set_framebuffers(image_array_270_current, image_array_270_previous);
	
	framebuffer_draw_rectangle(1, 1, DISPLAY_WIDTH, DISPLAY_HEIGHT, PIXEL_SET);
	framebuffer_draw_circle(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, 35*2, PIXEL_SET);
	framebuffer_draw_circle(DISPLAY_WIDTH/2-15*2, DISPLAY_HEIGHT/2-10*2, 16, PIXEL_SET);
	framebuffer_draw_circle(DISPLAY_WIDTH/2+15*2, DISPLAY_HEIGHT/2-10*2, 16, PIXEL_SET);
	framebuffer_draw_line(DISPLAY_WIDTH/2-16, DISPLAY_HEIGHT/2+16, DISPLAY_WIDTH/2+16, DISPLAY_HEIGHT/2+16, PIXEL_SET);
	
	framebuffer_draw_rectangle_filled(50, 50, 0, 0, PIXEL_SET);
	framebuffer_draw_circle_filled(25, 25, 25, PIXEL_CLR);
	framebuffer_draw_text_8pt(10, 5, (uint8_t*)"This is a string.", PIXEL_SET);

	epd_display(LANDSCAPE);	
}

//void sd_card_test(void){
	////EXT1_15 Active Low CS for microSD
	////EXT1_16 Connected to SPI_MOSI
	////EXT1_18 Connected to SPI_CLK
	////EXT1_17 Connected to SPI_MISO
	////EXT1_10 Connected to SD Card Detection switch ****Use Internal Pullup****
	//
	//sd_mmc_init();
	//memset(&FatFs_obj, 0, sizeof(FATFS));
//
	//if(f_mount(LUN_ID_SD_MMC_SPI_MEM, &FatFs_obj) == FR_OK){
 		//port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
 		//delay_ms(3000);
 		//port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
 		//delay_ms(500);
	//}
//
	//int err_val = f_open(&file_obj, "MyText.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
//
	//if(err_val == FR_OK){
 		//port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
	//}else{
//
 		//for(int i = 0 ; i < err_val ; i++){
 			//port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
 			//delay_ms(100);
 			//port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
 			//delay_ms(400);
 		//}
	//}
	//
//
	//err_val = sd_mmc_check(0);
	//for(int i = 0 ; i < err_val ; i++){
 		//port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
 		//delay_ms(250);
 		//port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
 		//delay_ms(750);
	//}
//}
//
//void serial_test(void){
		//
	////virtual COM Port is on PA22(D21TX)SERCOM3_PAD[0] and PA23(D21RX)SERCOM3_PAD[1]
	//while(1){
		//uint8_t string[] = "This is a test of things to come.\n"; //= "Hello World!\r\n";
		//usart_write_buffer_wait(&usart_instance, string, sizeof(string));
		//uint16_t temp;
		//while (true) {
			//if (usart_read_wait(&usart_instance, &temp) == STATUS_OK) {
		 		//while (usart_write_wait(&usart_instance, temp) != STATUS_OK) {
		 		//}
			//}
		//}
	//}
	//
//}