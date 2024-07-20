-- This is a file in which is described controlled loop (for) grammar.
--
stmt-loop-cntrld
  : 'for' '(' stmt-loop-cntrld-head ')' stmt
  | 'do'? stmt 'for' '(' stmt-loop-cntrld-head ')' ';'
  ;

stmt-loop-cntrld-head
  : expr-initlz? ';' expr? ';' expr?
  ;

stmt-loop-cntrld-countd
  : 'for' '(' stmt-loop-cntrld-countd-head ')' stmt
  | 'do'? stmt 'for' '(' stmt-loop-cntrld-countd-head ')' ';'
  ;
  
stmt-loop-cntrld-countd-head
  : expr-initlz? '..' expr?
  ;
