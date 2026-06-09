import java.util.Scanner;
import java.util.ArrayList;

public class TrieRun {
    public static void main(String[] args) {
	Scanner in = new Scanner(System.in);
	System.out.println("Enter a file name: ");
	String fn = in.nextLine();
	Trie trie = new Trie(fn);
	String input = "input";
	System.out.println("Type \"w\" to search for a specific word.");
	System.out.println("Type \"p\" to search for words by a spefici prefix.");
	System.out.println("Type \"q\" to quit.");
	while(!input.equals("q")) {
	    input = in.nextLine();
	    if(input.equals("w")) {
		System.out.println("Type the word: ");
		input = in.nextLine();
		ArrayList<int[]> locs = trie.get(input);
		if(locs == null) {
		    System.out.println("That word is not in the document.");
		} else {
		    for(int[] loc : locs) {
			System.out.println("Line " + loc[0] + ", Word " + loc[1]);
		    }
		}
	    } else if(input.equals("p")) {
		System.out.println("Type the prefix: ");
		input = in.nextLine();
		ArrayList<String> list = trie.getAll(input);
		if(list == null) {
		    System.out.println("That prefix is not in the document.");
		} else {
		    for(String s : list) {
			System.out.println(s);
		    }
		}
	    } else if(!input.equals("q")){
		System.out.println("That is not a valid input. Try again.");
	    }
	}
    }
}
		
			
	
