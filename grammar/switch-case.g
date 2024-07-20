-- This is a file in which is described multiple selection (switch-case) grammar.
--
stmt-select-multpl
  : 'switch' expr stmt-select-multpl-body
  ;

stmt-select-multpl-body
  : stmt-select-multpl-case
  | stmt-select-multpl-defl
  | '{' stmt-select-multpl-case* stmt-select-multpl-defl? '}'
  ;

stmt-select-multpl-case
  : 'case' expr-constn ':' stmt*
  ;

stmt-select-multpl-defl
  : 'default' ':' stmt*
  ;

stmt-symblc-select-multpl
  : '??' expr '->' stmt-symblc-select-multpl-body
  ;

stmt-symblc-select-multpl-body
  : stmt-symblc-select-multpl-case
  | stmt-symblc-select-multpl-defl
  | '{' stmt-symblc-select-multpl-case* stmt-symblc-select-multpl-defl? '}'
  ;

stmt-symblc-select-multpl-case
  : expr-constn '=>' stmt
  ;
  
