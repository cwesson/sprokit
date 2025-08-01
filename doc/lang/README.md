# Sprokit Language

## Keywords
| Name       | Description                         |
|------------|-------------------------------------|
| `break`    | Break out of a loop                 |
| `case`     | Switch case statement               |
| `const`    | Constant declaration, see [Variables](#variables)            |
| `continue` | Continue to next loop iteration     |
| `else`     | Else component of an `if` statement |
| `false`    | Boolean FALSE literal, see [Built-in Types](#built-in-types) |
| `for`      | Loop statement                      |
| `func`     | Function declaration                |
| `if`       | Conditional statement               |
| `operator` | Operator declaration                |
| `return`   | Function return statement           |
| `switch`   | Switch statement                    |
| `true`     | Boolean TRUE literal, see [Built-in Types](#built-in-types)  |
| `type`     | Type declaration, see [User Types](#user-types)              |
| `unit`     | Unit declaration, see [Units](#units)                        |
| `var`      | Variable declaration, see [Variables](#variables)            |
| `yield`    | Return result from initializer statement                     |

## Types

### Built-in Types
| Name      | Description             | Size (B) | Range                     |
|-----------|-------------------------|---------:|---------------------------|
| `bool`    | Boolean                 |        1 | `false` `true`            |
| `int8`    | 8-bit signed integer    |        1 | [-128, 127]               |
| `int16`   | 16-bit signed integer   |        2 | [-32768, 32767]           |
| `int32`   | 32-bit signed integer   |        4 | [−2147483648, 2147483647] |
| `int64`   | 64-bit signed integer   |        8 | [] |
| `uint8`   | 8-bit unsigned integer  |        1 | [] |
| `uint16`  | 16-bit unsigned integer |        2 | [] |
| `uint32`  | 32-bit unsigned integer |        4 | [] |
| `uint64`  | 64-bit unsigned integer |        8 | [] |
| `float16` | 16-bit floating-point   |        2 | [] |
| `float32` | 32-bit floating-point   |        4 | [] |
| `float64` | 64-bit floating-point   |        8 | [] |

### User Types

## Units
Numeric literals and variable declarations may be specified with units using `#`, for example: `6.674e-11 #m^3/kg*s^2`.  Unit names consist of dimensions with an optional positive integer exponent using `^`, separated by `*`.  A single `/` may be included to specify negative dimensions.  Units with no positive dimension must include a `1` before the `/`, for example Hertz could be specified as `#1/s`.  If an exponent is not specified it is assumed to be 1.  If a unit is not specified the unit `#1` is assumed.

Dimensional analysis is performed at compile-time when doing arithmetic with numeric values.

Note that because unit names include the arithmetic operators `*`, `^`, and `/`, a space is required between unit names and the those operators.  For example: `4.9 #m/s * s` means "4.9 meters per second times variable s" where as `4.9 #m/s*s` simplifies to "4.9 meters per second squared".

### Unit Declarations
While not necessary for simple dimensional analysis, units may be declared using the `unit` keyword when using more complex units and to allow automatic unit conversions.  Unit conversions are specified within the `unit` declaration.

```sprokit
uint #m {
	operator #km(const x) = x / 1000 #m/km;
	operator #mm(const x) = x * 1000 #mm/m;
}

unit #Hz : #1/s {
}
```

## Operators
| Operator | Name                    | Examples        | Description |
|----------|-------------------------|-----------------|-------------|
| `.`      | Member access           | `this.a`        | |
| `@`      | Address of              | `@b`            | Returns a pointer to the specified variable. |
| `^`      | Exponent                | `a ^ 2`         | |
| `*`      | Multiplication          | `4 * a`         | |
| `/`      | Division                | `1 / b`         | |
| `%`      | Modulo                  | `c % 10`        | |
| `+`      | Addition                | `a + b`         | |
| `-`      | Subtraction, Negation   | `a - b` `-12`   | |
| `~`      | Bitwise NOT             | `~toggle`       | |
| `&`      | Bitwise AND             | `a & mask`      | |
| `\|`     | Bitwise OR              | `a \| set`      | |
| `^\|`    | Bitwise XOR             | `a ^\| b`       | |
| `<<`     | Bitwise Left Shift      | `1 << 12`       | |
| `>>`     | Bitwise Right Shift     | `a >> 4`        | |
| `!`      | Boolean NOT             | `!a`            | |
| `&&`     | Boolean AND             | `a && b`        | |
| `\|\|`   | Boolean OR              | `a \|\| b`      | |
| `==`     | Equality                | `a == b`        | |
| `!=`     | Not equal               | `a != b`        | |
| `<`      | Less than               | `a < b`         | |
| `>`      | Greater than            | `a > b`         | |
| `<=`     | Less than or equal      | `a <= b`        | |
| `>=`     | Greater than or equal   | `a >= b`        | |
| `` ` ``  | Compile-time properties | ``a`size``      | |
| `?:`     | Ternary operator        | `cond ? t : f`  | |
| `=`      | Assignment              | `a = b`         | |
| `:`      | Type specifier          | ` var a: int32` | See [Types](#types) and [Variables](#variables) |
| `#`      | Unit specifier          | `9.8 #m/s^2`    | See [Units](#units) |

## Variables
All variables in Sprokit must be declared either `var` or `const`.  Variables declared `var` must be modified during their lifetime, while variables declared `const` (constants) may not be modified.  Every variable declaration must also include a type and initializer.  If a unit is specified in the variable declaration, it is used to verify the units of the initializer expression.

Example declarations:
```sprokit
const g #m/s^2: float64 = 49.05 #m/s / 5 #s;
const G: float64 = 6.674e-11 #m^3/kg*s^2;
var x: float64 = x0 + (v0 * t) + (0.5 * a * t^2);
```

## Properties
| Name     | Description                      |
|----------|----------------------------------|
| `size`   | Size of a variable in bytes      |
| `length` | Number of entries in an array    |
| `max`    | Maximum supported value for type |
| `min`    | Minimum supported value for type |
