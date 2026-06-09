package tests;

/*
 * A unit test for class GameTree in the Game of 20 questions project.
 *
 * This class contains three methods to get you started and to explain some behavior.
 *
 * @BeforeClass public static void setUp() throws FileNotFoundException
 * 
 * This setUp() method contains code that write a new file at the beginning so that 
 * the file always will start with the same exact questions and answers.
 *
 * @author Rick Mercer 
 */
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import org.junit.BeforeClass;
import org.junit.jupiter.api.Test;

import model.Choice;
import model.GameTree;

public class GameTreeTest {

	@BeforeClass
	public static void restoreFile() {
// 
		/*- Always make sure the input file has the same questions and answers.
		Has feathers?
		Barnyard?
		chicken
		owl
		Is it a mammal?
		tiger
		rattlesnake
		*/
		PrintWriter outFile = null;
		try {
			outFile = new PrintWriter(new FileOutputStream("animals.txt"));

			outFile.println("Has feathers?");
			outFile.println("Barnyard?");
			outFile.println("chicken");
			outFile.println("owl");
			outFile.println("Is it a mammal?");
			outFile.println("tiger");
			outFile.println("rattlesnake");
			outFile.close();
		} catch (FileNotFoundException fnfe) {
			System.out.println("--- ERROR: file restoration failed! ---");
		}
	}

	@Test
	public void toStringTest() {
		GameTree toStringGame = new GameTree("animals.txt");
		System.out.println("ToString: " + toStringGame.toString());
	}
	
	@Test
	public void addTest() {
		GameTree addTest = new GameTree("animals.txt");
		System.out.println("Before Add: " + addTest.toString());
		addTest.add("Does it swim?", "Duck");
		System.out.println("After Add: " + addTest.toString());
	}
	@Test
	public void preOderTest() {
		GameTree preOrderTest = new GameTree("animals.txt");
		System.out.println("preorder: \n" + preOrderTest.callToPreorder());
		
	}
	@Test
	public void testGameWith7() {
		GameTree aGame = new GameTree("animals.txt");
		// Go to the left
		System.out.println(aGame.toString());
		assertEquals("Has feathers?", aGame.getCurrent());
		assertFalse(aGame.foundAnswer());
		aGame.playerSelected(Choice.YES);
		assertEquals("Barnyard?", aGame.getCurrent());
		assertFalse(aGame.foundAnswer());
		aGame.playerSelected(Choice.YES);
		assertEquals("chicken", aGame.getCurrent());
		assertTrue(aGame.foundAnswer());

		aGame.reStart();
		// Go to the right
		assertEquals("Has feathers?", aGame.getCurrent());
		assertFalse(aGame.foundAnswer());
		aGame.playerSelected(Choice.NO);
		assertEquals("Is it a mammal?", aGame.getCurrent());
		aGame.playerSelected(Choice.NO);
		assertEquals("rattlesnake", aGame.getCurrent());
		assertTrue(aGame.foundAnswer());
	}
	
	@Test
	public void testGamesWithYesNo() {
		GameTree bGame = new GameTree("animals.txt");
		// Go to the left
		System.out.println(bGame.toString());
		assertEquals("Has feathers?", bGame.getCurrent());
		assertFalse(bGame.foundAnswer());
		bGame.playerSelected(Choice.YES);
		assertEquals("Barnyard?", bGame.getCurrent());
		assertFalse(bGame.foundAnswer());
		bGame.playerSelected(Choice.NO);
		assertEquals("owl", bGame.getCurrent());
		assertTrue(bGame.foundAnswer());
		
		bGame.reStart();
		
		System.out.println(bGame.toString());
		assertEquals("Has feathers?", bGame.getCurrent());
		assertFalse(bGame.foundAnswer());
		bGame.playerSelected(Choice.NO);
		assertEquals("Is it a mammal?", bGame.getCurrent());
		assertFalse(bGame.foundAnswer());
		bGame.playerSelected(Choice.YES);
		assertEquals("tiger", bGame.getCurrent());
		assertTrue(bGame.foundAnswer());
	}
	
	@Test
	public void testGamesWithAdd() {
		GameTree bGame = new GameTree("animals.txt");
		// Go to the left
		System.out.println(bGame.toString());
		assertEquals("Has feathers?", bGame.getCurrent());
		assertFalse(bGame.foundAnswer());
		bGame.playerSelected(Choice.YES);
		assertEquals("Barnyard?", bGame.getCurrent());
		assertFalse(bGame.foundAnswer());
		bGame.playerSelected(Choice.NO);
		assertEquals("owl", bGame.getCurrent());
		assertTrue(bGame.foundAnswer());
		
		bGame.add("Does it swim?", "duck");
		System.out.println(bGame.toString());
		System.out.println("current: " + bGame.getCurrent());
		assertEquals("owl", bGame.getCurrent());
		assertTrue(bGame.foundAnswer());
		bGame.playerSelected(Choice.NO);
		System.out.println("current: " + bGame.getCurrent());
//		assertEquals("Does it Swim?", bGame.getCurrent());
//		assertFalse(bGame.foundAnswer());
		
	}
}