function Run_SOR_1
    u0 = 0; 
    v0 = 0;
    w0 = 0;
    omega = 1.25
    [u,v,w] = SOR_1(0,0,0,omega);
    i = 1;
    while(abs(u-u0)>1e-4 && abs(v-v0)>1e-4 && i < 100)
        i                      % record the number of iteration
        u0 = u
        v0 = v
        w0 = w
        [u,v,w] = SOR_1(u,v,w,omega);
        i = i + 1;
    end
end

function [u1,v1,w1] = SOR_1(u0,v0,w0,omega)
         u1 = (1 - omega).*u0 + omega.*(4 - v0 + w0)./3;
         v1 = (1 - omega).*v0 + omega.*(1 - 2.*u1 - w0)./4;
         w1 = (1 - omega).*w0 + omega.*(1 + u1 - 2.*v1)./5;
end