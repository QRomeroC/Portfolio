import java.util.Scanner;

public class TrangleReassignment {

	public static void main(String[] args) {
		Scanner keyboard = new Scanner(System.in);

		System.out.println("Enter an integer a: ");

		int a = keyboard.nextInt();

		System.out.println("Enter an interger b: ");

		int b = keyboard.nextInt();

		System.out.println("Enter an char c: ");

		String c = keyboard.next();

		System.out.println("Enter an char d: ");

		String d = keyboard.next();

		keyboard.close();

		System.out.println("a: " + a);

		System.out.println("b: " + b);

		System.out.println("\n");

		System.out.println("a = a ^ b");

		a = a ^ b;

		System.out.println("a: " + a);

		System.out.println("\n");

		System.out.println("b = b ^ a");

		b = b ^ a;

		System.out.println("b: " + b);

		System.out.println("\n ");

		System.out.println("a = a ^ b");

		a = a ^ b;

		System.out.println("a: " + a);

		System.out.println("\n");

		System.out.println("Result");

		System.out.println("a: " + a);

		System.out.println("b: " + b);

		System.out.println("\n");

		System.out.println("c: " + c);

		System.out.println("d: " + d);

		System.out.println("\n");

		char c2 = c.charAt(0);

		char d2 = d.charAt(0);

		System.out.println(c2);

		System.out.println(d2);

		int c1 = c2;

		int d1 = d2;

		System.out.println(c1);

		System.out.println(d1);

		System.out.println("c = c ^ d");

		c1 = c1 ^ d1;

		System.out.println("c: " + c1);

		System.out.println("\n");

		System.out.println("d = d ^ c");

		d1 = d1 ^ c1;

		System.out.println("d: " + d1);

		System.out.println("\n ");

		System.out.println("c = c ^ d");

		c1 = c1 ^ d1;

		System.out.println("c: " + c1);

		System.out.println("\n");

		char c3 = (char) c1;

		char d3 = (char) d1;

		System.out.println("Result");

		System.out.println("c: " + c3);

		System.out.println("d: " + d3);// TODO Auto-generated method stub

	}

}
