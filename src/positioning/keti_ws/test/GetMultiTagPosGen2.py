import numpy as np

def GetMultiTagPosGen2(Pos2C, Va, tag_pos_b, Va_ind, PrevDist, RxIDprev, RxIDprevLen, xa, ya, PPprev):
    ia = np.zeros(len(Va), dtype=int)
    NP, _ = PrevDist.shape
    AnchIDList = np.arange(NP) + 1
    A = []
    for kk in range(4):
        if kk == 0:
            # A = RxIDprev[kk, :RxIDprevLen[kk]].tolist()
            A = RxIDprev[kk, :int(RxIDprevLen[kk])].tolist()

        else:
            for ll in range(int(RxIDprevLen[kk])):
                if RxIDprev[kk, ll] not in A:
                    A.append(RxIDprev[kk, ll])

    Est_P = np.zeros(Pos2C.shape[0], dtype=complex)

    for a in range(len(Va)):
        if (Va[a] == '0') and (Va_ind[a] != 0):
            ia[a] = 0
        elif (Va[a] == '1') and (Va_ind[a] != 0):
            ia[a] = 1
        else:
            ia[a] = 0

    NB = len(Va)
    Est_C = 0 + 0j
    for P in range(Pos2C.shape[0]):
        Est_C += (Pos2C[P, ia[P]]) / NB

    Est_H_p = 0 + 0j
    for P in range(Pos2C.shape[0]):
        Est_H_p += ((Pos2C[P, ia[P]]) - Est_C) / tag_pos_b[P]

    Est_H = np.angle(Est_H_p)

    Est_F = 0
    for P in range(Pos2C.shape[0]):
        Est_P[P] = Est_C + tag_pos_b[P] * np.exp(1j * Est_H)
        Est_F += abs(Est_P[P] - Pos2C[P, ia[P]]) ** 2

    for P in range(Pos2C.shape[0]):
        indp = np.where(PPprev == P)[0]
        if len(indp) > 1:
            indp = indp[-1]
        length = int(RxIDprevLen[indp])
        indices = RxIDprev[indp, :length].astype(int)


        anch_pos = xa[indices] + 1j * ya[indices]
        Est_Ft = np.zeros(int(RxIDprevLen[indp])).reshape(-1,1)
        anch_pos = anch_pos.reshape(1, -1)
        for q in range(int(RxIDprevLen[indp])):
            # 예를 들어, 해당 인덱스 값이 정수가 되도록 int() 함수를 사용하여 변환할 수 있습니다.
            index_value = int(RxIDprev[indp, q])
            Est_Ft[q] = abs(abs(anch_pos[0, q] - Est_P[P]) - PrevDist[index_value, P]) ** 2
                

            # Est_Ft[q] = abs(abs(anch_pos[q] - Est_P[P]) - PrevDist[RxIDprev[indp, q], P]) ** 2
        Tt = np.sort(Est_Ft)
        Est_F += np.sum(Tt[:2])

    return Est_F, Est_C, Est_H