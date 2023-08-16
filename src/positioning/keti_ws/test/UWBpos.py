import numpy as np
from GetInitPos import GetInitPos
from InterpPos import InterpPos
from GetPos3 import GetPos3
from GetPosRefine2 import GetPosRefine2

def UWBpos(Ln, Lp, TagNum, Nanchor, RxID, RxDist, s_time, tag_pos_b, xa, ya):
    np.set_printoptions(threshold=np.inf)
    average_len = 10
    NumInterpPoint = 12
    global r, InterpPosition, Tag_Pos_List, TagDistInitCount, TagDistInit, heading_est_a, centerest_a, centerest_a_aver, headingest_a_aver, RxIDprev, RxIDprevLen, PPprev
    # print("2 :", Ln, Lp, TagNum, Nanchor, RxID, RxDist, s_time, tag_pos_b, xa, ya)
    if 'r' not in globals():
        r = 0
        InterpPosition = np.zeros((Lp, 2)) + 1j
        Tag_Pos_List = np.zeros((NumInterpPoint, 2, Lp)) + 1j
        TagDistInitCount = np.zeros((Ln, Lp))
        TagDistInit = np.zeros((Ln, Lp))
        heading_est_a = np.zeros(average_len * 2)
        centerest_a = np.zeros((average_len * 2, 2))
        centerest_a_aver = np.zeros((average_len * 2, 2))
        headingest_a_aver = np.zeros(average_len * 2)
        RxIDprev = np.zeros((4, Ln))
        RxIDprevLen = np.zeros(4)
        PPprev = np.zeros(4)

    r += 1
    anch_pos = xa + 1j*ya
    PP = TagNum
    tag_pos_est = np.zeros(Lp)
    heading_est = 0
    tag_pos_est_aver = np.zeros(Lp)
    headingest_a_aver_v = 0
    if r < 10 * Lp:
        for NN in range(Nanchor):
            TagDistInitCount[RxID[NN], PP] += 1
            div = TagDistInitCount[RxID[NN], PP]
            TagDistInit[RxID[NN], PP] = TagDistInit[RxID[NN], PP] * (div - 1) / div + RxDist[NN] / div

        
    elif r == 10 * Lp:
        for NN in range(Nanchor):
            TagDistInitCount[RxID[NN], PP] += 1
            div = TagDistInitCount[RxID[NN], PP]
            TagDistInit[RxID[NN], PP] = TagDistInit[RxID[NN], PP] * (div - 1) / div + RxDist[NN] / div
            
        DistT = TagDistInit.copy()
        DistT[TagDistInit == 0] = 1000000
        IndT = np.where(DistT.sum(axis=1) < 100000)
        DistT = DistT[IndT]
        AnchID = IndT

        tag_pos_est, heading_est = GetInitPos(xa[AnchID], ya[AnchID], DistT, anch_pos[AnchID], tag_pos_b, len(IndT), Lp)

        for LLp in range(Lp):
            for lo in range(NumInterpPoint):
                Tag_Pos_List[lo, :, LLp] = [s_time + (-NumInterpPoint + lo) * 0.1, tag_pos_est[LLp]]
    else:
        #%%%%%%%%%%%%%%%%%%  Next Position Prediction    %%%%%%%%%%%%%%%%%%
        for PPC in range(Lp):
            InterpPosition[PPC-1, 0] = InterpPos(Tag_Pos_List[:, 0, PPC - 1].reshape(-1, 1), np.real(Tag_Pos_List[:, 1, PPC-1]).reshape(-1, 1), s_time)
            InterpPosition[PPC-1, 1] = InterpPos(Tag_Pos_List[:, 0, PPC-1].reshape(-1, 1), np.imag(Tag_Pos_List[:, 1, PPC-1]).reshape(-1, 1), s_time)
        Xt_c_e = np.mean(InterpPosition[:, 0])
        Yt_c_e = np.mean(InterpPosition[:, 1])

        PosC = []
        PosC_E = []

        #%%%%%%%%%%%%%%%% New Position Calc.   %%%%%%%%%%%%%%%%%%%%%%%%%%%
        if len(RxID) > 1:
            tag_pos_est, heading_est, CandPos = GetPos3(xa, ya, RxDist, RxID, tag_pos_b, Nanchor, PP, InterpPosition[:, 0] + 1j*InterpPosition[:, 1], TagDistInit, RxIDprev, RxIDprevLen, PPprev)
            tag_pos_est, heading_est, CandPos = GetPosRefine2(xa, ya, RxDist, RxID, tag_pos_b, Nanchor, PP, tag_pos_est, heading_est, CandPos)
        else:
            InterpPosT = InterpPosition[:, 0] + 1j * InterpPosition[:, 1]
            TempC = np.mean(InterpPosT)
            Est_H_p = 0 + 0j
            for P in range(len(InterpPosT)):
                Est_H_p = Est_H_p + ((InterpPosT[P])-TempC) / tag_pos_b[P]
            Est_H = np.angle(Est_H_p)

            for P in range(InterpPosT.shape[0]):
                tag_pos_est[P] = TempC + tag_pos_b[P] * np.exp(1j * Est_H)

            CandPos = InterpPosition[PP, 0] + 1j * InterpPosition[PP, 1]

        Tag_Pos_List[0:NumInterpPoint-2, :, PP] = Tag_Pos_List[1:NumInterpPoint-1, :, PP]
        Tag_Pos_List[NumInterpPoint-1, :, PP] = [s_time, CandPos]

        Xt_c_e = np.real(np.mean(tag_pos_est))
        Yt_c_e = np.imag(np.mean(tag_pos_est))
        
        print("UWBpos.py : [x, y]--> [",Xt_c_e,",", Yt_c_e,"]")

        tag_center_pos_est = np.mean(tag_pos_est)

        heading_est_a[:-1] = heading_est_a[1:]

        if (heading_est - heading_est_a[-2]) > np.pi:
            heading_est_a[-1] = heading_est - 2*np.pi
        elif (heading_est_a[-2] - heading_est) > np.pi:
            heading_est_a[-1] = heading_est + 2*np.pi
        else:
            heading_est_a[-1] = heading_est

        centerest_a[:-1, :] = centerest_a[1:, :]
        centerest_a[-1, :] = [np.real(tag_center_pos_est), np.imag(tag_center_pos_est)]
        centerest_a_aver[:-1, :] = centerest_a_aver[1:, :]
        headingest_a_aver = headingest_a_aver.reshape(-1,1)
        headingest_a_aver[:-1, :] = headingest_a_aver[1:, :]    

        if (r > (average_len * 2)):
            MeanA = np.mean(centerest_a[0:average_len, 0] + 1j * centerest_a[0:average_len, 1])
            MeanB = np.mean(centerest_a[average_len:average_len*2, 0] + 1j * centerest_a[average_len:average_len*2, 1])
            centerest_a_aver[-1, :] = [np.real(MeanB + (MeanB - MeanA) / 2), np.imag(MeanB + (MeanB - MeanA) / 2)]
            MeanA_head = np.mean(heading_est_a[0:average_len])
            MeanB_head = np.mean(heading_est_a[average_len:average_len*2])
            headingest_a_aver[-1] = np.mod([(MeanB_head + (MeanB_head - MeanA_head) / 2)], 2*np.pi)
        else:
            centerest_a_aver[-1, :] = [np.real(Xt_c_e), np.real(Yt_c_e)]
            headingest_a_aver[-1] = heading_est_a[-1]

        K_Xt_c_e = centerest_a_aver[-1, 0]
        K_Yt_c_e = centerest_a_aver[-1, 1]
        tag_pos_est_aver = tag_pos_b * np.exp(1j * (headingest_a_aver[-1])) + K_Xt_c_e + 1j * K_Yt_c_e
        headingest_a_aver_v = headingest_a_aver[-1]
    # RxIDprevLen의 마지막 요소를 제외한 모든 요소를 한 칸씩 앞으로 이동
    RxIDprevLen[:-1] = RxIDprevLen[1:]

    # RxIDprevLen의 마지막 요소에 새로운 값 할당
    RxIDprevLen[-1] = len(RxID)

    # RxIDprev의 각 행을 한 칸씩 앞으로 이동
    RxIDprev[:-1, :] = RxIDprev[1:, :]

    # RxIDprev의 마지막 행에 새로운 값 할당
    RxIDprev[-1, :len(RxID)] = RxID

    # PPprev의 값을 한 칸씩 앞으로 이동
    PPprev[:-1] = PPprev[1:]

    # PPprev의 마지막 요소에 새로운 값 할당
    PPprev[-1] = PP

    return r, tag_pos_est, heading_est, tag_pos_est_aver, headingest_a_aver_v
