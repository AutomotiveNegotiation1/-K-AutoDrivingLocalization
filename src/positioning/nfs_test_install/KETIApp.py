import os
import re
import signal
import subprocess
import sys
import time
import threading

# tag_data = {
#     1: {},
#     2: {},
#     3: {},
#     4: {}
# }

username = "umaps"
groupname = "umaps"


# 로그 파일에 쓰기 위한 락 객체
log_lock = threading.Lock()

def shutdown(signum, frame):
    global ros_pid
    os.kill(ros_pid, signal.SIGTERM)
    exit(0)

def is_interactive():
    return hasattr(sys.stdin, 'isatty') and sys.stdin.isatty()

# Start roscore in the background
os.system('source /opt/ros/noetic/setup.bash')  # noetic은 ROS 버전에 따라 변경될 수 있습니다.
roscore_process = subprocess.Popen(['roscore'])
ros_pid = roscore_process.pid

time.sleep(3)  # Wait for roscore to fully launch

# Handle SIGINT (CTRL+C) signals
signal.signal(signal.SIGINT, shutdown)

# log_file_path = "/srv/nfs_client/logfile.log"
log_file_name = "logfile.log"
file_path = "/mnt/nfs_client"
log_file_path = os.path.join(file_path, log_file_name)

# 파일이 없는 경우 파일을 생성합니다.
if not os.path.exists(log_file_path):
    with open(log_file_path, 'w') as f:
        f.close()
    os.chmod(log_file_path, 0o666)
        

# tag_data는 이 함수 외부에서 어딘가에 정의되어 있어야 합니다.
# 예: tag_data = {}
tag_data = {}

def parse_and_log(tag_id, data_type, output):
    lines = output.split("\n")
    ids, xs, ys, zs, distances = [], [], [], [], []
    id_lines = []
    collecting_ids = False
    
    coords = {"x": None, "y": None, "z": None, "w": None}

    if 'DIST' in data_type:
        for line in lines:
            if "id:" in line:
                collecting_ids = True
                id_lines.append(line)
                continue
            
            if collecting_ids and line.startswith("- "):
                id_lines.append(line)
                continue
            else:
                collecting_ids = False
            
            split_by_bracket = line.split('[')
            if len(split_by_bracket) > 1:
                data_inside_brackets = split_by_bracket[1].split(']')[0].replace(" ", "").split(",")
                if "x:" in line:
                    xs = data_inside_brackets
                elif "y:" in line:
                    ys = data_inside_brackets
                elif "z:" in line:
                    zs = data_inside_brackets
                elif "distanceFromTag:" in line:
                    distances = data_inside_brackets

        id_string = " ".join(id_lines)
        ids = re.findall(r"- '([^']+)'|- ([^\s]+)", id_string)
        ids = [x for tup in ids for x in tup if x]  # Flatten and remove empty strings
        
        tag_data[tag_id] = tag_data.get(tag_id, {})
        if isinstance(tag_data[tag_id], dict):  # Check if it's a dictionary
            tag_data[tag_id]["DIST"] = f"DIST,{len(ids)},{','.join([f'AN{i},{ids[i]},{xs[i]},{ys[i]},{zs[i]},{distances[i]}' for i in range(len(ids))])}"

    if 'POS' in data_type:
        for line in lines:
            for key in coords.keys():
                if f"{key}:" in line:
                    coords[key] = float(line.split(":")[1].strip())
        
        if all(val is not None for val in coords.values()):
            coords_str = f"{coords['x']:.2f},{coords['y']:.2f},{coords['z']:.2f},HEAD,{coords['w']:.2f}"
            tag_data[tag_id] = tag_data.get(tag_id, {})
            tag_data[tag_id]["POS"] = f"POS,{coords_str}"


    if "POS" in tag_data.get(tag_id, {}):
        # dist_data = tag_data[tag_id]["DIST"]
        pos_data = tag_data[tag_id]["POS"]
        tag_data[tag_id]['COMBINED'] = f"1,{pos_data}"



