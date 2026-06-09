package views_controllers;

import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Label;
import javafx.scene.layout.BorderPane;
import javafx.scene.paint.Color;
import model.OurObserver;
import model.TicTacToeGame;

public class DrawingView extends BorderPane implements OurObserver{
	
	private TicTacToeGame theGame;
	private Canvas canvas = new Canvas(100, 100);
	private Label label = new Label("Canvas");
	
	public DrawingView(TicTacToeGame theModel) {
		theGame = theModel;
		initializePanel();
	}
	
	public void initializePanel() {
		GraphicsContext gc = canvas.getGraphicsContext2D();
	
		gc.strokeLine(50, 50, 100, 100);
		gc.setLineWidth(3);
		gc.setStroke(Color.RED);
		gc.strokeRect(0,0,100,100);
		
		this.setCenter(canvas);
		this.setBottom(label);
	}
	
	@Override
	public void update(Object theObserved) {
		// TODO Auto-generated method stub
		
	}

}
