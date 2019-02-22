#ifndef ROVER_H
#define ROVER_H

// Mavlink related library
#include "serial_port.h"
#include <common/mavlink.h>
#include <ardupilotmega/ardupilotmega.h>
#include <unistd.h>

// Mavlink additionnal define
#define GUIDED 15
#define ARMED 128

// Rover hard setup:
#define BAUDRATE 57600
#define UARTNAME "/dev/ttyACM0"

class Rover
{
	private:
		int r_baudrate;
		char *r_uart_name;
		Serial_Port serial_port;
		uint8_t r_armed;
		uint8_t r_mode;
		
	public:
		// Constructor:
		Rover();
		
		// Get the current mode of the rover:
		int get_mode();
		
		// Get armed state of the rover:
		int get_armed();
		
		// Handle message from rover:
		int handle_message(mavlink_message_t *msg);
		
		// Recieved message:
		int recv_data();
		
		// Set rover in guided mode:
		int guided_mode();
		
		// Arme the rover (armed: state=1 / disarmed: state=0):
		int arm(int state);
		
		// Set the yaw and speed of the rover:
		int setAngleSpeed(float angle, float speed);
		
		// RC-overwrite the channel (not working...)
		int mavRCOVER(int angle, int speed);
		
};

#endif
