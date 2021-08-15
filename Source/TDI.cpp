/*
Entrega parcial: 11/04/2021 Enlazado de bordes y segmentacion basada en la deteccion de bordes 
La finalidad de esta pratica es comprobar el comportamiento de tecnicas diferentes de enlazado de bordes y luego segmentar la imagen 
con la tecnica.

He implementado 3 tecnicas de enlazado de bordes:
1) Enlazado basado su mascara rapida (mascara isotropica) que al contrario de las mascara de Sobel, Prewitt, etc. 
	solo tenemos que convolucionarla una vez sobre la imagen.

2) Operador de uniformidad, usa la resta para buscar un borde.
   Basicamente la idea es de hacer la resta de todos los 8 vecinos a un pixel central.
   El resultado es el maximo (en valor absoluto) de las restas.
   En una region uniforme la resta ha como resultado 0, en una region con bordes fuertes ha un maximo grande.

3) Enlazado basado en contraste, este operador puede gestir areas de luz non uniforme
   En entrada recibe el resultado de un detector de bordes y luego divide por le valor medio de una vecinidad.
   Esta division quita el effecto de luz no uniforme y se puede implementar convolucionando con una mascara con valores
   todo igual a 1.

-Salvatore Starace
*/


#include <iostream>
#include <C_Image.hpp>
#include <C_Arguments.hpp>
short e_mask[3][3] = {
{-9, 0, -9},
{ 0, 36, 0},
{-9, 0, -9} };

short contrast[3][3] = {
{ 1, 1, 1},
{ 1, 1, 1},
{ 1, 1, 1} };

void mat_print(const short mat[][3], int row)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			printf("%d\t", mat[i][j]);
		printf("\n");
	}
	printf("\n");
}

void homogenity(C_Image src, C_Image* out)
{
	C_Image::IndexT row, col;
	int max_diff, diff, abs_diff;
	for (row = src.FirstRow() + 1; row <= src.LastRow() - 1; row++) {
		for (col = src.FirstCol() + 1; col <= src.LastCol() - 1; col++) {
			max_diff = 0;
			for (int a = -1; a <= 1; a++) {
				for (int b = -1; b <= 1; b++) {
					diff = src(row, col) - src(row + a, col + b);
					abs_diff = abs(diff);
					if (abs_diff > max_diff)
						max_diff = abs_diff;
 				}
			}
			(*out)(row, col) = max_diff;
		}
	}
	
	bool threshold = true;
	if (threshold) 
	{
		for (int i = src.FirstRow(); i < src.LastRow(); i++) {
			for (int j = src.FirstCol(); j < src.LastCol(); j++) {
				if ((*out)(i, j) < 25)
					(*out)(i, j) = 0;

			}
		}
	}
}



void contrast_edge(C_Image src_img, C_Image* out_image) {
	int ad, d;
	int a, b, absdiff, absmax, diff, max, new_hi, new_low, sum_d, sum_n;
	C_Image::IndexT row, col;
	for (row = src_img.FirstRow() + 1; row <= src_img.LastRow() - 1; row++)
	{
		for (col = src_img.FirstCol() + 1; col <= src_img.LastCol() - 1; col++)
		{
			sum_d = sum_n = 0;
			for (a = -1; a <= 1; a++) {
				for (b = -1; b <= 1; b++) {
					sum_n = sum_n + src_img(row + a, col + b) * e_mask[a+1] [b+1];
					sum_d = sum_d + src_img(row + a, col + b) * contrast[a+1] [b+1];
					d = sum_d / 9;
					if (d == 0)
						d = 1;
				}
			}
			d = sum_d / 9;
			if (d == 0)
				d = 1;
			
			(*out_image)(row, col) = sum_n / d;
		}
	}
	(*out_image).Trunc(0, 255);
}




int main(int argc, char **argv)
{
	C_Arguments arguments(argc, argv, 1, 1, "", false);
	C_Matrix quick_mask(1, 3, 1, 3), conv_mask, after_conv;
	C_Image src_img, out_hom, out_contrast, out_enh_edge, out_gauss_edge;
	C_Image::IndexT row, col;

	if (arguments.Fail())
	{
		printf("\n Uso: Path source image\n");
		exit(-1);
	}
	src_img.ReadBMP(arguments.Param(1));
	src_img.Grey();
	
	quick_mask.Read("quick_mask.txt");
	printf("Convoluting source image with the following quick mask\n");
	quick_mask.Print(3, 3);
	after_conv.Convolution(src_img, quick_mask);
	after_conv.Trunc(0, 255);
	C_Image conv_img(after_conv); 
	printf("Writing output image 'Output_qm' to ../Run\n");
	conv_img.WriteBMP("Output_qm.bmp");
	conv_img.Free();
	
	out_hom.Resize(src_img.FirstRow(), src_img.LastRow(), src_img.FirstCol(), src_img.LastCol());
	out_hom.palette = src_img.palette;
	printf("\nHomogeneity operator\n");
	homogenity(src_img, &out_hom);
	printf("Writing output image 'Out_homogenity' to ../Run\n");
	out_hom.WriteBMP("Out_homogenity.bmp");
	out_hom.Free();
	
	out_contrast.Resize(src_img.FirstRow(), src_img.LastRow(), src_img.FirstCol(), src_img.LastCol());
	out_contrast.palette = src_img.palette;
	printf("\nConstrast edge detector\n");
	printf("Edge mask\n");
	mat_print(e_mask, 3);
	printf("Constrast mask\n");
	mat_print(contrast, 3);
	contrast_edge(src_img, &out_contrast);
	printf("Writing output image 'Out_contrast' to ../Run\n");
	out_contrast.WriteBMP("Out_contrast.bmp");
	out_contrast.Free();
	src_img.Free();
	return 0;
}