
public class SortGrid {
	public static void sortA(Grid g) {
		int col = 0;
		for (int row = 0; row <= g.size(); row++) {
			heapRow(g, g.getLoc(row,col),col);
			col++;
		}
		
		int m = g.size() - 1;
		int row = 0;
		col = 0;
		while (m > 0) {
			g.swap(0, 0, 0, m);
			m--;
			sink(g, g.getLoc(row, col).row, row, col);
			if (row < g.size()) {
				row++;
			} else {
				row = 0;
			}
			
			if (col < g.size()) {
				col++;
			} else {
				col = 0;
			}
			
		}
	}
	
	private static void heapRow(Grid g, Loc currLoc, int col) {
		for (int i = g.size()/2; i >= 0; i--) {
			sink(g, g.getLoc(i, col).row, i, g.size() - 1);
		}
	}
	
	private static void sink(Grid g, int row, int i,int  m) {
		while (2 * i + 1 <= m) {
			int j = 2 * i + 1;
			if (j < m && g.getIntVal(i, j) < g.getIntVal(i, j + 1)) {
				j++;
			}
			if (g.getIntVal(row, i) >= g.getIntVal(row, j)) {
				break;
			}
			g.swap(row, i, row, j);
			i = j;
		}
	}
	
	public static void sortB(Grid g) {
		
	}
}
