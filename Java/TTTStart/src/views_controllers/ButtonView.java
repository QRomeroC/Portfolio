package views_controllers;

import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import model.OurObserver;
import model.TicTacToeGame;

public class ButtonView extends BorderPane implements OurObserver {
	private TicTacToeGame theGame;

	Button[][] buttons = new Button[3][3];
	Label gameState = new Label("Click to make move");

	public ButtonView(TicTacToeGame theModel) {
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				buttons[row][col] = new Button("_");
			}
		}
		theGame = theModel;
		initializePanel();


	}

	private void initializePanel() {
		VBox userInterface = LayoutInterface();
		this.setTop(userInterface);
	}

	private VBox LayoutInterface() {
		Font buttonFont = Font.font("Courier New", 32);
		Font gameStateFont = Font.font("Courier New", 18);
		VBox userInterface = new VBox();
		HBox rowZero = new HBox();
		HBox rowOne = new HBox();
		HBox rowTwo = new HBox();
		
		
		rowZero.getChildren().addAll(buttons[0][0], buttons[0][1], buttons[0][2]);
		rowOne.getChildren().addAll(buttons[1][0], buttons[1][1], buttons[1][2]);
		rowTwo.getChildren().addAll(buttons[2][0], buttons[2][1], buttons[2][2]);
		
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				buttons[row][col].setPrefSize(80, 80);
				buttons[row][col].setFont(buttonFont);
				buttons[row][col].setDisable(false);
				buttons[row][col].setOnAction(new buttonClickEvent());
			}
		}
		
		// insets(top,right,bottom,left)
		
		rowZero.setAlignment(Pos.CENTER);
		rowOne.setAlignment(Pos.CENTER);
		rowTwo.setAlignment(Pos.CENTER);
		
		userInterface.setAlignment(Pos.CENTER);
		gameState.setFont(gameStateFont);
		userInterface.getChildren().addAll(rowZero, rowOne, rowTwo, gameState);

		return userInterface;
	}

	@Override
	public void update(Object observable) {
		System.out.println("update called from OurObservable TicTacToeGame \n" + theGame);
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				buttons[row][col].setDisable(false);
				String currChar = String.valueOf(theGame.getTicTacToeBoard()[row][col]);
				buttons[row][col].setText(currChar);
			}
		}
		if (theGame.stillRunning()) {
			gameState.setText("Click to make move");
		}
		if (!theGame.stillRunning()) {
			if (theGame.tied()) {
				gameState.setText("Tied");
			} else if (theGame.didWin('X')) {
				gameState.setText("X Wins");
			} else {
				gameState.setText("O Wins");
			}
		}
	}
	
	private class buttonClickEvent implements EventHandler<ActionEvent>{

		@Override
		public void handle(ActionEvent ae) {
			Object buttonSource = ae.getSource();
			for (int row = 0; row < 3; row++) {
				for (int col = 0; col < 3; col++) {
					if (buttons[row][col] == buttonSource) {
						theGame.humanMove(row,col,false);
						String buttonText = String.valueOf(theGame.getTicTacToeBoard()[row][col]);
						buttons[row][col].setText(buttonText);
						buttons[row][col].setDisable(true);
					}
				}
			}
		}
		
	}
}
