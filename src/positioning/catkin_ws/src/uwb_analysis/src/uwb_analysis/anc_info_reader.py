#! /usr/bin/env python
import os 

def get_uwb_dict() : 
    
    file_pth = os.path.join(os.path.dirname(__file__), 'anchor_chungra_info.txt') 
    with open(file_pth, 'r') as rf: 
        lines = rf.readlines() 
    
    UWB_DICT = dict() 

    header = [] 

    for i, line in enumerate(lines): 

        line = line.replace('\n', '')
        line_info = line.split(' ')

        if i==0: 
            header = line_info  # ['anchor_num_id', 'anchor_str_id', 'anchor_pos_x', 'anchor_pos_y'] 
            
        else:  
            anc_num_id = str(line_info[0])
            anc_str_id = str(line_info[1])
            anc_pos_x = float(line_info[2])
            anc_pos_y = float(line_info[3]) 

            UWB_DICT[anc_str_id] = []
            UWB_DICT[anc_str_id].append(anc_num_id) 
            UWB_DICT[anc_str_id].append((anc_pos_x, anc_pos_y)) 

    return UWB_DICT 
