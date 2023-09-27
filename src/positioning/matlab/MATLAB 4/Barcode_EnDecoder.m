clear all;
close all;
% 
% code = '8902080011445';
% [file,data] = barCode128gen(code);

% h11=figure(1);saveas(h11,'temp.png');

I = imread('barcode_0_3650.gif');
figure(2);imshow(I)
[msg,detectedFormat,loc] = readBarcode(I);

pos=str2num(msg)'