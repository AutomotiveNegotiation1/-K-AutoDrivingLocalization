#!/usr/bin/env python3.8
""" For more info on the documentation go to https://www.decawave.com/sites/default/files/dwm1001-api-guide.pdf
"""

import rospy, time


class DWM1001ApiCommands:
      
        def __init__(self, instance, rate=10, verbose=False):                        
                self.instance = instance
                
                self.verbose = verbose
                self.rate = rospy.Rate(rate)
                # serial initializing
                self.instance.close()
                
                # sleep for one sec
                time.sleep(1)
                
                # open serial port
                self.instance.open()
                
                self.serialReadLine=None

        def commands(self, input_command):
                output_command = None                        
                if input_command == 'DOUBLE_ENTER':
                        output_command = b'\r\r'   # ASCII char for double Enter
                elif input_command == 'SINGLE_ENTER':
                        output_command = b'\r'     # ASCII char for single Enter        
                elif input_command == 'HELP':
                        output_command = b'?'      # Display help
                elif input_command == 'QUIT':
                        output_command = b'quit'   # Quit API shell mode
                elif input_command == 'GC':
                        output_command = b'gc'     # Clears GPIO pin
                elif input_command == 'GG':
                        output_command = b'gg'     # Reads GPIO pin level
                elif input_command == 'GS':
                        output_command = b'gs'     # Sets GPIO as output and sets its value
                elif input_command == 'SINGLE_ENTER':
                        output_command = b'gt'     # Toggle GPIO(must be an output)
                elif input_command == 'F':
                        output_command = b'f'      # Show free memory on the heap
                elif input_command == 'PS':
                        output_command = b'ps'     # Show info about running threads
                elif input_command == 'PMS':
                        output_command = b'pms'    # Show power managements tasks. IDL means that task is idle. USE means that task is allocated in the power management
                elif input_command == 'RESET':
                        output_command = b'reset'  # reset the dev board
                elif input_command == 'UT':
                        output_command = b'ut'     # Show device uptime
                elif input_command == 'FRST':
                        output_command = b'frst'   # Factory reset
                elif input_command == 'TWI':
                        output_command = b'twi'    # General purpose I2C/TWI read
                elif input_command == 'AID':
                        output_command = b'aid'    # Read ACC device ID
                elif input_command == 'AV':
                        output_command = b'av'     # Rad ACC values
                elif input_command == 'LES':
                        output_command = b'les'    # Show distances to ranging anchors and the position if location engine is enabled
                elif input_command == 'LEC':
                        output_command = b'lec'    # Show measurement and position in CSV format
                elif input_command == 'LEP':
                        output_command = b'lep'    # Show position in CSV format.Sending this command multiple times will turn on/off this functionality.
                elif input_command == 'SI':
                        output_command = b'si'     # System Info
                elif input_command == 'NMG':
                        output_command = b'nmg'    # Get node mode info
                elif input_command == 'NMO':
                        output_command = b'nmo'    # Enable passive offline option and resets the node
                elif input_command == 'NMP':
                        output_command = b'nmp'    # Enable active offline option and resets the node.
                elif input_command == 'NMA':
                        output_command = b'nma'    # Configures node to as anchor, active and reset the node.
                elif input_command == 'NMI':
                        output_command = b'nmi'    # Configures node to as anchor initiator, active and reset the node.
                elif input_command == 'NMT':
                        output_command = b'nmt'    # Configures node to as tag, active and reset the node
                elif input_command == 'NMTL':
                        output_command = b'nmtl'   # Configures node to as tag, active, low power and resets the node.
                elif input_command == 'BPC':
                        output_command = b'bpc'    # Toggle UWB bandwidth / tx power compensation.
                elif input_command == 'LA':
                        output_command = b'la'     # Show anchor list
                elif input_command == 'STG':
                        output_command = b'stg'    # Display statistics
                elif input_command == 'STC':
                        output_command = b'stc'    # Clears statistics
                elif input_command == 'TLS':
                        output_command = b'tlv'    # Parses given tlv frame, see section 4 for valid TLV commands
                elif input_command == 'AURS':
                        output_command = b'aurs'   # Set position update rate. See section 4.3.3 for more detail.
                elif input_command == 'AURG':
                        output_command = b'aurg'   # Get position update rate. See section 4.3.4 for more details
                elif input_command == 'APG':   
                        output_command = b'apg'    # Get position of the node.See section 3.4.2 for more detail
                elif input_command == 'APS':
                        output_command = b'aps'    # Set position of the node.See section 3.4.2for more detail
                elif input_command == 'ACAS':
                        output_command = b'acas'   # Configures node as anchor with given options
                elif input_command == 'ACTS':
                        output_command = b'acts'   # Configures node as tag with given options
                self.instance.write(output_command)
                if self.verbose == True:
                        rospy.loginfo("DWM1001 API Commands > ", input_command)
                
                
        def is_Open(self):
                if (self.instance.isOpen()):
                        if self.verbose == True:
                                rospy.loginfo("Port opened: "+ str(self.instance.name))
                        # start sending commands to the board so we can initialize the board
                        self.initialize_DWM1001API()
                        # give some time to DWM1001 to wake up
                        time.sleep(2)
                        # send command lec, so we can get positions is CSV format
                        self.commands('LEC')
                        self.commands('SINGLE_ENTER')
                        if self.verbose == True:
                                rospy.loginfo("Reading DWM1001 coordinates")
                else:
                        if self.verbose == True:
                                rospy.loginfo("Can't open port: "+ str(self.serialPortDWM1001.name))
        
        def initialize_DWM1001API(self):
                if self.verbose == True:
                        rospy.loginfo("Start DWM1001 API initializing...")
                        
                # reset incase previuos run didn't close properly
                self.commands('RESET')
                # send ENTER two times in order to access api
                self.commands('SINGLE_ENTER')
                # sleep for half a second
                time.sleep(0.5)
                self.commands('SINGLE_ENTER')
                # sleep for half second
                time.sleep(0.5)
                # send a third one - just in case
                self.commands('SINGLE_ENTER')
        
        def close(self):
                return self.instance.close()
        
        def raw(self):
                serialReadLine = self.instance.read_until().decode('utf-8')
                return serialReadLine
        
        def read(self):
                return self.raw(), self.parsed_data()
        
        def parsed_data(self):
                data = [x.strip() for x in self.serialReadLine.strip().split(',')]
                ANNum = int((len(data) - 7)/6)
                anchor = self.AnchorParsed(ANNum, data)
                tag = self.TagParsed(data)
        
        def AnchorParsed(self, ANNum, data):
                identity = self.AnchorIdentity(ANNum, data)
                pose = self.AnchorPose(ANNum, data)
                
        def AnchorIdentity(self, ANNum, data):
                ID = data[0]
                ANSerial = [data[3+6*i] for i in range(ANNum)]
                        
        def AnchorPose(self, ANNum, data):
                x = [data[4+6*i] for i in range(ANNum)]
                y = [data[5+6*i] for i in range(ANNum)]
                z = [data[6+6*i] for i in range(ANNum)]
                distance = [data[7+6*i] for i in range(ANNum)]
        
        def TagParsed(self, data):
                identity = self.TagIdentity(data)
                pose = self.TagPose(data)
                
        def TagIdentity(self, data):
                ID = data[-5]
                TASerial = self.instance.name
                
        def TagPose(self, data):
                x = data[-4]
                y = data[-3]
                z = data[-2]