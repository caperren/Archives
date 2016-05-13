/*
 * InitFunctions.c
 *
 * Created: 8/9/2014 4:24:25 PM
 *  Author: corwin
 */ 

#include "InitFunctions.h"


void setupDriverBoardComplete()
{
	setupXMEGAIO();
	setupXMEGAClock();
	setupXMEGAUART();
	setupXMEGATimers();
	setupStepperPararms();
	setupPinChangeInterrupts();
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	sei();
}

void testDriverBoardComplete()
{
	testStatusLeds();
	testLightBoard();
	testSteppers();
	testUART();
}

void setupXMEGAIO()
{
	//Setup motor enable control
	setupIOPin(&PORT__STEP_EN, PIN__STEP_EN, PINDIR__STEP_EN, INITSTATE__STEP_EN);
	
	//Setup X Axis motor driver
	setupIOPin(&PORT__X_FAULT, PIN__X_FAULT, PINDIR__X_FAULT, INITSTATE__X_FAULT);
	setupIOPin(&PORT__X_DIR, PIN__X_DIR, PINDIR__X_DIR, INITSTATE__X_DIR);
	setupIOPin(&PORT__X_STEP, PIN__X_STEP, PINDIR__X_STEP, INITSTATE__X_STEP);
	setupIOPin(&PORT__X_M0, PIN__X_M0, PINDIR__X_M0, INITSTATE__X_M0);
	setupIOPin(&PORT__X_M1, PIN__X_M1, PINDIR__X_M1, INITSTATE__X_M1);
	setupIOPin(&PORT__X_M2, PIN__X_M2, PINDIR__X_M2, INITSTATE__X_M2);
	
	//Setup Y Axis motor driver
	setupIOPin(&PORT__Y_FAULT, PIN__Y_FAULT, PINDIR__Y_FAULT, INITSTATE__Y_FAULT);
	setupIOPin(&PORT__Y_DIR, PIN__Y_DIR, PINDIR__Y_DIR, INITSTATE__Y_DIR);
	setupIOPin(&PORT__Y_STEP, PIN__Y_STEP, PINDIR__Y_STEP, INITSTATE__Y_STEP);
	setupIOPin(&PORT__Y_M0, PIN__Y_M0, PINDIR__Y_M0, INITSTATE__Y_M0);
	setupIOPin(&PORT__Y_M1, PIN__Y_M1, PINDIR__Y_M1, INITSTATE__Y_M1);
	setupIOPin(&PORT__Y_M2, PIN__Y_M2, PINDIR__Y_M2, INITSTATE__Y_M2);
	
	//Setup Z Axis motor driver
	setupIOPin(&PORT__Z_FAULT, PIN__Z_FAULT, PINDIR__Z_FAULT, INITSTATE__Z_FAULT);
	setupIOPin(&PORT__Z_DIR, PIN__Z_DIR, PINDIR__Z_DIR, INITSTATE__Z_DIR);
	setupIOPin(&PORT__Z_STEP, PIN__Z_STEP, PINDIR__Z_STEP, INITSTATE__Z_STEP);
	setupIOPin(&PORT__Z_M0, PIN__Z_M0, PINDIR__Z_M0, INITSTATE__Z_M0);
	setupIOPin(&PORT__Z_M1, PIN__Z_M1, PINDIR__Z_M1, INITSTATE__Z_M1);
	setupIOPin(&PORT__Z_M2, PIN__Z_M2, PINDIR__Z_M2, INITSTATE__Z_M2);
	
	//Setup A Axis motor driver
	setupIOPin(&PORT__A_FAULT, PIN__A_FAULT, PINDIR__A_FAULT, INITSTATE__A_FAULT);
	setupIOPin(&PORT__A_DIR, PIN__A_DIR, PINDIR__A_DIR, INITSTATE__A_DIR);
	setupIOPin(&PORT__A_STEP, PIN__A_STEP, PINDIR__A_STEP, INITSTATE__A_STEP);
	setupIOPin(&PORT__A_M0, PIN__A_M0, PINDIR__A_M0, INITSTATE__A_M0);
	setupIOPin(&PORT__A_M1, PIN__A_M1, PINDIR__A_M1, INITSTATE__A_M1);
	setupIOPin(&PORT__A_M2, PIN__A_M2, PINDIR__A_M2, INITSTATE__A_M2);
	
	//Setup up limits
	setupIOPin(&PORT__X_LIM_LEFT, PIN__X_LIM_LEFT, PINDIR__X_LIM_LEFT, INITSTATE__X_LIM_LEFT);
	setupIOPin(&PORT__X_LIM_RIGHT, PIN__X_LIM_RIGHT, PINDIR__X_LIM_RIGHT, INITSTATE__X_LIM_RIGHT);
	
	setupIOPin(&PORT__Y_LIM_FORW, PIN__Y_LIM_FORW, PINDIR__Y_LIM_FORW, INITSTATE__Y_LIM_FORW);
	setupIOPin(&PORT__Y_LIM_BACK, PIN__Y_LIM_BACK, PINDIR__Y_LIM_BACK, INITSTATE__Y_LIM_BACK);
	
	setupIOPin(&PORT__Z_LIM_DOWN, PIN__Z_LIM_DOWN, PINDIR__Z_LIM_DOWN, INITSTATE__Z_LIM_DOWN);
	
	setupIOPin(&PORT__A_LIM_1, PIN__A_LIM_1, PINDIR__A_LIM_1, INITSTATE__A_LIM_1);
	setupIOPin(&PORT__A_LIM_2, PIN__A_LIM_2, PINDIR__A_LIM_2, INITSTATE__A_LIM_2);
	
	
	//Setup control panel buttons
	setupIOPin(&PORT__SYS_CHNG_STATE, PIN__SYS_CHNG_STATE, PINDIR__SYS_CHNG_STATE, INITSTATE__SYS_CHNG_STATE);
	setupIOPin(&PORT__SYS_START, PIN__SYS_START, PINDIR__SYS_START, INITSTATE__SYS_START);
	setupIOPin(&PORT__SYS_STOP, PIN__SYS_STOP, PINDIR__SYS_STOP, INITSTATE__SYS_STOP);
	setupIOPin(&PORT__SYS_HOME, PIN__SYS_HOME, PINDIR__SYS_HOME, INITSTATE__SYS_HOME);
	setupIOPin(&PORT__SYS_CAL, PIN__SYS_CAL, PINDIR__SYS_CAL, INITSTATE__SYS_CAL);
	
	//Setup ESTOP
	setupIOPin(&PORT__ESTOP, PIN__ESTOP, PINDIR__ESTOP, INITSTATE__ESTOP);
	
	//Setup pin for light board control
	setupIOPin(&PORT__LIGHT_CTRL, PIN__LIGHT_CTRL, PINDIR__LIGHT_CTRL, INITSTATE__LIGHT_CTRL);
	
	//Setup pins for uart communication
	setupIOPin(&PORT__UART_RX, PIN__UART_RX, PINDIR__UART_RX, INITSTATE__UART_RX);
	setupIOPin(&PORT__UART_TX, PIN__UART_TX, PINDIR__UART_TX, INITSTATE__UART_TX);
	
	//Setup status led board
	setupIOPin(&PORT__RDY_LED, PIN__RDY_LED, PINDIR__RDY_LED, INITSTATE__RDY_LED);
	setupIOPin(&PORT__RUN_LED, PIN__RUN_LED, PINDIR__RUN_LED, INITSTATE__RUN_LED);
	setupIOPin(&PORT__ERR_LED, PIN__ERR_LED, PINDIR__ERR_LED, INITSTATE__ERR_LED);
	
	//Show initialization led sequence to show end of initialization
}

