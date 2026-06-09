/* Simulates a physical device that performs (signed) addition on
 * a 32-bit input.
 *
 * Author: Cesar D. Quihuis-Romero
 */

public class Sim1_ADD
{
	public void execute()
	{
		Boolean carry = false;
		
		for (int i=0; i<32;i++) {
			Boolean tempA = a[i].get();
			Boolean tempB = b[i].get();
			sum[i].set(tempA != tempB != carry);
			
			if (carry == false && tempA == true && tempB == true) {
				carry = true;
			} else if (carry == true && (tempA == true || tempB == true)) {
				carry = true;
			} else if (carry == true && (tempA == false && tempB == false)){
				carry = false;
			} else if (carry == false && tempA == false && tempB == false) {
				carry = false;
			} else if (carry == false && (tempA == true || tempB == true)) {
				carry = false;
			} else {
				carry = false;
			}
		}
		
		carryOut.set(carry);
		
		if ((a[31].get() == b[31].get()) && sum[31].get() != a[31].get()) {
			overflow.set(true);
		} else {
			overflow.set(false);
		}
		
	}



	// ------ 
	// It should not be necessary to change anything below this line,
	// although I'm not making a formal requirement that you cannot.
	// ------ 

	// inputs
	public RussWire[] a,b;

	// outputs
	public RussWire[] sum;
	public RussWire   carryOut, overflow;

	public Sim1_ADD()
	{
		/* Instructor's Note:
		 *
		 * In Java, to allocate an array of objects, you need two
		 * steps: you first allocate the array (which is full of null
		 * references), and then a loop which allocates a whole bunch
		 * of individual objects (one at a time), and stores those
		 * objects into the slots of the array.
		 */

		a   = new RussWire[32];
		b   = new RussWire[32];
		sum = new RussWire[32];

		for (int i=0; i<32; i++)
		{
			a  [i] = new RussWire();
			b  [i] = new RussWire();
			sum[i] = new RussWire();
		}

		carryOut = new RussWire();
	
		overflow = new RussWire();
		
	}
}

