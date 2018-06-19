function Solve_Large_Sparse_System_Examp_2_25
     n = 100000;
     [A,b] = sparsesetup(n);
     [x,i] = jacobi(A,b);
     x
     i
end


% Program 2.2 Jacobi Method
% Inputs: full or sparse matrix a, r.h.s. b,
%         number of Jacobi iterations, k
% Output: solution x
function [x, i] = jacobi(a,b)
    n=length(b); % find n
    d=diag(a);   % extract diagonal of a
    r=a-diag(d); % r is the remainder
    x0 = zeros(n,1); % initialize vector x
    x1 = 2*ones(n,1);
    i = 0;
    aposterror = x1 - x0;
    while(norm(aposterror,inf)> 1e-4)    % loop for Jacobi iteration
      x1 = (b-r*x0)./d;
      i = i + 1;
      aposterror = x1 - x0;
      x0 = x1;
    end
    x = x1;
end


% Program 2.1 Sparse matrix setup
% Input: n = size of system
% Outputs: sparse matrix a, r.h.s. b
function [a,b] = sparsesetup(n)
    e = ones(n,1); n2=n/2;
    a = spdiags([-e 3*e -e],-1:1,n,n);
    c=spdiags([e/2],0,n,n);c=fliplr(c);a=a+c;
    a(n2+1,n2) = -1; a(n2,n2+1) = -1;    % Fix up 2 entries
    b=zeros(n,1);                        % Entries of r.h.s. b
    b(1)=2.5;b(n)=2.5;b(2:n-1)=1.5;b(n2:n2+1)=1;
end