void setupXMEGAClock()
{
	int temp = 0;																			//Temporary variable for helping avoid 4 clock cycle limitation when updating secure registers
	
	//Enable external 8MHz oscillator
	OSC.XOSCCTRL = (OSC_FRQRANGE_2TO9_gc | OSC_XOSCSEL_XTAL_16KCLK_gc);						//Set external oscillator to be between 2 and 9 MHz and select it
	OSC.CTRL |= OSC_XOSCEN_bm;																//Enable the external oscillator
	while(!(OSC.STATUS & OSC_XOSCRDY_bm)){ERR_LED_SET();};									//While the external oscillator is not ready, set the error led
	ERR_LED_CLR();																			//Clear the error led if the external oscillator has stabilized
	
	//Enable phase locked loop to multiply external oscillator by 4 to get 32MHz
	temp = ((OSC_PLLSRC_XOSC_gc & OSC_PLLSRC_gm) | (OSC_PLLFAC_gm & 4));				//Set the external oscillator as the clock source for the pll and set to multiply by 4
	CCP = CCP_IOREG_gc;																		//Disable register security so we can update the pll control settings
	OSC.PLLCTRL = temp;																		//Write pll control settings to register
	OSC.CTRL |= OSC_PLLEN_bm;																//Enable the pll
	while(!(OSC.STATUS & OSC_PLLRDY_bm)){ERR_LED_SET();};									//While the pll is not ready, set the error led
	ERR_LED_CLR();																			//Disable the error led if successfully stabilized
	
	//Set system pll clock divisions and set up as source for all system clocks
	temp = ((CLK_PSADIV_gm & CLK_PSADIV_1_gc) | (CLK_PSBCDIV_gm & CLK_PSBCDIV_1_1_gc));		//Set system to use pll divided by 1 (no division)
	CCP = CCP_IOREG_gc;																		//Disable register security so we can update the clock source division setting
	CLK.CTRL = temp;																		//Write division settings to register
	
	temp = CLK_SCLKSEL_PLL_gc;																//Set pll as system clock source
	CCP = CCP_IOREG_gc;																		//Disable register security so we can update the system clock
	CLK.CTRL = temp;																		//Write clock source settings to register
}

