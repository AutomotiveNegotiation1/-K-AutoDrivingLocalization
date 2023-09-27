function [Est_F,Est_C,Est_H] = GetMultiTagPos(Pos2C,Va,tag_pos_b)

for a = 1 : length(Va)
    if Va(a)=='0'
        ia(a) = 1;
    else
        ia(a) = 2;
    end
end

NB = length(tag_pos_b);

Est_C = 0;
for P = 1 : size(Pos2C,1)
    Est_C = Est_C + (Pos2C(P,ia(P)))/NB;
end

Est_H_p = 0;
for P = 1 : size(Pos2C,1)
    Est_H_p = ((Pos2C(P,ia(P)))-Est_C)/tag_pos_b(P);
end
Est_H = angle(Est_H_p);

Est_F = 0;
for P = 1 : size(Pos2C,1)
    Est_P(P) = Est_C + tag_pos_b(P)*exp(j*Est_H);
    Est_F = Est_F + abs(Est_P(P)-Pos2C(P,ia(P)))^2;
end
