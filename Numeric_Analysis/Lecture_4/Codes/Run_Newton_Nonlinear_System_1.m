function Run_Newton_Nonlinear_System_1
    x0 = [-1,-2]';
    x = [0,0]';
    i = 0;
    e = x - x0;
    while (norm(e,inf) > 1e-8)
%         Jacobi_Matrix(x0(1),x0(2))
%         Function_F(x0(1),x0(2))
%         Jacobi_Matrix(x0(1),x0(2))\Function_F(x0(1),x0(2))
%         pause
        x = x0 - Jacobi_Matrix(x0(1),x0(2))\Function_F(x0(1),x0(2)); 
        e = x - x0;
        i = i + 1;
        x0 = x;
    end
    format long
    i
    x
end

function DF = Jacobi_Matrix(u,v)
         DF = zeros(2,2);
         DF(1,1) = -3.*u^2;
         DF(1,2) = 1;
         DF(2,1) = 2.*u;
         DF(2,2) = 2.*v;
end

function F = Function_F(u,v)
         F = zeros(2,1);
         F(1) = v - u.^3;
         F(2) = u.^2 + v.^2 -1;
end

