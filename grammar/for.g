-- This is a file in which is described controlled loop (for) grammar.
--
stmt-loop-cntrld
  : 'for' expr-initlz ';' expr ';' expr stmt
  | 'for' '(' expr-initlz? ';' expr? ';' expr? ')' stmt
  ;

stmt-loop-cntrld-countd
  : 'for' expr-initlz '..' expr stmt
  | 'for' '(' expr-initlz '..' expr? ')' stmt
  ;
  
