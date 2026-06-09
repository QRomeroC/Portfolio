/* Testcase for 252 Sim 1.
 *
 * Author: Hamlet Taraz & Russ Lewis
 */

public class Test_HT_2sComp {
	public static void main(String[] args) {
		Sim1_2sComplement p = new Sim1_2sComplement();

		for (int i = 0; i < 32; i++) {
			p.in[i].set((i % 5) % 2 == 0);
		}

		p.execute();

		System.out.printf("- ");
		print_bits(p.in);
		System.out.printf("\n");

		System.out.printf("----------------------------------\n");

		System.out.printf("  ");
		print_bits(p.out);
	}

	public static void print_bits(RussWire[] bits) {
		for (int i = 31; i >= 0; i--) {
			if (bits[i].get())
				System.out.print("1");
			else
				System.out.print("0");
		}
	}

	public static char bit(boolean b) {
		if (b)
			return '1';
		else
			return '0';
	}
}
