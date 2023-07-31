import numpy as np
import cmath
# from scipy import linalg

def TwoAnchPos3(Xa, Ya, dist, tag_pos, EstCenter, anch_pos, dist_a):
    AA = np.sqrt((Xa[0] - Xa[1])**2 + (Ya[0] - Ya[1])**2)

    # print(AA)
    B = dist[0]
    C = dist[1]

    if B**2 - ((B**2 - C**2 + AA**2) / (2*AA))**2 > 0:
        d = np.sqrt(B**2 - ((B**2 - C**2 + AA**2) / (2*AA))**2)
    else:
        d = 0

    A = np.array([[Ya[1] - Ya[0], -(Xa[1] - Xa[0])], [2 * (Xa[1] - Xa[0]), 2 * (Ya[1] - Ya[0])]])
    Y1 = np.array([[d * np.sqrt((Ya[1] - Ya[0])**2 + (Xa[1] - Xa[0])**2) + Xa[0] * Ya[1] - Xa[1] * Ya[0], -(C**2 - B**2 - Xa[0]**2 + Xa[1]**2 - Ya[0]**2 + Ya[1]**2)]]).T
    Y2 = np.array([[-d * np.sqrt((Ya[1] - Ya[0])**2 + (Xa[1] - Xa[0])**2) + Xa[0] * Ya[1] - Xa[1] * Ya[0], -(C**2 - B**2 - Xa[0]**2 + Xa[1]**2 - Ya[0]**2 + Ya[1]**2)]]).T


    X1 = np.linalg.inv(A.T @ A) @ A.T @ Y1
    X2 = np.linalg.inv(A.T @ A) @ A.T @ Y2


    X1S = np.sum(np.abs(np.abs(anch_pos - (X1[0] + 1j * X1[1])) - dist_a.T))
    X2S = np.sum(np.abs(np.abs(anch_pos - (X2[0] + 1j * X2[1])) - dist_a.T))
    Y1S = np.sum((X1 - tag_pos.T)**2)
    Y2S = np.sum((X2 - tag_pos.T)**2)
    Z1S = np.abs(np.sum((X1 - EstCenter.T)**2) - 0.5)
    Z2S = np.abs(np.sum((X2 - EstCenter.T)**2) - 0.5)
    
    Prob = np.array([[]])
    

    if (X1S + Y1S) > (X2S + Y2S):
        list_of_arrays = [X2.T, X1.T]
        Pos = np.vstack(list_of_arrays)
        Prob = np.array([X2S + Y2S, X1S + Y1S]).reshape(-1, 1)
    else:
        list_of_arrays = [X1.T, X2.T]
        Pos = np.vstack(list_of_arrays)
        Prob = np.array([X1S + Y1S, X2S + Y2S]).reshape(-1, 1)
        
    return Pos, Prob
    
def GetMultiTagPosGen(Pos2C, Va, tag_pos_b, Va_ind):
    ia = np.zeros(len(Va), dtype=int)
    for a in range(len(Va)):
        if (Va[a] == '0') and (Va_ind[a] != 0):
            ia[a] = 0
        elif (Va[a] == '1') and (Va_ind[a] != 0):
            ia[a] = 1
        else:
            ia[a] = 0

    NB = len(Va)

    Est_C = 0
    for P in range(Pos2C.shape[0]):
        # print("Pos2C", Pos2C[P, ia[P]])
        Est_C = Est_C + (Pos2C[P, ia[P]]) / NB
        # print("Est_C_0", Est_C) 
    # print("Est_C_1", Est_C)
    Est_H_p = 0
    for P in range(Pos2C.shape[0]):
        Est_H_p = ((Pos2C[P][ia[P]]) - Est_C) / tag_pos_b[P]
    Est_H = np.angle(Est_H_p)

    Est_F = 0
    Est_P = np.zeros(Pos2C.shape[0], dtype=complex)
    for P in range(Pos2C.shape[0]):
        Est_P[P] = Est_C + tag_pos_b[P] * np.exp(1j * Est_H)
        Est_F = Est_F + abs(Est_P[P] - Pos2C[P, ia[P]])**2

    return Est_F, Est_C, Est_H


def GetInitPos(xa, ya, dist, anch_pos, tag_pos_b, Ln, Lp):
    Sel_F = 10000000
    Est_F = []
    Est_C = []
    Est_H = []
    for L1 in range(Ln):
        for L2 in range(L1 + 1, Ln):
            Pos2C = np.zeros((Lp, 2), dtype=complex)
            for Pn in range(Lp):
                Pos2, Prob2 = TwoAnchPos3([xa[L1], xa[L2]], [ya[L1], ya[L2]], [dist[L1, Pn], dist[L2, Pn]], np.array([0, 0]), np.array([0, 0]), anch_pos[0][:Ln], dist[:, Pn])
                Pos2C[Pn, 0] = Pos2[0, 0] + 1j * Pos2[0, 1]
                Pos2C[Pn, 1] = Pos2[1, 0] + 1j * Pos2[1, 1]
            
            for Qn in range(2 ** Lp):
                Va = format(Qn, f'0{Lp}b')  # MATLAB의 dec2bin과 같은 기능을 수행하는 코드입니다.
                F, C, H= GetMultiTagPosGen(Pos2C, Va, tag_pos_b[0], [1, 1, 1, 1])
                Est_F.append(F)
                Est_C.append(C)
                Est_H.append(H)
            val, ind = min((val, ind) for ind, val in enumerate(Est_F))
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



