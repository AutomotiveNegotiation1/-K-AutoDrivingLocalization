import time
from TestTotal import TestTotal

# Start the timer
start_time = time.time()

# Assuming the TestTotal function is defined elsewhere in your code and returns the required values
tag_pos_est, heading_est, tag_pos_est_aver, headingest_a_aver_v = TestTotal()

# Print the elapsed time
end_time = time.time()
print("Elapsed time:", end_time - start_time, "seconds")
