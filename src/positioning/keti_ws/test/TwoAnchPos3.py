import numpy as np

def TwoAnchPos3(Xa, Ya, dist, tag_pos, EstCenter, anch_pos, dist_a):
    AA  = np.sqrt((Xa[0]-Xa[1])**2 + (Ya[0]-Ya[1])**2)
    B = dist[0]
    C = dist[1]
    s = (AA + B + C) / 2
    temp = s * (s - AA) * (s - B) * (s - C)
    S = np.sqrt(abs(temp))
    d = 2 * S / AA

    A = np.array([[Ya[1]-Ya[0], - Xa[1]-Xa[0]], [2*(Xa[1]-Xa[0]), 2*(Ya[1]-Ya[0])]])
    Y1 = np.array([d * np.sqrt((Ya[1]-Ya[0])**2 + (Xa[1]-Xa[0])**2) + Xa[0]*Ya[1] - Xa[1]*Ya[0],
                   -C**2 + B**2 - Xa[0]**2 + Xa[1]**2 - Ya[0]**2 + Ya[1]**2])
    Y2 = np.array([-d * np.sqrt((Ya[1]-Ya[0])**2 + (Xa[1]-Xa[0])**2) + Xa[0]*Ya[1] - Xa[1]*Ya[0],
                   -C**2 + B**2 - Xa[0]**2 + Xa[1]**2 - Ya[0]**2 + Ya[1]**2])

    inv_mat = np.linalg.inv(np.matmul(A.T, A))
    X1 = np.matmul(np.matmul(inv_mat, A.T), Y1)
    X2 = np.matmul(np.matmul(inv_mat, A.T), Y2)

    X1S = np.sum(np.abs(np.abs(anch_pos - (X1[0] + 1j*X1[1])) - dist_a))
    X2S = np.sum(np.abs(np.abs(anch_pos - (X2[0] + 1j*X2[1])) - dist_a))
    Y1S = np.sum((X1 - tag_pos) ** 2)
    Y2S = np.sum((X2 - tag_pos) ** 2)
    Z1S = np.abs(np.sum((X1 - EstCenter) ** 2) - 0.5)
    Z2S = np.abs(np.sum((X2 - EstCenter) ** 2) - 0.5)

    if (X1S + Y1S) > (X2S + Y2S):
        Pos = np.array([X2, X1])
        Prob = np.array([X2S + Y2S, X1S + Y1S])
    else:
        Pos = np.array([X1, X2])
        Prob = np.array([X1S + Y1S, X2S + Y2S])

    return Pos, Prob
