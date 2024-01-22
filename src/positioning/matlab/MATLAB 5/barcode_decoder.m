I=imread('barcode.jpg');%read image from drive;
I=im2bw(I);             
imshow(I);  
hold on;
[x,y]=ginput(2);        %get data from click mouse
u1=x(1);
u2=y(1);
v1=x(2);
v2=y(2);
line(x,y);              %paint the line between 2 point at click mouse
a = [u1:1:v1];          %find out function line between 2 point
if (u1~=v1) & (u2~=v2)
    g = round(((v2-u2)/(v1-u1))*a + u2 -(u1/(v1-u1)));
elseif (u1==v1)
    a = u1;
end                      
i=1;                    %find the value each pixel that the line go through
h=v1-u1+1;
b=[1:1:h];
for a=u1:v1
    if (I(g,a)==0)
        b(i)=1;
        i=i+1;
    elseif (I(g,a)==1)
        b(i)=0;
        i=i+1;
    end
end
c=b
i=1;                %find number pixels of each line in barcode picture;
s=[1:1:60];
for k=1:60
    j=0;
    if c(i)==0
        while (c(i)==0)&(i<=h)
            j=j+1;
            i=i+1;
            s(k)=j;
        end
    elseif c(i)==1
        while (c(i)==1)&(i<=h)
            j=j+1;
            i=i+1;
            s(k)=j;
        end
    end
end
mau=s(2);       %the first line is the sample for barcode,in another line is ratio with this
q=s./mau;
p=round(q);     
doc1=[1:1:6];   %decode 
k=1;
for i=5:4:25
    if (p(i)==3)& (p(i+1)==2) &(p(i+2)==1) &(p(i+3)==1)
        doc1(k)=0
    elseif (p(i)==2)& (p(i+1)==2) &(p(i+2)==2) &(p(i+3)==1)
        doc1(k)=1  
    elseif (p(i)==2)& (p(i+1)==1) &(p(i+2)==2) &(p(i+3)==2)
        doc1(k)=2
    elseif (p(i)==1)& (p(i+1)==4) &(p(i+2)==1) &(p(i+3)==1)
        doc1(k)=3
    elseif (p(i)==1)& (p(i+1)==1) &(p(i+2)==3) &(p(i+3)==2)
        doc1(k)=4
    elseif (p(i)==1)& (p(i+1)==2) &(p(i+2)==3) &(p(i+3)==1)
        doc1(k)=5
    elseif (p(i)==1)& (p(i+1)==1) &(p(i+2)==1) &(p(i+3)==4)
        doc1(k)=6
    elseif (p(i)==1)& (p(i+1)==3) &(p(i+2)==1) &(p(i+3)==2)
        doc1(k)=7
    elseif (p(i)==1)& (p(i+1)==2) &(p(i+2)==1) &(p(i+3)==3)
        doc1(k)=8
    elseif (p(i)==3)& (p(i+1)==1) &(p(i+2)==1) &(p(i+3)==2)
        doc1(k)=9
    end
    k=k+1;
end
doc2=[1:1:6];
k=1;
for i=34:4:54
    if (p(i)==3)& (p(i+1)==2) &(p(i+2)==1) &(p(i+3)==1)
        doc2(k)=0
    elseif (p(i)==2)& (p(i+1)==2) &(p(i+2)==2) &(p(i+3)==1)
        doc2(k)=1  
    elseif (p(i)==2)& (p(i+1)==1) &(p(i+2)==2) &(p(i+3)==2)
        doc2(k)=2
    elseif (p(i)==1)& (p(i+1)==4) &(p(i+2)==1) &(p(i+3)==1)
        doc2(k)=3
    elseif (p(i)==1)& (p(i+1)==1) &(p(i+2)==3) &(p(i+3)==2)
        doc2(k)=4
    elseif (p(i)==1)& (p(i+1)==2) &(p(i+2)==3) &(p(i+3)==1)
        doc2(k)=5
    elseif (p(i)==1)& (p(i+1)==1) &(p(i+2)==1) &(p(i+3)==4)
        doc2(k)=6
    elseif (p(i)==1)& (p(i+1)==3) &(p(i+2)==1) &(p(i+3)==2)
        doc2(k)=7
    elseif (p(i)==1)& (p(i+1)==2) &(p(i+2)==1) &(p(i+3)==3)
        doc2(k)=8
    elseif (p(i)==3)& (p(i+1)==1) &(p(i+2)==1) &(p(i+3)==2)
        doc2(k)=9
    end
    k=k+1;
end 