# TinyPico-Cheerlights

If you are not familiar with them, cheerlights are smart lights (real or virtual) that are linked throughout the world.  If someone tweets [@cheerlights](https://twitter.com/cheerlights)  and a color, they all change the color.  More info can be found [on the Cheerlights site.](https://cheerlights.com/) In order to use this code you need to install both the TinyPico and ThingSpeak headers in your Arduino environment.

**CheerlightsDotstar** uses the onboard dotstar to create the color.  No other hardware is required.
**Neo_Cheer** uses the FastLED library to add neopixels.  I am using just a few leds so I am wiring them directly to the 3v power.  If you do the same, do not use more than 6 @ 255, 255, 255.
