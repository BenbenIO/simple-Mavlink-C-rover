#include "rover.h"

Rover::Rover():serial_port((char*)UARTNAME, BAUDRATE)
{
	//initilisation of the serial communication:
	printf("Serial - interface start\n");
	serial_port.start();
	printf("Rover serial connection established\n");

}

int Rover::get_mode()
{
	//printf("get_mode call: %d\n", r_mode);
	return(r_mode);
}

int Rover::get_armed()
{
	//printf("get_armed call: %d\n",r_armed);
	return(r_armed);
}

int Rover::handle_message(mavlink_message_t *msg)
{
	mavlink_heartbeat_t hb;
	
	switch (msg->msgid)
	{
		case MAVLINK_MSG_ID_HEARTBEAT:
		{
			printf("MAVLINK_MSG_ID_HEARTBEAT\n");
			mavlink_msg_heartbeat_decode(msg, &hb);
			printf("SystemID: %d\n", msg->sysid);
			printf("Component ID: %d\n", msg->compid);
			printf("status: %d\n", hb.system_status);
			printf("custom mode: %d\n", hb.custom_mode);
			printf("autopilot: %d\n", hb.autopilot);
			printf("type: %d\n", hb.type);
			//printf("base_mode: %d\n", hb.base_mode);
			printf("system_status: %d\n",hb.system_status);
			
			// Check the arm status:
			int armed_state = hb.base_mode & MAV_MODE_FLAG_SAFETY_ARMED;
			printf("Armed? %s \n", armed_state ? "yes" : "no");
			//printf("raw: %d", armed_state);
			printf("\n");
			
			// Setup the rover parameters:
			r_mode = hb.custom_mode;
			r_armed = armed_state;
			return(1);
		}
		default:
		{
			//printf("Warning, did not handle message id %i\n",msg->msgid);
			return(-1);
		}
	}		
}

int Rover::recv_data()
{
	bool success;
	mavlink_message_t msg;
	
	success = serial_port.read_message(msg); 
	
	if(success)
	{
		int id = handle_message(&msg);
		return(id);
	}
	return(-1);
}

int Rover::guided_mode()
{
	// Format the data:
	mavlink_command_long_t set_mode = {0};
	set_mode.target_system = 1;
	set_mode.target_component = 0;
	set_mode.command = MAV_CMD_DO_SET_MODE;		//176
	set_mode.confirmation = true;
	set_mode.param1 = 1; 				//need to be 1 ?? check			 	
	set_mode.param2 = GUIDED; 			//guided_mode: 4 for drone / 15 for rover... 
	
	// Encode:
	mavlink_message_t msg;
	mavlink_msg_command_long_encode(1, 255, &msg, &set_mode);
		
	// Write in the serial:
	int len = serial_port.write_message(msg);
	//printf("Guided mode (%d)\n", len);
	return(len);
}

int Rover::arm(int state)
{
	// Format the data:
	mavlink_command_long_t armed = {0};
	armed.target_system = 1;
	armed.target_component = 0;
	armed.command = MAV_CMD_COMPONENT_ARM_DISARM; //400
	armed.confirmation = true;
	armed.param1 = (int) state;
	
	// Encode:
	mavlink_message_t msg;
	mavlink_msg_command_long_encode(1, 255, &msg, &armed);
		
	// Write in the serial:
	int len = serial_port.write_message(msg);
	//printf("Rover armed = %d (%d)\n",state, len);
	return(len);
}

int Rover::setAngleSpeed(float angle, float speed)
{
	//Valid input:
	if(( speed < 0.0 ) || ( speed > 1.0 ))
	{
		printf("print invalid speed input: [0-1] * cruise speed\n");
		speed = 0.0;
	}

	// Format the data:
	mavlink_command_long_t set_yawspeed = {0};
	set_yawspeed.target_system = 1;
	set_yawspeed.target_component = 0;
	set_yawspeed.command = MAV_CMD_NAV_SET_YAW_SPEED; //; 	//213 (MAV_CMD_DO_SET_POSITION_YAW_THRUST)
	set_yawspeed.confirmation = false;
	set_yawspeed.param1 = angle;			 	//angle (centridegree) [-4500 - 4500]
	set_yawspeed.param2 = speed;	 			//speed normalized [0 - 1]
	
	// Encode:
	mavlink_message_t msg;
	mavlink_msg_command_long_encode(1, 255, &msg, &set_yawspeed);
		
	// Write in the serial:
	int len = serial_port.write_message(msg);
	printf("Send angle: %f | speed: %f\n", angle, speed);
	return(len);
}

int Rover::mavRCOVER(int angle, int speed)
{
	// Format the data:
	mavlink_rc_channels_override_t rcover = {0};
	rcover.target_system = 1;
	rcover.target_component = 0;
	rcover.chan1_raw = (int) angle; //[1000 - 1500]
	rcover.chan3_raw = (int) speed; //[1000 - 1500]
	
	// Encode:
	mavlink_message_t msg;
	int lenover = mavlink_msg_rc_channels_override_encode(1, 1, &msg, &rcover);
	printf("mavl lenover %d\n", lenover);
	
	// Write in the serial:
	int len = serial_port.write_message(msg);
	printf("Send angle: %d | speed: %d\n", angle, speed);
	printf("len: %d\n", len);
	return(len);
}	
