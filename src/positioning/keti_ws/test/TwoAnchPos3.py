import numpy as np

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