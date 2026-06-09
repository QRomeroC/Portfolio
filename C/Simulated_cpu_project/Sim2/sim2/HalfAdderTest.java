import org.junit.Test;
import static org.junit.Assert.assertTrue;

public class HalfAdderTest {

    @Test   
    public void testOne() {
        Sim2_HalfAdder adder = new Sim2_HalfAdder();

        adder.a.set(false);
        adder.b.set(false);

        adder.execute();

        assertTrue(! adder.sum.get());
        assertTrue(! adder.carry.get());

    }


    @Test   
    public void testTwo() {
        Sim2_HalfAdder adder = new Sim2_HalfAdder();

        adder.a.set(true);
        adder.b.set(false);

        adder.execute();

        assertTrue(adder.sum.get());
        assertTrue(! adder.carry.get());

    }

    @Test   
    public void testThree() {
        Sim2_HalfAdder adder = new Sim2_HalfAdder();

        adder.a.set(false);
        adder.b.set(true);

        adder.execute();

        assertTrue(adder.sum.get());
        assertTrue(! adder.carry.get());

    }

    @Test   
    public void testFour() {
        Sim2_HalfAdder adder = new Sim2_HalfAdder();

        adder.a.set(true);
        adder.b.set(true);

        adder.execute();

        assertTrue(! adder.sum.get());
        assertTrue(adder.carry.get());

    }

}