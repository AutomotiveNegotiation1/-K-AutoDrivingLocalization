import numpy as np
def dec2bin(n, fill):
    return format(n, '0{}b'.format(fill))

def angle(z):
    return np.angle(z)

def reverse(s): 
    return s[::-1]