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
The root directory contains cpp files that have a `main()` function and can be compiled into a binary

`include/` contains the libraries that you can `#include` in your program
`build/` is a directory ignored by git, all object files and binaries are compiled into it

#### Plotting
To plot `lines` and `points` which might help in visualizing the map generated, use the script `plot.py`

```bash
./plot.py --help
```

```bash
./plot.py -l lines.txt -p points.txt out.png
```
