import numpy as np
import matplotlib.pyplot as plt
import cmath
from scipy.stats import norm
import time
import numpy as np
from UWBpos import UWBpos

def randperm(n, k):
    return np.random.permutation(n)[:k]

def awgn(signal, SNR_dB):
    L = len(signal)
    SNR = 10**(SNR_dB/10)
    esym = np.sum(np.abs(signal)**2)/L
    n0 = esym / SNR
    noise_std = np.sqrt(n0)
    noise = noise_std * (np.random.randn(L) + 1j * np.random.randn(L)) / np.sqrt(2)
    noise = noise.reshape(-1, 1) # Reshape the noise to have shape (6, 1)
    return signal + noise       # Broadcasting will handle the addition


def TestTotal():
    average_len = 10
    heading_est_a = np.zeros(average_len * 2)
    centerest_a = np.zeros((average_len * 2, 2))
    centerest_a_aver = np.zeros((average_len * 2, 2))
    headingest_a_aver = np.zeros(average_len * 2)
    Kalman_on = 0
    OverDel = 1
    InitLeng = 100
    xa = [10, 10, -10, -10, 20, -20]
    ya = [10, -10, -10, 10, 15, -15]
    xt_b = [-0.5, 0.5, -0.5, 0.5]
    yt_b = [0.5, 0.5, -0.5, -0.5]
    xtc_b = [xb + yb * 1j for xb, yb in zip(xt_b, yt_b)]

    Ln = len(xa)
    Lp = len(xt_b)
    NumInterpPoint = 20

    xt_b_center = np.mean(xt_b)
    yt_b_center = np.mean(yt_b)
    angles_from_heading = np.arctan2(yt_b, xt_b)
    rl = np.sqrt(np.array(xt_b)**2 + np.array(yt_b)**2)

    global_coor = [0, 0]
    global_radius = 7
    tag_center = [0, 0]
    heading_init = 90 * np.pi / 180

    theta = np.arange(0, 2 * np.pi, 0.01)
    rho = global_radius * np.ones(len(theta))
    x, y = rho * np.cos(theta) + global_coor[0], rho * np.sin(theta) + global_coor[1]
    heading = heading_init + theta

    x = np.concatenate(([x[0]] * InitLeng, x))
    y = np.concatenate(([y[0]] * InitLeng, y))
    heading = np.concatenate(([heading[0]] * InitLeng, heading))

    # plt.figure(100)
    # plt.plot(x, y)
    # plt.axis('equal')
    # plt.grid(True)

    # Continue the rest of your code here
    anch_pos = np.array(xa) + 1j * np.array(ya)
    s_time = np.arange(len(x)) / 10
    v_pred_x = 0
    v_pred_y = 0
    TagDistInit = np.zeros((Ln, Lp))
    TagDistInitCount = np.zeros((Ln, Lp))
    heading_est_a = []
    NumRxID = np.zeros(len(x))

    for r in range(len(x)):
        tag_pos_b = np.array(xt_b) + 1j * np.array(yt_b)
        tag_pos_g = tag_pos_b * cmath.exp(1j * (heading[r])) + x[r] + 1j * y[r]

        xt = np.real(tag_pos_g)
        yt = np.imag(tag_pos_g)

        dist_o = np.zeros((Ln, Lp))
        for n in range(Ln):
            for p in range(Lp):
                dist_o[n, p] = np.sqrt((xa[n] - xt[p]) ** 2 + (ya[n] - yt[p]) ** 2)

        dist_a = awgn(dist_o, 20)

        if r > InitLeng:
            dist = dist_a
            dist[0, 0:3] = dist_a[0, 0:3] + 1
        else:
            dist = dist_a
            dist[1, 0:2] = dist_a[1, 0:2] + 1

        Nanchor = min(np.random.randint(20) + 1, 4)
        RxID = randperm(Ln, Nanchor)
        RxTag = np.random.randint(Lp)
        RxTag = (r - 1) % Lp

        PP = RxTag
        # print(dist.shape)
        # print(RxID)
        # print(PP)

        RxDist = dist[RxID, PP]
        NumRxID[r] = Nanchor

        start_time = time.time()

        # Assuming you have already defined the necessary parameters:
        # r, Ln, Lp, TagNum, Nanchor, RxID, RxDist, s_time, tag_pos_b, xa, ya

        # Call the UWBpos function
        # print("1: ", Ln, Lp, PP, Nanchor, RxID, RxDist, s_time[r], tag_pos_b, np.array(xa), np.array(ya))
        r, tag_pos_est, heading_est, tag_pos_est_aver, headingest_a_aver_v = UWBpos(Ln, Lp, PP, Nanchor, RxID, RxDist, s_time[r], tag_pos_b, np.array(xa), np.array(ya))

        import matplotlib.pyplot as plt

        tag_center_pos_est = np.mean(tag_pos_est)
        centerest_a[r, 0] = np.real(tag_center_pos_est)
        centerest_a[r, 1] = np.imag(tag_center_pos_est)
        print(np.real(tag_center_pos_est))
        heading_est_a[r] = heading_est
        centerest_a_aver[r, 0] = np.real(np.mean(tag_pos_est_aver))
        centerest_a_aver[r, 1] = np.imag(np.mean(tag_pos_est_aver))
        headingest_a_aver[r] = headingest_a_aver_v


        plt.title('Original(Blue) and estimated(Red) Tag to Anchor distance')
        plt.figure(4)

        # Assuming x and y are defined similarly as in MATLAB code
        plt.plot(x[:], y[:], 'g^')

        # Assuming centerest_a and centerest_a_aver are defined similarly as in MATLAB code
        InitLeng = 0 # You may need to set this value to match your specific use case

        plt.plot([x[0] for x in centerest_a[InitLeng:r]], [x[1] for x in centerest_a[InitLeng:r]], 'ko')
        plt.plot([x[0] for x in centerest_a_aver[InitLeng:r]], [x[1] for x in centerest_a_aver[InitLeng:r]], 'bo')

        plt.show()


        # ...

        # Print the execution time
        end_time = time.time()
        # print("Execution time:", end_time - start_time, "seconds")
        
    return tag_pos_est, heading_est, tag_pos_est_aver, headingest_a_aver_v        
