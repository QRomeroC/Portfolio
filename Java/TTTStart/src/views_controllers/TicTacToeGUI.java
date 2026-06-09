package views_controllers;

/**
 * Play TicTacToe the computer that can have different AIs to beat you. 
 * Select the Options menus to begin a new game, switch strategies for 
 * the computer player (BOT or AI), and to switch between the two views.
 * 
 * This class represents an event-driven program with a graphical user 
 * interface as a controller between the view and the model. It has 
 * event handlers to mediate between the view and the model.
 * 
 * This controller employs the Observer design pattern that updates two 
 * views every time the state of the Tic Tac Toe game changes:
 * 
 *  1) whenever you make a move by clicking a button or an area of either view
 *  2) whenever the computer AI makes a move
 *  3) whenever there is a win or a tie
 *    
 * You can also select two different strategies to play against from the menus
 * 
 * @author Rick Mercer and Cesar D. Quihuis-Romero
 */
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;
import model.IntermediateAI;
import model.OurObserver;
import model.RandomAI;
import model.TicTacToeGame;

public class TicTacToeGUI extends Application {

	public static void main(String[] args) {
		launch(args);
	}

	private TicTacToeGame theGame;

	private OurObserver currentView;
	private OurObserver textAreaView;
	private OurObserver buttonView;
	private OurObserver drawingView;

	private BorderPane window;
	public static final int width = 254;
	public static final int height = 360;

	private Button intermediateAI = new Button("Intermediate");
	private Button randomAI = new Button("Random");
	
	private IntermediateAI interAI = new IntermediateAI();
	private RandomAI randAI = new RandomAI();

	public void start(Stage stage) {
		stage.setTitle("Tic Tac Toe");
		window = new BorderPane();

		Menu optionsMenu = new Menu("Options");
		MenuItem newGame = new MenuItem("New Game");

		Menu switchView = new Menu("Switch View");
		MenuItem buttonViewMenuItem = new MenuItem("Button View");
		MenuItem textViewMenuItem = new MenuItem("Text View");
		MenuItem drawingViewMenuItem = new MenuItem("Drawing View");
		switchView.getItems().addAll(buttonViewMenuItem, textViewMenuItem, drawingViewMenuItem);

		Menu difficulty = new Menu("Select Difficulty");
		MenuItem randomAI = new MenuItem("Random AI");
		MenuItem intermidateAI = new MenuItem("Intermidate AI");
		difficulty.getItems().addAll(randomAI, intermidateAI);

		optionsMenu.getItems().addAll(newGame, switchView, difficulty);
		MenuBar optionsBar = new MenuBar(optionsMenu);

		newGame.setOnAction(new MenuItemListener());
		intermidateAI.setOnAction(new MenuItemListener());
		randomAI.setOnAction(new MenuItemListener());

		buttonViewMenuItem.setOnAction(new MenuItemListener());
		textViewMenuItem.setOnAction(new MenuItemListener());
		drawingViewMenuItem.setOnAction(new MenuItemListener());

		window.setTop(optionsBar);
		Scene scene = new Scene(window, width, height);
		initializeGameForTheFirstTime();

		textAreaView = new TextAreaView(theGame);
		buttonView = new ButtonView(theGame);
		drawingView = new DrawingView(theGame);

		theGame.addObserver(textAreaView);
		theGame.addObserver(buttonView);
		theGame.addObserver(drawingView);

		setViewTo(textAreaView);

		stage.setScene(scene);
		stage.show();
	}

	/**
	 * Set the game to the default of an empty board and the random AI.
	 */
	public void initializeGameForTheFirstTime() {
		theGame = new TicTacToeGame();
		// This event driven program will always have
		// a computer player who takes the second turn
		theGame.setComputerPlayerStrategy(new RandomAI());
	}

	private void setViewTo(OurObserver newView) {
		window.setCenter(null);
		currentView = newView;
		currentView.update(theGame);
		window.setCenter((Node) currentView);
	}
	
	private class MenuItemListener implements EventHandler<ActionEvent>{

		@Override
		public void handle(ActionEvent ae) {
			String text = ((MenuItem) ae.getSource()).getText();
			if (text.equals("New Game")) {
				theGame.startNewGame();
			} else if (text.equals("Text View")) {
				setViewTo(textAreaView);
			} else if (text.equals("Button View")) {
				setViewTo(buttonView);
			} else if (text.equals("Drawing View")) {
				System.out.println("View: Drawing View");
				setViewTo(drawingView);
			} else if (text.equals("Intermidate AI")){
				theGame.setComputerPlayerStrategy(interAI);
			} else if (text.equals("Random AI")) {
				theGame.setComputerPlayerStrategy(randAI);
			}
		}
		
	}




}