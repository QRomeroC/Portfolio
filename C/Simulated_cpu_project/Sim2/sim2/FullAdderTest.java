import org.junit.Test;
import static org.junit.Assert.assertTrue;

public class FullAdderTest {
    
    @Test
    public void testOne() {
        Sim2_FullAdder adder = new Sim2_FullAdder();

        adder.a.set(false);
        adder.b.set(false);
        adder.carryIn.set(false);

        adder.execute();

        assertTrue(! adder.sum.get());
        assertTrue(! adder.carryOut.get());
    }

    @Test
    public void testTwo() {
        Sim2_FullAdder adder = new Sim2_FullAdder();

        adder.a.set(true);
        adder.b.set(false);
        adder.carryIn.set(false);

        adder.execute();

        assertTrue(adder.sum.get());
        assertTrue(! adder.carryOut.get());
    }

    @Test
    public void testThree() {
        Sim2_FullAdder adder = new Sim2_FullAdder();

        adder.a.set(false);
        adder.b.set(true);
        adder.carryIn.set(false);

        adder.execute();

        assertTrue(adder.sum.get());
        assertTrue(! adder.carryOut.get());
    }

    @Test
    public void testFour() {
        Sim2_FullAdder adder = new Sim2_FullAdder();

        adder.a.set(false);
        adder.b.set(false);
        adder.carryIn.set(true);

        adder.execute();

        assertTrue(adder.sum.get());
        assertTrue(! adder.carryOut.get());
    }

    @Test
    public void testFive() {
        Sim2_FullAdder adder = new Sim2_FullAdder();

        adder.a.set(true);
        adder.b.set(true);
        adder.carryIn.set(false);

        adder.execute();

        assertTrue(! adder.sum.get());
        assertTrue(adder.carryOut.get());
    }

    @Test
    public void testSix() {
        Sim2_FullAdder adder = new Sim2_FullAdder();

        adder.a.set(true);
        adder.b.set(false);
        adder.carryIn.set(true);

        adder.execute();

        assertTrue(! adder.sum.get());
        assertTrue(adder.carryOut.get());
    }

    @Test
    public void testSeven() {
        Sim2_FullAdder adder = new Sim2_FullAdder();

        adder.a.set(false);
        adder.b.set(true);
        adder.carryIn.set(true);

        adder.execute();

        assertTrue(! adder.sum.get());
        assertTrue(adder.carryOut.get());
    }

    @Test
    public void testEight() {
        Sim2_FullAdder adder = new Sim2_FullAdder();

        adder.a.set(true);
        adder.b.set(true);
        adder.carryIn.set(true);

        adder.execute();

        assertTrue(adder.sum.get());
        assertTrue(adder.carryOut.get());
    }
}
