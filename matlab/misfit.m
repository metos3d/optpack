function J = misfit(y,ctx)
    % compute average from result trajectory and store as current iterate
    ctx.y = average(y,ctx);

    % annual
    Jan = 0.5 * norm(ctx.yd(:,1) - ctx.y(:,1))^2;

    % monthly
    Jjan = 0.5 * norm(ctx.yd(:,2) - ctx.y(:,2))^2;
    Jfeb = 0.5 * norm(ctx.yd(:,3) - ctx.y(:,3))^2;
    Jmar = 0.5 * norm(ctx.yd(:,4) - ctx.y(:,4))^2;
    Japr = 0.5 * norm(ctx.yd(:,5) - ctx.y(:,5))^2;
    Jmay = 0.5 * norm(ctx.yd(:,6) - ctx.y(:,6))^2;
    Jjun = 0.5 * norm(ctx.yd(:,7) - ctx.y(:,7))^2;
    Jjul = 0.5 * norm(ctx.yd(:,8) - ctx.y(:,8))^2;
    Jaug = 0.5 * norm(ctx.yd(:,9) - ctx.y(:,9))^2;
    Jsep = 0.5 * norm(ctx.yd(:,10) - ctx.y(:,10))^2;
    Joct = 0.5 * norm(ctx.yd(:,11) - ctx.y(:,11))^2;
    Jnov = 0.5 * norm(ctx.yd(:,12) - ctx.y(:,12))^2;
    Jdec = 0.5 * norm(ctx.yd(:,13) - ctx.y(:,13))^2;

    % seasonal
    Jwin = 0.5 * norm(ctx.yd(:,14) - ctx.y(:,14))^2;
    Jspr = 0.5 * norm(ctx.yd(:,15) - ctx.y(:,15))^2;
    Jsum = 0.5 * norm(ctx.yd(:,16) - ctx.y(:,16))^2;
    Jaut = 0.5 * norm(ctx.yd(:,17) - ctx.y(:,17))^2;

    % store in context
    ctx.J = [Jan;Jjan;Jfeb;Jmar;Japr;Jmay;Jjun;Jjul;Jaug;Jsep;Joct;Jnov;Jdec;Jwin;Jspr;Jsum;Jaut];

    % return the annual mean as objective value
    J = Jan;
    disp(['# J:        ' num2str(J, '%.16e')])
end


