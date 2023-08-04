# distutils: language = c++
import numpy as np
cimport numpy as np
from TwoAnchPos3 cimport TwoAnchPos3
from GetMultiTagPosGen cimport GetMultiTagPosGen
from libc.math cimport exp

def GetInitPos(np.ndarray[np.double_t, ndim=1] xa, np.ndarray[np.double_t, ndim=1] ya, np.ndarray[np.double_t, ndim=2] dist, np.ndarray[np.complex128_t, ndim=1] anch_pos, np.ndarray[np.complex128_t, ndim=1] tag_pos_b, double Ln, double Lp):

    cdef:
        double Sel_F = 10000000
        double Prob2 = 0
        np.ndarray[np.complex128_t, ndim=2] Pos2C = np.zeros((Lp, 2), dtype=np.complex128)
        np.ndarray[np.double_t, ndim=1] Est_F = np.zeros(2 ** Lp)
        np.ndarray[np.complex128_t, ndim=1] Est_C = np.zeros(2 ** Lp, dtype=np.complex128)
        np.ndarray[np.double_t, ndim=1] Est_H = np.zeros(2 ** Lp)
        double Cand_F = 0
        np.complex128_t Cand_C = 0j
        double Cand_H = 0
        np.complex128_t Sel_C = 0
        double Sel_H = 0

        int L1, L2, Pn, Qn, ind
        np.ndarray[double, ndim=1] Va
        np.ndarray[np.complex128_t, ndim=2] Pos2
        double val
        np.complex128_t heading_est, tag_pos_est

    for L1 in range(int(Ln)):
        for L2 in range(L1 + 1, int(Ln)):

            for Pn in range(int(Lp)):
                Pos2, Prob2 = TwoAnchPos3([xa[L1], xa[L2]], 
                                          [ya[L1], ya[L2]], 
                                          [dist[L1, Pn], dist[L2, Pn]], 
                                          [0, 0], [0, 0], xa + 1j * ya, dist[:, Pn])
                Pos2C[Pn, 0] = Pos2[0, 0] + 1j * Pos2[0, 1]
                Pos2C[Pn, 1] = Pos2[1, 0] + 1j * Pos2[1, 1]
            
            for Qn in range(2 ** int(Lp)):
                Va = np.array(list(map(int, bin(Qn)[2:].zfill(int(Lp)))), dtype=np.double)
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
    tag_pos_est = Sel_C + tag_pos_b * exp(1j * Sel_H)

    return tag_pos_est, heading_est
