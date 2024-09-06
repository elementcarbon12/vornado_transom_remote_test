Basic demonstration of emulating the IR remote for a Vornado Transom window fan.
This sketch is based around an ESP8266 and uses the IrRemoteESP8266 library. Code will need porting for other libraries.

In a nutshell, the fan's protocol looks like it's PWM based with a bit period of about 1680 microseconds and uses a standard 38KHz carrier frequency.
The two bits are a 75% duty cycle and a 25% duty cycle. Some slop is allowed.

However, to use the irsend.sendRaw() method, a command is defined as a set of marks and spaces. 
So the 75% duty cycle is defined as  a 1260us mark and 400us space. The 25% ducty cycle is defined as a 420us mark and 1260us space.
TODO: make 75% bit's space 420us and see if it breaks anything

For the this sketch, the 75% duty cycle bit will be assigned as a 1 and the 20% duty cycle as a 0.

Commands are 3 nibbles and need to be repeated at least twice. 
The gap between repeats of the command cannot be longer than about 7ms or it will be interpreted as a separate button press. 
A gap of about 9ms is enough for a second command to be sent while ensuring the fan's control panel doesn't sleep. 

Any correctly formatted 12 bit combination will make the fan attemt to decode it whether the command is valid or not.
This is useful for waking the fan's control panel before sending the desired command.

The following comamnds were determined by capturing the waveform across the IR LED of the offical remote with 1s and 0s defined as above.
The commands are recorded as 3 digit hexadecimal numbers.

Power button : 0xD84
Up arrow button : 0xDC6
Down arrow button : 0xD82
Direct mode button : 0xD81
Auto mode button (thermometer icon) : 0xDC3

I have not been able to deterine if there is an additional encoding scheme to these commands

A little fuzzing has determined some alternatives for some commands (usally 1 bit of difference in the last nibble)
Power button alternative: 0xD85
Down Arrow alternative: 0xD83

TODO: Automated fuzzing differnet 3 digit commnands of the interface to identfy any additional commands. 
I would like to determine if there are comands to explicityly set exhaust and direct modes rather than toggle between them (to enable setting a known state)
It would be nice if there was a better way to set the temperature in auto mode besides using the up and down arrows. The range of 60 to 90 (fahrenheit) means 'homing' the auto control value could take some time. 

There may be more to this protocol such as longer commands but that's not something I will persue.
For now, this will meet my goals of being able to turn the fan on and off and adust it's speed via an ESP8266 and MQTT. 
Auto control won't be used as I will use a home automation framework and additional temperature sensors to control when the fan runs and at what speed.