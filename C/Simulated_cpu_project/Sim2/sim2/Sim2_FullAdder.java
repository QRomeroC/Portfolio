//Author: Cesar D. Quihuis-Romero
public class Sim2_FullAdder {
	public void execute() {
		halfAdder1.a.set(a.get());
		halfAdder1.b.set(b.get());
		halfAdder1.execute();
		
		halfAdder2.a.set(halfAdder1.sum.get());
		halfAdder2.b.set(carryIn.get());
		
		halfAdder2.execute();
		
		sum.set(halfAdder2.sum.get());
		
		boolean carryVal = halfAdder1.carry.get() | halfAdder2.carry.get();
		carryOut.set(carryVal);	
	}
	
	public RussWire a,b,carryIn,sum,carryOut;
	public Sim2_HalfAdder halfAdder1;
	public Sim2_HalfAdder halfAdder2;
	
	public Sim2_FullAdder() {
		a = new RussWire();
		b = new RussWire();
		carryIn = new RussWire();
		
		sum = new RussWire();
		carryOut = new RussWire();
		
		halfAdder1 = new Sim2_HalfAdder();
		halfAdder2 = new Sim2_HalfAdder();
	}
}
