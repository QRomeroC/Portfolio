/* Simulates a physical device that performs (signed) subtraction on
 * a 32-bit input.
 *
 * Author: Cesar D. Quihuis-Romero
 */

public class Sim1_SUB
{
	public void execute()
	{
		// TODO: fill this in!
		//
		// REMEMBER: You may call execute() on sub-objects here, and
		//           copy values around - but you MUST NOT create
		//           objects while inside this function.
		for (int i=0;i<32;i++) {
			comp.in[i].set(b[i].get());
		}
		
		comp.execute();
		
		for (int i=0;i<32;i++) {
			add.a[i].set(a[i].get());
			add.b[i].set(comp.out[i].get());
		}
		
		add.execute();
		
		for (int i=0;i<32;i++) {
			sum[i].set(add.sum[i].get());
		}
	}



	// --------------------
	// Don't change the following standard variables...
	// --------------------

	// inputs
	public RussWire[] a,b;

	// output
	public RussWire[] sum;

	// --------------------
	// But you should add some *MORE* variables here.
	// --------------------
	// TODO: fill this in
	private Sim1_2sComplement comp;
	private Sim1_ADD add;


	public Sim1_SUB()
	{	
		a = new RussWire[32];
		b = new RussWire[32];
		sum = new RussWire[32];
		
		for (int i=0;i<32;i++) {
			a[i] = new RussWire();
			b[i] = new RussWire();
			sum[i] = new RussWire();
		}
		comp = new Sim1_2sComplement();
		add = new Sim1_ADD();
	}
}

