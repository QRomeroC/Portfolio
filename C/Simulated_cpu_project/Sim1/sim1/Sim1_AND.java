/* Simulates a physical AND gate.
 *
 * Author: Cesar D. Quihuis-Romero
 */

public class Sim1_AND
{
	public void execute()
	{
		Boolean tempA = a.get();
		Boolean tempB = b.get();
		
		out.set(tempA && tempB);
	}



	public RussWire a,b;   // inputs
	public RussWire out;   // output
	

	public Sim1_AND()
	{
		a = new RussWire();
		b = new RussWire();
		out = new RussWire();
	}
}

