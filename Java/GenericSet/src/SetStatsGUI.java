
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;

import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.stage.Stage;

//Author: Cesar D. Quihuis-Romero
public class SetStatsGUI extends Application {

	public static void main(String[] args) {
		launch(args);
	}

	private MySet<Double> set = new MySet<Double>();
	private Label enterLabel = new Label("Enter a number");
	private TextField userField = new TextField();
	private Label maxLabel = new Label("Max: ");
	private Label maxValueLabel = new Label();
	private Label minLabel = new Label("Min: ");
	private Label minValueLabel = new Label();
	private Label avgLabel = new Label("Average: ");
	private Label avgValueLabel = new Label();
	private TextArea elements = new TextArea();
	private Double minValue;
	private Double maxValue;
	private Double avgValue;

	@Override

	public void start(Stage stage) {

		// The main entry point into this program:

		stage.setTitle("Set Stats");

		BorderPane window = new BorderPane();
		GridPane grid1 = new GridPane();
		GridPane grid2 = new GridPane();
		userField.setPrefWidth(150);

		Font boldFont = Font.font("Courier New", FontWeight.BOLD, 16);
		Font standardFont = Font.font("Courier New", 14);
		elements.setFont(boldFont);
		enterLabel.setFont(standardFont);
		maxLabel.setFont(standardFont);
		maxValueLabel.setFont(standardFont);
		minLabel.setFont(standardFont);
		minValueLabel.setFont(standardFont);
		avgLabel.setFont(standardFont);
		avgValueLabel.setFont(standardFont);

		grid1.setPadding(new Insets(10, 20, 10, 10));
		grid1.add(enterLabel, 0, 0);
		grid1.add(userField, 1, 0);

		grid2.setPadding(new Insets(10, 20, 10, 10));
		grid2.add(maxLabel, 0, 1);
		grid2.add(maxValueLabel, 1, 1);
		grid2.add(minLabel, 2, 1);
		grid2.add(minValueLabel, 3, 1);
		grid2.add(avgLabel, 4, 1);
		grid2.add(avgValueLabel, 5, 1);

		grid1.setVgap(10);
		grid1.setHgap(20);
		grid1.setGridLinesVisible(false);

		grid2.setVgap(10);
		grid2.setHgap(20);
		grid2.setGridLinesVisible(false);

		elements.setEditable(false);
		elements.setWrapText(true);

		userField.setOnAction(new TextFieldHandler());
		window.setTop(grid1);
		window.setCenter(grid2);
		window.setBottom(elements);
		Scene scene = new Scene(window, 600, 400);
		stage.setScene(scene);

		stage.show();

	}

	private class TextFieldHandler implements EventHandler<ActionEvent> {
		public void handle(ActionEvent ae) {
			String userInput = userField.getText();
			Boolean isNumberic = checkString(userInput);
			if (!isNumberic) {
				// throw error not valid input.
				String alertString = "Invalid number " + userInput;
				Alert error = new Alert(AlertType.NONE);
				error.setAlertType(AlertType.INFORMATION);
				error.setHeaderText(alertString);

				error.show();
			} else {
				Double inputAsDouble = Double.parseDouble(userInput);
				if (!set.add(inputAsDouble)) {
					String duplicateAlertString = "The set already contains " + userInput;
					Alert dupAlert = new Alert(AlertType.INFORMATION);
					dupAlert.setHeaderText(duplicateAlertString);
					dupAlert.showAndWait();
				} else {
					set.add(inputAsDouble);
				}
				set.add(inputAsDouble);
				userField.setText("");
				calcMin(set);
				calcMax(set);
				calcAvg(set);
				String minAsString = String.valueOf(minValue);
				String maxAsString = String.valueOf(maxValue);
				String avgAsString = String.valueOf(avgValue);
				minValueLabel.setText(minAsString);
				maxValueLabel.setText(maxAsString);
				avgValueLabel.setText(avgAsString);
				String setAsString = makeString(set);
				elements.setText(setAsString);

			}
		}

		private Boolean checkString(String userInput) {
			if (userInput == null) {
				return false;
			}
			try {
				@SuppressWarnings("unused")
				Double currValue = Double.parseDouble(userInput);
			} catch (NumberFormatException nfe) {
				return false;
			}
			return true;
		}

		private void calcMin(MySet<Double> set) {
			minValue = (Double) set.get(0);
			for (int index = 0; index < set.size(); index++) {
				Double currValue = (Double) set.get(index);
				if (currValue < minValue) {
					minValue = currValue;
				} else {
					continue;
				}
			}
		}

		private void calcMax(MySet<Double> set) {
			maxValue = (Double) set.get(0);
			for (int index = 0; index < set.size(); index++) {
				Double currValue = (Double) set.get(index);
				if (currValue > maxValue) {
					maxValue = currValue;
				} else {
					continue;
				}
			}
		}

		private void calcAvg(MySet<Double> set) {
			Double currSum = 0.0;
			for (int index = 0; index < set.size(); index++) {
				currSum += (Double) set.get(index);
			}
			avgValue = Math.round((currSum / set.size()) * 10.0) / 10.0;
		}

		private String makeString(MySet<Double> set) {
			String toString = "";
			for (int index = 0; index < set.size(); index++) {
				Double currValueAsDouble = (Double) set.get(index);
				String currValueAsString = String.valueOf(currValueAsDouble);
				toString += currValueAsString + " ";
			}
			return toString;
		}
	}

}
