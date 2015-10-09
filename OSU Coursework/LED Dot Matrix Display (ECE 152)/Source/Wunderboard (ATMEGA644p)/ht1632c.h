/* Number of Boards */
#define	NUM_BOARDS	2

/* Number of HT1632C Chips */
#define CHIP_MAX (NUM_BOARDS*4)
#define Y_MAX 15
#define X_MAX ((32*NUM_BOARDS)-1)

/* Pin Assignments for PORTC from 0 to 7 */
#define DATA_PIN 	0
#define	CLK_PIN		1
#define WR_PIN		2
#define CS_PIN		3

unsigned char ht1632_shadowram[63][CHIP_MAX];
unsigned char buffer[8][8];

#define HT1632_ID_CMD 4		/* ID = 100 - Commands */
#define HT1632_ID_RD  6		/* ID = 110 - Read RAM */
#define HT1632_ID_WR  5		/* ID = 101 - Write RAM */

/* HT1632C Control Commands */
// #define HT_MASTER			0b100000110000	//Sets chip as master and clock reference
// #define EXT_CLK_HT_MASTER	0b100000111000
// #define HT_SLAVE			0b100000100000	//Set chips as slave and looks for master clock reference
// #define SYS_EN				0b100000000010	//Turns on the boards oscillator
// #define LED_ON				0b100000000110	//Turns on the LED Duty cycle generator (for pwm brightness control)
// #define COM_OPTION			0b100001000000	//N-MOS Open Drain Option and 8 Common Option (Figure out what this means)
// #define PWM_10				0b100101010010	//Used to set a 10/16 PWM Brightness
#define HT1632_CMD_SYSDIS 0x00	/* CMD= 0000-0000-x Turn off oscil */
#define HT1632_CMD_SYSON  0x01	/* CMD= 0000-0001-x Enable system oscil */
#define HT1632_CMD_LEDOFF 0x02	/* CMD= 0000-0010-x LED duty cycle gen off */
#define HT1632_CMD_LEDON  0x03	/* CMD= 0000-0011-x LEDs ON */
#define HT1632_CMD_BLOFF  0x08	/* CMD= 0000-1000-x Blink ON */
#define HT1632_CMD_BLON   0x09	/* CMD= 0000-1001-x Blink Off */
#define HT1632_CMD_SLVMD  0x10	/* CMD= 0001-0xxx-x Slave Mode */
#define HT1632_CMD_MSTMD  0x18	/* CMD= 0001-10xx-x Use on-chip clock */
#define HT1632_CMD_EXTCLK 0x1C	/* CMD= 0001-11xx-x Use external clock */
#define HT1632_CMD_COMS00 0x20	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS01 0x24	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS10 0x28	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS11 0x2C	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_PWM    0b100101010010//0xA0	/* CMD= 101x-PPPP-x PWM duty cycle */

// possible values for a pixel;
#define BLACK  0
#define GREEN  1
#define RED    2
#define ORANGE 3

#define ASSERT(condition) //nothing
#define calcBit(y) (8>>(y&3))

unsigned char xyToIndex(unsigned char x, unsigned char y){
  unsigned char nChip, addr;

  if (x>=32) {
    nChip = 3 + x/16 + (y>7?2:0);
  } else {
    nChip = 1 + x/16 + (y>7?2:0);
  }

  x = x % 16;
  y = y % 8;
  addr = (x<<1) + (y>>2);

  return addr;
}

static void ht1632_senddata (unsigned char chipNo, unsigned char address, unsigned char data){
  ht1632_chipselect(chipNo);
  ht1632_writebits(HT1632_ID_WR, 1<<2);  // send ID: WRITE to RAM
  ht1632_writebits(address, 1<<6); // Send address
  ht1632_writebits(data, 1<<3); // send 4 bits of data
  ht1632_chipselect(0);
}

int get_pixel(unsigned char x, unsigned char y) {
  unsigned char addr, bitval, nChip;

  if (x>=32) {
    nChip = 3 + x/16 + (y>7?2:0);
  } else {
    nChip = 1 + x/16 + (y>7?2:0);
  }

  addr = xyToIndex(x,y);
  bitval = calcBit(y);

  if ((ht1632_shadowram[addr][nChip-1] & bitval) && (ht1632_shadowram[addr+32][nChip-1] & bitval)) {
    return ORANGE;
  } else if (ht1632_shadowram[addr][nChip-1] & bitval) {
    return GREEN;
  } else if (ht1632_shadowram[addr+32][nChip-1] & bitval) {
    return RED;
  } else {
    return 0;
  }
}

