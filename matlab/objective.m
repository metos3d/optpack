function J = objective(u,ctx)
    disp(['# i:        ' num2str(ctx.i, '%03d')])

    y = model(u,ctx);
    J = misfit(y,ctx);
%    J = 0.0;

    store(ctx);
    ctx.i = ctx.i + 1;
end