def InterpPos(Px, Py, s_time):
    Px = Px[0] - s_time
    Pm = np.column_stack([Px**2, Px, np.ones_like(Px)])
    A = np.linalg.inv(Pm.T @ Pm) @ Pm.T @ Py
    InterpPosition = A[2]

    return InterpPosition


def GetPos(xa, ya, dist, anch_pos, tag_pos_b, Ln, PP, PredPos):
    Pos2C = np.zeros((len(PredPos), 2), dtype=complex)
    Pos2C[:, 0] = PredPos
    Va_ind = np.zeros(len(PredPos))
    Sel_F = 10000000
    Est_F = []
    Est_C = []
    Est_H = []
    for L1 in range(Ln):
        for L2 in range(L1 + 1, Ln):
            for Pn in range(PP, PP+1):
                Pos2, Prob2 = TwoAnchPos3([xa[L1], xa[L2]], [ya[L1], ya[L2]], [dist[L1], dist[L2]], np.array([0, 0]), np.array([0, 0]), anch_pos[0][:Ln], dist)
                Pos2C[Pn, 0] = Pos2[0, 0] + 1j*Pos2[0, 1]
                Pos2C[Pn, 1] = Pos2[1, 0] + 1j*Pos2[1, 1]
            for Qn in range(2):
                # Va = np.array(list(bin((2**(PP*Qn) - 1))[2:].zfill(len(PredPos))[::-1]), dtype=int)
                Va = format(Qn, f'0{len(PredPos)}b')
                Va_ind[PP] = 1
                Est_F, Est_C, Est_H = GetMultiTagPosGen(Pos2C, Va, tag_pos_b[0], Va_ind)
                Est_F.append(F)
                Est_C.append(C)
                Est_H.append(H)
                
            val, ind = min((val, ind) for ind, val in enumerate(Est_F))
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

def GetPos2(xa, ya, dist, RxID, tag_pos_b, Ln, PP, PredPos):
    print("init", xa, ya, dist, RxID, tag_pos_b, Ln, PP, PredPos)
    Pos2C = np.zeros((len(PredPos), 2), dtype=complex)
    Pos2C[:, 0] = PredPos
    Va_ind = np.zeros(len(PredPos))
    Sel_F = 10000000
    Sel_C = None
    Sel_H = None
    cand_tag_pos = None

    for L1 in range(Ln):
        for L2 in range(L1 + 1, Ln):
            for Pn in range(PP, PP+1):
                Pos2, Prob2 = TwoAnchPos3([xa[RxID[L1]], xa[RxID[L2]]], [ya[RxID[L1]], ya[RxID[L2]]], 
                                          [dist[L1], dist[L2]], np.array([0, 0]), np.array([0, 0]),
                                          xa[RxID] + 1j * ya[RxID], dist)
                Pos2C[Pn, 0] = complex(Pos2[0, 0], Pos2[0, 1])
                Pos2C[Pn, 1] = complex(Pos2[1, 0], Pos2[1, 1])
            Est_F = []
            Est_C = []
            Est_H = []
            for Qn in range(2):
                # Va = list(map(int, reversed(bin(2 ** (PP * Qn) - 1)[2:].zfill(len(PredPos)))))
                Va = format(Qn, f'0{len(PredPos)}b')
                Va_ind[PP] = 1
                F, C, H = GetMultiTagPosGen(Pos2C, Va, tag_pos_b, Va_ind)
                # print("C", C)
                Est_F.append(F)
                Est_C.append(C)
                Est_H.append(H)
                # print("Est_C_2", Est_C)
            # val, ind = min((val, ind) for ind, val in enumerate(Est_F))
            val, ind = min((val, idx) for idx, val in enumerate(Est_F))
            Cand_F = val
            Cand_C = Est_C[ind]
            Cand_H = Est_H[ind]
            # print("Cand_C",Cand_C)
            # print("Cand_H",Cand_H)
                
            if Sel_F > Cand_F:
                Sel_F = Cand_F
                Sel_C = Cand_C
                Sel_H = Cand_H
                
                cand_tag_pos = Pos2C[PP, ind]

    heading_est = Sel_H
    # tag_pos_est = Sel_C + tag_pos_b * np.exp(1j * Sel_H)
    tag_pos_est = Sel_C + tag_pos_b * cmath.exp(1j * Sel_H)
    return tag_pos_est, heading_est, cand_tag_pos

def get_tag_pos(tag_center_pos_est, heading_est, tag_pos_b):
    tag_pos_est = tag_pos_b * np.exp(1j * heading_est) + tag_center_pos_est
    return tag_pos_est.reshape(1, -1)