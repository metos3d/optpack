function yd = data(ctx)
    datapath = ['../../../../twin-data/' ctx.modname '/work/'];
    nt = ctx.nt;
    nx = ctx.nx;
    y = zeros(nx, nt);
    for i = 1:nt;
        filepath  = [datapath num2str(i-1, '%04d') '-N.petsc'];
%        if mod(i,500)==0 disp(['# ' filepath]), end;
        y(:, i) = readPETScVector(filepath);
    end;
    yd = average(y,ctx);
end


