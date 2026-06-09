/* Testcase for 252 Sim 1.
 *
 * Author: Hamlet Taraz & Russ Lewis
 */

public class Test_HT_ADD2 {
	public static void main(String[] args) {
		Sim1_ADD p = new Sim1_ADD();

		for (int i = 0; i < 32; i++) {
			p.a[i].set(i % 3 >= 1);
			p.b[i].set(i % 5 <= 1);
		}

		p.execute();

		System.out.printf("  ");
		print_bits(p.a);
		System.out.print("\n");

		System.out.printf("+ ");
		print_bits(p.b);
		System.out.printf("\n");

		System.out.printf("----------------------------------\n");

		System.out.printf("  ");
		print_bits(p.sum);
		System.out.printf("\n");

		System.out.printf("\n");
		System.out.printf("  carryOut = %c\n", bit(p.carryOut.get()));

		System.out.printf("  overflow = %c\n", bit(p.overflow.get()));
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
