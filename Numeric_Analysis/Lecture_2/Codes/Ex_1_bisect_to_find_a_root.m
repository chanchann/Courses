format long
for i = 1 : 10
  [xc,a,b]=bisect(@f_1,0,1,10^(-i));
  fprintf('%12.10f %12.10f %12.10f \n',[xc;a;b])
end

% format long
% f =@(x) x^3+ x - 1;
% xc=bisect(f,0,1,0.1)
