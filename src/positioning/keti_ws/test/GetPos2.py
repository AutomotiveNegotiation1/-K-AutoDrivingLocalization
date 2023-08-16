import numpy as np
from utils import *
from TwoAnchPos3 import TwoAnchPos3
from GetMultiTagPosGen import GetMultiTagPosGen

def GetPos2(xa, ya, dist, RxID, tag_pos_b, Ln, PP, PredPos):
    Pos2C = np.zeros((len(PredPos), 2), dtype=complex)
    Pos2C[:, 0] = PredPos
    Va_ind = np.zeros(len(PredPos), dtype=int)

    Sel_F = 1e7
    Est_F = np.zeros(2)
    Est_C = np.zeros(2, dtype=complex)
    Est_H = np.zeros(2)

    Cand_F = 0
    Cand_C = 0 + 0j
    Cand_H = 0
    Sel_C = 0 + 0j
    Sel_H = 0
    cand_tag_pos = 0 + 0j

    for L1 in range(Ln):
        for L2 in range(L1+1, Ln):
            for Pn in range(PP):
                Pos2, Prob2 = TwoAnchPos3([xa[RxID[L1]], xa[RxID[L2]]], [ya[RxID[L1]], ya[RxID[L2]]], 
                                          [dist[L1], dist[L2]], np.array([0, 0]), np.array([0, 0]), xa[RxID]+1j*ya[RxID], dist)
                Pos2C[Pn, 0] = Pos2[0, 0] + 1j*Pos2[0, 1]
                Pos2C[Pn, 1] = Pos2[1, 0] + 1j*Pos2[1, 1]
            for Qn in range(2):
                Va = reverse(dec2bin(2**(PP*Qn)-1, len(PredPos)))
                Va_ind[PP-1] = 1
                Est_F[Qn], Est_C[Qn], Est_H[Qn] = GetMultiTagPosGen(Pos2C, Va, tag_pos_b)
            Cand_F = min(Est_F)
            Cand_C = Est_C[np.argmin(Est_F)]
            Cand_H = Est_H[np.argmin(Est_F)]
            if Sel_F > Cand_F:
                Sel_F = Cand_F
                Sel_C = Cand_C
                Sel_H = Cand_H
                cand_tag_pos = Pos2C[PP-1, np.argmin(Est_F)]

    heading_est = Sel_H
    tag_pos_est = Sel_C + tag_pos_b * np.exp(1j*Sel_H)

    return tag_pos_est, heading_est, cand_tag_pos