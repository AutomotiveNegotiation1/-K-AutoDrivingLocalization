import os
import subprocess
import json
import argparse

def parse_options():
    parser = argparse.ArgumentParser(description='Control execution of UWB and IMU')
    parser.add_argument('--IMU', action='store_false', default=True, help='Disable IMU if this flag is provided')
    parser.add_argument('--UWB', action='store_false', default=True, help='Disable UWB if this flag is provided')
    parser.add_argument('--disable_ports', type=str, default='', help='Comma separated list of ports to disable')
    return parser.parse_args()

def main():
    args = parse_options()
    # Convert the disable_ports string into a list of ports
    disabled_ports = args.disable_ports.split(',')

    # Run the ListCOMPorts.py script and get the output
    result = subprocess.run(['sudo', 'python3', 'ListCOMPorts.py'], stdout=subprocess.PIPE)
    # Decode the output and convert from JSON
    ports = json.loads(result.stdout.decode())

    # For each port in the list
    for num, port in enumerate(ports):
        if port["port"] in disabled_ports:
            continue # Skip this port if it's in the list of disabled ports
            
        # New: Change permissions for each port
        os.system(f'sudo chmod 777 /dev/{port["port"]}')

        if port["type"] == "IMU" and args.IMU:
            # Construct the roslaunch command for IMU
            # command = f"roslaunch localizer_zed_f9r zed_f9r.launch port:='{port['port']}' baud_rate:='{port['baud_rate']}' &"
            command = f"roslaunch localizer_zed_f9r zed_f9r.launch &"
            # Execute the command
            os.system(command)

        if port["type"] == "UWB" and args.UWB:
            # Construct the roslaunch command for UWB
            # command = f"roslaunch localizer_dwm1001 dwm1001.launch node_index:='{num}' port:='{port['port']}' baud_rate:='{port['baud_rate']}' &"
            command = f"roslaunch localizer_dwm1001 dwm1001.launch &"
            # Execute the command
            os.system(command)

if __name__ == "__main__":
    main()
