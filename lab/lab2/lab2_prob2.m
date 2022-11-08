function lab2
  x = 1:0.1:32;
  
  [ax, h1, h2] = plotyy(x, [En(x); Un(x)],
                        x, [Sn(x); Rn(x); Qn(x)]);


  
  legend(["En"; "Un"; "Sn"; "Rn"; "Qn"]);

  title("Quick SOrt");

  xlabel("n");
  ylabel(ax(1), "E(n) U(n)");
  ylabel(ax(2), "S(n), R(n), Q(n)");

endfunction

function ret = On(n)
  ret = n.^2;
endfunction

function ret = Tn(n)
  ret = n.* log2(n);
endfunction

function ret = Sn(n)
  ret = 1 ./ Tn(n);
endfunction

function ret = En(n)
  ret = Sn(n) ./ n;
endfunction

function ret = Rn(n)
  ret = On(n);
endfunction

function ret = Un(n)
  ret = Rn(n) .* En(n);
endfunction

function ret = Qn(n)
  ret = (Sn(n) .* En(n)) ./ Rn(n);
endfunction