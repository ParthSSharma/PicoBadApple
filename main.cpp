#include <stdint.h>
#include <stdio.h>
#include "Bitmap.h"
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico-ssd1306/ssd1306.h"
#include "pico-ssd1306/textRenderer/TextRenderer.h"

#define I2C_PORT        i2c0
#define I2C_SCL         28U
#define I2C_SDA         29U
#define ROW_START       0U
#define COL_START       32U
#define FRAME_RATE      30U
#define TEST_ALIGN      10U

void RunBadApple(pico_ssd1306::SSD1306 display){
    uint64_t u64StartTime;
    uint64_t u64EndTime;
    uint64_t u64TimeTaken;

    uint32_t u32NumFrames = sizeof(BadAppleBitmap) / sizeof(BadAppleBitmap[0]);
    drawText(&display, font_12x16, "Bad Apple", TEST_ALIGN, NUM_ROWS);
    for(uint32_t frame = 0; frame < u32NumFrames; frame++){
        u64StartTime = time_us_64();
        for(uint32_t row = 0; row < NUM_ROWS; row++){
            for(uint32_t col = 0; col < NUM_COLS; col++){
                /*
                    Each element of BadAppleBitmap 2D array is a uint16_t integer. The rows of the array give information related to each frame.
                    Each bit in an element represents a pixel of the screen. If the bit is 0, the pixel is black and if the bit is 1, the pixel
                    is white. Therefore, each element gives us information related to 16 pixels. With some bit shifts and a mask to test the bit,
                    we can decode the pixel value. In the function "display.setPixel", the last parameter is 0 for set and 1 for clear. Therefore,
                    we have to logically invert the pixel.
                */
                display.setPixel(col + COL_START, row + ROW_START, (pico_ssd1306::WriteMode) !(BadAppleBitmap[frame][(row * (NUM_COLS / PIXELS_PER_ELEMENT)) + (col / PIXELS_PER_ELEMENT)] & (1 << ((PIXELS_PER_ELEMENT - 1) - (col % PIXELS_PER_ELEMENT)))));
            }
        }
        display.sendBuffer();
        u64EndTime = time_us_64();

        u64TimeTaken = u64EndTime - u64StartTime;
        sleep_us((1000000 / FRAME_RATE) - u64TimeTaken);
    }
}

int main(){
    stdio_init_all();

    i2c_init(I2C_PORT, 1000000);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL);
    gpio_pull_up(I2C_SDA);

    pico_ssd1306::SSD1306 display = pico_ssd1306::SSD1306(I2C_PORT, 0x3C, pico_ssd1306::Size::W128xH64);

    RunBadApple(display);
    return 0;
}