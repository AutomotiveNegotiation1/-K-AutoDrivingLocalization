function VetorPlot(A,Fignum,Title)

figure(Fignum);
hold off;
plot(A(1,:));
hold on;
plot(A(2,:),'r');
plot(A(3,:),'g');
title(Title)
