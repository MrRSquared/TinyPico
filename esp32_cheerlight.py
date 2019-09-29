#!/usr/bin/env python
'''This is the regular esp32 version (without the dotstar code).  
It should work on a Tiny Pico as well, but the Tiny Pico code will not work on an esp32 that has no dotstar.
The code is connects the TinyPico to cheerlights.  in order for this to work, one must connect it to their network.  You may use
the tutorial found at https://techtutorialsx.com/2017/06/06/esp32-esp8266-micropython-automatic-connection-to-wifi/ to help you get started with that.
Our main cheerlight code is adapted from Jousef Rousek  and can be found here https://github.com/stlk/micropython/blob/master/workshop/
'''
import socket, machine, neopixel
from machine import Pin
#import tinypico as TinyPICO
#from micropython_dotstar import DotStar
import time, random, micropython

# Configure SPI for controlling the DotStar
# Internally we are using software SPI for this as the pins being used are not hardware SPI pins
#spi = SPI(sck=Pin( TinyPICO.DOTSTAR_CLK ), mosi=Pin( TinyPICO.DOTSTAR_DATA ), miso=Pin( TinyPICO.SPI_MISO) ) 
# Create a DotStar instance
#dotstar = DotStar(spi, 1, brightness = 0.5 ) # Just one DotStar, half brightness
# Turn on the power to the DotStar
# TinyPICO.set_dotstar_power( True )

# The pin the neopixels are plugged into
neo_pin = 25

#Number of leds in your strand (that you want to light)
neo_pixels = 4

np = neopixel.NeoPixel(machine.Pin(neo_pin),neo_pixels)
brightness = .5

'''Number of leds on your strand.  Be careful how many leds you drive at full power. 
You can use the formula of 60mA * neo_pixels to find out how many amps you are drawing.  
If you lower the brightness, you can lower the 60mA, but if you look  at the dictionary, 
at full brightness the cheer code uses full power.

check out Adafruit's https://learn.adafruit.com/adafruit-neopixel-uberguide for more info.'''



# Define color dictionary
cheerMap = {
    "FF0000" : (255,  0,  0,),
    "008000" : (0, 255,  0,),
    "0000FF" : (0,  0, 255,),
    "00FFFF" : (0, 255, 255),
    "FFFFFF" : (255, 255, 255),
    "FDF5E6" : (255, 223, 223),
    "800080" : (128,  0, 128),
    "FF00FF" : (255,  0, 255),
    "FFFF00" : (255, 255,  0),
    "FFA500" : (255,165,  0),
    "FFC0CB" : (255, 119, 218),}
#Define the color variable (we use that to grab the color from the Cheerlights server.)
color = ''
# Connect to the cheerlights server


url = 'http://api.thingspeak.com/channels/1417/field/2/last.txt'
_, _, host, path = url.split('/', 3)
ai = socket.getaddrinfo(host, 80)
print('Address infos:', ai)
addr = ai[0][-1]
print('Connect address:', addr)

try:
    while True:
        x = 0
        s = socket.socket()
        s.connect(addr)
        s.send(bytes('GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n' % (path, host), 'utf8'))
        while (x is not 1 ):
            # s.send(bytes('GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n' % (path, host), 'utf8'))
            data = s.recv(100)
            print(data)

            if data:
                chunk = str(data, 'utf8')
                # print(chunk, end='')
                hash_index = chunk.find('#')
                if hash_index >= 0:
                    color = chunk[hash_index + 1: hash_index + 7]
                    led_color = tuple(int(c/brightness) for c in cheerMap.get(color))
                    # dotstar[0] = led_color
                    for i in range(neo_pixels):
                        np[i] = (led_color)
                        np.write()
                    x = 1
           
        time.sleep(5)
        s.close()
        
except:
    # dotstar[0] = (0,0,0)
    for i in range(neo_pixels):
        np[i] = (0,0,0)
        np.write()
    s.close()

    print("Exiting gracefully")
