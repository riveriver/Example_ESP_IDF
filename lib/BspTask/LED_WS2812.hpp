#include <FastLED.h>
class LED_WS2812{
private:
    const uint8_t LED_NUM = 1;
    const uint8_t DATA_PIN = 45;
    const uint8_t CLOCK_PIN = 13;
    CRGB leds[NUM_LED];
public:
    LED_WS2812(){
        FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, LED_NUM);
        FastLED.setBrightness(255);
    }
    void setColor(uint8_t r, uint8_t g, uint8_t b){
        leds[0] = CRGB(r, g, b);
        FastLED.show(); 
    }
    void test_setColor(){
       setColor(255, 42, 0);
    }
};