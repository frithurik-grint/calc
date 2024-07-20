-- This is a file in which is described condition controlled loop (while) grammar.
--
stmt-loop-condit
  : 'while' expr stmt
  | 'do'? stmt 'while' expr ';'
  ;

stmt-symblc-loop-condit
  : '[]' expr stmt
  ; 
