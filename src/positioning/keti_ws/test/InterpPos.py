import numpy as np

def InterpPos(Px, Py, s_time):
    Px = Px - s_time
    Pm = np.column_stack((Px**2, Px, np.ones(len(Px))))

    A = np.linalg.inv(Pm.T @ Pm) @ Pm.T @ Py
    InterpPosition = A[2]

    return InterpPosition
