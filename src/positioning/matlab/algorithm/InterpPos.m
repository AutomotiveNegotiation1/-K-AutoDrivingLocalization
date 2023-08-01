function [InterpPosition] = InterpPos(Px,Py,s_time)
Px = Px-s_time;
Pm = [Px.^2 Px ones(size(Px))];

A = inv(Pm'*Pm)*Pm'*Py;
InterpPosition = A(3);


% Py = pm x A
% tr(pm)xPy = tr(Pm)pm x A
% inv(tr(Pm)Pm) x tr(pm)xPy = A