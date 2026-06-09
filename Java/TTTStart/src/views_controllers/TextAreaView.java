package views_controllers;


import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;

/**
 * This is the beginning of one view of a Tic Tac Toe game using
 * two TextField objects and one TextArea. The other two views
 * of ButtonView and DrawingView follow the same structure as this.
 * 
 * @author Rick Mercer and YOUR NAME 
 */
import javafx.scene.control.Button;
import javafx.scene.control.Label;

import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;

import model.OurObserver;
import model.TicTacToeGame;

//Author: Rick Mercer and Cesar D. Quihuis-Romero
public class TextAreaView extends BorderPane implements OurObserver {

  private TicTacToeGame theGame;
  private Label rowLabel = new Label("Row");
  private Label colLabel = new Label("Col");
  private TextField rowInput = new TextField();
  private TextField colInput = new TextField();
  private Button submitButton = new Button("Make Move");
  private TextArea gameBoard = new TextArea();
  
  public TextAreaView(TicTacToeGame theModel) {
    theGame = theModel;
    initializePanel();
  }

  private void initializePanel() {
	VBox userInterface = LayoutInterface();
    Font font = new Font("Courier New", 32);
    gameBoard.setFont(font);
    gameBoard.setText("");
    gameBoard.setEditable(false);
    
    submitButton.setDisable(true);
    
    this.setCenter(gameBoard);
    this.setTop(userInterface);
    submitButton.setOnAction(new submitButtonMoveEventHandler());
  }
  
  private VBox LayoutInterface() {
	  VBox userInterface = new VBox();
	  HBox rowArea = new HBox();
	  HBox colArea = new HBox();
	  
	  Font labelFont = Font.font("Courier New", FontWeight.BOLD, 24);
	  rowLabel.setFont(labelFont);
	  colLabel.setFont(labelFont);
	  rowInput.setMinWidth(20);
	  rowInput.setMaxWidth(25);
	  colInput.setMinWidth(20);
	  colInput.setMaxWidth(25);
	  
	  
	  rowArea.getChildren().addAll(rowInput, rowLabel);
	  colArea.getChildren().addAll(colInput, colLabel);
	  HBox.setMargin(rowLabel, new Insets(10, 10, 5, 10));
	  HBox.setMargin(colLabel, new Insets(5,10,10, 10));
	  VBox.setMargin(submitButton, new Insets(5, 10, 5, 10));
	  
	  userInterface.getChildren().addAll(rowArea, colArea, submitButton);
	  rowArea.setAlignment(Pos.CENTER);
	  colArea.setAlignment(Pos.CENTER);
	  userInterface.setAlignment(Pos.CENTER);
	  return userInterface;
	  
  }
  
  // This method is called by Observable's notifyObservers()
  @Override
  public void update(Object observable) {
    System.out.println("update called from OurObservable TicTacToeGame \n" + theGame);
    gameBoard.setText(theGame.toString());
    submitButton.setDisable(false);
    submitButton.setText("Make Move");
  }
  
  
  private class submitButtonMoveEventHandler implements EventHandler<ActionEvent>{

	@Override
	public void handle(ActionEvent ae) {
		String rowAsStr = rowInput.getText().strip();
		String colAsStr= colInput.getText().strip();
		int rowAsInt = Integer.parseInt(rowAsStr);
		int colAsInt = Integer.parseInt(colAsStr);
		if (rowAsInt < 0 && rowAsInt > 2 && colAsInt < 0 && colAsInt > 2) {
			submitButton.setText("Invalid Move");
		} else {
			rowInput.setText("");
			colInput.setText("");
			if (!theGame.available(rowAsInt, colAsInt)) {
				submitButton.setText("Invalid Move");
			}
			theGame.humanMove(rowAsInt, colAsInt, false);
			gameBoard.setText(theGame.toString());
			if (!theGame.stillRunning()) {
				if (theGame.tied()) {
					submitButton.setText("Tied");
					submitButton.setOnAction(new submitButtonPostGameEventHandler());
				} else if (theGame.didWin('X')) {
					submitButton.setText("X wins");
					submitButton.setOnAction(new submitButtonPostGameEventHandler());
				} else {
					submitButton.setText("O wins");
					submitButton.setOnAction(new submitButtonPostGameEventHandler());
				}
			}
		}
	}
	  
  }
  
  private class submitButtonPostGameEventHandler implements EventHandler<ActionEvent>{

	@Override
	public void handle(ActionEvent ae) {
		if (theGame.tied()) {
			submitButton.setText("Tied");
		} else if (theGame.didWin('X')) {
			submitButton.setText("X wins");
		} else if (theGame.didWin('O')) {
			submitButton.setText("O wins");
		}
		
	}
	  
  }
  
}