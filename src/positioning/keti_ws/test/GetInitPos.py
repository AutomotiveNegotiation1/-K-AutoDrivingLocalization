
import numpy as np
from TwoAnchPos3 import TwoAnchPos3
from GetMultiTagPosGen import GetMultiTagPosGen

def GetInitPos(xa, ya, dist, anch_pos, tag_pos_b, Ln, Lp):

    Sel_F = 10000000
    Prob2 = 0
    Pos2C = np.zeros((Lp, 2), dtype=complex)
    Est_F = np.zeros(2 ** Lp)
    Est_C = np.zeros(2 ** Lp, dtype=complex)
    Est_H = np.zeros(2 ** Lp)
    Cand_F = 0
    Cand_C = 0j
    Cand_H = 0
    Sel_C = 0
    Sel_H = 0

    for L1 in range(Ln):
        for L2 in range(L1 + 1, Ln):

            for Pn in range(Lp):
                Pos2, Prob2 = TwoAnchPos3([xa[L1], xa[L2]], 
                                          [ya[L1], ya[L2]], 
                                          [dist[L1, Pn], dist[L2, Pn]], 
                                          [0, 0], [0, 0], xa + 1j * ya, dist[:, Pn])
                Pos2C[Pn, 0] = Pos2[0, 0] + 1j * Pos2[0, 1]
                Pos2C[Pn, 1] = Pos2[1, 0] + 1j * Pos2[1, 1]
            
            for Qn in range(2 ** Lp):
                Va = list(map(int, bin(Qn)[2:].zfill(Lp)))
                Est_F[Qn], Est_C[Qn], Est_H[Qn] = GetMultiTagPosGen(Pos2C, Va, tag_pos_b, [1, 1, 1, 1])
            
            val, ind = np.min(Est_F), np.argmin(Est_F)
            Cand_F = val
            Cand_C = Est_C[ind]
            Cand_H = Est_H[ind]
            
            if Sel_F > Cand_F:
                Sel_F = Cand_F
                Sel_C = Cand_C
                Sel_H = Cand_H
    
    heading_est = Sel_H
    tag_pos_est = Sel_C + tag_pos_b * np.exp(1j * Sel_H)

    return tag_pos_est, heading_est
