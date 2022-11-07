function [Grp] = data_save(Grp, Nav, idx)
        Grp.Nav.mode(idx) = Nav.mode;
        Grp.Nav.pos(idx,:) = Nav.output.pos';
        Grp.Nav.vel(idx,:) = Nav.output.vel';
        Grp.Nav.att(idx,:) = Nav.output.att';
        Grp.Nav.eulr(idx,:) = dcm2eulr(qua2dcm(Nav.output.att))*(180/pi);
        Grp.Nav.cov(idx,:) = diag(Nav.KF.P);

end