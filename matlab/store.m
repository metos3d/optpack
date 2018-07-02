function store(ctx)
    if ctx.i > 1;
        % update J, u and y
        logfile = matfile(ctx.logfile, 'writable', true);
        logfile.J(:,ctx.i) = ctx.J;
        logfile.u(:,ctx.i) = ctx.u';        % transposed
        logfile.y(:,:,ctx.i) = ctx.y;
    else;
        % we are at the beginning of the optimization and
        % store initially J, lb, u, ub, ud, y, yd
        J   = ctx.J;
        lb  = ctx.lb';                      % transposed
        u   = ctx.u';                       % transposed
        ub  = ctx.ub';                      % transposed
        ud  = ctx.ud';                      % transposed
        % this is a trick, we make the stored variable 3D,
        % so it can be extended during the other iteratons
        y   = cat(3,ctx.y,ctx.y);
        yd  = ctx.yd;
        save(ctx.logfile, 'J', 'lb', 'u', 'ub', 'ud', 'y', 'yd', '-v7.3');
    end;
end


