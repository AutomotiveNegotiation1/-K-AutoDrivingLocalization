import glob 
from pathlib import Path  

if __name__ == "__main__": 

    root = Path('/media/keti/xavier_SSD/Datasets/bagfiles/inout_ai_images') 

    train_folder = root / 'train'
    valid_folder = root / 'valid' 
    test_folder = root / 'test'


    with open('./valid.txt', 'a') as wf: 

        for datafile in valid_folder.iterdir(): 
            #print(datafile.name)
            for p in datafile.iterdir(): 
                
                class_name = p.name 
                #print(class_name)
                for pp in p.iterdir(): 
                    #print(pp) 
                    wf.writelines(str(pp)+'\t'+ str(class_name)+'\n') 
    
    
    with open('./train.txt', 'a') as wf: 

        for datafile in train_folder.iterdir(): 
            #print(datafile.name)
            for p in datafile.iterdir(): 
                
                class_name = p.name 
                #print(class_name)
                for pp in p.iterdir(): 
                    #print(pp) 
                    wf.writelines(str(pp)+'\t'+ str(class_name)+'\n') 
    
    with open('./test.txt', 'a') as wf: 

        for datafile in test_folder.iterdir(): 
            #print(datafile.name)
            for p in datafile.iterdir(): 
                
                class_name = p.name 
                #print(class_name)
                for pp in p.iterdir(): 
                    #print(pp) 
                    wf.writelines(str(pp)+'\t'+ str(class_name)+'\n') 
    