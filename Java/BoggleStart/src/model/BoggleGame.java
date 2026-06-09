package model;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;

//Author: Cesar D. Quihuis-Romero
public class BoggleGame {
	@SuppressWarnings("unused")
	private int score;
	private Random randInt = new Random();
	@SuppressWarnings("unused")
	private Boolean endGameFlag = false;
	private ArrayList<String> boardWords;
	private ArrayList<String> userWords;
	private ArrayList<String> invalidWords;
	private ArrayList<String> unplayedWords;
	private ArrayList<String> validPlayedWords;
	private ArrayList<String> wordList;
	private static char[][] DICE = { { 'L', 'R', 'Y', 'T', 'T', 'E' }, { 'V', 'T', 'H', 'R', 'W', 'E' },
			{ 'E', 'G', 'H', 'W', 'N', 'E' }, { 'S', 'E', 'O', 'T', 'I', 'S' }, { 'A', 'N', 'A', 'E', 'E', 'G' },
			{ 'I', 'D', 'S', 'Y', 'T', 'T' }, { 'O', 'A', 'T', 'T', 'O', 'W' }, { 'M', 'T', 'O', 'I', 'C', 'U' },
			{ 'A', 'F', 'P', 'K', 'F', 'S' }, { 'X', 'L', 'D', 'E', 'R', 'I' }, { 'H', 'C', 'P', 'O', 'A', 'S' },
			{ 'E', 'N', 'S', 'I', 'E', 'U' }, { 'Y', 'L', 'D', 'E', 'V', 'R' }, { 'Z', 'N', 'R', 'N', 'H', 'L' },
			{ 'N', 'M', 'I', 'H', 'U', 'Q' }, { 'O', 'B', 'B', 'A', 'O', 'J' } };
	private char[][] gameBoard = new char[4][4];

	public BoggleGame() {
		ArrayList<Integer> usedDice = new ArrayList<Integer>();
		this.boardWords = new ArrayList<>();
		this.userWords = new ArrayList<>();
		this.invalidWords = new ArrayList<>();
		this.unplayedWords = new ArrayList<>();
		this.validPlayedWords = new ArrayList<>();
		this.wordList = new ArrayList<>();
		this.score = 0;

		int selectedDie;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				do {
					selectedDie = selectDie(usedDice);
				} while (selectedDie == -1);
				gameBoard[i][j] = DICE[selectedDie][randInt.nextInt(6)];
			}
		}
		DiceTray gameTray = new DiceTray(gameBoard);
		readFile();
		processBoard(gameTray);

	}

	private void readFile() {
		try {
			File boggleWords = new File("BoggleWords.txt");
			Scanner wordReader = new Scanner(boggleWords);
		
			while (wordReader.hasNextLine()) {
				String word = wordReader.nextLine();
				wordList.add(word);
			}
			wordReader.close();
		} catch (FileNotFoundException e) {
			System.out.println("File not found");
		}
	}

	private void processBoard(DiceTray gameTray) {
		for (String word : wordList) {
			Boolean found = gameTray.found(word);
			if (found) {
				boardWords.add(word);
			}
		}
	}

	public void findValidAndInvalidWords() {
		for (String userWord : userWords) {
			if (boardWords.contains(userWord)) {
				validPlayedWords.add(userWord);
			} else {
				invalidWords.add(userWord);
			}
		}
	}

	public void findUnplayedWords() {
		for (String word : boardWords) {
			if (!validPlayedWords.contains(word)) {
				unplayedWords.add(word);
			}
		}
	}

	public int selectDie(ArrayList<Integer> usedDice) {
		int currDice = randInt.nextInt(16);
		if (usedDice.contains(currDice)) {
			return -1;
		} else {
			usedDice.add(currDice);
			return currDice;
		}
	}

	public void processUserInput(String userInput) {
		userInput = userInput.toLowerCase();
		
		String[] parsedInput = userInput.split(" ");
		for (String word : parsedInput) {
			if (word.equals("zz")) {
				break;
			} else {
				if (!userWords.contains(word)) {
					userWords.add(word);
				} else {
					continue;
				}
				
			}
		}
	}

	public void calcScore() {
		for (String currWord : validPlayedWords) {
			if (currWord.length() == 3 || currWord.length() == 4) {
				score += 1;
			} else if (currWord.length() == 5) {
				score += 2;
			} else if (currWord.length() == 6) {
				score += 3;
			} else if (currWord.length() == 7) {
				score += 5;
			} else {
				score += 11;
			}
		}
	}

	public int getScore() {
		return score;
	}
	
	public int getUnusedWordCount() {
		int count = 0;
		for (@SuppressWarnings("unused") String word : unplayedWords) {
			count++;
		}
		return count;
	}
	public String getBoardWords() {
		String strToReturn = "";
		int wordCount = 0;
		for (String word : boardWords) {
			if (wordCount == 20) {
				wordCount = 0;
				strToReturn += "\n";
			} else {
				wordCount++;
				strToReturn += word + " ";
			}
		}
		return strToReturn;
	}

	public String getUserWords() {
		String strToReturn = "";
		int wordCount = 0;
		for (String word : userWords) {
			if (wordCount == 20) {
				wordCount = 0;
				strToReturn += "\n";
			} else {
				wordCount++;
				strToReturn += word + " ";
			}
		}
		return strToReturn;
	}

	public String getInvalidWords() {
		String strToReturn = "";
		int wordCount = 0;
		for (String word : invalidWords) {
			if (wordCount == 20) {
				wordCount = 0;
				strToReturn += "\n";
			} else {
				wordCount++;
				strToReturn += word + " ";
			}
		}
		return strToReturn;
	}

	public String getValidPlayedWords() {
		String strToReturn = "";
		int wordCount = 0;
		for (String word : validPlayedWords) {
			if (wordCount == 20) {
				wordCount = 0;
				strToReturn += "\n";
			} else {
				wordCount++;
				strToReturn += word + " ";
			}
		}
		return strToReturn;
	}

	public String getUnplayedWords() {
		String strToReturn = "";
		int wordCount = 0;
		for (String word : unplayedWords) {
			if (wordCount == 20) {
				wordCount = 0;
				strToReturn += "\n";
			} else {
				wordCount++;
				strToReturn += word + " ";
			}
		}
		return strToReturn;
	}

	public String toString() {
		String toString = "";

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				char currChar = gameBoard[i][j];
				toString += currChar + " ";
			}
			toString += "\n";
		}

		return toString;
	}
}
