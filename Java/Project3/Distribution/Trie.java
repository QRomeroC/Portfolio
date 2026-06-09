import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Set;


public class Trie {
    private Node root;//root node of the trie

    //constructor for an empty Trie
    public Trie() {
	this.root = new Node();
    }

    //constructor that builds a Trie from a file
    public Trie(String fn) {
	this();
	this.build(fn);
    }

    /* search method for a specific word
     * if the word is not in the trie, return null;
     * if the word is in the trie, return a list of
     * all the locations where the word exists;
     * the elements in the list are 2-element arrays
     * where the first element is the Line # and the
     * second element is the Word # 
     */
    public ArrayList<int[]> get(String word) {
	//TO BE IMPLEMENTED
    }

    /* return a list of all the words in the trie
     * that share the given prefix;
     * return null if the prefix does not exist in the trie
     * do not return duplicates
     */
    public ArrayList<String> getAll(String prefix) {
	//TO BE IMPLEMENTED
    }

    /* build the trie from a file 
     */
    private void build(String fn) {
	int lineNum = 1;
	BufferedReader reader;
	try {
	    reader = new BufferedReader(new FileReader(fn));
	    String line = reader.readLine();
	    while(line != null) {
		processLine(lineNum, line);
		line = reader.readLine();
		lineNum++;
	    }
	    reader.close();
	} catch (IOException e) {
	    e.printStackTrace();
	}
    }

    
    /*
     * put a new word into the trie;
     * int[] loc: a 2-element array where
     * the first element is the Line #
     * and the second element is the Word #;
     * indexing starts at 1
     */
    private void put(String word, int[] loc) {
	//TO BE IMPLEMENTED
    }

    /*
     * helper method for building the trie;
     * processes a line from the file
     */
    private void processLine(int lineNum, String line) {
	String[] words = line.split(" ");
	int wordNum = 1;
	while(wordNum <= words.length) {
	    this.put(words[wordNum-1], new int[]{lineNum, wordNum});
	    wordNum++;
	}
    }
	

    /*
     * private Node class
     */
    private class Node {
	String val; //the character value int he node
	ArrayList<int[]> locs; //list of the (Line, Word) locations of the given word; use this only at the end of a word
	HashMap<String,Node> children; //maps characters (as Strings) to children Nodes in order to easily find the next node in the path

	// basic constructor
	Node() {
	    this.val = null;
	    this.locs = null;
	    this.children = new HashMap<String,Node>();
	}

	// constructure if the node has a character value (note that some do not)
	Node(String val) {
	    this();
	    this.val = val;
	}
    }
}
