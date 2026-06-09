/* Simulates a physical NOT gate.
 *
 * Author: Cesar D. Quihuis-Romero
 */

public class Sim1_NOT
{
	public void execute()
	{
		Boolean tempIn = in.get();
		out.set(!tempIn);
	}



	public RussWire in;    // input
	public RussWire out;   // output

	public Sim1_NOT()
	{
		in = new RussWire();
		out = new RussWire();
	}
}

