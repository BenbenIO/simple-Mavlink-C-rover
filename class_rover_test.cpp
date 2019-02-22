#include "rover.h"

// to compile: g++ -I mavlink/include/mavlink/v2.0 class_rover_test.cpp serial_port.cpp rover.cpp -o class
 


int main()
{
	// Create a rover:
	Rover roverA;
	
	// Set to guided mode:
	roverA.guided_mode();
	printf("Setting guide mode...\n");
	while((roverA.get_mode() != GUIDED)){
		roverA.recv_data();
		roverA.guided_mode();
		}
	sleep(0.5);
	printf("\nDone! Arming the rover...\n");
	
	// Wait until the rover is armed and in standby state:
	roverA.arm(1);
	while(roverA.get_armed() != ARMED){
		roverA.recv_data();
		roverA.arm(1);
		}
	
	printf("\nDone! Ready to go...\n");
	sleep(1);
	printf("Try to send NavSetYaw msg...\n");

	while(1)
	{
				
		roverA.setAngleSpeed(0.0, 0.2);
		sleep(1);
	}
	
	roverA.arm(0);	
	
	return(0);
}
