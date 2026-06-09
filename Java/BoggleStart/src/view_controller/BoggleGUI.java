package view_controller;

import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundImage;
import javafx.scene.layout.BackgroundPosition;
import javafx.scene.layout.BackgroundRepeat;
import javafx.scene.layout.BackgroundSize;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.stage.Stage;
import model.BoggleGame;

//Author: Cesar D. Quihuis-Romero
public class BoggleGUI extends Application {

	public static void main(String[] args) {
		launch(args);
	}// end of launch

	BoggleGame newGame = new BoggleGame();
	TextArea gameBoard = new TextArea();
	TextArea scoreArea = new TextArea();
	TextField userInput = new TextField("Press Start Game to Play!");
	Button startGame = new Button("Start Game");
	Button endGame = new Button("End Game");
	Label gameBoardLabel = new Label("GAME BOARD");
	Label scoreLabel = new Label("SCORE");
	TextArea display = new TextArea();
	// listener to create a clean user entry field
	ChangeListener<Boolean> listener = new ChangeListener<Boolean>() {
		public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
			if (newValue) {
				userInput.setText("");
			}
		}
	};

	@Override
	public void start(Stage stage) {
		stage.setTitle("My Boggle");
		stage.setResizable(false);

		endGame.setDisable(true);
		endGame.setOpacity(.5);
		// setPrefSize(width, height)
		VBox buttonAndGameBoard = LayOutGUI();
		BorderPane border = new BorderPane();
		border.setCenter(buttonAndGameBoard);

		Image image = new Image("file:cloudBackgroundSample1.jpg");
		BackgroundSize backgroundSize = new BackgroundSize(BackgroundSize.AUTO, BackgroundSize.AUTO, false, false, true,
				false);
		BackgroundImage backgroundImage = new BackgroundImage(image, BackgroundRepeat.NO_REPEAT,
				BackgroundRepeat.NO_REPEAT, BackgroundPosition.CENTER, backgroundSize);
		Background background = new Background(backgroundImage);
		border.setBackground(background);
		
		gameBoard.setOpacity(.75);
		scoreArea.setOpacity(.75);
		display.setOpacity(.75);

		userInput.setEditable(false);
		display.setEditable(false);

		startGame.setOnAction(new StartButtonHandler());

		endGame.setOnAction(new EndButtonHandler());
		// scene(obj,width,height)
		Scene scene = new Scene(border, 720, 720);

		stage.setScene(scene);
		stage.show();
	}// end of start

	private VBox LayOutGUI() {
		Font gameBoardFont = Font.font("Courier New", FontWeight.BOLD, 36);
		Font userInputFont = Font.font("Courier New", 14);
		Font scoreFont = Font.font("Courier New", FontWeight.BOLD, 48);
		Font labelFont = Font.font("Courier New", FontWeight.BOLD, 24);

		gameBoard.setStyle("-fx-background-color: olivedrab");
		gameBoardLabel.setFont(labelFont);
		scoreLabel.setFont(labelFont);
		gameBoardLabel.setTextFill(Color.GOLD);
		scoreLabel.setTextFill(Color.GOLD);
	
		userInput.setStyle("-fx-background-color: peru; -fx-text-fill: gold;");
	

		VBox insetArea = new VBox();
		HBox interfaceArea = new HBox();
		HBox gameBoardArea = new HBox();
		gameBoardArea.setStyle("-fx-background-color: peru");
		insetArea.setAlignment(Pos.TOP_CENTER);
		insetArea.setSpacing(10);
		interfaceArea.setSpacing(10);
		interfaceArea.setAlignment(Pos.CENTER);
		gameBoardArea.setAlignment(Pos.CENTER);

		scoreArea.setMinWidth(140);
		scoreArea.setMaxWidth(180);
		scoreArea.setMinHeight(100);
		scoreArea.setMaxHeight(100);
		scoreArea.setFont(scoreFont);
		scoreArea.setEditable(false);

		gameBoard.setFont(gameBoardFont);
		gameBoard.setEditable(false);

		userInput.setFont(userInputFont);

		userInput.setMinWidth(120);
		userInput.setMaxWidth(680);

		gameBoard.setMaxSize(240, 240);

		// setting up images used in scene
		ImageView boggleImage1 = new ImageView(new Image("file:boggleDiceSample.jpg"));
		ImageView boggleImage2 = new ImageView(new Image("file:boggleDiceRolling.jpg"));
		boggleImage1.setFitHeight(200);
		boggleImage1.setFitWidth(200);
		boggleImage2.setFitHeight(200);
		boggleImage2.setFitWidth(200);

		// setMaxSize(width, height)
		display.setMaxSize(680, 150);
		// insets(top,right,bottom,left)
		HBox.setMargin(boggleImage1, new Insets(50, 20, 5, 20));
		HBox.setMargin(boggleImage2, new Insets(50, 20, 5, 20));
		HBox.setMargin(startGame, new Insets(20, 10, 5, 20));
		HBox.setMargin(endGame, new Insets(20, 20, 5, 10));
		HBox.setMargin(scoreArea, new Insets(5, 5, 5, 40));
		HBox.setMargin(gameBoardLabel, new Insets(5, 5, 5, 5));
		
		interfaceArea.getChildren().addAll(boggleImage1, startGame, endGame, boggleImage2);
		gameBoardArea.getChildren().addAll(gameBoardLabel, gameBoard, scoreArea, scoreLabel);
		insetArea.getChildren().addAll(interfaceArea, gameBoardArea, userInput, display);

		return insetArea;
	}// end of vbox layout

	private class StartButtonHandler implements EventHandler<ActionEvent> {

		@Override
		public void handle(ActionEvent ae) {
			startGame.setDisable(true);
			startGame.setOpacity(.5);
			endGame.setDisable(false);
			endGame.setOpacity(1);

			gameBoard.setText(newGame.toString());

			userInput.setEditable(true);
			userInput.setText("Enter your words press enter to submit: ");
			userInput.focusedProperty().addListener(listener);
			userInput.setOnAction(new UserInputHandler());
		}
	}// end of private class start button event handler

	private class UserInputHandler implements EventHandler<ActionEvent> {

		@Override
		public void handle(ActionEvent ae) {
			userInput.focusedProperty().removeListener(listener);
			gameBoard.setStyle("-fx-text-alignment: center");
			userInput.setEditable(false);
			userInput.setOnAction(null);
			String userWords = userInput.getText();
			newGame.processUserInput(userWords);
			newGame.findValidAndInvalidWords();
			newGame.findUnplayedWords();
			newGame.calcScore();
			scoreArea.setText("" + newGame.getScore());
			display.setText("Valid Words: " + newGame.getValidPlayedWords() + "\n" + "Unplayed Words: "
					+ newGame.getUnplayedWords() + "\n" + "Incorrect Words: " + newGame.getInvalidWords());
		}
	}// end of private class user input event handler

	private class EndButtonHandler implements EventHandler<ActionEvent> {

		@Override
		public void handle(ActionEvent ae) {
			startGame.setDisable(false);
			userInput.setDisable(false);
			userInput.setOnAction(null);
			startGame.setOpacity(1);
			endGame.setDisable(true);
			userInput.focusedProperty().removeListener(listener);
			endGame.setOpacity(.5);

			gameBoard.setText("");

			userInput.setEditable(false);
			userInput.setText("Press Start Game to Play!");

			display.setText("");
			scoreArea.setText("");

			newGame = new BoggleGame();
		}

	}
}// end of application
