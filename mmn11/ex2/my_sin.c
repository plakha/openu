#include <math.h> /* sin() */
#include <stdio.h> /* puts() */
#include <assert.h> /* assertt() */

#define PRECISION (0.000001)

double my_sin(double x);

int main()
{
	return 0;
}



/*x^1 / 0! - x^3 / 3! + x^5 / 5! - x^7 / 7! + ...*/
double my_sin(double x)
{
	double res = 0.0;
	
	const double x_sqr = x * x;
	double next_memb = x; /* the first member x^1/0! */
	unsigned int fac = 1; /* used to calculate the "trailing" factorial*/
	
	while ((next_memb > PRECISION) || (-next_memb > PRECISION))
	{
		assert ( next_memb * next_memb > PRECISION * PRECISION);
		
		res += next_memb; /* if above the precision, add the next_memb to the res */
		
		next_memb = (-1) * next_memb * x_sqr / ((fac + 1) * (fac + 2)); /* flip-flopping "-", add 2 powers, add increase the factorial twice */
		
		fac += 2;
	}
	
	
	return res;
}
