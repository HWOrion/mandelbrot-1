# mandelbrot
Mandelbrot, multibrot and Julia set generator written in C. Program output is 24-bit uncompresed BMP image. Usage: 
```
./mandelbrot [OPTION]
```

Options:

* `--mandelbrot` (default) program calculates mandelbrot set
* `--multibrot` program calculates multibrot set
* `--julia` program calculates Julia set
* `--monochrome` (default) output image will be black and white (fastest rendering)
* `--bicolor` output image will be in shades of two colors (pink and blue)
* `--continous` program will use continous coloring (still bugy!)
* `--width` or `-w` specifies width of image in pixels
* `--height` or `-h` specifies height of image in pixels
* `--iteration` or `-d` specifies number of iterations used for calculating sets. Default is 1000
* `-x` abscissa of imgae center 
* `-y` ordinate of image center
* `--imaginary` specifies imaginary part of complex number C used for computing Julia set (see below)
* `--real` specifies real part of complex number C used for computing Julia set (see below)
* `--exponent` specifies exponent `r` used for computing multibrot or Julia set (see below)
* `--help` print help message


**Mandelbrot set** is the set of complex numbers `c` for which the function `f(z)=z²+c` does not diverge when iterated.

**Multibrot set** is the set of complex numbers `c` for which the function `f(z)=zʳ+c` does not diverge when iterated. Real number `r` is specified wih  `--exponent`.

Some **Julia sets** are sets of complex numbers `c` for which the function `f(z)=cʳ+z` does not diverge when iterated. Complex number `z` is specified with `--real` an `--imaginary` comands and and real number `r` is specified wih `--exponent`.


#### compiling and running:
Program use `gmp.h` and `mpfr.h` for arbitrary precision floating point math and `getopt.h` for parsing input, so you must have these libraries on your machine if you want to compile mandelbrot program. To compile run makefile or type following comand:
```
gcc -lgmp -lmpfr mandelbrot.c -o mandelbrot
```
then run: `./mandelbrot`. Program is tested on x86_64 linux machine . 

#### examples:
All examples are scaled-down anti-aliased png versions of program output. Program currently doesn't have antaliasing support.

![mandelbrot set with 20 iterations](https://raw.githubusercontent.com/ubavic/mandelbrot/master/examples/mandelbrot.png)

#### Thank you...
Thank you for checking my project. Please inform me of any bugs/mistakes.

This project is under MIT licence.



