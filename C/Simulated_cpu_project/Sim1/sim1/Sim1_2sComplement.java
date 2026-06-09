/* Simulates a physical device that performs 2's complement on a 32-bit input.
 *
 * Author: Cesar D. Quihuis-Romero
 */

public class Sim1_2sComplement {
	public void execute() {
		// TODO: fill this in!
		//
		// REMEMBER: You may call execute() on sub-objects here, and
		// copy values around - but you MUST NOT create
		// objects while inside this function.

		for (int i = 0; i < 32; i++) {
			not_in[i].in.set(in[i].get());
			not_in[i].execute();
		}

		add.a[0].set(not_in[0].out.get());
		add.b[0].set(true);

		for (int i = 1; i < 32; i++) {
			add.a[i].set(not_in[i].out.get());
			add.b[i].set(false);
		}

		add.execute();

		for (int i = 0; i < 32; i++) {
			out[i].set(add.sum[i].get());
		}
	}

	// you shouldn't change these standard variables...
	public RussWire[] in;
	public RussWire[] out;

	// TODO: add some more variables here. You must create them
	// during the constructor below. REMEMBER: You're not
	// allowed to create any object inside the execute()
	// method above!
	private Sim1_NOT[] not_in;
	private Sim1_ADD add;

	public Sim1_2sComplement() {
		// TODO: this is where you create the objects that
		// you declared up above.

		in = new RussWire[32];
		out = new RussWire[32];
		not_in = new Sim1_NOT[32];
		for (int i = 0; i < 32; i++) {
			in[i] = new RussWire();
			out[i] = new RussWire();
			not_in[i] = new Sim1_NOT();
		}

		add = new Sim1_ADD();
	}
}
