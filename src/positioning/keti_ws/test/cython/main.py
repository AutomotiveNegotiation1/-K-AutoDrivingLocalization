import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import savgol_filter
from scipy.stats import norm
import random
import cmath
from UWBpos import UWBpos

# Parameters
Kalman_on = 0
OverDel = 1
InitLeng = 100
xa = np.array([10, 10, -10, -10, 20, -20])
ya = np.array([10, -10, -10, 10, 15, -15])
xt_b = np.array([-0.5, 0.5, -0.5, 0.5])
yt_b = np.array([0.5, 0.5, -0.5, -0.5])
xtc_b = xt_b + yt_b * 1j
Ln = len(xa)
Lp = len(xt_b)
NumInterpPoint = 20
xt_b_center = np.mean(xt_b)
yt_b_center = np.mean(yt_b)
angles_from_heading = np.arctan2(yt_b, xt_b)
rl = np.sqrt(xt_b**2 + yt_b**2)
global_coor = np.array([0, 0])
global_radius = 7
tag_center = np.array([0, 0])
heading_init = 90 * np.pi / 180
theta = np.arange(0, 2*np.pi, 0.01)
rho = global_radius * np.ones(len(theta))
x, y = rho * np.cos(theta) + global_coor[0], rho * np.sin(theta) + global_coor[1]
heading = (heading_init + theta) % (2*np.pi)
x = np.concatenate((np.ones(InitLeng) * x[0], x))
y = np.concatenate((np.ones(InitLeng) * y[0], y))
heading = np.concatenate((np.ones(InitLeng) * heading[0], heading))
average_len = 10
anch_pos = xa + ya * 1j
s_time = np.arange(len(x)) / 10
v_pred_x = 0
v_pred_y = 0
TagDistInit = np.zeros((Ln, Lp))
TagDistInitCount = np.zeros((Ln, Lp))
heading_est_a = []
NumRxID = np.zeros(len(x))

# Initialize lists and convert them to numpy arrays
centerest_a = []
centerest_a_aver = []
headingest_a_aver = []
# Initialize lists and convert them to numpy arrays
centerest_a = np.empty((0, 2))  # Initialize centerest_a as an empty 2D array
centerest_a_aver = np.empty((0, 2))  # Initialize centerest_a_aver as an empty 2D array

# 시작 부분에 이 코드를 추가하세요.
plt.ion()

# Plotting 부분을 다음과 같이 수정하세요.
fig1 = plt.figure(1)
ax1 = fig1.add_subplot(111)
ax1.plot(xa, ya, 'bo')


# Create ground-truth tag position and distance
for r in range(len(x)):
    tag_pos_b = xt_b + yt_b * 1j
    tag_pos_g = tag_pos_b * cmath.exp(1j * heading[r]) + x[r] + y[r] * 1j
    xt = np.real(tag_pos_g)
    yt = np.imag(tag_pos_g)
    dist_o = np.sqrt((xa[:, None] - xt)**2 + (ya[:, None] - yt)**2)

    # Add Gaussian noise to distance
    dist_a = dist_o + norm.rvs(size=dist_o.shape)
    if r > InitLeng:
        dist = dist_a
    else:
        dist = dist_a

    # Select random anchors and tag, calculate distance
    Nanchor = min(random.randint(0, 8) + 1, 4)
    RxID = random.sample(range(Ln), Nanchor)
    # Original line
    # RxTag = mod(r-1,Lp)+1;

    # Updated line
    RxTag = (r-1) % Lp

    # Ensure PP is within the range
    PP = RxTag if RxTag < Lp else Lp - 1

    RxDist = dist[RxID, PP]
    NumRxID[r] = Nanchor

    # Estimate tag position and heading using UWBpos()
    tag_pos_est, heading_est, tag_pos_est_aver, headingest_a_aver_v = UWBpos(Ln, Lp, PP, Nanchor, RxID, RxDist, s_time[r], tag_pos_b, xa, ya)

    # Save estimated position and heading
    # The rest of the code...
    tag_center_pos_est = np.mean(tag_pos_est)
    centerest_a = np.append(centerest_a, [[np.real(tag_center_pos_est), np.imag(tag_center_pos_est)]], axis=0)
    centerest_a_aver = np.append(centerest_a_aver, [[np.real(np.mean(tag_pos_est_aver)), np.imag(np.mean(tag_pos_est_aver))]], axis=0)
    # The rest of the code...
    heading_est_a.append(heading_est)
    headingest_a_aver.append(headingest_a_aver_v)

    # Plotting 부분에서 이 부분을 추가하세요.
    ax1.clear()
    ax1.plot(xa, ya, 'bo')
    ax1.plot(tag_pos_g.real, tag_pos_g.imag, 'go')
    ax1.plot(tag_pos_est.real, tag_pos_est.imag, 'bo')
    ax1.quiver(x[r], y[r], np.cos(heading[r]), np.sin(heading[r]), color='g', linewidth=1, headwidth=0.5)
    ax1.quiver(np.real(tag_center_pos_est), np.imag(tag_center_pos_est), np.cos(headingest_a_aver_v), np.sin(headingest_a_aver_v), color='b', linewidth=1, headwidth=0.5)
    ax1.set_title('Original(Blue) and estimated(Red) Tag to Anchor distance')  

    plt.draw()
    plt.pause(0.01)

# Calculate average root mean square error of center position and average absolute error of heading
centerpos_error = np.sqrt(np.mean((centerest_a[InitLeng:, 0] - x[InitLeng:])**2 + (centerest_a[InitLeng:, 1] - y[InitLeng:])**2))
heading_error = np.mean(np.abs(heading[InitLeng:] - heading_est_a[InitLeng:])) * 180 / np.pi

plt.ioff()
# plt.show()
