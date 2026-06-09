import org.junit.Test;
import static org.junit.Assert.assertTrue;

public class AdderXTest {

    @Test
    public void testOne() {
        Sim2_AdderX adder = new Sim2_AdderX(2);

        for (int i = 0; i < 2; i++) {
            adder.a[i].set(false);
            adder.b[i].set(false);
        }

        adder.execute();

        assertTrue(! adder.carryOut.get());
        assertTrue(! adder.overflow.get());

        for (int i = 0; i < 2; i++) {
            assertTrue(! adder.sum[i].get());
        }
    }

    @Test
    public void testTwo() {
        Sim2_AdderX adder = new Sim2_AdderX(2);

        for (int i = 0; i < 2; i++) {
            adder.a[i].set(true);
            adder.b[i].set(true);
        }

        adder.execute();

        assertTrue(adder.carryOut.get());
        assertTrue(! adder.overflow.get());

        assertTrue(! adder.sum[0].get());
        assertTrue(adder.sum[1].get());
    }

    @Test
    public void testThree() {
        Sim2_AdderX adder = new Sim2_AdderX(2);

        adder.a[0].set(false);
        adder.b[0].set(false);
        adder.a[1].set(true);
        adder.b[1].set(true);

        adder.execute();

        assertTrue(adder.carryOut.get());
        assertTrue(adder.overflow.get());

        assertTrue(! adder.sum[0].get());
        assertTrue(! adder.sum[1].get());
    }

    @Test
    public void testFour() {
        Sim2_AdderX adder = new Sim2_AdderX(2);

        adder.a[0].set(true);
        adder.b[0].set(true);
        adder.a[1].set(false);
        adder.b[1].set(false);

        adder.execute();

        assertTrue(! adder.carryOut.get());
        assertTrue(adder.overflow.get());

        assertTrue(! adder.sum[0].get());
        assertTrue(adder.sum[1].get());
    }

    @Test
    public void testFive() {
        Sim2_AdderX adder = new Sim2_AdderX(2);

        adder.a[0].set(true);
        adder.b[0].set(false);
        adder.a[1].set(false);
        adder.b[1].set(true);

        adder.execute();

        assertTrue(! adder.carryOut.get());
        assertTrue(! adder.overflow.get());

        assertTrue(adder.sum[0].get());
        assertTrue(adder.sum[1].get());
    }

    @Test
    public void testSix() {
        Sim2_AdderX adder = new Sim2_AdderX(10);

        adder.a[0].set(false);
        adder.a[1].set(false);
        adder.a[2].set(false);
        adder.a[3].set(false);
        adder.a[4].set(false);
        adder.a[5].set(false);
        adder.a[6].set(false);
        adder.a[7].set(false);
        adder.a[8].set(false);
        adder.a[9].set(false);

        adder.b[0].set(false);
        adder.b[1].set(false);
        adder.b[2].set(false);
        adder.b[3].set(false);
        adder.b[4].set(false);
        adder.b[5].set(false);
        adder.b[6].set(false);
        adder.b[7].set(false);
        adder.b[8].set(false);
        adder.b[9].set(false);

        adder.execute();

        assertTrue(! adder.carryOut.get());
        assertTrue(! adder.overflow.get());

        assertTrue(! adder.sum[0].get());
        assertTrue(! adder.sum[1].get());
        assertTrue(! adder.sum[2].get());
        assertTrue(! adder.sum[3].get());
        assertTrue(! adder.sum[4].get());
        assertTrue(! adder.sum[5].get());
        assertTrue(! adder.sum[6].get());
        assertTrue(! adder.sum[7].get());
        assertTrue(! adder.sum[8].get());
        assertTrue(! adder.sum[9].get());
    }

    @Test
    public void testSeven() {
        Sim2_AdderX adder = new Sim2_AdderX(10);

        adder.a[0].set(true);
        adder.a[1].set(true);
        adder.a[2].set(true);
        adder.a[3].set(true);
        adder.a[4].set(true);
        adder.a[5].set(true);
        adder.a[6].set(true);
        adder.a[7].set(true);
        adder.a[8].set(true);
        adder.a[9].set(true);

        adder.b[0].set(true);
        adder.b[1].set(true);
        adder.b[2].set(true);
        adder.b[3].set(true);
        adder.b[4].set(true);
        adder.b[5].set(true);
        adder.b[6].set(true);
        adder.b[7].set(true);
        adder.b[8].set(true);
        adder.b[9].set(true);

        adder.execute();

        assertTrue(adder.carryOut.get());
        assertTrue(! adder.overflow.get());

        assertTrue(! adder.sum[0].get());
        assertTrue(adder.sum[1].get());
        assertTrue(adder.sum[2].get());
        assertTrue(adder.sum[3].get());
        assertTrue(adder.sum[4].get());
        assertTrue(adder.sum[5].get());
        assertTrue(adder.sum[6].get());
        assertTrue(adder.sum[7].get());
        assertTrue(adder.sum[8].get());
        assertTrue(adder.sum[9].get());
    }

    @Test
    public void testEight() {
        Sim2_AdderX adder = new Sim2_AdderX(10);

        adder.a[0].set(true);
        adder.a[1].set(true);
        adder.a[2].set(true);
        adder.a[3].set(true);
        adder.a[4].set(true);
        adder.a[5].set(true);
        adder.a[6].set(true);
        adder.a[7].set(true);
        adder.a[8].set(true);
        adder.a[9].set(true);

        adder.b[0].set(false);
        adder.b[1].set(false);
        adder.b[2].set(false);
        adder.b[3].set(false);
        adder.b[4].set(false);
        adder.b[5].set(false);
        adder.b[6].set(false);
        adder.b[7].set(false);
        adder.b[8].set(false);
        adder.b[9].set(true);

        adder.execute();

        assertTrue(adder.carryOut.get());
        assertTrue(adder.overflow.get());

        assertTrue(adder.sum[0].get());
        assertTrue(adder.sum[1].get());
        assertTrue(adder.sum[2].get());
        assertTrue(adder.sum[3].get());
        assertTrue(adder.sum[4].get());
        assertTrue(adder.sum[5].get());
        assertTrue(adder.sum[6].get());
        assertTrue(adder.sum[7].get());
        assertTrue(adder.sum[8].get());
        assertTrue(! adder.sum[9].get());
    }

