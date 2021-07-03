/*Question 1

"a" is a string literal
'a' is an integer literal

so the lines:

#define STR1 'a'
#define STR2 "a"
 
are not the same

"a" takes 2 bytes of  
'a' takes as much as integer does

*/

/*

Question 2

In C language, a function parameters are passed by value.
So the function parameters are local in the function scope. 
Any changes to the parameters will not reflect outside of the function.

That being said, it is possible to pass pointer to a variable; in this case the function will be able to alter the variables value.

*/