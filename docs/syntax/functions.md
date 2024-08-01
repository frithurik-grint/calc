# Functions

## Function Declarations

In calc functions are variables that stores the addres to a specificly
typed procedure. This means that functions are referenced entities - like
objects - and their value can change.

Generally, to declare a function must be follwed this syntax producion:

    decl-func
        : decl-modifiers? id '(' id-list ')' ':' expr-type '->' expr-type
        | decl-modifiers? id '(' decl-param-list ')' '->' expr-type
        | decl-modifiers? id '()' '->' expr-type
        ;
