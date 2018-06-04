/*
 * epd_graphics_library.h
 *
 * Created: 10/12/2015 1:06:49 AM
 *  Author: corwinperren
 */ 


#ifndef EPD_GRAPHICS_LIBRARY_H_
#define EPD_GRAPHICS_LIBRARY_H_

#include "config/conf_EPD.h"
#include "Fonts/CustomFonts.h"
#include "image_data.h"

#define DISPLAY_WIDTH 264
#define DISPLAY_HEIGHT 176
#define PIXELS_PER_BYTE 8
#define BYTES_PER_ROW (DISPLAY_WIDTH/PIXELS_PER_BYTE)

#define PIXEL_SET 0
#define PIXEL_CLR 1
#define PIXEL_XOR 2

#define LANDSCAPE 0
#define LANDSCAPE_FLIPPED 1

uint8_t *current_framebuffer;
uint8_t *previous_framebuffer;

////////////// Display and Framebuffer Functions ///////////////
void epd_display(uint8_t orientation);
void set_framebuffers(uint8_t *current, uint8_t *previous);
void framebuffer_clear_screen(void);
void framebuffer_x_y_mirror(void);

////////////// Drawing Functions ///////////////
void framebuffer_draw_pixel(uint16_t x, uint16_t y, uint8_t color);
void framebuffer_draw_line(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint8_t color);
void framebuffer_draw_rectangle(uint16_t p1_x, uint16_t p1_y, uint16_t p2_x, uint16_t p2_y, uint8_t color);
void framebuffer_draw_rectangle_filled(uint16_t p1_x, uint16_t p1_y, uint16_t p2_x, uint16_t p2_y, uint8_t color);
void framebuffer_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint8_t color);
void framebuffer_draw_circle_filled(uint16_t x, uint16_t y, uint16_t r, uint8_t color);
void framebuffer_draw_text_8pt(uint16_t x, uint16_t y, uint8_t *text, uint8_t color);

////////////// Miscellaneous Functions ///////////////
unsigned char reverse_byte(unsigned char input);

#endif /* EPD_GRAPHICS_LIBRARY_H_ */