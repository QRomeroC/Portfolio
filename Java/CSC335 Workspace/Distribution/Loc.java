public class Loc {
    public final int row;
    public final int col;
    private String val;
    
    //constructor
    //x is row, y is column
    public Loc(int x, int y, String val) {
	this.row = x;
	this.col = y;
	this.val = val;
    }

    //returns Loc in the form (row, col)
    public String toString() {
	return "(" + row + ", " + col + ")";
    }

    //returns the String value at this location
    public String getVal() {
	return val;
    }
}
