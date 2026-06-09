/* Implementation of a 32-bit adder in C.
 *
 * Author: Cesar D. Quihuis-Romero
 */


#include "sim1.h"



void execute_add(Sim1Data *obj)
{
	// TODO: implement me!
	int carryIn = obj->isSubtraction;
	
	int a, b, sum;

	for (int i=0;i<32;i++){

		a = obj -> a;
		b = obj -> b;
		sum = obj -> sum;

		a = a >> i;
		b = b >> i;
		a = a & 1;
		b = b & 1;

		if(obj -> isSubtraction){
			b = !b;
		}
	
		sum = a ^ b ^ carryIn;
		
		if (a == 1 && b == 1){
			carryIn = 1;
		} else if (carryIn == 1 && (a == 1 || b == 1)){
			carryIn = 1;
		} else {
			carryIn = 0;
		}

		obj -> carryOut = carryIn;
		obj -> sum = (sum << i) | obj -> sum;
	}	
	
	int aNonNeg = (obj -> a >> 31) & 1;
	int bNonNeg = (obj -> b >> 31) & 1;
	int sumNonNeg = (obj -> sum >> 31) & 1;
	obj -> aNonNeg = !aNonNeg;
	obj -> bNonNeg = !bNonNeg;
	obj -> sumNonNeg = !sumNonNeg;

	obj -> overflow = !(aNonNeg ^ bNonNeg ^ obj -> isSubtraction) && (aNonNeg ^ sumNonNeg);

}

