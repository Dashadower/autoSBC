# My notes for stan syntax and grammer

Values in parentheses are keyword for certain expressions/statements
Values also in braces mean optional grammer.
*All statements/expressions are terminated by a semicolon*
Note that spaces do not always act as a delimeter!
## variable/parameter naming rules:
For scalar/array parameters:
```
(type){<(constraint)>} (name){[(array_dim)]}
```
where `type` is one of `int, real`, `constraint` follows the form `{<lower=(real_a),upper=(real_b)>}` where `a`, `b` are real values.
`name` has no restrictions except for restricted keywords, `array_dim` has the form `[(int_a),(int_b),(int_c)]` up to some specified dimension,
comma separated.

For matrix/vector parameters:
```
(type){[dim]}{<constraint>} (name){[(array_dim)]}
```
where `type` is one of `ordered, simplex, unit_vector, positive_ordered, row_vector, cov_matrix, corr_matrix, choleskey_factor_cov, choleskey_factor`.

For vector types(`ordered, simplex, unit_vector, positive_ordered, row_vector`), `dim` is a one dimensional integer. For other types, 
2 dimensions. `name` and `array_dim` follow the same rules as scalar parameters.

## assignments:
```
(vari_name)=(expr)
```
the yielding type of `expr` must match the type of `vari_name`.
Data, parameter, and variables can be assigned to a static value or an expression.

## Arthimetic and primitive operators

Primitive binary arthimetic operators (`+ - * / %`) are supported for same types of elements.

Functions are applied in the following expression:
```
(function_name)((arg_expr))
```

## array indexing:

arr_name[(int_expr)]

## loops

for loops:
```
for((a) in (b:c)){
  (expr...)
}
```

while loops:

while((bool_expr)){
    (expr/statement)
}
## comments

Single line comments are started with `//`.
Multi-line comments use the same syntax as C, `/* ... */`