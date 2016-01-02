# mandelbrot
Mandelbrot, multibrot and Julia set generator written in C. Program output is 24-bit uncompressed BMP image. Usage: 
```
./mandelbrot [OPTION]
```

Options:

* `--mandelbrot` (default) program calculates mandelbrot set
* `--multibrot` program calculates multibrot set
* `--julia` program calculates Julia set
* `--monochrome` (default) output image will be black and white (fastest rendering)
* `--bicolor` output image will be in shades of two colors (pink and blue)
* `--continous` program will use continous coloring (slowest rendering)
* `--width` or `-w` specifies width of image in pixels
* `--height` or `-h` specifies height of image in pixels
* `--iteration` or `-d` specifies number of iterations used for calculating sets. Default value is `1000`
* `-x` specifies abscissa of image center  Default value is `0.0`
* `-y` specifies ordinate of image center. Default value is `0.0`
* `--imaginary` or `-i` specifies imaginary part of complex number C used for computing Julia set (see below)
* `--real` or `-r` specifies real part of complex number C used for computing Julia set (see below)
* `--exponent` or `-e` specifies exponent `r` used for computing multibrot or Julia set (see below)
* `--help` print help message


**Mandelbrot set** is the set of complex numbers `c` for which the function `f(z)=z²+c` does not diverge when iterated.

**Multibrot set** is the set of complex numbers `c` for which the function `f(z)=zʳ+c` does not diverge when iterated. Real number `r` is specified wih  `--exponent`.

Some **Julia sets** are sets of complex numbers `c` for which the function `f(z)=cʳ+z` does not diverge when iterated. Complex number `z` is specified with `--real` an `--imaginary` commands and and real number `r` is specified wih `--exponent`.

Abscissa, ordianate, exponent, imaginary and real part are floating point numbers. For negative numbers you must type `n` instead minus sign (So `-2.7` is typed like `n2.7`). All other values (width, height, iterations) are positive integer numbers. 

### Planed features
* interior distance estimation
* cardioid checking
* periodicty checking
* antialiasing
* series aproximantion
* burning ship fractal


#### Compiling and running:
Program use `gmp.h` and `mpfr.h` for arbitrary precision floating point math and `getopt.h` for parsing input, so you must have these libraries on your machine if you want to compile mandelbrot program. To compile run makefile or type following comand:
```
gcc -lgmp -lmpfr mandelbrot.c -o mandelbrot
```
then run: `./mandelbrot`. Program is tested on x86_64 linux machine . 

#### Examples:
All examples are scaled-down anti-aliased png versions of program output. Program currently doesn't have antaliasing support.

![mandelbrot set with 20 iterations](https://raw.githubusercontent.com/ubavic/mandelbrot/master/examples/mandelbrot.png)

#### Thank you...
Thank you for checking my project. Please inform me of any bugs/mistakes. This project is under MIT licence.

May the force be with you.
