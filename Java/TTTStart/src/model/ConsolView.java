package model;

import java.util.Scanner;
//Author: Cesar D. Quhuis-Romero
public class ConsolView {

	public static void main(String[] args) {
		TicTacToeGame newGame = new TicTacToeGame();
		Scanner keyboard = new Scanner(System.in);
		System.out.println("Let's play TicTacToe!");
		System.out.println("Choose your difficulty: (R) for random (I) for Intermediate");

		String difficulty = keyboard.nextLine();
		difficulty = difficulty.strip().toLowerCase();

		while (!difficulty.equals("r") && !difficulty.equals("i")) {
			System.out.println("Invaild selection, please select difficulty");
			System.out.println("Choose your difficulty: (R) for random (I) for Intermediate");
			difficulty = keyboard.nextLine();
			difficulty = difficulty.strip().toLowerCase();
		}
		if (difficulty.equals("i")) {
			IntermediateAI intermediateAI = new IntermediateAI();
			newGame.setComputerPlayerStrategy(intermediateAI);
		}
		playGame(newGame, keyboard);
		keyboard.close();
	}

	private static void playGame(TicTacToeGame newGame, Scanner keyboard) {
		while (newGame.stillRunning()) {
			System.out.print("Enter row and Column: ");
			String playerMoveAsStr = keyboard.nextLine();

			playerMoveAsStr = playerMoveAsStr.strip();
			String[] playerMoveStringArray = playerMoveAsStr.split(" ");
			int row = Integer.parseInt(playerMoveStringArray[0]);
			int col = Integer.parseInt(playerMoveStringArray[1]);
			if (row > 2 || col > 2) {
				System.out.println("Invalid move, please try again");
				continue;
			}
			if (!newGame.available(row, col)) {
				System.out.println("Square take, try agin");
				continue;
			}
			newGame.humanMove(row, col, false);
			System.out.println(newGame.toString());
		}
		if (newGame.tied()) {
			System.out.println("Tie");
		} else if (newGame.didWin('X')) {
			System.out.println("X wins");
		} else {
			System.out.print("O wins");
		}
	}

}