    @Test
    public void testNine() {
        Sim2_AdderX adder = new Sim2_AdderX(10);

        adder.a[0].set(true);
        adder.a[1].set(true);
        adder.a[2].set(false);
        adder.a[3].set(true);
        adder.a[4].set(true);
        adder.a[5].set(false);
        adder.a[6].set(true);
        adder.a[7].set(true);
        adder.a[8].set(false);
        adder.a[9].set(true);

        adder.b[0].set(true);
        adder.b[1].set(false);
        adder.b[2].set(true);
        adder.b[3].set(true);
        adder.b[4].set(false);
        adder.b[5].set(false);
        adder.b[6].set(true);
        adder.b[7].set(false);
        adder.b[8].set(true);
        adder.b[9].set(true);

        adder.execute();

        assertTrue(adder.carryOut.get());
        assertTrue(! adder.overflow.get());

        assertTrue(! adder.sum[0].get());
        assertTrue(! adder.sum[1].get());
        assertTrue(! adder.sum[2].get());
        assertTrue(adder.sum[3].get());
        assertTrue(! adder.sum[4].get());
        assertTrue(adder.sum[5].get());
        assertTrue(! adder.sum[6].get());
        assertTrue(! adder.sum[7].get());
        assertTrue(! adder.sum[8].get());
        assertTrue(adder.sum[9].get());
    }

    @Test
    public void testTen() {
        Sim2_AdderX adder = new Sim2_AdderX(15);

        adder.a[0].set(true);
        adder.a[1].set(true);
        adder.a[2].set(false);
        adder.a[3].set(true);
        adder.a[4].set(true);
        adder.a[5].set(false);
        adder.a[6].set(true);
        adder.a[7].set(true);
        adder.a[8].set(false);
        adder.a[9].set(true);
        adder.a[10].set(false);
        adder.a[11].set(true);
        adder.a[12].set(true);
        adder.a[13].set(false);
        adder.a[14].set(false);

        adder.b[0].set(true);
        adder.b[1].set(false);
        adder.b[2].set(true);
        adder.b[3].set(true);
        adder.b[4].set(false);
        adder.b[5].set(false);
        adder.b[6].set(true);
        adder.b[7].set(false);
        adder.b[8].set(true);
        adder.b[9].set(true);
        adder.b[10].set(false);
        adder.b[11].set(true);
        adder.b[12].set(false);
        adder.b[13].set(true);
        adder.b[14].set(true);

        adder.execute();

        assertTrue(adder.carryOut.get());
        assertTrue(! adder.overflow.get());

        assertTrue(! adder.sum[0].get());
        assertTrue(! adder.sum[1].get());
        assertTrue(! adder.sum[2].get());
        assertTrue(adder.sum[3].get());
        assertTrue(! adder.sum[4].get());
        assertTrue(adder.sum[5].get());
        assertTrue(! adder.sum[6].get());
        assertTrue(! adder.sum[7].get());
        assertTrue(! adder.sum[8].get());
        assertTrue(adder.sum[9].get());
        assertTrue(adder.sum[10].get());
        assertTrue(! adder.sum[11].get());
        assertTrue(! adder.sum[12].get());
        assertTrue(! adder.sum[13].get());
        assertTrue(! adder.sum[14].get());
    }

    @Test
    public void testEleven() {
        Sim2_AdderX adder = new Sim2_AdderX(15);

        adder.a[0].set(true);
        adder.a[1].set(true);
        adder.a[2].set(false);
        adder.a[3].set(true);
        adder.a[4].set(true);
        adder.a[5].set(false);
        adder.a[6].set(true);
        adder.a[7].set(true);
        adder.a[8].set(false);
        adder.a[9].set(true);
        adder.a[10].set(true);
        adder.a[11].set(true);
        adder.a[12].set(true);
        adder.a[13].set(true);
        adder.a[14].set(false);

        adder.b[0].set(true);
        adder.b[1].set(true);
        adder.b[2].set(true);
        adder.b[3].set(true);
        adder.b[4].set(false);
        adder.b[5].set(true);
        adder.b[6].set(true);
        adder.b[7].set(false);
        adder.b[8].set(true);
        adder.b[9].set(true);
        adder.b[10].set(false);
        adder.b[11].set(true);
        adder.b[12].set(false);
        adder.b[13].set(true);
        adder.b[14].set(false);

        adder.execute();

        assertTrue(! adder.carryOut.get());
        assertTrue(adder.overflow.get());

        assertTrue(! adder.sum[0].get());
        assertTrue(adder.sum[1].get());
        assertTrue(! adder.sum[2].get());
        assertTrue(adder.sum[3].get());
        assertTrue(! adder.sum[4].get());
        assertTrue(! adder.sum[5].get());
        assertTrue(adder.sum[6].get());
        assertTrue(! adder.sum[7].get());
        assertTrue(! adder.sum[8].get());
        assertTrue(adder.sum[9].get());
        assertTrue(! adder.sum[10].get());
        assertTrue(adder.sum[11].get());
        assertTrue(! adder.sum[12].get());
        assertTrue(adder.sum[13].get());
        assertTrue(adder.sum[14].get());
    }
}
