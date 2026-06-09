//Author: Cesar D. Quihuis-Romero.
public class Sim2_HalfAdder {
	
	public void execute() {
		boolean aVal = a.get();
		boolean bVal = b.get();
		boolean sumVal = aVal ^ bVal;
		boolean carryVal = aVal & bVal;
		sum.set(sumVal);
		carry.set(carryVal);
	}
	
	public RussWire a,b,sum,carry;
	
	public Sim2_HalfAdder() {
		a = new RussWire();
		b = new RussWire();
		sum = new RussWire();
		carry = new RussWire();
	}
	
}
