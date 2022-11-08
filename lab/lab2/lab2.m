function lab2
  x = 1:0.1:32;
  
  [ax, h1, h2] = plotyy(x, [En(x); Un(x)],
                        x, [Sn(x); Rn(x); Qn(x)]);


  
  legend(["En"; "Un"; "Sn"; "Rn"; "Qn"]);

  title("Problema 6");

  xlabel("n");
  ylabel(ax(1), "E(n) U(n)");
  ylabel(ax(2), "S(n), R(n), Q(n)");

endfunction

function ret = On(n)
  ret = n.^3 + n.^2 .* log2(n);
endfunction

function ret = Tn(n)
  ret = (4 .* n.^3) ./ (n + 3);
endfunction

function ret = Sn(n)
  ret = (n + 3) ./ 4;
endfunction

function ret = En(n)
  ret = (n + 3) ./ (4 .* n);
endfunction

function ret = Rn(n)
  ret = (n + log2(n)) ./ n;
endfunction

function ret = Un(n)
  ret = (n + 3) .* (n + log2(n)) ./ (4 .* (n.^2));
endfunction

function ret = Qn(n)
  ret = (Sn(n) .* En(n)) ./ Rn(n);
endfunction