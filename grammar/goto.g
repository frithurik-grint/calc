-- This is a file in which is described inconditional jump (goto) grammar.
--
stmt-jump
  : 'goto' expr ';'
  ;

decl-labl
  : expr ':'
  ;
