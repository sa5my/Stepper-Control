# Stepper-Control
This is a system used to run and configure the speed of a stepper motor via RS-485 communication protocol.

The system comprises of two parts:

1. Handheld Device
2. MotorEnd Device

The handheld device is responsible for configuring the following three modes and speed from (0-25RPM) of 1.8deg step angle stepper motor.

	i. Jog Mode: This drives the motor as long as the key stays pressed
	ii. Incremental Mode: This only moves the stepper motor by 0.1125 deg(6.75") at 1/32 microstep selection when the key is pressed once.
	iii. Automatic Mode: This mode move the motor by a step limit , wait and move the end limit.
											 The step limit, wait time and end limits are selectable by the user.
											 
Hardware Used:

Handheld Device: 

1. Arduino Mega
2. ILI9341 LCD Module
3. 1*4 keypad
4. Push buttons
5. MAX-485 Module

MotorEnd Device:

1. Arduino Uno/Mega
2. MAX-485 Module
3. TP6600 Driver Module
4. Stepper Motor (20kg-cm torque)
5. SMPS- 24V/5A rating

