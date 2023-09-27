function Reliability = ReliCalc(tag_pos_est, anchor_pos, RxDist, RxID, PP)

TT = abs(abs(tag_pos_est(PP)-anchor_pos(RxID)) - RxDist');
TTs = sort(TT);
VarTT = sqrt(mean(TT(1:3).^2));

Reliability = 1/VarTT;
