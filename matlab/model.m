function y = model(u,ctx)
    disp(num2str(u, '# u:        %.16e\n'))

    % store current parameter set
    ctx.u = u;

    % option
    optionfile = [ctx.expname '.' ctx.modname '.' num2str(ctx.nexp) '.' num2str(ctx.i, '%03d') '.option.sh'];
    optionfilepath = [ctx.expname '/option/' optionfile];
    option(optionfilepath,ctx);

    % run
    logfile = [ctx.expname '.' ctx.modname '.' num2str(ctx.nexp) '.' num2str(ctx.i, '%03d') '.log.txt'];
    logfilepath = [ctx.expname '/log/' logfile];
    runcmd = ['. model/de.uni-kiel.rz.nesh-fe.petsc-3.3-p7.opt.sh; '...
              'mpiexec ' getenv('NQSII_MPIOPTS') ' ./model/metos3d-simpack-' ctx.modname '.exe ' ...
              optionfilepath ' > ' logfilepath];
    disp(['# run:      ' runcmd])
    [status, result] = system(runcmd);
%    disp(['# status:   ' num2str(status)])

    % read result from scratch
    readpath = getenv('SCRATCH');
    nx = ctx.nx;
    nt = ctx.nt;
    y = zeros(nx, nt);
    for i = 1:nt;
        filepath = [readpath num2str(i-1, '%04d') '-N.petsc'];
%        if mod(i,500)==0 disp(['# ' filepath]), end;
        y(:, i) = readPETScVector(filepath);
    end;

    % clean scratch
    cleancmd = ['rm ' readpath '*'];
    disp(['# clean:    ' cleancmd])
    [status, result] = system(cleancmd);
%    disp(['# status:   ' num2str(status)])
end


