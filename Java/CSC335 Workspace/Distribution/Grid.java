import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Random;
import java.util.Arrays;

public class Grid {
    private Loc[][] grid;
    private int accessCount;

    //constructor: creates a new Grid based on 
    //an input file
    //filename: the name of the file
    //isStrings: true if the the grid contains Strings;
    //false if it contains integers
    public Grid(String filename, boolean isStrings) {
	this.accessCount = 0;
	BufferedReader reader;
	try {
	    reader = new BufferedReader(new FileReader(filename));
	    String line = reader.readLine();
	    if(line != null) {
		int n = Integer.parseInt(line);
		int i = 0;
		grid = new Loc[n][n];
		line = reader.readLine();
		int r = 0;
		while(line != null) {
		    String[] str = line.split(" ");
		    if(str.length < n)
			break;
		    for(int c = 0; c < n; c++) {
			grid[r][c] = new Loc(r, c, str[c]);
			i++;
		    }
		    r++;
		    line = reader.readLine();
		}
	    } 
	}catch (Exception e) {
		e.printStackTrace();
	}
    }

    //creates a String representation of the Grid
    public String toString() {
	String str = "";
	for(int r = 0; r < grid.length; r++) {
	    for(int c = 0; c < grid[0].length; c++) {
		str += grid[r][c].getVal() + " ";
	    }
	    str += "\n";
	}
	return str;
    }

    //returns the size of the Grid (i.e. n for 
    //an n X n Grid--all Grids are square!    
    public int size() {
	return grid.length;
    }

    //returns the location at (i, j) or null if (i, j)
    //is outside the grid
    public Loc getLoc(int i, int j) {
	if(checkIndex(i) && checkIndex(j)) {
	    accessCount++;
	    return grid[i][j];
	}
	return null;
    }

    //returns the String value at (i, j) or
    //null if it is outside the grid
    public String getVal(int i, int j) {
	Loc loc = getLoc(i, j);
	if(loc != null)
	    return loc.getVal();
	return null;
    }

    //returns the integer value at (i, j)
    //or -999 if the location is outside the grid
    //NOTE: This only works for integer grids!
    public int getIntVal(int i, int j) {
	String str = getVal(i, j);
	if(str != null)
	    return Integer.parseInt(str);
	return -999;
    }

    //returns the access count
    public int getAccessCount() {
	return this.accessCount;
    }

    //reset access count
    public void resetAccessCount() {
	this.accessCount = 0;
    }

    //checks the index to see if it is in the grid
    private boolean checkIndex(int i) {
	return i >= 0 && i < grid.length;
    }
}
    
	
