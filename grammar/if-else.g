-- This is a file in which is described single selection (if-else) grammar.
--
stmt-select-single
  : 'if' expr stmt
  | 'if' expr stmt 'else' stmt
  ;

stmt-symblc-select-single
  : '?' expr '=>' stmt
  | '?' expr '=>' stmt (':' expr '=>' stmt)+
  | '?' expr '=>' stmt (':' expr '=>' stmt)+ ':' stmt
  ;

expr-select-single
  : expr '?' expr ':' expr
  ;
