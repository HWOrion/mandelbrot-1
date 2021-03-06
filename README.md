# mandelbrot
Mandelbrot, multibrot, Julia set and burning ship set generator written in C, with arbitrary precision floating point support. Program output is 24-bit uncompressed BMP image. Usage:
```
./mandelbrot [OPTION]
```

Options:

* `--mandelbrot` (default) program calculates mandelbrot set
* `--multibrot` program calculates multibrot set
* `--julia` program calculates Julia set
* `--ship`program calculates burning ship fractal
* `--monochrome` (default) output image will be black and white (fastest rendering)
* `--bicolor` output image will be in shades of two colors (pink and blue)
* `--continuous` program will use continuous coloring (slowest rendering)
* `--width` or `-w` specifies width of image in pixels
* `--height` or `-h` specifies height of image in pixels
* `--iteration` or `-d` specifies number of iterations used for calculating sets. Default value is `1000`
* `-x` specifies abscissa of image center  Default value is `0.0`
* `-y` specifies ordinate of image center. Default value is `0.0`
* `--zoom` or `-z` specifies zoom factor. Default value is `1.0`
* `--imaginary` or `-i` specifies imaginary part of complex number `C` used for computing Julia set (see below)
* `--real` or `-r` specifies real part of complex number `C` used for computing Julia set (see below)
* `--exponent` or `-e` specifies exponent `r` used for computing multibrot or Julia set (see below)
* `--help` print help message


**Mandelbrot set** is the set of complex numbers `c` for which the function `f(z)=z²+c` does not diverge when iterated (`z₀ = 0`).

**Multibrot set** is the set of complex numbers `c` for which the function `f(z)=zʳ+c` does not diverge when iterated (`z₀ = 0`). Real number `r` is specified with  `--exponent`.

Some **Julia sets** are sets of complex numbers `z` for which the function `f(z)=zʳ+C` does not diverge when iterated. Complex number `C` is specified with `--real` an `--imaginary` commands and and real number `r` is specified with `--exponent`.

**Burning ship** fractal is the set of complex numbers `c` for which the function `f(z)=(|Re(z)|+i|Im(z)|)²+c` does not diverge when iterated (`z₀ = 0`).

Abscissa, ordinate, exponent, zoom, imaginary and real part of `C`, are floating point numbers. All other values (width, height, iterations) are positive integer numbers.

#### Planed features
* periodicty checking
* series approximation
* color themes

#### Compiling and running:
Program use `gmp.h` and `mpfr.h` for arbitrary precision floating point math and `getopt.h` for parsing input, so you must have these libraries on your machine if you want to compile mandelbrot program. To compile, run makefile or type following command:
```
gcc -lgmp -lmpfr mandelbrot.c -o mandelbrot
```
then run: `./mandelbrot`. Program is tested on x86_64 Linux machine .

#### Examples:
All examples are scaled-down anti-aliased png versions of program output. Program doesn't have antialiasing support.

Mandelbrot set computed with 20 iterations:
![mandelbrot set with 20 iterations](https://raw.githubusercontent.com/ubavic/mandelbrot/master/examples/mandelbrot.png)


Mulribrot set computed with 20 iterations:
![multibrot set with 50 iterations](https://raw.githubusercontent.com/ubavic/mandelbrot/master/examples/mandelbrot2.png)


Burning ship computed with 100 iterations:
![burning ship 100 iterations](https://raw.githubusercontent.com/ubavic/mandelbrot/master/examples/mandelbrot3.png)


Detail of Mandelbrot set (with `--monochorme` flag):
![mandelbrot detail](https://raw.githubusercontent.com/ubavic/mandelbrot/master/examples/mandelbrot4.png)


Changes in output image depends on number of iterations (from 1 to 100):
![number of iterations](https://raw.githubusercontent.com/ubavic/mandelbrot/master/examples/mandelbrot1.gif)


Multibrot sets (`r` from 1 to 15):


![multibrot set](https://raw.githubusercontent.com/ubavic/mandelbrot/master/examples/mandelbrot2.gif)


#### Thank you...
Thank you for checking my project. Please inform me of any bugs/mistakes. This project is under MIT license.
```
Q: What is Benoit B. Mandelbrot's middle name?
A: Benoit B. Mandelbrot.
```
