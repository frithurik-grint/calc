# Functions

## Syntactic Productions

### Functions

    decl-param
        : id ':' expr-type
        ;
        
    decl-param-list
        : decl-param ',' decl-param-list
        | decl-param
        ;

    decl-funct
        : id '(' decl-param-list ')' '->' expr-type
        | id '()' '->' expr_type
        ;

    defn-funct-body
        : '{' stmt-list '}'
        | '=' stmt
        ;

    defn-funct
        : decl-funct defn-funct-body
        ;


### Variadic Functions

    decl-param-variadic
        : '...' decl-param
        | '...'
        ;

    decl-param-variadic-list
        : decl-param-list ',' decl-param-variadic
        ;

    decl-funct-variadic
        : id '(' decl-param-variadic-list ')' '->' expr-type
        | id '()' '->' expr-type
        ;

    defn-funct
        : decl-funct-variadic defn-funct-body
        ;
        
### Map Functions

    decl-param-map
        : id
        ;

    decl-param-map-list
        : decl-param-map ',' decl-param-map-list
        | decl-param-map
        ;

    decl-funct-map
        : id '(' decl-param-map-list ')' ':' expr-type '->' expr-type
        ;
        
    defn-funct-map
        : decl-funct-map defn-funct-body
        ;

### Variadic Map Functions

    decl-param-map-variadic
        : '...' id
        | '...'
        ;

    decl-param-map-variadic-list
        : decl-param-map-list ',' decl-param-map-variadic
        ;

    decl-funct-map-variadic
        : id '(' decl-param-map-variadic-list ')' ':' expr-type '->' expr-type
        ;
        
    defn-funct-map-variadic
        : decl-funct-map-variadic defn-funct-body
        ;

## Examples

A C function might be:

    int f(int x)
    {
        return x + 1;
    }

and in calc is possible to declare the same thing as a map
function:

    f(x) : int -> int
        = x + 1;

The C `printf` function declaration:

    int printf(const char *const format, ...);

becomes

    printf(format : const char *, ...) -> int;