void setupXMEGATimers()
{
	setupRTC();
	setupStepDriveTimer();	
}

void setupXMEGAUART()
{
	USART_InterruptDriver_Initialize(&usartData, &USARTF0, USART_DREINTLVL_LO_gc);				//Initialize USARTC0 as interrupt driven serial and clear it's buffers
	USART_Format_Set(usartData.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);	//Set the data format of 8 bits, no parity, 1 stop bit
	USART_RxdInterruptLevel_Set(usartData.usart, USART_RXCINTLVL_LO_gc);						//Enable the receive interrupt
	USART_Baudrate_Set(&USARTF0, 207 , 0);														//Set baudrate to 9600 with 32Mhz system clock
	USART_Rx_Enable(usartData.usart);															//Enable receiving over serial
	USART_Tx_Enable(usartData.usart);															//Enable transmitting over serial
	//PMIC.CTRL |= PMIC_LOLVLEX_bm;																//Enable serial interrupts
}

void setupStepperPararms()
{
	XAxis.dirPort = &PORT__X_DIR;
	XAxis.dirPin = PIN__X_DIR;
	XAxis.stepPort = &PORT__X_STEP;
	XAxis.stepPin = PIN__X_STEP;
	XAxis.directionReversed = TRUE;
	XAxis.inManualMode = FALSE;
	XAxis.stepsPerMM = 250;
	XAxis.mmFromZero = 0;
	XAxis.acceptableError = .005;
	XAxis.steps = 0;
	XAxis.shouldMove = FALSE;
	
	YAxis.dirPort = &PORT__Y_DIR;
	YAxis.dirPin = PIN__Y_DIR;
	YAxis.stepPort = &PORT__Y_STEP;
	YAxis.stepPin = PIN__Y_STEP;
	YAxis.directionReversed = FALSE;
	YAxis.inManualMode = FALSE;
	YAxis.stepsPerMM = 250;
	YAxis.mmFromZero = 0;
	YAxis.acceptableError = .005;
	YAxis.steps = 0;
	YAxis.shouldMove = FALSE;
		
	ZAxis.dirPort = &PORT__Z_DIR;
	ZAxis.dirPin = PIN__Z_DIR;
	ZAxis.stepPort = &PORT__Z_STEP;
	ZAxis.stepPin = PIN__Z_STEP;
	ZAxis.directionReversed = FALSE;
	ZAxis.inManualMode = FALSE;
	ZAxis.stepsPerMM = 323;
	ZAxis.mmFromZero = 8.25;
	ZAxis.acceptableError = .004;
	ZAxis.steps = 0;
	ZAxis.shouldMove = FALSE;
	
	
	AAxis.dirPort = &PORT__A_DIR;
	AAxis.dirPin = PIN__A_DIR;
	AAxis.stepPort = &PORT__A_STEP;
	AAxis.stepPin = PIN__A_STEP;
	AAxis.directionReversed = TRUE;
	AAxis.inManualMode = FALSE;
	AAxis.stepsPerMM = 250;
	AAxis.mmFromZero = 0;
	AAxis.acceptableError = .005;
	AAxis.steps = 0;
	AAxis.shouldMove = FALSE;
}

