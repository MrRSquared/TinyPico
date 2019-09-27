# TinyPico-Cheerlights

If you are not familiar with them, cheerlights are smart lights (real or virtual) that are linked throughout the world.  If someone tweets [@cheerlights](https://twitter.com/cheerlights)  and a color, they all change the color.  More info can be found [on the Cheerlights site.](https://cheerlights.com/) In order to use this code you need to install both the TinyPico and ThingSpeak headers in your Arduino environment.

**CheerlightsDotstar** uses the onboard dotstar to create the color.  No other hardware is required.

**Neo_Cheer** uses the FastLED library to add neopixels.  I am using just a few leds so I am wiring them directly to the 3v power.  If you do the same, do not use more than 6 @ 255, 255, 255.

**BLE_Cheer**  Uses a phone as the server to send the Cheerlight data.  Use the app that I created through App Inventor to send the data.

**bleCheerTouch** Adds two touch buttons to control brightness.

**CheerTinyPico.aia** This is the App Inventor file I used to create the app to cennect the cheerlight data to the light.  In order for this to work, you must use a beta version of the BLE stack for App Inventor.  Though I do not yet have permission to host it here, the [Google Group] (https://groups.google.com/forum/?utm_medium=email&utm_source=footer#!msg/mitappinventortest/Jdb6j82Op28/Uo_fghkRBQAJ) is public.  You want the BLE-with-new-ConnectDevice.aix.

**cheerlights.py** This is a non-app Inventor version that uses micropython instead of C++.  It does require an Internet Connection as I could not get BLE working with micropython.  It is a possiblity, but early in development.  **Note:** At the moment, all this does is go grab the color and print it.  I need a bit more work to set the dotstar and power the neoPixels. I will update it shortly.
