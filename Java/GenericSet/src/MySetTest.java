import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;
import java.util.Iterator;

//Author: Cesar D. Quihuis-Romero.

class MySetTest {

	@Test
	void testAdd() {
		MySet<Integer> testSetIntegers = new MySet<Integer>();
		assertTrue(testSetIntegers.isEmpty());
		testSetIntegers.add(1);
		testSetIntegers.add(2);
		testSetIntegers.add(3);
		testSetIntegers.add(4);
		assertFalse(testSetIntegers.isEmpty());
		assertFalse(testSetIntegers.add(4));
		assertEquals(4, testSetIntegers.size());
		assertTrue(testSetIntegers.contains(1));
		assertEquals(1, testSetIntegers.get(0));
		assertTrue(testSetIntegers.contains(2));
		assertEquals(2, testSetIntegers.get(1));
		assertTrue(testSetIntegers.contains(3));
		assertEquals(3, testSetIntegers.get(2));
		assertTrue(testSetIntegers.contains(4));
		assertEquals(4, testSetIntegers.get(3));
		String integerSet = "";
		Iterator<Integer> integerIter = testSetIntegers.iterator();
		while (integerIter.hasNext()) {
			integerSet += integerIter.next() + "_";
		}
		System.out.println("Integer Set: " + integerSet);
		assertEquals("1_2_3_4_", integerSet);

		MySet<String> testSetStrings = new MySet<String>();
		assertTrue(testSetStrings.isEmpty());
		testSetStrings.add("a");
		testSetStrings.add("b");
		testSetStrings.add("c");
		testSetStrings.add("d");
		assertFalse(testSetStrings.isEmpty());
		assertFalse(testSetStrings.add("d"));
		assertEquals(4, testSetStrings.size());
		assertTrue(testSetStrings.contains("a"));
		assertEquals("a", testSetStrings.get(0));
		assertTrue(testSetStrings.contains("b"));
		assertEquals("b", testSetStrings.get(1));
		assertTrue(testSetStrings.contains("c"));
		assertEquals("c", testSetStrings.get(2));
		assertTrue(testSetStrings.contains("d"));
		assertEquals("d", testSetStrings.get(3));
		String stringTest = "";
		Iterator<String> stringIter = testSetStrings.iterator();
		while (stringIter.hasNext()) {
			stringTest += stringIter.next() + "_";
		}
		System.out.println("string set: " + stringTest);
		assertEquals("a_b_c_d_", stringTest);
	}

	@Test
	void testRemove() {
		MySet<Integer> intSet = new MySet<Integer>();
		assertFalse(intSet.remove(1));
		intSet.add(1);
		intSet.add(2);
		intSet.add(3);
		assertFalse(intSet.remove(4));
		assertEquals(3, intSet.size());
		String intSetBeforeRemove = "";
		Iterator<Integer> intIterBefore = intSet.iterator();
		while (intIterBefore.hasNext()) {
			intSetBeforeRemove += intIterBefore.next();
		}
		assertEquals("123", intSetBeforeRemove);
		intSet.remove(2);
		String intSetAfterRemove = "";
		Iterator<Integer> intIterAfter = intSet.iterator();
		while (intIterAfter.hasNext()) {
			intSetAfterRemove += intIterAfter.next();
		}
		assertEquals(2, intSet.size());
		assertEquals("13", intSetAfterRemove);

		intSet.remove(1);
		intSet.add(4);
		intSet.remove(4);
		intSet.remove(3);
		assertTrue(intSet.isEmpty());
	}
}
