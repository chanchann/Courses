function Run_Newton_Nonlinear_System_2
    x0 = [-2,-2]';
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
         DF(1,1) = 18.*u.^2 + v;
         DF(1,2) = u - 9.*v.^2;
         DF(2,1) = 2.*u - 18.*v.^2;
         DF(2,2) = -36.*u.*v + 48.*v.^2;
end

function F = Function_F(u,v)
         F = zeros(2,1);
         F(1) = 6.*u.^3 + u.*v - 3.*v.^3 - 4;
         F(2) = u.^2 - 18.*u.*v.^2 + 16.*v.^3 + 1;
end