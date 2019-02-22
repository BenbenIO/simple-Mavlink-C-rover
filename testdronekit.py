from dronekit import connect, VehicleMode, LocationGlobal, LocationGlobalRelative
from pymavlink import mavutil
import time
import math


def send_YAW_SPEED(rover, yaw, speed):
    # check speed value
    if((speed < 0) or (speed > 1)):
        speed = 0
    
    # Format the data
    confirmation = 0
    msg = rover.message_factory.command_long_encode(sysID, compID, 213, confirmation,yaw,speed,0, 0, 0, 0, 0)
    
    # Send:
    print("Sending: yaw {} | speed {}".format(yaw, speed))
    rover.send_mavlink(msg)



#Rover config:
sysID = 1
compID = 1
rover = connect("/dev/ttyAMA0")
print " Mode: %s" % rover.mode.name

# Set to guide mode
rover.mode = VehicleMode('GUIDED')

# Armed the rover
rover.armed = True
  

while(1):
    
    time.sleep(1)
    send_YAW_SPEED(rover, 0, 0.5)
