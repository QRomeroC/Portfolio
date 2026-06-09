/**
 * Rick suggests, the IntermediateAI first check to stop a win of the opponent, 
 * then look for its own win. If neither is found, select any other open
 * spot randomly. You may use any other strategy as long as it beats RandomAI.
 * 
 * @authors Rick Mercer and Cesar D. Quihuis-Romero
 */
package model;

import java.util.ArrayList;
import java.util.Random;
//Author: Cesar D. Quihuis-Romero
public class IntermediateAI implements TicTacToeStrategy {
	private ArrayList<OurPoint> openPositions = new ArrayList<OurPoint>();
	private ArrayList<OurPoint> playerPositions = new ArrayList<OurPoint>();
	private ArrayList<OurPoint> computerPositions = new ArrayList<OurPoint>();

	@Override
	public OurPoint desiredMove(TicTacToeGame theGame) {
		openPositions.clear();
		char[][] currBoard = theGame.getTicTacToeBoard();
		
		for (int row = 0; row <= 2; row++) {
			for (int col = 0; col <= 2; col++) {
				if (theGame.available(row, col) == true) {
					OurPoint move = new OurPoint(row, col);
					if (!openPositions.contains(move)) {
						openPositions.add(move);
					} else {
						continue;
					}
				} else if (theGame.available(row, col) == false) {
					if (currBoard[row][col] == 'X') {
						OurPoint playerMove = new OurPoint(row, col);
						if (!playerPositions.contains(playerMove)) {
							playerPositions.add(playerMove);
						} else {
							continue;
						}
					} else {
						OurPoint computerMove = new OurPoint(row, col);
						if (!computerPositions.contains(computerMove)) {
							computerPositions.add(computerMove);
						} else {
							continue;
						}
					}
				}
			}
		}

		if (openPositions.size() == 1) {
			OurPoint move = openPositions.get(0);
			openPositions.remove(0);
			return move;

		} else {
			OurPoint move = evaluateBoard(currBoard);
			openPositions.remove(move);
			return move;
		}
	}
	
	private OurPoint evaluateBoard(char[][] currBoard) {
		for (OurPoint pos : openPositions) {
			int row = pos.row;
			int col = pos.col;
			currBoard[row][col] = 'O';
			if (wonByRow('O',currBoard) || wonByCol('O', currBoard) || wonByDiagonal('O', currBoard)) {
				currBoard[row][col] = '_';
				return pos;
			} else {
				currBoard[row][col] = 'X';
				if (wonByRow('X',currBoard) || wonByCol('X', currBoard) || wonByDiagonal('X', currBoard)) {
					currBoard[row][col] = '_';
					return pos;
				} else {
					currBoard[row][col] = '_';
				}
			}
		}
		Random rand = new Random();
		int randPos = rand.nextInt(openPositions.size());
		return openPositions.get(randPos);
	}

	private boolean wonByRow(char playerChar, char[][] currBoard) {
	    for (int r = 0; r < 3; r++) {
	      int rowSum = 0;
	      for (int c = 0; c < 3; c++)
	        if (currBoard[r][c] == playerChar)
	          rowSum++;
	      if (rowSum == 3)
	        return true;
	    }
	    return false;
	  }

	  private boolean wonByCol(char playerChar, char[][] currBoard) {
	    for (int c = 0; c < 3; c++) {
	      int colSum = 0;
	      for (int r = 0; r < 3; r++)
	        if (currBoard[r][c] == playerChar)
	          colSum++;
	      if (colSum == 3)
	        return true;
	    }
	    return false;
	  }

	  private boolean wonByDiagonal(char playerChar, char[][] currBoard) {
	    // Check Diagonal from upper left to lower right
	    int sum = 0;
	    for (int r = 0; r < 3; r++)
	      if (currBoard[r][r] == playerChar)
	        sum++;
	    if (sum == 3)
	      return true;

	    // Check Diagonal from upper right to lower left
	    sum = 0;
	    for (int r = 3 - 1; r >= 0; r--)
	      if (currBoard[3 - r - 1][r] == playerChar)
	        sum++;
	    if (sum == 3)
	      return true;

	    // No win on either diagonal
	    return false;
	  }

}
