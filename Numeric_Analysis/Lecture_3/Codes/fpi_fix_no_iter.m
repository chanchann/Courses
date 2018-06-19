%Program 1.2 Fixed-Point Iteration
%Computes approximate solution of g(x)=x
%Input: function handle g, starting guess x0,
%       number of iteration steps k
%Output: Approximate solution xc
function xc=fpi_fix_no_iter(g, x0, k)
    x(1)=x0;
    for i=1:k
       x(i+1)=g(x(i));
    end
    xc=x(k+1);
end