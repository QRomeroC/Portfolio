package view_controller;

import java.util.Scanner;

import model.BoggleGame;

//Author: Cesar D. Quihuis-Romero
public class BoggleConsole {

  public static void main(String[] args) {
	Scanner keyboard = new Scanner(System.in);
	BoggleGame newGame = new BoggleGame();
    System.out.println("Play one game of Boogle");
    System.out.println(newGame.toString());
    System.out.println("Enter words or ZZ to quit");
    String userInput = keyboard.nextLine();
    newGame.processUserInput(userInput);
    newGame.findValidAndInvalidWords();
    newGame.findUnplayedWords();
    newGame.calcScore();
    keyboard.close();
    
    System.out.println("Your score: " + newGame.getScore());

    System.out.println("Words you found: ");
    System.out.println("================");
    System.out.println(newGame.getValidPlayedWords() + "\n");
        
    System.out.println("Incorrect words: ");
    System.out.println("================");
    System.out.println(newGame.getInvalidWords() + "\n");
    
    String formatedStatement = String.format("You could have found %d more words" , newGame.getUnusedWordCount());
    System.out.println(formatedStatement);
    System.out.println("The computer found all of your words plus these: ");
    System.out.println("============================================================");
    System.out.println(newGame.getUnplayedWords());
    
  }

}