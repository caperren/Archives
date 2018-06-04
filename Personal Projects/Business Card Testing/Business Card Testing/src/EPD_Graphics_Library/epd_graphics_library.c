/*
 * epd_graphics_library.c
 *
 * Created: 10/12/2015 1:06:32 AM
 *  Author: corwinperren
 */ 
#include "epd_graphics_library.h"


void epd_display(uint8_t orientation){
	
	if (orientation != LANDSCAPE_FLIPPED){
		framebuffer_x_y_mirror();
	
		EPD_display_from_pointer(EPD_270, previous_framebuffer, current_framebuffer);
		memcpy(previous_framebuffer, current_framebuffer, sizeof(previous_framebuffer)*1452);
		
		framebuffer_x_y_mirror();
		
	}else{
		EPD_display_from_pointer(EPD_270, previous_framebuffer, current_framebuffer);
		memcpy(previous_framebuffer, current_framebuffer, sizeof(previous_framebuffer)*1452);
		
	}
}

void set_framebuffers(uint8_t *current, uint8_t *previous){
	current_framebuffer = current;
	previous_framebuffer = previous;
	
	for(int i = 0 ; i < ((DISPLAY_WIDTH/8)*DISPLAY_HEIGHT) ; i++){
		current_framebuffer[i] = 0xFF;
		previous_framebuffer[i] = 0xFF;
	}
}


void framebuffer_clear_screen(void)
{
	for(int i = 0 ; i < ((DISPLAY_WIDTH/8)*DISPLAY_HEIGHT) ; i++){
		current_framebuffer[i] = 0xFF;
	}
}

void framebuffer_x_y_mirror(void)
{
	uint16_t max = BYTES_PER_ROW*DISPLAY_HEIGHT;
	
	char temp;
	for(uint16_t i = 0 ; i < (max/2) ; i++){
		temp = current_framebuffer[i];
		current_framebuffer[i] = reverse_byte(current_framebuffer[max-1-i]);
		current_framebuffer[max-1-i] = reverse_byte(temp);
	}
	
}


void framebuffer_draw_pixel(uint16_t x, uint16_t y, uint8_t color){
	
	if ((x > DISPLAY_WIDTH) || (y > DISPLAY_HEIGHT ) || (x < 1) || (y < 1)) {
		return;
	}
	
 	x--;
 	y--;
	
	uint8_t page = x/PIXELS_PER_BYTE;
	uint8_t pixel_mask = (1 << (7-(x-(page*PIXELS_PER_BYTE))));
	
	uint32_t byte_offset = page + (y*BYTES_PER_ROW);
	uint8_t current_pixel_byte = current_framebuffer[byte_offset];
	
	switch(color){
		case PIXEL_SET:
			current_pixel_byte &= ~pixel_mask;
			break;
			
		case PIXEL_CLR:
			current_pixel_byte |= pixel_mask;
			break;
			
		case PIXEL_XOR:
			current_pixel_byte ^= pixel_mask;
			break;
			
		default:
			break;	
	}
	
	current_framebuffer[byte_offset] = current_pixel_byte;
}


//Draw line credit to Po-Han Lin at http://www.edepot.com
void framebuffer_draw_line(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint8_t color){

		bool yLonger=false;
		int incrementVal;
		int shortLen=y2-y;
		int longLen=x2-x;

		if (abs(shortLen)>abs(longLen)) {
			int swap=shortLen;
			shortLen=longLen;
			longLen=swap;
			yLonger=true;
		}

		if (longLen<0) incrementVal=-1;
		else incrementVal=1;

		double multDiff;
		if (longLen==0.0) multDiff=(double)shortLen;
		else multDiff=(double)shortLen/(double)longLen;
		if (yLonger) {
			for (int i=0;i!=longLen;i+=incrementVal) {
				framebuffer_draw_pixel(x+(int)((double)i*multDiff),y+i, color);
			}
			} else {
			for (int i=0;i!=longLen;i+=incrementVal) {
				framebuffer_draw_pixel(x+i,y+(int)((double)i*multDiff), color);
			}
		}
}

