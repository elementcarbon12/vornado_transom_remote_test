This is just a rough description for this right now.
The protocol for the Vornado Transom fan is very simple. One bit consists of a lang mark followed by a short space, and the otehr is a short mark followed by a long space. 
If I arbitraranny name the long mark bit as a 1 and the otehr a s a 0 then the bits are as follows:
1: ~1300 uS mark followed by a ~400uS space.
0: ~400uS mark followed by a ~1300 uS space.

It uses a standard 38khz carrier frequency for the remote. 


A command is a 12 bit pattern that repeats every 8ms. represented as hex, the commands are as follows:
Power: 0xD84
up arrow: 0xDC6
down arrow: 0xD82
airflow direction (top of the circle on the remote): 0xD81
Thermometer icon: 0xD82

This repo implements this but it's not yet working consistantly. I think the issue is that I need to drive the LED with a transistor rather than try to rely on the ESP8266 directly.
