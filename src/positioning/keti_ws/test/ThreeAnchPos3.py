import numpy as np

def ThreeAnchPos3(xa, ya, dist):
    M = len(xa)
    A = np.zeros((M-1, 2))
    y = np.zeros((M-1, 1))

    for k in range(M-1):
        A[k, :] = [-2 * (xa[k] - xa[M-1]), -2 * (ya[k] - ya[M-1])]
        y[k, 1] = dist[k]**2 - dist[M-1]**2 - (xa[k]**2 - xa[M-1]**2) - (ya[k]**2 - ya[M-1]**2)

    Pos = np.linalg.inv(A.T @ A) @ A.T @ y
    Prob = np.sqrt(np.mean(np.abs((xa - Pos[0])**2 + (ya - Pos[1])**2 - dist**2)))

    return Pos, Prob
