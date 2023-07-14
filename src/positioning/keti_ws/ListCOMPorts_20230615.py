import os
import argparse
import serial.tools.list_ports
import subprocess
import json

def parse_options():
    parser = argparse.ArgumentParser(description='List COM ports')
    parser.add_argument('-v', '--verbose', action='count', default=0, help='Increase verbosity')
    parser.add_argument('-vid', '--vid', type=str, help='Specify VID')
    parser.add_argument('-pid', '--pid', type=str, help='Specify PID')
    return parser.parse_args()

def create_udev_rule(device, new_name):
    udev_rules_path = '/etc/udev/rules.d/99-usb-serial.rules'
    udev_rule = 'KERNEL=="{}", SYMLINK+="{}"\n'.format(device.replace("/dev/", ""), new_name)

    with open(udev_rules_path, 'r') as f:
        lines = f.readlines()

    # check if rule already exists
    for i, line in enumerate(lines):
        if line.startswith('KERNEL=="{}"'.format(device.replace("/dev/", ""))):
            lines[i] = udev_rule  # replace the rule
            break
    else:  # if the rule was not found, append it
        lines.append(udev_rule)

    with open(udev_rules_path, 'w') as f:
        f.writelines(lines)

def reload_udev_rules():
    subprocess.run(['udevadm', 'control', '--reload-rules'])
    subprocess.run(['udevadm', 'trigger'])

def list_com_ports(verbose, vid, pid):
    serial_order = ['000760178940', '000760179526', '000760009648', '000760009277'] # The desired order of serial numbers
    baud_rate_mapping = {'/dev/ttyUWB0': 115200, '/dev/ttyUWB1': 115200, '/dev/ttyUWB2': 115200, '/dev/ttyUWB3': 115200, '/dev/ttyIMU': 230400} # add other ports and their baud rates here

    if verbose:
        print("Searching for COM ports...")

    ports = [port for port in serial.tools.list_ports.comports() if (vid is None or port.vid == vid) and (pid is None or port.pid == pid)]
    ports_with_serial = [port for port in ports if port.serial_number is not None]
    ports_with_serial.sort(key=lambda port: serial_order.index(port.serial_number)) # Sort by the order in serial_order
    ports_without_serial = [port for port in ports if port.serial_number is None]

    port_info = []
    for i, port in enumerate(ports_with_serial + ports_without_serial):
        print(f"{port.device} - {port.manufacturer} - {port.serial_number}")
        if port.serial_number is not None:
            port_name = "ttyUWB{}".format(i)
            create_udev_rule(port.device, port_name)
            baud_rate = baud_rate_mapping.get(port.device, 115200) # default to 115200 if the port is not in the mapping
            port_info.append({"type": "UWB", "port": port_name, "baud_rate": baud_rate})
        else:
            port_name = "ttyIMU"
            create_udev_rule(port.device, port_name)
            baud_rate = baud_rate_mapping.get(port.device, 230400) # default to 230400 if the port is not in the mapping
            port_info.append({"type": "IMU", "port": port_name, "baud_rate": baud_rate})

    print(json.dumps(port_info, indent=4))
    reload_udev_rules()

if __name__ == "__main__":
    args = parse_options()
    list_com_ports(args.verbose, args.vid, args.pid)
