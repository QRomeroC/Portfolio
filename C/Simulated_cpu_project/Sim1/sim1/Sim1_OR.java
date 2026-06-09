/* Simulates a physical OR gate.
 *
 * Author: Cesar D. Quihuis-Romero
 */

public class Sim1_OR
{
	public void execute()
	{
		Boolean tempA = a.get();
		Boolean tempB = b.get();
		
		out.set(tempA || tempB);
	}



	public RussWire a,b;   // inputs
	public RussWire out;   // output

	public Sim1_OR()
	{
		a = new RussWire();
		b = new RussWire();
		out = new RussWire();
		
	}
}

