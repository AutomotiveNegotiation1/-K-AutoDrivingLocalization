import numpy as np
from scipy.linalg import expm
from GetMultiTagPosGen2 import GetMultiTagPosGen2
from TwoAnchPos3 import TwoAnchPos3

def dec2bin(n, width):
    return bin(n)[2:].zfill(width)

def GetPos3(xa, ya, dist, RxID, tag_pos_b, Ln, PP, PredPos, DistPrev, RxIDprev, RxIDprevLen, PPprev):
    Pos2C = np.zeros((len(PredPos), 2), dtype=complex)
    Pos2C[:, 0] = PredPos
    Va_ind = np.zeros(len(PredPos), dtype=int)
    Sel_F = 10000000
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
        for L2 in range(L1 + 1, Ln):
            for Pn in range(PP, PP + 1):
                Pos2, Prob2 = TwoAnchPos3([xa[RxID[L1]], xa[RxID[L2]]],
                                          [ya[RxID[L1]], ya[RxID[L2]]],
                                          [dist[L1], dist[L2]], np.array([0, 0]).reshape(1,-1), np.array([0, 0]).reshape(1,-1),
                                          np.array(xa[RxID] + 1j * ya[RxID]).reshape(1,-1), np.array(dist).reshape(-1,1))
                Pos2C[Pn, 0] = Pos2[0, 0] + 1j * Pos2[0, 1]
                Pos2C[Pn, 1] = Pos2[1, 0] + 1j * Pos2[1, 1]
            for Qn in range(2):
                # Va = reversed(bin(2 ** (PP * Qn) - 1)[2:].zfill(len(PredPos)))
                Va = ''.join(reversed(dec2bin(2 ** (PP * Qn) - 1, len(PredPos))))
                Va_ind[PP] = 1
                Est_F[Qn], Est_C[Qn], Est_H[Qn] = GetMultiTagPosGen2(Pos2C, Va, tag_pos_b, Va_ind, DistPrev, RxIDprev, RxIDprevLen, xa, ya, PPprev)
            val, ind = np.min(Est_F), np.argmin(Est_F)
            Cand_F = val
            Cand_C = Est_C[ind]
            Cand_H = Est_H[ind]
            if Sel_F > Cand_F:
                Sel_F = Cand_F
                Sel_C = Cand_C
                Sel_H = Cand_H
                cand_tag_pos = Pos2C[PP, ind]

    heading_est = Sel_H
    # tag_pos_est = Sel_C + tag_pos_b * expm(1j * Sel_H)
    tag_pos_est = Sel_C + tag_pos_b * np.exp(1j * Sel_H)
    return tag_pos_est, heading_est, cand_tag_pos

