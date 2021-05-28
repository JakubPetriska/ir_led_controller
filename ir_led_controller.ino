#include "FastLED.h"

#define DECODE_NEC

/*
 * Define macros for input and output pin etc.
 */
#include "PinDefinitionsAndMore.h"

#include <IRremote.h>

#define NUM_LEDS 13
CRGB leds[NUM_LEDS];

int activeLed = 0;
int dir = 1;
int brightness = 255;
int hue = 0;

const int COMMAND_BRIGHTNESS_UP = 0x5;
const int COMMAND_BRIGHTNESS_DOWN = 0x4;
const int COMMAND_OFF = 0x6;
const int COMMAND_ON = 0x7;
const int COMMAND_RED = 0x9;
const int COMMAND_RED_1 = 0xD;
const int COMMAND_RED_2 = 0x15;
const int COMMAND_RED_3 = 0x19;
const int COMMAND_RED_4 = 0x11;
const int COMMAND_GREEN = 0x8;
const int COMMAND_GREEN_1 = 0xC;
const int COMMAND_GREEN_2 = 0x14;
const int COMMAND_GREEN_3 = 0x18;
const int COMMAND_GREEN_4 = 0x10;
const int COMMAND_BLUE = 0xA;
const int COMMAND_BLUE_1 = 0xE;
const int COMMAND_BLUE_2 = 0x16;
const int COMMAND_BLUE_3 = 0x1A;
const int COMMAND_BLUE_4 = 0x12;
const int COMMAND_WHITE = 0xB;
const int COMMAND_FLASH = 0xF;
const int COMMAND_STROBE = 0x17;
const int COMMAND_FADE = 0x1B;
const int COMMAND_SMOOTH = 0x13;

const int BRIGHTNESS_STEP = 50;

void setup()
{
    Serial.begin(9600);

    FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
    FastLED.setBrightness(brightness);

    // FastLED.showColor(CRGB(0, 0, 255));

    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);

    Serial.print(F("Ready to receive IR signals at pin "));
    Serial.println(IR_RECEIVE_PIN);
}

void loop()
{
    if (IrReceiver.decode())
    {

        // Print a short summary of received data
        IrReceiver.printIRResultShort(&Serial);
        if (IrReceiver.decodedIRData.protocol == UNKNOWN)
        {
            // We have an unknown protocol here, print more info
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        }
        Serial.println();

        /*
         * Finally, check the received data and perform actions according to the received command
         */
        if (IrReceiver.decodedIRData.command == COMMAND_RED)
        {
            FastLED.showColor(CRGB::Red);
        }
        else if (IrReceiver.decodedIRData.command == COMMAND_GREEN)
        {
            FastLED.showColor(CRGB::Green);
        }
        else if (IrReceiver.decodedIRData.command == COMMAND_BLUE)
        {
            FastLED.showColor(CRGB::Blue);
        }
        else if (IrReceiver.decodedIRData.command == COMMAND_BRIGHTNESS_UP)
        {
            brightness = max(min(brightness + BRIGHTNESS_STEP, 255), 0);

            Serial.print("New brightness ");
            Serial.print(brightness);
            Serial.println();

            FastLED.setBrightness(brightness);
            FastLED.show();
        }
        else if (IrReceiver.decodedIRData.command == COMMAND_BRIGHTNESS_DOWN)
        {
            brightness = max(min(brightness - BRIGHTNESS_STEP, 255), 0);

            Serial.print("New brightness ");
            Serial.print(brightness);
            Serial.println();

            FastLED.setBrightness(brightness);
            FastLED.show();
        }

        delay(1000);
        IrReceiver.resume();
    }
}