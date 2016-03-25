## Raspi mapgen code

Compile using:
```bash
make
```
To compile individual libraries/binaries do:

```bash
# For libs in include/
make build/libname.o

# For binaries
make build/binname
```

#### Directory Structure
The main directory contains cpp files that have a `main()` function and use libs in `include/`

`include/` contains the libraries that you can `#include` in your program
`build/` is a directory ignored by git, in which all object files and binaries are compiled into
