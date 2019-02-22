# Simple Mavlink C++ example: Rover
<br>Mavlink (Micro Air Vehicle Link) is a protocol for unmanned vehicle (drone / rover). It is used to communication with the mobile from the ground control station or companion computer. This repository aims to provide an easy explanation on Mavlink, how to communication with a Pixhawk and link useful resources. </br>
<br>The proposed code is used to control a small rover equiped with Pixhawk2 from a companion computer (Raspberry3) in C++ using mavlink v2.</br>
<br>Finally, if your project can be done in python, I recommend using the [dronekit](http://python.dronekit.io/guide/quick_start.html) library, which is very complet and easy to use, you can still use the resources to get a better understanding of mavlink protocol.</br>

<br>The first experience with Mavlink - Ardupiplot - Mission planner can be difficult but I hope this documents will help you. __If you see any mistake or want to help please feel free to push any change.__</br>

# Mavlink
## Architecture

<p align="center">
  <img src="/images/structure.PNG" width="500">
</p>

## Packet structure

<p align="center">
  <img src="/images/packet.png" width="400">
</p>

* __STX__: Packet start sequence
* __LEN__: Payload length ( 0 - 255)
* __SEQ__: sequence number for each component to detect packet loss
* __SYS__: System ID. The system ID is of the SENDING system. 
* __COMP__: Component ID. The Component ID is the identifier of the message target inside the autopilot. I only used 1 component, but if you had component to your pixhawk (Gimbal - ToF sensor) you can specify the target. COMP = 0 for broadcast in the autopilot.
* __MSG__: Message - command ID. Mavlink assigns an unique ID for a command ([list](https://mavlink.io/en/messages/common.html)) 
* __PAYLOAD__: size depends on the specific commands and number of parameters
* __CKA - CKB__: check sum

## Program structure
If you want to design your own mavlink message communication program, you will need to ensure the following step:
1. Ensure connectivity with the vehicle: UART - UDP - TCP
2. Receive data from the vehicle: 
    1. Parse incoming data: _mavlink_parse_char_ byte-to-byte basis
    2. Handling the message: _mavlink_decode_ decode message based on the message ID
    3. Example: 1) heartbeat to get all setting information 2) Other requested data
3. Send data to vehicle
    1. Format a new mavlink message structure with target id / command id / parameters...
    2. Pack and write the message to the communication port: _mavlink_finalize_message_ 

# Mavlink message in C++
## Starting point
The best (only) official example for C++ mavlink message is [c_uart_interface_example](https://github.com/mavlink/c_uart_interface_example). It shows a complete solution to control an autopilot, but it may looks a bit complicated (at least for me). So I only used the structure and the serial communication functions.
## Code sample
compile with ``` g++ -I mavlink/include/mavlink/v2.0 class_rover_test.cpp serial_port.cpp rover.cpp -o class```

```c
int main()
{
	// Create a rover:
	Rover roverA;
	
	// Set to guided mode:
	roverA.guided_mode();
	printf("Setting guide mode...\n");
	sleep(0.5);
	printf("\nDone! Arming the rover...\n");
	roverA.arm(1);
	printf("\nDone! Ready to go...\n");
	sleep(0.5);
  
	roverA.setAngleSpeed(30, 0.4);
	sleep(4);
	roverA.arm(0);	
	
	return(0);
}

```
## Used function
I recommend to search on both Ardupilot - Mavlink command list to make sure the message is supported by ardupilot, that there is no format change and for a better understanding of the message. For example:
#### List
* https://github.com/ArduPilot/ardupilot/blob/master/APMrover2/Rover.h
* https://github.com/mavlink/c_library_v2/blob/master/common/common.h
#### Change mode (guided mode=15):
* https://mavlink.io/en/messages/common.html#MAV_CMD_DO_SET_MODE
* http://ardupilot.org/rover/docs/common-mavlink-mission-command-messages-mav_cmd.html#mav-cmd-do-set-mode
* https://github.com/ArduPilot/ardupilot/blob/0f5041e8737b08fa19f2128fa76ecb5c3658271b/APMrover2/mode.h
#### YAW - SPEED:
* https://mavlink.io/en/messages/common.html#MAV_CMD_NAV_SET_YAW_SPEED
* http://ardupilot.org/rover/docs/guided-mode.html?highlight=mav_cmd_nav_set_yaw_speed

## Currently working on
- [ ] Multi-thread for receiving-sending
- [ ] Generate local way point
- [ ] Battery monitoring message

# Resouces 
#### Mavlink for dummy
* https://diydrones.com/forum/topics/mavlink-tutorial-for-absolute-dummies-part-i?groupUrl=arducopterusergroup&groupId=705844%3AGroup%3A394475&id=705844%3ATopic%3A1472930&page=3#comments
* http://ardupilot.org/dev/docs/mavlink-basics.html
* https://www.youtube.com/watch?v=qum_S8GvDRw
#### Example
* http://ardupilot.org/dev/docs/raspberry-pi-via-mavlink.html
* https://discuss.ardupilot.org/t/mavlink-and-arduino-step-by-step/25566
#### Japanese explaination
* https://kazkojima.github.io/mavlink-howto.html
#### Clear and Good C++ example (on simulator):
* https://software.intel.com/en-us/articles/takeoff-and-landing-in-a-simulator-sitl
#### Clear description of the communication flow + sample code (on simulator)  +++
* https://software.intel.com/en-us/articles/takeoff-and-landing-in-a-simulator-sitl
#### Communication flow on CBG: good to have an inside  ++
* https://dev.qgroundcontrol.com/en/communication_flow.html
#### Mavlink message from the "drone": good to understand how the vehicle generate message ++
* https://github.com/danzimmerman/barebones_MAVLink/blob/master/barebones_MAVLink.ino
#### Give inside on Mavlink and industrial application +
* https://auterion.com/wp-content/uploads/2018/11/Auterion-Webinar-SDK-Presentation.pdf
#### Complete-Complex example with threading mgmt (for drone) +++
* https://github.com/rijesha/mavlink-interface
#### Good simple C++ example (with thread) ++
* https://github.com/SubhiH/mavlink_interface
#### Rc-overwrite example
* https://github.com/iamczar/mavlink_example/blob/master/main.cpp
#### Complet example inspired by the c_uart example (with multiplatform support)
* https://github.com/phildue/c_uart_mavlink
