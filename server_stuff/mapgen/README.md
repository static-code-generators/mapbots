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
usage: plot.py [-h] [-l [LINEFILE]] [-p [POINTFILE]] [--xmin XMIN]
               [--xmax XMAX] [--ymin YMIN] [--ymax YMAX]
               outfile

Plot lines (Hesse normal form) and points (Cartesian coordinates)

positional arguments:
  outfile               Output image file

optional arguments:
  -h, --help            show this help message and exit
  -l [LINEFILE], --linefile [LINEFILE]
                        File from which to read lines
  -p [POINTFILE], --pointfile [POINTFILE]
                        File from which to read points
  --xmin XMIN           Minimum value to plot on X-axis (default = 0)
  --xmax XMAX           Maximum value to plot on X-axis (default = 100)
  --ymin YMIN           Minimum value to plot on Y-axis (default = 0)
  --ymax YMAX           Maximum value to plot on Y-axis (default = 100)
```

To read from stdin do
```bash
./plot.py -p - out.png
1 2
3 4
5 6
EOF
```
