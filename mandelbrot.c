#include <gmp.h>
#include <mpfr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

unsigned char header [54] = {'B', 'M', 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 54, 0x00, 0x00, 0x00, 40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 24, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

const unsigned char pad [3] = {0x00, 0x00, 0x00};
unsigned char *img;
unsigned char  	red [] = {0x00, 0xFF, 0x00},
		grn [] = {0x00, 0x00, 0xFF},
		blu [] = {0xFF, 0x00, 0x00};

int width = 1000, height = 1000, x = 0, y = 0, i = 0, iterations = 1000, mndl = 1, mono = 1, extra;
mpfr_t x_min, y_min, real, imag, res, x_new, y_new, x_tmp, y_tmp, zoom, exponent;
float nu = 0;
FILE *bmp;

static struct option long_options[] ={
	{"mandelbrot",   no_argument, &mndl, 1},
	{"multibrot",    no_argument, &mndl, 2},
	{"julia",        no_argument, &mndl, 3},
	{"monochrome",   no_argument, &mono, 1},
	{"bicolor",      no_argument, &mono, 2},
	{"continous",    no_argument, &mono, 3},
	{"width",        required_argument, 0, 'w'},
	{"height",       required_argument, 0, 'h'},
	{"iteration",    required_argument, 0, 'd'},
	{"imaginary",    required_argument, 0, 'i'},
	{"real",         required_argument, 0, 'r'},
	{"zoom",         required_argument, 0, 'z'},
	{"x",            required_argument, 0, 'x'},
	{"y",            required_argument, 0, 'y'},
	{"exponent",     required_argument, 0, 'e'},
	{"help",         no_argument, 0, 'u'},
	{0, 0, 0, 0}
};


int color_pixel (int pixel, int i, float f);
int mandelbrot ();
int multibrot ();
int julia ();
int parse (int argc, char* argv[]);
int write_int();
int write_row();

int main (int argc, char* argv[])
{
	mpfr_inits (x_min, y_min, real, imag, res, x_new, y_new, x_tmp, y_tmp, zoom, exponent, (mpfr_ptr) 0);
	
	parse (argc, argv);

       	img = (unsigned char*)malloc(3 * width);
	if (img == NULL){printf("Memory error!"); return 0;}
	while (i < 3 * width){img[i] = 0xFF; i++;}

	printf("Calculating set...\n");
	
	write_int();

	if (mndl == 1)
		{mandelbrot();}
	else if (mndl == 2)
		{multibrot();}
	else if (mndl == 2)
		{julia();}

	mpfr_clears(x_min, y_min, real, imag, res, x_new, y_new, x_tmp, y_tmp, zoom, exponent, (mpfr_ptr) 0);
	free(img);
	fclose(bmp);

	printf("Done! Bye!\n\n");
	return 0;
}

int color_pixel (int pixel, int i, float f)
{
	if (mono == 3 && i < iterations) {
		i = i%3;
		img [pixel]   = blu [i] + f*(blu[(i+1)%3] - blu[i]);
		img [pixel+1] = grn [i] + f*(grn[(i+1)%3] - grn[i]);
		img [pixel+2] = red [i] + f*(red[(i+1)%3] - red[i]);
	}
	else if (mono == 2 && i < iterations) {
		img [pixel]   = 0xFF;
		img [pixel+1] = 200 - 3 * i;
		img [pixel+2] = 255 - 4*i;
	}
	else if (mono == 2 && i == iterations) {
		img [pixel]   = 0xFF;
		img [pixel+1] = 0xFF;
		img [pixel+2] = 0xFF;
	}
	else if (i == iterations){
		img[pixel]   = 0x00;
		img[pixel+1] = 0x00;
		img[pixel+2] = 0x00;
	}
	else{
		img[pixel]   = 0xFF;
		img[pixel+1] = 0xFF;
		img[pixel+2] = 0xFF;
	}

	return 0;
}

int mandelbrot ()
{
	while (y < height){
		x = 0;
		mpfr_set(real, x_min, MPFR_RNDN);

		while (x < 3 * width){	
			i = 0;
			mpfr_set_flt(x_new, 0.0, MPFR_RNDN);
			mpfr_set_flt(y_new, 0.0, MPFR_RNDN);

			while (i < iterations){
				mpfr_mul(x_tmp, x_new, x_new, MPFR_RNDN);
				mpfr_mul(y_tmp, y_new, y_new, MPFR_RNDN);
				mpfr_sub(x_tmp, x_tmp, y_tmp, MPFR_RNDN);
				mpfr_add(x_tmp, x_tmp, real, MPFR_RNDN);
				
				mpfr_mul(y_tmp, x_new, y_new, MPFR_RNDN);
				mpfr_mul_ui(y_tmp, y_tmp, 2, MPFR_RNDN);
				mpfr_add(y_new, y_tmp, imag, MPFR_RNDN);
				mpfr_set(x_new, x_tmp, MPFR_RNDN);

				mpfr_mul(x_tmp, x_new, x_new, MPFR_RNDN);
				mpfr_mul(y_tmp, y_new, y_new, MPFR_RNDN);
				mpfr_add(x_tmp, x_tmp, y_tmp, MPFR_RNDN);
				
				if (mono != 3 && mpfr_cmp_ui(x_tmp, 2) > 0)
				{break;}
				else if (mpfr_cmp_ui(x_tmp, 256) > 0)
				{break;} 

				i++;
			}

			if (mono == 3 && i < iterations){
				mpfr_hypot(x_tmp, x_new, y_new, MPFR_RNDN);
				mpfr_log10(x_tmp, x_tmp, MPFR_RNDN);
				mpfr_const_log2(y_tmp, MPFR_RNDN);
				mpfr_div(x_tmp, x_tmp, y_tmp, MPFR_RNDN);
				mpfr_log2(x_tmp, x_tmp, MPFR_RNDN);
				mpfr_ui_sub(x_tmp, i, x_tmp, MPFR_RNDN);
				nu = mpfr_get_flt(x_tmp, MPFR_RNDN);
			}
	
			color_pixel(x, i, nu);

			x = x + 3;
			mpfr_add(real, real, res, MPFR_RNDN);
		}
		write_row();
		mpfr_add(imag, imag, res, MPFR_RNDN);
		y++;
	}

	return 0;
}

int multibrot ()
{return 0;}

int julia ()
{
	return 0;
}

int parse (int argc, char* argv[])
{ 
	int c, option_index;
	float argument;
	
	mpfr_set_si(zoom, 1, MPFR_RNDN);
	mpfr_set_si(exponent, 2, MPFR_RNDN);
	mpfr_set_si(x_min, 0, MPFR_RNDN);
	mpfr_set_si(y_min, 0, MPFR_RNDN);
	mpfr_set_si(imag, 0, MPFR_RNDN);
	mpfr_set_si(real, 0, MPFR_RNDN);

	while (1){
		c = getopt_long (argc, argv, "w:h:d:i:r:z:x:y:e:u",long_options, &option_index);

		if (c == -1)
		       	break;
	      
		switch (c){
			case 0:
				if (long_options[option_index].flag != 0)
					break;
				printf ("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
				printf ("\n");
				break;

			case 'w':
				width = atoi(optarg);
				if (width < 200 || width > 5000){
					printf("Width must be between 200 and 5000 px.\nUsing default vaule.\n");
					width = 1000;
				}
				break;
		      
			case 'h':
				height = atoi(optarg);
				if (height < 200 || height > 5000){
					printf("Height must be between 200 and 5000 px.\nUsing default value.\n");
					height = 1000;
				}
				break;
			
			case 'd':
				iterations = atoi(optarg);
				if (iterations < 1 || iterations > 5000){
					printf("Number of iterations must be between 1 and 5000.\nUsing default value.\n");
					iterations = 1000;
				}
				break;
		      
			case 'i':
				argument = atof(optarg);
				mpfr_set_flt(imag, argument, MPFR_RNDN);
				break;
					  						  
			case 'r':
				argument = atof(optarg);
				mpfr_set_flt(real, argument, MPFR_RNDN);
				break;

			case 'z':
				argument = atof(optarg);
				mpfr_set_flt(zoom, argument, MPFR_RNDN);
				break;

			case 'x':
				argument = atof(optarg);
				mpfr_set_flt(x_min, argument, MPFR_RNDN);
				break;

			case 'y':
				argument = atof(optarg);
				mpfr_set_flt(y_min, argument, MPFR_RNDN);
				break;

			case 'e':
				argument = atof(optarg);
				if (mndl == 1 && argument != 2.0)
					{mndl = 2;}
				mpfr_set_flt(exponent, argument, MPFR_RNDN);
				break;

			case 'u':
				printf("Usage\n");
				break;
											    
			default:
				break;
		}
	}
  
	if (optind < argc){
		printf ("Invalid options: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		printf("Type mandelbrot --help for usage.\n");
	}
	
	if (mndl == 1){
		printf("Calculating Mandelbrot set, ");
	}
	else if (mndl == 2){
		printf("Calculating Multibrot set with following settings:\n");
		mpfr_printf("exponent=%Rf, ", exponent);
	}
	else{
		printf("Calculating Julia set with following settings:\n");
		mpfr_printf("R(c)=%Rf I(c)=%Rf exponent=%Rf, ", real, imag, exponent);
	}

	mpfr_printf("at x = %Rf y = %Rf (zoom: %Rf), ", x_min, y_min, zoom);
        printf("on %dpx X %dpx image with %d iterations.\n", width, height, iterations);

	mpfr_ui_div(zoom, 4, zoom, MPFR_RNDN);

	mpfr_div_ui(zoom, zoom, 2, MPFR_RNDN);
	if (width >= height){
		mpfr_div_ui(res, zoom, height, MPFR_RNDN);
		mpfr_sub(y_min, y_min, zoom, MPFR_RNDN);
		mpfr_mul_d(zoom, y_min, (double) -width/height, MPFR_RNDN);
		mpfr_sub(x_min, x_min, zoom, MPFR_RNDN);
	} else {
		mpfr_sub(x_min, x_min, zoom, MPFR_RNDN);
		mpfr_div_ui(res, zoom, width, MPFR_RNDN);
	}
	
	
	mpfr_printf("at x_min = %Rf y_min = %Rf \n", x_min, y_min);
	mpfr_printf("Resolution %.12Rf\n", res);
	printf("Monochrome: %d\n", mono);

	return 0;
}


int write_int()
{ 	
	extra = 4 - ((width * 3) % 4);
	if (extra == 4) {extra = 0;}

	int filesize = 54 + ((width * 3 + extra) * height);

	header [2]  = (unsigned char)(filesize);
	header [3]  = (unsigned char)(filesize >> 8);
	header [4]  = (unsigned char)(filesize >> 16);
	header [5]  = (unsigned char)(filesize >> 24);
	header [18] = (unsigned char)(width);
	header [19] = (unsigned char)(width >> 8);
	header [20] = (unsigned char)(width >> 16);
	header [21] = (unsigned char)(width >> 24);
	header [22] = (unsigned char)(height);
	header [23] = (unsigned char)(height >> 8);
	header [24] = (unsigned char)(height >> 16);
	header [25] = (unsigned char)(height >> 24);

	bmp = fopen("/root/desktop/mandelbrot/mandelbrot.bmp","w+");
	fwrite(header, sizeof(char), 54, bmp);
	return 0;
}

int write_row()
{ 	
	fwrite(img, 3 * sizeof(char), width, bmp);
	if (extra != 0){fwrite(pad, sizeof(char), extra, bmp);}
	return 0;
}
