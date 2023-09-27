function [y] = encoder(a,x)

for i = 1:length(a)
    switch (a(i))
        case 'b'
            rectangle('position',[x,1,1,3],'FaceColor','k')
            x = x+1;
            hold on
        case 'B'
            rectangle('position',[x,1,2,3],'FaceColor','k')
            x = x+2;
            hold on
        case 'w'
            rectangle('position',[x,1,1,3],'FaceColor','w')
            x = x+1;
            hold on
        case 'W'
            rectangle('position',[x,1,2,3],'FaceColor','w')
            x = x+2;
            hold on
    end
end
rectangle('position',[x,1,1,3],'FaceColor','w')
x = x+1;
y = x;
        
end