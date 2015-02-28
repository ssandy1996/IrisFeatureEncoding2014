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
 * 		Used to calculate the magnitude of a complex number defined as
 * 		a structure of containing real and imaginary parts.
 * ----------------------------------------------------------------------
 * Author:
 * 		S Sandeep Kumar
 * 		E-mail : ee13b1025@iith.ac.in
************************************************************************/

#include <math.h>
#include "complexNo.h"

double magnitude(struct complexNo x){
	double temp=0;
	temp = (x.real)*(x.real) + (x.imag)*(x.imag);
	temp = sqrt(temp);
	return(temp);
}

/************************************************************************
 * End of complexNo.c
 ***********************************************************************/