unsigned char decToBcd(unsigned char val){
  return ( (val/10*16) + (val%10) );
}
unsigned char bcdToDec(unsigned char val){
  return ( (val/16*10) + (val%16) );
}
void ht1632_plot (unsigned char x, unsigned char y, unsigned char color){
  unsigned char nChip, addr, bitval;

  if (x<0 || x>X_MAX || y<0 || y>Y_MAX)
    return;

  if (color != BLACK && color != GREEN && color != RED && color != ORANGE)
    return;

  if (x>=32) {
    nChip = 3 + x/16 + (y>7?2:0);
  } else {
    nChip = 1 + x/16 + (y>7?2:0);
  }

  addr = xyToIndex(x,y);
  bitval = calcBit(y);

  switch (color)
  {
    case BLACK:
      if (get_pixel(x,y) != BLACK) { // compare with memory to only set if pixel is other color
        // clear the bit in both planes;
        ht1632_shadowram[addr][nChip-1] &= ~bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
        addr = addr + 32;
        ht1632_shadowram[addr][nChip-1] &= ~bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      }
      break;
    case GREEN:
      if (get_pixel(x,y) != GREEN) { // compare with memory to only set if pixel is other color
        // set the bit in the green plane and clear the bit in the red plane;
        ht1632_shadowram[addr][nChip-1] |= bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
        addr = addr + 32;
        ht1632_shadowram[addr][nChip-1] &= ~bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      }
      break;
    case RED:
      if (get_pixel(x,y) != RED) { // compare with memory to only set if pixel is other color
        // clear the bit in green plane and set the bit in the red plane;
        ht1632_shadowram[addr][nChip-1] &= ~bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
        addr = addr + 32;
        ht1632_shadowram[addr][nChip-1] |= bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      }
      break;
    case ORANGE:
      if (get_pixel(x,y) != ORANGE) { // compare with memory to only set if pixel is other color
        // set the bit in both the green and red planes;
        ht1632_shadowram[addr][nChip-1] |= bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
        addr = addr + 32;
        ht1632_shadowram[addr][nChip-1] |= bitval;
        ht1632_senddata(nChip, addr, ht1632_shadowram[addr][nChip-1]);
      }
      break;
  }
}

void null_buffer(){
  for(int i=0;i<8;i++)
    for(int j=0; j<8;j++)
      buffer[i][j] = 0;
}

void set_buffer(unsigned char chr){
  for(int i=0; i<sizeof(chl); i++){
    if(chl[i] == chr){
      int pos = i*8;
      for(int j=0;j<8;j++){
        memcpy_P(buffer[j], (PGM_P)pgm_read_word(&(CHL[j+pos])), 8);
      }
    }
  }
}

void ht1632_clear(){
  unsigned char i;

  for (int j=1;j<=CHIP_MAX;j++) {
    ht1632_chipselect(j);
    ht1632_writebits(HT1632_ID_WR, 1<<2);  // send ID: WRITE to RAM
    ht1632_writebits(0, 1<<6); // Send address
    for (i=0; i<64/2; i++) // Clear entire display
      ht1632_writebits(0, 1<<7); // send 8 bits of data
    ht1632_chipselect(0);
    for (i=0; i<64; i++)
      ht1632_shadowram[i][j] = 0;
  }
}

void ht1632_chipselect(int select){
  unsigned char tmp = 0;
  if (select < 0) { // Enable all HT1632C
    setBPin(CS_PIN, "LOW");
    for (tmp=0; tmp<CHIP_MAX; tmp++) {
      clockPulse();
    }
  } else if(select==0) { //Disable all HT1632Cs
    setBPin(CS_PIN, "HIGH");
    for(tmp=0; tmp<CHIP_MAX; tmp++) {
      clockPulse();
    }
  } else {
    setBPin(CS_PIN, "HIGH");
    for(tmp=0; tmp<CHIP_MAX; tmp++) {
      clockPulse();
    }
    setBPin(CS_PIN, "LOW");
    clockPulse();
    setBPin(CS_PIN, "HIGH");
    tmp = 1;
    for( ; tmp<select; tmp++) {
      clockPulse();
    }
  }
}

void ht1632_writebits (unsigned char bits, unsigned char firstbit){
  while (firstbit) {
    setBPin(WR_PIN, "LOW");
    if (bits & firstbit) {
	setBPin(DATA_PIN, "HIGH");
    }
    else {
	setBPin(DATA_PIN, "LOW");
    }
    setBPin(WR_PIN, "HIGH");
    firstbit >>= 1;
  }
}

static void ht1632_sendcmd (unsigned char chipNo, unsigned char command){
  ht1632_chipselect(chipNo);
  ht1632_writebits(HT1632_ID_CMD, 1<<2);  // send 3 bits of id: COMMMAND
  ht1632_writebits(command, 1<<7);  // send the actual command
  ht1632_writebits(0, 1); 	/* one extra dont-care bit in commands. */
  ht1632_chipselect(0);
}

static void ht1632_initialize(){
	setBPin(CS_PIN, "HIGH");
	for (int i=1; i<=CHIP_MAX; i++) {
		ht1632_sendcmd(i, HT1632_CMD_SYSDIS); // Disable system
		ht1632_sendcmd(i, HT1632_CMD_COMS00); // 16*32, PMOS drivers
		ht1632_sendcmd(i, HT1632_CMD_MSTMD);  // MASTER MODE
		ht1632_sendcmd(i, HT1632_CMD_SYSON);  // System on
		ht1632_sendcmd(i, HT1632_CMD_LEDON);  // LEDs on 
	}
	ht1632_clear(); // Clear the display
}