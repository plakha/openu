#include <math.h> /* sin() */
#include <stdio.h> /* puts() */
#include <assert.h> /* assertt() */

#include "my_sin.h"

#define MIN_INPUT (-25.0)
#define MAX_INPUT (25.0)

int main()
{
	double angle = 0.0;
	
	double my_sin_res = 0.0;
	double sin_res = 0.0;
	
	int should_ask_for_input = 1;
	
	printf("This program receives a angle in radians and calculates sinus up to the precision of %f \n", PRECISION);
	
	while (should_ask_for_input)
	{
		printf("Please enter an angle value in radians between %f and %f inclusively \n", MIN_INPUT, MAX_INPUT);
		scanf("%lf", &angle);
		printf("Received input of %f \n", angle);	
		
		if (angle <= MAX_INPUT && angle >= MIN_INPUT)
		{
			should_ask_for_input = 0;
		}
	}
	
	
	puts("*************************\nCalculating...\n");
	
	my_sin_res = my_sin(angle);
	sin_res = sin(angle);
	
	printf("my_sin() result is %f,\nstandard sin() result is %f\n", my_sin_res, sin_res);

	
	return 0;
}



/*x^1 / 0! - x^3 / 3! + x^5 / 5! - x^7 / 7! + ...*/
double my_sin(double x)
{
	double res = 0.0;
	
	const double x_sqr = x * x;
	double next_memb = x; /* the first member x^1/0! */
	size_t fac = 1; /* used to calculate the "trailing" factorial*/
	
	while ((next_memb > PRECISION) || (-next_memb > PRECISION))
	{
		assert ( next_memb * next_memb > PRECISION * PRECISION);
		
		res += next_memb; /* if above the precision, add the next_memb to the res */
		
		next_memb = (-1) * next_memb * x_sqr / ((fac + 1) * (fac + 2)); /* flip-flopping "-", add 2 powers, add increase the factorial twice */
		
		fac += 2;
	}
	
	
	return res;
}