void framebuffer_draw_rectangle(uint16_t p1_x, uint16_t p1_y, uint16_t p2_x, uint16_t p2_y, uint8_t color)
{
		framebuffer_draw_line(p1_x, p1_y, p2_x, p1_y, color);
		framebuffer_draw_line(p2_x, p1_y, p2_x, p2_y, color);
		framebuffer_draw_line(p2_x, p2_y, p1_x, p2_y, color);
		framebuffer_draw_line(p1_x, p2_y, p1_x, p1_y, color);
}


void framebuffer_draw_rectangle_filled(uint16_t p1_x, uint16_t p1_y, uint16_t p2_x, uint16_t p2_y, uint8_t color)
{
	uint16_t x1, x2, y1, y2;
	
	if(p2_y < p1_y){
		x1 = p2_x;
		x2 = p1_x;
		y1 = p2_y;
		y2 = p1_y;
	}else{
		x1 = p1_x;
		x2 = p2_x;
		y1 = p1_y;
		y2 = p2_y;
	}
	
	for(uint16_t i = y1 ; i < y2 ; i++){
		framebuffer_draw_line(x1, i, x2, i, color);
	}
	
}


void framebuffer_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint8_t color)
{

	
	int pos_x,
	pos_y = -r,
	tx = 0,
	ty = 4*r,
	a = 0,
	b = 2*ty+9,
	x1 = (int)r*0.707010678 + 0.5;
	
	framebuffer_draw_pixel(x+r,y,color);
	framebuffer_draw_pixel(x-r,y,color);
	framebuffer_draw_pixel(x,y+r,color);
	framebuffer_draw_pixel(x,y-r,color);
	framebuffer_draw_pixel(x+x1,y+x1,color);
	framebuffer_draw_pixel(x+x1,y-x1,color);
	framebuffer_draw_pixel(x-x1,y+x1,color);
	framebuffer_draw_pixel(x-x1,y-x1,color);
	
	for(pos_x = 1;pos_x < x1;pos_x++){
	a += 8;
	tx += a;
	if(tx > ty){
	pos_y++;
	b -= 8;
	ty += b;
	}
	
	framebuffer_draw_pixel(x+pos_x,y+pos_y,color);
	framebuffer_draw_pixel(x-pos_x,y+pos_y,color);
	
	framebuffer_draw_pixel(x+pos_x,y-pos_y,color);
	framebuffer_draw_pixel(x-pos_x,y-pos_y,color);
	
	framebuffer_draw_pixel(x+pos_y,y+pos_x,color);
	framebuffer_draw_pixel(x-pos_y,y+pos_x,color);
	
	framebuffer_draw_pixel(x+pos_y,y-pos_x,color);
	framebuffer_draw_pixel(x-pos_y,y-pos_x,color);
	}
	
}

void framebuffer_draw_circle_filled(uint16_t x, uint16_t y, uint16_t r, uint8_t color)
{
	for (int i = -r; i < r ; i++)
	{
		int height = (int)sqrt(r * r - i * i);

		for (int j = -height; j < height; j++){
			framebuffer_draw_pixel(i + x, j + y, color);
		}
	}
}

void framebuffer_draw_text_8pt(uint16_t x, uint16_t y, uint8_t *text, uint8_t color)
{
	uint8_t height = 12;
	uint8_t offset = 87-32;
	uint8_t width = Roboto_Thin10x12[offset];
	
	for(uint16_t i = 0; i < width ; i += 2){
		uint8_t col_top = Roboto_Thin10x12[offset + 1 + i];
		uint8_t col_bottom = Roboto_Thin10x12[offset + 2 + i];
		uint8_t mask = 0b00000001;
		
		for(uint16_t j = 0 ; j < 8 ; j++){
			
			if (col_top & mask){
				framebuffer_draw_pixel(x+i, y+j, color);
			}
			
			mask <<= 1;//(mask << 1);
		}
		
		for(uint16_t j = 0 ; j < 4 ; j++){

		}
	}	
}

unsigned char reverse_byte(unsigned char input)
{
	static const unsigned char table[] = {
		0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
		0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
		0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
		0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
		0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
		0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
		0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
		0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
		0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
		0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
		0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
		0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
		0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
		0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
		0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
		0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
		0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
		0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
		0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
		0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
		0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
		0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
		0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
		0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
		0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
		0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
		0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
		0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
		0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
		0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
		0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
		0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
	};
	return table[input];
}