function Run_Jacobi_2
    u0 = 0; 
    v0 = 0;
    [u,v] = Jacobi_2(0,0);
    i = 1;
    while(abs(u-u0)>1e-4 && abs(v-v0)>1e-4 && i < 100)
        i
        u0 = u
        v0 = v
        [u,v] = Jacobi_2(u,v);
        i = i + 1;
    end
end

function [u1,v1] = Jacobi_2(u0,v0)
         u1 = 5 - 2.*v0;
         v1 = 5 - 3.*u0;
end