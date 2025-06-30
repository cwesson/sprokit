# Sprokit Language

## Keywords
`const`
`else`
`false`
`for`
`func`
`if`
`operator`
`return`
`true`
`type`
`unit`
`var`

## Operators
| Operator | Name                    | Examples      | Description |
|----------|-------------------------|---------------|-------------|
| `.`      | Member access           | `this.a`      | |
| `@`      | Address of              | `@b`          | |
| `^`      | Exponent                | `a ^ 2`       | |
| `*`      | Multiplication          | `4 * a`       | |
| `/`      | Division                | `1 / b`       | |
| `%`      | Modulo                  | `c % 10`      | |
| `+`      | Addition                | `a + b`       | |
| `-`      | Subtraction, negation   | `a - b` `-12` | |
| `~`      | Bitwise NOT             | `~toggle`     | |
| `&`      | Bitwise AND             | `a & mask`    | |
| `\|`     | Bitwise OR              | `a \| set`    | |
| `<<`     | Left Shift              | `1 << 12`     | |
| `>>`     | Right Shift             | `a >> 4`      | |
| `!`      | Boolean NOT             | `!a`          | |
| `&&`     | Boolean AND             | `a && b`      | |
| `\|\|`   | Boolean OR              | `a \|\| b`    | |
| `==`     | Equality                | `a == b`      | |
| `!=`     | Not equal               | `a !+ b`      | |
| `<`      | Less than               | `a < b`       | |
| `>`      | Greater than            | `a > b`       | |
| `<=`     | Less than or equal      | `a <= b`      | |
| `>=`     | Greater than or equal   | `a >= b`      | |
| `        | Compile-time properties | a`size        | |
| `=`      | Assignment              | `a = b`       | |

## Variables
All variables in Sprokit must be declared either `var` or `const`.  Variables declared `var` must be modified during their lifetime, while variables declared `const` (constants) may not be modified.
