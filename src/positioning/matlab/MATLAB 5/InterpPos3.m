function [InterpPosition] = InterpPos3(Px,Py,s_time,d)
Px = Px(:);
Py = Py(:);

ind = find(Px==0);
Px(ind)=[];
Py(ind)=[];

Px = Px-s_time;

if d == 2
%     if length(find(Px==0)) == 0
        Pm = [Px.^2 Px ones(size(Px))];

        A = inv(Pm'*Pm)*Pm'*Py;
        InterpPosition = A(3);
%     else
%         InterpPosition = Py(3);
%     end
else
%     if length(find(Px==0)) == 0
        Pm = [Px ones(size(Px))];

        A = inv(Pm'*Pm)*Pm'*Py;
        InterpPosition = A(2);
%     else
%         InterpPosition = Py(3);
%     end
end



% Py = pm x A
% tr(pm)xPy = tr(Pm)pm x A
% inv(tr(Pm)Pm) x tr(pm)xPy = A