import numpy as np
from utils import *

def GetMultiTagPosGen(Pos2C,Va,tag_pos_b,Va_ind):
    ia = np.zeros(len(Va), dtype=int)
    Est_P  = np.zeros(len(Pos2C), dtype=complex)

    for a in range(len(Va)):
        if (Va[a]=='0') and (Va_ind[a] != 0):
            ia[a] = 0
        elif (Va[a]=='1') and (Va_ind[a] != 0):
            ia[a] = 1
        else:
            ia[a] = 0

    NB = len(Va)

    Est_C = sum((Pos2C[p, ia[p]] for p in range(len(Pos2C)))) / NB

    Est_H_p = sum(((Pos2C[p, ia[p]] - Est_C) / tag_pos_b[p] for p in range(len(Pos2C))))
    Est_H = angle(Est_H_p)

    Est_F = sum(abs((Est_C + tag_pos_b[p] * np.exp(1j*Est_H) - Pos2C[p, ia[p]])**2) for p in range(len(Pos2C)))

    return Est_F, Est_C, Est_H
