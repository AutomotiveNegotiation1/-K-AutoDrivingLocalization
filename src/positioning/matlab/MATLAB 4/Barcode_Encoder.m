import Barcode1.*;   %Get the constants declared in Barcode1
Barcode1
In = input('Enter input: ','s');
deltr = 'bWbwBwBwb';
x = 0;
c = encoder(deltr,x);
x = c;
for i=1:length(In)
    s = In(i);
switch(s)
    case {'A','a'} 
        a = A;
    case {'B','b'} 
        a = B;    
    case {'C','c'} 
        a = C;
    case {'D','d'} 
        a = D;
    case {'E','e'} 
        a = E;
    case {'F','f'} 
        a = F;    
    case {'G','g'} 
        a = G;
    case {'H','h'} 
        a = H;
    case {'I','i'} 
        a = I;
    case {'J','j'} 
        a = J;    
    case {'K','k'} 
        a = K;
    case {'L','l'} 
        a = L;
    case {'M','m'} 
        a = M;
    case {'N','n'} 
        a = N;    
    case {'O','o'} 
        a = O;
    case {'P','p'} 
        a = P;
    case {'Q','q'} 
        a = Q;
    case {'R','r'} 
        a = R;    
    case {'S','s'} 
        a = S;
    case {'T','t'} 
        a = T;
    case {'U','u'} 
        a = U;
    case {'V','v'} 
        a = V;    
    case {'W','w'} 
        a = W;
    case {'X','x'} 
        a = X;
    case {'Y','y'} 
        a = Y;
    case {'Z','z'} 
        a = Z;    
    case '-' 
        a = hyp;
    case '.' 
        a = per;
    case ' ' 
        a = space;
    case '$' 
        a = dollar;    
    case '\' 
        a = slash;
    case '+' 
        a = plus;
    case '%' 
        a = dollar;
    case '*' 
        a = ast;    
    case '0' 
        a = zero;
    case '1' 
        a = one;
    case '2' 
        a = two;
    case '3' 
        a = three;    
    case '4' 
        a = four;
    case '5' 
        a = five;
    case '6' 
        a = six;
    case '7' 
        a = seven;    
    case '8' 
        a = eight;
    case '9' 
        a = nine;
end
c = encoder(a,x);
x = c;
end
c = encoder(deltr,x);
