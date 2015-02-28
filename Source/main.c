/************************************************************************
 * Copyright (C) 2014 S Sandeep Kumar (ee13b1025@iith.ac.in)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ----------------------------------------------------------------------
 * Function:
 *		Main code for generating IrisCode using the polar and noise data
 *		of iris images calculated using the iris recognition system
 *		code of Libor Masek, available at
 *		http://www.csse.uwa.edu.au/~pk/studentprojects/libor/
 *
 *		This code was made to be used on TI TMS320C6670 but can be run
 *		on other platforms with or without any modifications.
 * ----------------------------------------------------------------------
 * Usage:
 *		The IrisCode information i.e. template and mask	information
 *		is stored into two arrays which can be saved as .dat files
 *		from the Memory Browser.
 * ----------------------------------------------------------------------
 * Author:
 * 		S Sandeep Kumar
 * 		E-mail : ee13b1025@iith.ac.in
************************************************************************/

#include <stdio.h>
#include <math.h>
#include "complexNo.h"
#include "irisDatabaseSmall.h"

#include "fft.h"
// Includes the definition for Arbitrary-n FFT
// FFT code for non-commercial use only
// Available: http://www.corix.dk/html/fft.html

#define inHeight 20
#define inWidth 240
#define outHeight 20
#define outWidth 480
#define filterWidth 121
// Definitions of size of image, filter and template/mask

int encode(int (*)[outWidth], int (*)[outWidth], const double (*)[inWidth],
		const int (*)[inWidth]);
// Function to generate a biometric template from the normalised iris
// region and also the corresponding noise mask

int gaborConvolve(struct complexNo (*)[inWidth], double (*),
		const double (*)[inWidth]);
// Function for convolving each row of an image with 1D Log-Gabor filters

void radInit(float (*), int, float, float);
// Linear Assembly function for initialising filter

void arrayZero(float (*), int);
// Linear Assembly function for zero initialising an array

void storeEncode(int (*)[outWidth], int (*)[outWidth], int (*)[inWidth],
		int (*)[inWidth], int (*)[inWidth], const int (*)[inWidth], int, int);
// Linear Assembly function for storing the biometric template and noise mask

int templateData[outHeight][outWidth], maskData[outHeight][outWidth];
// Biometric template and noise mask arrays

int main(void) {
	int check;
	check = encode(templateData, maskData, polar_array_12, noise_array_12);
	/* Uncomment this if not running on TI DSP Board Emulator
	 * if(check == 1){
	 * printf("The IrisCode has been generated");
	 * }else{
	 * printf("The IrisCode has not been generated");
	 * }
	 */
	return (0);
}

int encode(int templat[outHeight][outWidth], int mask[outHeight][outWidth],
		const double polar_array[inHeight][inWidth],
		const int noise_array[inHeight][inWidth]) {
	int j, k, h1[inHeight][inWidth], h2[inHeight][inWidth],
			h3[inHeight][inWidth];
	struct complexNo e1[inHeight][inWidth];
	double filtersum[inWidth];

	j = gaborConvolve(e1, filtersum, polar_array);
	// Convolve normalised region with 1D Log-Gabor filters

	// Creating the Iris Template
	for (j = 0; j < inHeight; j++) {
		for (k = 0; k < inWidth; k++) {

			// Phase Quantisation
			if (e1[j][k].real > 0) {
				h1[j][k] = 1;
			} else {
				h1[j][k] = 0;
			}

			// Phase Quantisation
			if (e1[j][k].imag > 0) {
				h2[j][k] = 1;
			} else {
				h2[j][k] = 0;
			}

			// If amplitude is close to zero then
			// phase data is useless
			if (magnitude(e1[j][k]) < 0.0001) {
				h3[j][k] = 1;
			} else {
				h3[j][k] = 0;
			}
		}
	}

	// Constructing the Biometric template and noise mask
	storeEncode(templat, mask, h1, h2, h3, noise_array, inHeight, inWidth);
	return (1);
}

int gaborConvolve(struct complexNo e1[inHeight][inWidth],
		double filtersum[inWidth], const double im[inHeight][inWidth]) {
	int j, k;
	float logGabor[inWidth], filter[inWidth], radius[filterWidth], temp;
	double xRe[inWidth], xIm[inWidth], yRe[inWidth], yIm[inWidth];

	radInit(radius, filterWidth, 1.0, 1 / 240.0);	// Frequency values 0 - 0.5
	arrayZero(logGabor, inWidth);	// Zero Initialising logGabor array

	// Construct 1D Log-Gabor filter
	for (j = 1; j < filterWidth; j++) {
		logGabor[j] = exp(
				-(pow(log(radius[j] * 18), 2)) / (2 * pow(log(0.5), 2)));
	}

	for (j = 0; j < inWidth; j++) {
		filter[j] = logGabor[j];
		filtersum[j] = filtersum[j] + filter[j];
	}

	// Row wise convolution and inverse transformation
	for (j = 0; j < inHeight; j++) {
		for (k = 0; k < inWidth; k++) {
			xRe[k] = im[j][k];
			xIm[k] = 0;
		}

		fft(inWidth, xRe, xIm, yRe, yIm);

		for (k = 0; k < inWidth; k++) {
			yRe[k] *= (double) filter[k];
			yIm[k] *= (-1) * (double) filter[k];
		}

		fft(inWidth, yRe, yIm, xRe, xIm);

		for (k = 0; k < inWidth; k++) {
			e1[j][k].real = xRe[k] * (1 / 240.0);
			e1[j][k].imag = xIm[k] * (-1 / 240.0);
		}
	}

	// fftshift for filtersum
	for (j = 0; j < filterWidth - 1; j++) {
		temp = filtersum[j];
		filtersum[j] = filtersum[j + 120];
		filtersum[j + 120] = temp;
	}
	return (2);
}

/************************************************************************
 * End of main.c
 ***********************************************************************/
