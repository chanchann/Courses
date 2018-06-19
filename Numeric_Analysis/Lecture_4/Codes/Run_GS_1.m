function Run_GS_1
    u0 = 0; 
    v0 = 0;
    [u,v] = GS_1(0,0);
    i = 1;
    while(abs(u-u0)>1e-4 && abs(v-v0)>1e-4 && i < 100)
        i                      % record the number of iteration
        u0 = u
        v0 = v
        [u,v] = GS_1(u,v);
        i = i + 1;
    end
end

function [u1,v1] = GS_1(u0,v0)
         u1 = (5 - v0)./3;
         v1 = (5 - u1)./2;
end