void setupPinChangeInterrupts(){
	PORT__ESTOP.PIN0CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORT__ESTOP.INT0MASK |= PIN__ESTOP;
	PORT__ESTOP.INTCTRL |= PORT_INT0LVL_LO_gc;
	
	PORT__SYS_HOME.PIN2CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORT__SYS_HOME.INT0MASK |= PIN__SYS_HOME;
	PORT__SYS_HOME.INTCTRL |= PORT_INT0LVL_LO_gc;
	
	PORT__SYS_CAL.PIN3CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORT__SYS_CAL.INT0MASK |= PIN__SYS_CAL;
	PORT__SYS_CAL.INTCTRL |= PORT_INT0LVL_LO_gc;
	
	PORT__SYS_START.PIN0CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORT__SYS_START.INT0MASK |= PIN__SYS_START;
	PORT__SYS_START.INTCTRL |= PORT_INT0LVL_LO_gc;
}

void setupSystemParams(){
	btnState.estopPressed = FALSE;
	btnState.startPressed = FALSE;
	btnState.stopPressed = FALSE;
	btnState.homePressed = FALSE;
	btnState.calibratePressed = FALSE;
}

void testStatusLeds()
{
	for(int i = 0 ; i < 3 ; i++){
		RDY_LED_SET();
		ERR_LED_CLR();
		_delay_ms(200);
		RUN_LED_SET();
		RDY_LED_CLR();
		_delay_ms(200);
		RUN_LED_CLR();
		ERR_LED_SET();
		_delay_ms(200);
	}
	
	for(int i = 0 ; i < 3 ; i++){
		RDY_LED_SET();
		RUN_LED_SET();
		ERR_LED_SET();
		_delay_ms(200);
		RUN_LED_CLR();
		RDY_LED_CLR();
		ERR_LED_CLR();
		_delay_ms(200);
	}	
	RDY_LED_CLR();
	RUN_LED_CLR();
	ERR_LED_CLR();
}

void testLightBoard()
{
	LIGHT_CTRL_SET();
	_delay_ms(1250);
	LIGHT_CTRL_CLR();
}

void testSteppers()
{
}

void testUART()
{
	
}

void setupIOPin(PORT_t *port, int pinbm, char direction, char defaultstate)
{
	if(direction == ISINPUT){
		port->DIRCLR = pinbm;
	}else if(direction == ISOUTPUT){
		port->DIRSET = pinbm;
	}
	
	if(defaultstate == LOWSTATE){
		port->OUTCLR = pinbm;
	}else if(defaultstate == HIGHSTATE){
		port->OUTSET = pinbm;
	}
	
}

void setupRTC()
{
	OSC.CTRL |= OSC_RC32KEN_bm;											//Enable 32.768KHz oscillator
	while(!(OSC.STATUS & OSC_RC32KRDY_bm)){ERR_LED_SET();}	;			//Wait for oscillator to stabilize, light error led while not
	ERR_LED_CLR();														//Now stabilized, turn off the error led
	CLK.RTCCTRL = (CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm);					//Set RTC clock source to be the internal oscillator and enable
	while(RTC.STATUS & RTC_SYNCBUSY_bm){ERR_LED_SET();};				//Wait for RTC to no longer be busy, set error led while it is
	ERR_LED_CLR();
	
	RTC.PER = 10;														//
	RTC.CNT = 0;														//Zero the count register
	RTC.COMP = 0;														//Set compare value to zero
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;									//Set prescaler to clk / 1
	RTC.INTCTRL = (RTC_OVFINTLVL_LO_gc | RTC_COMPINTLVL_OFF_gc);
}

void setupStepDriveTimer()
{
	//For X Axis
	TCC0.CTRLA = TC_CLKSEL_DIV1_gc; //125000 counts per second with 32Mhz Processor
	TCC0.CTRLB = TC_WGMODE_NORMAL_gc;
	TCC0.PER = 9216;
	TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
	
	//For Y Axis
	TCD0.CTRLA = TC_CLKSEL_DIV1_gc; //31250 counts per second with 32Mhz Processor
	TCD0.CTRLB = TC_WGMODE_NORMAL_gc;
	TCD0.PER = 9216;
	TCD0.INTCTRLA = TC_OVFINTLVL_LO_gc;
	
	//For Z Axis
	TCE0.CTRLA = TC_CLKSEL_DIV1_gc; //31250 counts per second with 32Mhz Processor
	TCE0.CTRLB = TC_WGMODE_NORMAL_gc;
	TCE0.PER = 9216;
	TCE0.INTCTRLA = TC_OVFINTLVL_LO_gc;
	
	//For A Axis
	TCF0.CTRLA = TC_CLKSEL_DIV1_gc; //31250 counts per second with 32Mhz Processor
	TCF0.CTRLB = TC_WGMODE_NORMAL_gc;
	TCF0.PER = 9216;
	TCF0.INTCTRLA = TC_OVFINTLVL_LO_gc;
}
