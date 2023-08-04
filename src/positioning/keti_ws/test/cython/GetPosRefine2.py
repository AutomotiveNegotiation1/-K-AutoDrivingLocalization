import numpy as np
from ThreeAnchPos3 import ThreeAnchPos3

def GetPosRefine2(xa, ya, dist, RxID, tag_pos_b, Ln, PP, tag_pos_est, heading_est, cand_tag_pos):
    anch_pos = xa[RxID] + 1j * ya[RxID]

    calc_dist_err = np.zeros(Ln)
    Est_P = np.zeros(len(tag_pos_b)) + 0j

    if Ln != 1:
        for kk in range(Ln):
            calc_dist_err[kk] = abs(abs(anch_pos[kk]-tag_pos_est[PP-1]) - dist[kk])**2

        mean_err = np.sqrt(np.mean(calc_dist_err))

        ind = np.where(calc_dist_err < max(mean_err * 0.3, 0.2))

        Len = len(ind[0])

        NB = len(tag_pos_b)

        if Len >= 3:
            Pos, Prob = ThreeAnchPos3(np.real(anch_pos[ind]), np.imag(anch_pos[ind]), dist[ind])
            tag_pos_est[PP-1] = Pos[0] + 1j * Pos[1]
            cand_tag_pos = Pos[0] + 1j * Pos[1]

            Est_C = np.sum(tag_pos_est) / NB

            Est_H_p = np.sum([(tag_pos_est[P] - Est_C) / tag_pos_b[P] for P in range(len(tag_pos_est))])
            Est_H = np.angle(Est_H_p)

            Est_F = np.sum([abs((Est_C + tag_pos_b[P] * np.exp(1j * Est_H)) - tag_pos_est[P])**2 for P in range(len(tag_pos_est))])

            tag_pos_est = Est_P
            heading_est = Est_H

    return tag_pos_est, heading_est, cand_tag_pos
