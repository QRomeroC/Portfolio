//Author: Cesar D. Quihuis-Romero
public class Sim2_AdderX {
	public void execute() {

		boolean carryIn = false;

		for (int i = 0; i < size; i++) {
			adders[i].a.set(a[i].get());
			adders[i].b.set(b[i].get());
			adders[i].carryIn.set(carryIn);
			adders[i].execute();
			carryIn = adders[i].carryOut.get();
			sum[i].set(adders[i].sum.get());
		}

		carryOut.set(carryIn);

		int MSB = size - 1;

		notGate1.in.set(sum[MSB].get());
		andGate1.a.set(a[MSB].get());
		andGate1.b.set(b[MSB].get());

		notGate1.execute();
		andGate1.execute();

		andGate2.a.set(notGate1.out.get());
		andGate2.b.set(andGate1.out.get());
		andGate2.execute();

		boolean aANDbANDNOTsum = andGate2.out.get();

		andGate3.a.set(a[MSB].get());
		andGate3.b.set(b[MSB].get());
		andGate3.execute();

		notGate2.in.set(andGate3.out.get());
		notGate2.execute();

		andGate4.a.set(notGate2.out.get());
		andGate4.b.set(sum[MSB].get());
		andGate4.execute();

		boolean NOTaANDbANDsum = andGate4.out.get();

		andGate5.a.set(a[MSB].get());
		andGate5.b.set(sum[MSB].get());

		notGate3.in.set(b[MSB].get());

		andGate5.execute();
		notGate3.execute();

		andGate6.a.set(andGate5.out.get());
		andGate6.b.set(notGate3.out.get());

		andGate6.execute();

		boolean aANDsumANDNOTb = andGate6.out.get();

		orGate1.a.set(aANDbANDNOTsum);
		orGate1.b.set(NOTaANDbANDsum);
		orGate1.execute();

		orGate2.a.set(orGate1.out.get());
		orGate2.b.set(aANDsumANDNOTb);
		orGate2.execute();

		overflow.set(orGate2.out.get());
	}

	public RussWire[] a, b, sum;
	public RussWire carryOut, overflow;
	public Sim2_FullAdder[] adders;
	public NOT notGate1, notGate2, notGate3;
	public AND andGate1, andGate2, andGate3, andGate4, andGate5, andGate6;
	public OR orGate1, orGate2;

	public int size;

	public Sim2_AdderX(int n) {
		size = n;
		a = new RussWire[n];
		b = new RussWire[n];
		sum = new RussWire[n];
		adders = new Sim2_FullAdder[n];

		for (int i = 0; i < size; i++) {
			adders[i] = new Sim2_FullAdder();
			a[i] = new RussWire();
			b[i] = new RussWire();
			sum[i] = new RussWire();
		}

		carryOut = new RussWire();
		overflow = new RussWire();

		notGate1 = new NOT();
		notGate2 = new NOT();
		notGate3 = new NOT();

		andGate1 = new AND();
		andGate2 = new AND();
		andGate3 = new AND();
		andGate4 = new AND();
		andGate5 = new AND();
		andGate6 = new AND();

		orGate1 = new OR();
		orGate2 = new OR();
	}

}
