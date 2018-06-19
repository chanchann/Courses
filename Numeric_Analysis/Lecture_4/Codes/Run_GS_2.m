function Run_GS_2
    u0 = 0; 
    v0 = 0;
    w0 = 0;
    [u,v,w] = GS_2(0,0,0);
    i = 1;
    while(abs(u-u0)>1e-4 && abs(v-v0)>1e-4 && i < 100)
        i                      % record the number of iteration
        u0 = u
        v0 = v
        w0 = w
        [u,v,w] = GS_2(u,v,w);
        i = i + 1;
    end
end

function [u1,v1,w1] = GS_2(u0,v0,w0)
         u1 = (4 - v0 + w0)./3;
         v1 = (1 - 2.*u1 - w0)./4;
         w1 = (1 + u1 - 2.*v1)./5;
end