# log_to_file 함수에서의 변경점
def log_to_file(tag_data):
    with log_lock:
        with open(log_file_path, 'a') as f:
            for tag_id, data in tag_data.items():
                combined_data = data.get('COMBINED', 'N/A')
                if combined_data != 'N/A':
                    f.write(f"{combined_data}\n")
                else:
                    dist_data = data.get('DIST', 'N/A')
                    pos_data = data.get('POS', 'N/A')
                    if dist_data != 'N/A' or pos_data != 'N/A':
                        f.write(f"Tag{tag_id}:DIST,{dist_data},POS,{pos_data}\n")
                # print("Data written to log.")
                f.flush()
                
                
def run_rostopic_echo(topic1, topic2, tag_id):
    if tag_data.get(tag_id, {}).get('COMBINED'):
        # Skip further processing if 'COMBINED' is already set for this tag_id
        return
    process1 = subprocess.Popen(
        ["rostopic", "echo", "-c", topic1],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1
    )
    
    process2 = subprocess.Popen(
        ["rostopic", "echo", "-c", topic2],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1
    )
    
    threads = []
    threads.append(threading.Thread(target=process_topic_output, args=(process1, tag_id, "DIST")))
    threads.append(threading.Thread(target=process_topic_output, args=(process2, tag_id, "POS")))
    
    for thread in threads:
        thread.start()
    
    for thread in threads:
        thread.join()

def process_topic_output(process, tag_id, data_type):
    full_output = ""
    while True:
        line = process.stdout.readline().strip()
        if line:
            full_output += line + "\n"
            if '---' in line:
                parse_and_log(tag_id, data_type, full_output)
                full_output = ""
                
            # process_topic_output 함수에서의 변경점
            if all(tag in tag_data and "DIST" in tag_data[tag] and "POS" in tag_data[tag] for tag in [1, 2, 3, 4]):
                log_to_file(tag_data)
                # 데이터 초기화: 키만 삭제
                for tag in [1, 2, 3, 4]:
                    tag_data[tag].pop('DIST', None)
                    tag_data[tag].pop('POS', None)
                    tag_data[tag].pop('COMBINED', None)
            # 프로세스 종료 확인
            if process.poll() is not None:
                break

def create_log():
    topics_and_tags = [
        ("/dwm1001/anchor/ttyUWB0", "/Position", 1),
        ("/dwm1001/anchor/ttyUWB1", "/Position", 2),
        ("/dwm1001/anchor/ttyUWB2", "/Position", 3),
        ("/dwm1001/anchor/ttyUWB3", "/Position", 4),
        # ... more topics and tags
    ]

    threads = []
    for topic1, topic2, tag_id in topics_and_tags:
        thread = threading.Thread(target=run_rostopic_echo, args=(topic1, topic2, tag_id))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()



# Function to play a bag file
def play_bag():
    bag_files = [f for f in os.listdir("./rosbag/") if f.endswith(".bag")]
    bag_files.append("back")
    selected_bag = handle_selection(play_selected_bag, bag_files)

def play_selected_bag(bag_file):
    if bag_file == "back":
        return
    rosplay_process = subprocess.Popen(["rosbag", "play", f"./rosbag/{bag_file}"])

def handle_selection(func, items):
    for i, item in enumerate(items):
        print(f"{i+1}: {item}")
    try:
        if is_interactive():
            selection = input("Select an option: ")
        else:
            print("Not running interactively.")
            return  # or other logic
    except EOFError:
        print("EOF detected. Exiting.")
        sys.exit(0)  # Using sys.exit() instead of exit()
    except Exception as e:  # Optional: To catch other exceptions and print them
        print(f"An exception occurred: {e}")
        sys.exit(1)

    if not selection.isnumeric():
        print("Invalid selection.")
        return

    selected_index = int(selection) - 1
    if 0 <= selected_index < len(items):
        func(items[selected_index])
    else:
        print("Invalid selection.")

def main():
    play_bag()
    create_log()
    

if __name__ == "__main__":
    main()
