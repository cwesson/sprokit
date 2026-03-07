# Building
The Sprokit compiler

## Dependencies
The Sprokit compiler requires `make`, `clang++`, `flex`, and `bison` to be compiled and uses the `llvm-dev` library.
```shell
sudo apt install make clang++ flex bison llvm-dev
```

## Building
After the dependencies are installed, sprokit can be compiled using `make`:
```shell
make all -j 8
```

Once `make` completes, the compiler is at `bin/sprokit`.
