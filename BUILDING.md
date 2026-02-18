# Building
The Sprokit compiler

## Dependencies
The Sprokit compiler requires `make`, `g++`, `flex`, and `bison` to be compiled.
```shell
sudo apt install make g++ flex bison
```

## Building
After the dependencies are installed, sprokit can be compiled useing `make`:
```shell
make all -j 8
```

Once `make` completes, the compiler is at `bin/sprokit`.
