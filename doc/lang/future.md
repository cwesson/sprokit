# Planned Features

## Initializer statements
```sprokit
const a : int32 = if b > 100 {
	yield 100;
} else {
	yield b;
}
```

## break and continue
```
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
	// Condition was false the first time
	// a is in scope here as well
}
```

## Attributes

### Variables
| Name         | Value      | Description                                           |
|--------------|------------|-------------------------------------------------------|
| `align`      | Bytes      | Required alignment in bytes                           |
| `endian`     | `big`      | Use big-endianness                                    |
|              | `little`   | Use little-endianness                                 |
| `section`    | name       | Linker section to store the variable in               |
| `static`     |            | Static lifetime                                       |
| `volatile`   |            | Compiler must not optimize out reads and writes       |

### Function
| Name         | Value      | Description                                           |
|--------------|------------|-------------------------------------------------------|
| `interrupt`  |            | Function is an interrupt service routine              |
| `visibility` | `public`   | Public visibility                                     |
|              | `private`  | Private visibility (default)                          |

### Struct

| Name     | Values | Description                              |
|----------|--------|------------------------------------------|
| `packed` |        | Packs the member variables with no gaps  |

#### Member Attributes
In addition to the [variable attributes](#attributes):
| Name         | Value      | Description                                           |
|--------------|------------|-------------------------------------------------------|
| `mutable`    |            | Variable can be modified even if the object is const  |
| `visibility` | `public`   | Public visibility                                     |
|              | `private`  | Private visibility (default)                          |
| `virtual`    | (none)     | Automatically determine `base` or `override`          |
|              | `base`     | Base implementation, default if no parameter is given |
|              | `final`    | Final implementation, may not be overridden           |
|              | `override` | Override implementation                               |

### Enum

| Name     | Values   | Description                                       |
|----------|----------|---------------------------------------------------|
| `coding` | `count`  | Numbers enum with sequential value (default)      |
|          | `gray`   | Numbers enum with values using Gray Code          |
|          | `onehot` | Numbers enum with values that have a single 1-bit |
