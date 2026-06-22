# Planned Features

## Variable Units
The units of function arguments may include variables, this allows the same function to be used for multiple units.

```sprokit
func acceleration(const v0 #%d/%t : float64, const vt #%d/%t : float64, const time #%t : float64) #%d/%t^2 : float64 {
	return (vt - v0) / time;
}
```

## Arithmetic Catch
Sprokit can catch integer overflows and division by zero.

```sprokit
const result : int32 = a + b catch 0;
```

## Memory Safety
Sprokit uses a borrow checker similar to Rust.

## Initializer statements
```sprokit
const a : int32 = if b > 100 {
	yield 100;
} else {
	yield b;
}
```

## break and continue
```sprokit
for var i = 0; i < 100; i = i + 1 {
	for var j = 0; j < 100; j = j + 1{
		if j == a {
			break i; // break out of outer loop
		}
		if i == b {
			break; // break out of inner most loop
		}
	}
}
```

## switch/case statements
```sprokit
switch a {
	case 0 {

	}
}
```

## for/else
```sprokit
for var a = data.length(); a > 0; a = a - 1 {

} else {
	// Loop ended normally, no break
	// a is in scope here as well
}
```

## Attributes
Attributes may be attached to the declaration of any variable, function, struct, or enum.  These attributes modify how the compiler handles the object.

```sprokit
var `volatile `endian(big) x : int32 = 0;
```

Attribute names may include a namespace, attributes without a namespace are reserved by the Sprokit spec and must either be supported by the compiler or produce a compile error.  Attributes with an unrecognized namespace are ignored by the compiler, these may be used for other tools such as static analysis.  Each attribute may be specified on once per declaration.

Some attribute take arguments, which may be either a word or a number.  Word arguments may also include a namespace, this is useful for compiler extensions.

### Variable Attributes
| Name         | Value      | Description                                           |
|--------------|------------|-------------------------------------------------------|
| `align`      | Bytes      | Required alignment in bytes                           |
| `endian`     | `big`      | Use big-endianness                                    |
|              | `little`   | Use little-endianness                                 |
| `section`    | name       | Linker section to store the variable in               |
| `static`     |            | Static lifetime                                       |
| `volatile`   |            | Compiler must not optimize out reads and writes       |

### Function Attributes
| Name         | Value      | Description                                           |
|--------------|------------|-------------------------------------------------------|
| `interrupt`  |            | Function is an interrupt service routine              |
| `visibility` | `public`   | Public visibility                                     |
|              | `private`  | Private visibility (default)                          |

### Struct Attributes

| Name     | Values | Description                              |
|----------|--------|------------------------------------------|
| `packed` |        | Packs the member variables with no gaps  |

#### Member Attributes
In addition to the [Variable Attributes](#variable-attributes):
| Name         | Value      | Description                                           |
|--------------|------------|-------------------------------------------------------|
| `mutable`    |            | Variable can be modified even if the object is const  |
| `visibility` | `public`   | Public visibility                                     |
|              | `private`  | Private visibility (default)                          |
| `virtual`    | (none)     | Automatically determine `base` or `override`          |
|              | `base`     | Base implementation, default if no parameter is given |
|              | `final`    | Final implementation, may not be overridden           |
|              | `override` | Override implementation                               |

### Enum Attributes

| Name     | Values               | Description                                                  |
|----------|----------------------|--------------------------------------------------------------|
| `coding` | `count`, init, step  | Numbers enum with sequential value (default, init=0, step=1) |
|          | `gray`               | Numbers enum with values using Gray Code                     |
|          | `onehot`             | Numbers enum with values that have a single 1-bit            |
