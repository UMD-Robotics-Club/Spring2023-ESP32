'''
This file was made with help from Riley Bruning and Quinn Henthorne
'''

import pygame
from SerialInterface.SerialInterface import SerialInterface
from math import pi as PI
from math import sin

serial = SerialInterface('ttyACM0', 115200) #TODO: Change this to the correct COM port

pygame.init()


control_axes = [0, 0, 0, 0, 0, 0]
deadzone_angle = 0.1

if __name__ == "__main__":
    # Set the width and height of the screen (width, height), and name the window.
    # screen = pygame.display.set_mode((500, 700))
    pygame.display.set_caption("Joystick example")

    # Used to manage how fast the screen updates.
    clock = pygame.time.Clock()

    # This dict can be left as-is, since pygame will generate a
    # pygame.JOYDEVICEADDED event for every joystick connected
    # at the start of the program.
    joysticks = {}

    done = False
    while not done:
        # Event processing step.
        # Possible joystick events: JOYAXISMOTION, JOYBALLMOTION, JOYBUTTONDOWN,
        # JOYBUTTONUP, JOYHATMOTION, JOYDEVICEADDED, JOYDEVICEREMOVED
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                done = True  # Flag that we are done so we exit this loop.

            if event.type == pygame.JOYBUTTONDOWN:
                print("Joystick button pressed.")
                if event.button == 0:
                    joystick = joysticks[event.instance_id]
                    if joystick.rumble(0, 0.7, 500):
                        print(f"Rumble effect played on joystick {event.instance_id}")

            if event.type == pygame.JOYBUTTONUP:
                print("Joystick button released.")

            # Handle hotplugging
            if event.type == pygame.JOYDEVICEADDED:
                # This event will be generated when the program starts for every
                # joystick, filling up the list without needing to create them manually.
                joy = pygame.joystick.Joystick(event.device_index)
                joysticks[joy.get_instance_id()] = joy
                print(f"Joystick {joy.get_instance_id()} connencted")

            if event.type == pygame.JOYDEVICEREMOVED:
                del joysticks[event.instance_id]
                print(f"Joystick {event.instance_id} disconnected")

        # Get count of joysticks.
        joystick_count = pygame.joystick.get_count()


        # For each joystick:
        for joystick in joysticks.values():

            # Usually axis run in pairs, up/down for one, and left/right for
            # the other. Triggers count as axes.
            axes_num = joystick.get_numaxes()

            for i in range(axes_num):
                axis = joystick.get_axis(i)
                control_axes[i] = axis

            buttons = joystick.get_numbuttons()

            for i in range(buttons):
                button = joystick.get_button(i)
        
        
        # handle the steering
        
        if abs(control_axes[0]) < deadzone_angle:
            control_axes[0] = 0
        if abs(control_axes[1]) < deadzone_angle:
            control_axes[1] = 0
            
        if control_axes[0] != 0 or control_axes[1] != 0:
            serial.setTargetPose(control_axes[0]*255, control_axes[1]*255)
            print(control_axes[0], control_axes[1])
                        
        # Limit to 30 frames per second.
        clock.tick(30)
    # If you forget this line, the program will 'hang'
    # on exit if running from IDLE.
    pygame.quit()