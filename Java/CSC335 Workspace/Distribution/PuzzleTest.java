import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class PuzzleTest {
    private static Deque<Integer> starts; 
    private static String[] test1Words = new String[] {"AFGHANISTAN", "ALBANIA", "ALGERIA", "ANDORRA", "ANGOLA"};
    private static int[] test1Starts = new int[] {11, 11, 0, 0, 5, 12, 7, 8, 14, 0};
    private static String[] test2Words = new String[] {"BAHAMAS", "BAHRAIN", "BANGLADESH", "BARBADOS", "BELARUS", "BELGIUM", "BELIZE", "BENIN", "BHUTAN", "BOLIVIA"};
    private static int[] test2Starts = new int[] {0, 24, 5, 19, 23, 22, 10, 10, 11, 15, 2, 8, 13, 22, 8, 18, 3, 14, 15, 9};
    private static String[] test3Words = new String[] {"CABOVERDE", "CAMBODIA", "CAMEROON", "CANADA", "CENTRALAFRICANREPUBLIC", "CHAD", "CHILE", "CHINA", "COLOMBIA", "COMOROS", "CONGO", "COOKISLANDS", "COSTARICA", "CROATIA", "CUBA"};
    private static int[] test3Starts = new int[] {0, 0, 2, 48, 31, 4, 15, 9, 2, 0, 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 5, 39, 44, 33, 27, 10, 37, 47, 34, 49}; 
    private static String[] paths;
    private static int[] accessCounts;
    
    public static void main(String[] args) {
	double score = 0.0;
	for(int i = 1; i <= 3; i++) 
	    score += runTest(i);
	System.out.println("Total: " + Math.min(score, 80));
    }

    private static double runTest(int num) {
	System.out.println("***TEST " + num + "***\n");
	double score = 0.0;
	String[] words = null;
	int[] starts = null;
	if(num == 1) {
	    words = test1Words;
	    starts = test1Starts;
	} else if(num == 2) {
	    words = test2Words;
	    starts = test2Starts;
	} else if(num == 3) {
	    words = test3Words;
	    starts = test3Starts;
	}
	String fn = "puzzle" + num + "_test.txt";
	Grid grid = new Grid(fn, true);
	Puzzle p = new Puzzle(grid);
	setExpected(num, words.length);
	for(int i = 0; i < words.length; i++) {
	    System.out.println("Searching puzzle for " + words[i] + "...");
	    int x = starts[2*i];
	    int y = starts[2*i+1];
	    String path = p.find(words[i], x, y);
	    int count = grid.getAccessCount();
	    score += checkPath(path, paths[i]);
	    score += checkCounts(words[i].length(), count, accessCounts[i]);
	    grid.resetAccessCount();
	}
	System.out.println("\nTest " + num + " expected score: " + score + "\n");
	return score;
    }

    private static double checkPath(String act, String exp) {
	if(act != null && act.equals(exp)) {
	    System.out.println("Found correct path!\n");
	    return 2.0;
	}
	System.out.println("Failed to find correct path...");
	System.out.println("Expected: " + exp);
	System.out.println("Actual: " + act + "\n");
	return 0.0;
    }

    private static double checkCounts(int lb, int act, int exp) {
	double score = 0.0;
	if(act < lb-1) {
	    System.out.println("Access count is way to low! Something isn't right...");
	    return 0;
	}
	if(act <= exp*3)
	    score += 0.25;
	if(act <= exp*2)
	    score += 0.25;
	if(act <= exp)
	    score += 0.5;
	if(score < 0.25)
	    System.out.println("Access count is too high!");
	else if(score < 1.0)
	    System.out.println("Access count is not too bad, but it could be lower!");
	else if(score >= 1.0)
	    System.out.println("Access count looks good!");
	System.out.println("Expected access count: " + exp);
	System.out.println("Your access count: " + act + "\n"); 
	return score;
    }

    private static void setExpected(int testNum, int numWords) {
	String fn = "test_output" + testNum + ".txt";
	paths = new String[numWords];
	accessCounts = new int[numWords];
	BufferedReader reader;
	try {
	    reader = new BufferedReader(new FileReader(fn));
	    int i = 0;
	    while(i < numWords) {
		String line = reader.readLine();
		if(line != null)
		    paths[i] = line;
		line = reader.readLine();
		if(line != null)
		    accessCounts[i] = Integer.parseInt(line);
		i++;
	    }
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }
}
