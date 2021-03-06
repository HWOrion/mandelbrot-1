#include <gmp.h>
#include <mpfr.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define NRM  "\x1B[0m" 
#define RED  "\x1B[31m" 
#define GRN  "\x1B[32m" 
#define BLU  "\x1B[34m" 

unsigned char header [54] = {'B', 'M', 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 54, 0x00, 0x00, 0x00, 40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 24, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

const unsigned char pad [3] = {0x00, 0x00, 0x00};
unsigned char *img;
unsigned char  	red [] = {0x00, 0xFF, 0x00},
		grn [] = {0x00, 0x00, 0xFF},
		blu [] = {0xFF, 0x00, 0x00};

int width = 1000, height = 1000, x = 0, y = 0, i = 0, iterations = 1000, mndl = 1, profile = 1, extra;
mpfr_t x_min, y_min, real, imag, res, x_new, y_new, x_tmp, y_tmp, zoom, exponent, c_real, c_imag;
float nu = 0;
FILE *bmp;

static struct option long_options[] ={
	{"mandelbrot",   no_argument, &mndl, 1},
	{"multibrot",    no_argument, &mndl, 2},
	{"julia",        no_argument, &mndl, 3},
	{"ship",       	 no_argument, &mndl, 4},
	{"monochrome",   no_argument, &profile, 1},
	{"bicolor",      no_argument, &profile, 2},
	{"continous",    no_argument, &profile, 3},
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


void (*color_pixel)(int, int);

void color_pixel_monochrome (int pixel, int i);
void color_pixel_bicolor (int pixel, int i);
void color_pixel_continous (int pixel, int i);
int mandelbrot ();
int multibrot ();
int julia ();
int ship ();
int parse (int argc, char* argv[]);
float parse_number(char * string);
int write_int();
int write_row();

int main (int argc, char* argv[])
{
	//mpfr_set_default_prec(1000);
	//mpfr_set_emin (mpfr_get_emin_min());
	mpfr_inits (x_min, y_min, real, imag, res, x_new, y_new, x_tmp, y_tmp, zoom, exponent, c_real, c_imag, (mpfr_ptr) 0);
	
	parse (argc, argv);

       	img = (unsigned char*)malloc(3 * width);
	if (img == NULL){printf(RED"[!]"NRM"Memory error!\n"NRM); return 0;}
	while (i < 3 * width){img[i] = 0xFF; i++;}

	printf("Calculating set...\n");
	
	write_int();
	
	if (profile == 1)
		{color_pixel = &color_pixel_monochrome;}
	else if (profile == 2)
		{color_pixel = &color_pixel_bicolor;}
	else if (profile == 3)
		{color_pixel = &color_pixel_continous;}
	
	if (mndl == 1)
		{mandelbrot();}
	else if (mndl == 2)
		{multibrot();}
	else if (mndl == 3)
		{julia();}
	else if (mndl == 4)
		{ship();}

	mpfr_clears(x_min, y_min, real, imag, res, x_new, y_new, x_tmp, y_tmp, zoom, exponent, c_real, c_imag, (mpfr_ptr) 0);
	free(img);
	fclose(bmp);

	printf(GRN"Done!"NRM"\nBye!\n\n"NRM);
	return 0;
}

void color_pixel_monochrome (int pixel, int i)
{
	if (i == iterations){
		img[pixel]   = 0x00;
		img[pixel+1] = 0x00;
		img[pixel+2] = 0x00;
	}
	else{
		img[pixel]   = 0xFF;
		img[pixel+1] = 0xFF;
		img[pixel+2] = 0xFF;
	}
}


void color_pixel_bicolor (int pixel, int i)
{
	if (i < iterations) {
		img [pixel  ] = 250;
		img [pixel+1] = 200 - 2*i;
		img [pixel+2] = 200 - 4*i;
	}
	else {
		img [pixel  ] = 0xE0;
		img [pixel+1] = 0xFF;
		img [pixel+2] = 0xFF;
	}
}

void color_pixel_continous (int pixel, int i)
{
	if (i < iterations){
		i = i%3;
		img [pixel]   = blu [i] + nu*(blu[(i+1)%3] - blu[i]);
		img [pixel+1] = grn [i] + nu*(grn[(i+1)%3] - grn[i]);
		img [pixel+2] = red [i] + nu*(red[(i+1)%3] - red[i]);
	}
	else{
		img[pixel]   = 0x00;
		img[pixel+1] = 0x00;
		img[pixel+2] = 0x00;
	}
}

int mandelbrot ()
{
	mpfr_set(imag, y_min, MPFR_RNDN);
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

				mpfr_hypot(x_tmp, x_new, y_new, MPFR_RNDN);
				
				if (profile == 3 && mpfr_cmp_ui(x_tmp, 256) > 0)
				{break;}
				else if (mpfr_cmp_ui(x_tmp, 2) > 0)
				{break;} 

				i++;
			}

	
			if (profile == 3 && i < iterations){
				mpfr_log10(x_tmp, x_tmp, MPFR_RNDN);
				mpfr_const_log2(y_tmp, MPFR_RNDN);
				mpfr_div(x_tmp, x_tmp, y_tmp, MPFR_RNDN);
				mpfr_log2(x_tmp, x_tmp, MPFR_RNDN);
				mpfr_ui_sub(x_tmp, i, x_tmp, MPFR_RNDN);
				nu = mpfr_get_flt(x_tmp, MPFR_RNDN);
			}

			color_pixel(x, i);
	
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
{
	mpfr_set(imag, y_min, MPFR_RNDN);
	while (y < height){
		x = 0;
		mpfr_set(real, x_min, MPFR_RNDN);

		while (x < 3 * width){	
			i = 0;
			mpfr_set_flt(x_new, 0.0, MPFR_RNDN);
			mpfr_set_flt(y_new, 0.0, MPFR_RNDN);

			while (i < iterations){
				mpfr_hypot(x_tmp, x_new, y_new, MPFR_RNDN);
				mpfr_pow(x_tmp, x_tmp, exponent, MPFR_RNDN);
							
				mpfr_atan2(y_tmp, y_new, x_new, MPFR_RNDN);
				mpfr_mul(y_tmp, y_tmp, exponent, MPFR_RNDN);

				mpfr_cos(x_new, y_tmp, MPFR_RNDN);
				mpfr_mul(x_new, x_new, x_tmp, MPFR_RNDN);
				mpfr_add(x_new, x_new, real, MPFR_RNDN);

				mpfr_sin(y_new, y_tmp, MPFR_RNDN);
				mpfr_mul(y_new, y_new, x_tmp, MPFR_RNDN);
				mpfr_add(y_new, y_new, imag, MPFR_RNDN);

				mpfr_hypot(x_tmp, x_new, y_new, MPFR_RNDN);
				
				if (profile == 3 && mpfr_cmp_ui(x_tmp, 256) > 0)
				{break;}
				else if (mpfr_cmp_ui(x_tmp, 2) > 0)
				{break;} 

				i++;
			}

			if (profile == 3 && i < iterations){
				mpfr_log10(x_tmp, x_tmp, MPFR_RNDN);
				mpfr_const_log2(y_tmp, MPFR_RNDN);
				mpfr_div(x_tmp, x_tmp, y_tmp, MPFR_RNDN);
				mpfr_log2(x_tmp, x_tmp, MPFR_RNDN);
				mpfr_ui_sub(x_tmp, i, x_tmp, MPFR_RNDN);
				nu = mpfr_get_flt(x_tmp, MPFR_RNDN);
			}
	
			color_pixel(x, i);

			x = x + 3;
			mpfr_add(real, real, res, MPFR_RNDN);
		}
		write_row();
		mpfr_add(imag, imag, res, MPFR_RNDN);
		y++;
	}
	
	return 0;
}

int julia ()
{
	
	mpfr_set(imag, y_min, MPFR_RNDN);
	while (y < height){
		x = 0;
		mpfr_set(real, x_min, MPFR_RNDN);

		while (x < 3 * width){	
			i = 0;

			mpfr_set(x_new, real, MPFR_RNDN);
			mpfr_set(y_new, imag, MPFR_RNDN);

			while (i < iterations){
				///*
				mpfr_hypot(x_tmp, x_new, y_new, MPFR_RNDN);
				mpfr_pow(x_tmp, x_tmp, exponent, MPFR_RNDN);
							
				mpfr_atan2(y_tmp, y_new, x_new, MPFR_RNDN);
				mpfr_mul(y_tmp, y_tmp, exponent, MPFR_RNDN);

				mpfr_cos(x_new, y_tmp, MPFR_RNDN);
				mpfr_mul(x_new, x_new, x_tmp, MPFR_RNDN);
				mpfr_add(x_new, x_new, c_real, MPFR_RNDN);

				mpfr_sin(y_new, y_tmp, MPFR_RNDN);
				mpfr_mul(y_new, y_new, x_tmp, MPFR_RNDN);
				mpfr_add(y_new, y_new, c_imag, MPFR_RNDN);

				mpfr_hypot(x_tmp, x_new, y_new, MPFR_RNDN);
				
				if (profile == 3 && mpfr_cmp_ui(x_tmp, 256) > 0)
				{break;}
				else if (mpfr_cmp_ui(x_tmp, 2) > 0)
				{break;} 

				i++;
			}

			if (profile == 3 && i < iterations){
				mpfr_log10(x_tmp, x_tmp, MPFR_RNDN);
				mpfr_const_log2(y_tmp, MPFR_RNDN);
				mpfr_div(x_tmp, x_tmp, y_tmp, MPFR_RNDN);
				mpfr_log2(x_tmp, x_tmp, MPFR_RNDN);
				mpfr_ui_sub(x_tmp, i, x_tmp, MPFR_RNDN);
				nu = mpfr_get_flt(x_tmp, MPFR_RNDN);
			}

			color_pixel(x, i);

			x = x + 3;
			mpfr_add(real, real, res, MPFR_RNDN);
		}
		write_row();
		mpfr_add(imag, imag, res, MPFR_RNDN);
		y++;
	}
	
	return 0;
}

int ship ()
{
	mpfr_set(imag, y_min, MPFR_RNDN);
	while (y < height){
		x = 0;
		mpfr_set(real, x_min, MPFR_RNDN);

		while (x < 3 * width){	
			i = 0;
			mpfr_set_flt(x_new, 0.0, MPFR_RNDN);
			mpfr_set_flt(y_new, 0.0, MPFR_RNDN);

			while (i < iterations){
				mpfr_abs(x_new, x_new, MPFR_RNDN);
				mpfr_abs(y_new, y_new, MPFR_RNDN);
				mpfr_mul(x_tmp, x_new, x_new, MPFR_RNDN);
				mpfr_mul(y_tmp, y_new, y_new, MPFR_RNDN);
				mpfr_sub(x_tmp, x_tmp, y_tmp, MPFR_RNDN);
				mpfr_add(x_tmp, x_tmp, real, MPFR_RNDN);
				
				mpfr_mul(y_tmp, x_new, y_new, MPFR_RNDN);
				mpfr_mul_ui(y_tmp, y_tmp, 2, MPFR_RNDN);
				mpfr_add(y_new, y_tmp, imag, MPFR_RNDN);
				mpfr_set(x_new, x_tmp, MPFR_RNDN);

				mpfr_hypot(x_tmp, x_new, y_new, MPFR_RNDN);
				
				if (profile == 3 && mpfr_cmp_ui(x_tmp, 256) > 0)
				{break;}
				else if (mpfr_cmp_ui(x_tmp, 2) > 0)
				{break;} 

				i++;
			}

			if (profile == 3 && i < iterations){
				mpfr_log10(x_tmp, x_tmp, MPFR_RNDN);
				mpfr_const_log2(y_tmp, MPFR_RNDN);
				mpfr_div(x_tmp, x_tmp, y_tmp, MPFR_RNDN);
				mpfr_log2(x_tmp, x_tmp, MPFR_RNDN);
				mpfr_ui_sub(x_tmp, i, x_tmp, MPFR_RNDN);
				nu = mpfr_get_flt(x_tmp, MPFR_RNDN);
			}
	
			color_pixel(x, i);

			x = x + 3;
			mpfr_add(real, real, res, MPFR_RNDN);
		}
		write_row();
		mpfr_add(imag, imag, res, MPFR_RNDN);
		y++;
	}
	
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
					printf(RED"[!]"NRM"Width must be between 200 and 5000 px. Using default vaule (1000).\n");
					width = 1000;
				}
				break;
		      
			case 'h':
				height = atoi(optarg);
				if (height < 200 || height > 5000){
					printf(RED"[!]"NRM"Height must be between 200 and 5000 px. Using default value (1000).\n");
					height = 1000;
				}
				break;
			
			case 'd':
				iterations = atoi(optarg);
				if (iterations < 1 || iterations > 5000){
					printf(RED"[!]"NRM"Number of iterations must be between 1 and 5000. Using default value (1000).\n");
					iterations = 1000;
				}
				break;
		      
			case 'i':
				argument = parse_number(optarg);
				mpfr_set_flt(c_imag, argument, MPFR_RNDN);
				break;
					  						  
			case 'r':
				argument = parse_number(optarg);
				mpfr_set_flt(c_real, argument, MPFR_RNDN);
				break;

			case 'z':
				argument = atof(optarg);
				if (argument < 0)
					{printf(RED"[!]"NRM"Zoom must be bigger then 0. Using default value (1).\n");
					argument = 1;}
				mpfr_set_flt(zoom, argument, MPFR_RNDN);
				break;

			case 'x':
				argument = parse_number(optarg);
				mpfr_set_flt(x_min, argument, MPFR_RNDN);
				break;

			case 'y':
				argument = parse_number(optarg);
				mpfr_set_flt(y_min, argument, MPFR_RNDN);
					break;

			case 'e':
				argument = parse_number(optarg);
				if (mndl == 2 && argument == 2.0)
					{mndl = 1;}
				mpfr_set_flt(exponent, argument, MPFR_RNDN);
				break;

			case 'u':
				printf("\nUsage: ./mandelbrot " RED "[OPTIONS]\n--mandelbrot" NRM " (default) program calculates mandelbrot set\n" RED "--multibrot" NRM " program calculates multibrot set \n" RED "--julia" NRM " program calculates Julia set \n" RED "--ship" NRM " program calculates burning ship fractal \n" RED "--monochrome" NRM " (default) output image will be black and white (fastest rendering) \n" RED "--bicolor" NRM " output image will be in shades of two colors (pink and blue) \n" RED "--continous" NRM " program will use continous coloring (slowest rendering) \n" RED "--width"NRM" or "RED"-w"NRM" specifies width of image in pixels \n" RED "--height" NRM " or " RED "-h" NRM " specifies height of image in pixels \n" RED "--iteration" NRM " or " RED "-d" NRM " specifies number of iterations used for calculating sets. Default value is 1000 \n" RED " -x" NRM " specifies abscissa of image center Default value is 0.0 \n" RED " -y" NRM " specifies ordinate of image center. Default value is 0.0 \n" RED "--zoom" NRM " or " RED "-z" NRM " specifies zoom factor. Default value is 1.0 \n" RED "--imaginary" NRM "  or " RED " -i " NRM " specifies imaginary part of complex number C used for computing Julia set\n" RED "--real" NRM " or " RED "-r" NRM " specifies real part of complex number C used for computing Julia set\n" RED "--exponent" NRM " or " RED "-e" NRM " specifies exponent r used for computing multibrot or Julia set\n" RED "--help" NRM " print this message \n\n");
				break;
											    
			default:
				break;
		}
	}
  
	if (optind < argc){
		printf (RED"[!]"NRM"Invalid options: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		printf("Type mandelbrot --help for usage.\n");
	}
	
	if (mndl == 1){
		printf("Calculating Mandelbrot set ");
	}
	else if (mndl == 2){
		mpfr_printf("Calculating Multibrot set, with exponent=%Rf, ", exponent);
	}
	else if (mndl == 3){
		mpfr_printf("Calculating Julia set, with c = %Rf + %Rfi and exponent=%Rf, ", c_real, c_imag, exponent);
	}
	else if (mndl == 4){
		printf("Calculating burning ship fractal, ");
	}

	mpfr_printf("centered at %Rf + %Rfi and with %Rf zoom factor.\n", x_min, y_min, zoom);
        printf("Image dimensios are %dpx by %dpx. Set is calculated with with max %d iterations per point, and rendered with clolor profile No. %d.\n", width, height, iterations, profile);

	mpfr_ui_div(zoom, 2, zoom, MPFR_RNDN);

	if (width >= height){
		mpfr_sub(y_min, y_min, zoom, MPFR_RNDN);
		mpfr_mul_ui(zoom, zoom, 2, MPFR_RNDN);
		mpfr_div_ui(res, zoom, height, MPFR_RNDN);
		mpfr_mul_d(zoom, res, width/2, MPFR_RNDN);
		mpfr_sub(x_min, x_min, zoom, MPFR_RNDN);
	} else {
		mpfr_sub(x_min, x_min, zoom, MPFR_RNDN);
		mpfr_mul_ui(zoom, zoom, 2, MPFR_RNDN);
		mpfr_div_ui(res, zoom, width, MPFR_RNDN);
		mpfr_mul_d(zoom, res, height/2, MPFR_RNDN);
		mpfr_sub(y_min, y_min, zoom, MPFR_RNDN);
	}
	
	
	//mpfr_printf("at x_min = %Rf y_min = %Rf \n", x_min, y_min);
	//mpfr_printf("Resolution: %.12Rf per pixel. \n", res);

	return 0;
}

float parse_number(char * string)
{  
		return atof(string);
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
