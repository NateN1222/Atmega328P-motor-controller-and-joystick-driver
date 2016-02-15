# Atmega328p analog joystick driver and motor controller

The goal of this project is to control 2 DC motors in a natural way with a 2 axis joystick. 
It could be useful to anybody who wants to drive a joystick for any application. 

It uses 2 PWM channels for power control and 4 I/O pins which are meant for an H-bridge.

The code is set up to calibrate itself saving maximum/minimum values from the ADC, so in
theory it should work with a wide variety of joysticks. Perhaps it could even be configured
to work with standard Windows USB HID joysticks since it runs off of 5v and uses the same 
2 axis voltage divider configuration. 

I have not built the full circuit yet, nor have I ran it outside of my dev board or tested it 
on anything but a salvaged 2DS joystick I had lying around. 

Hopefully someone will find this useful
