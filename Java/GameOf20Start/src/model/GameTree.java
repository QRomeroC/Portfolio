package model;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

// A model for the game of 20 questions. This type can be used to
// build a console based game of 20 questions or a GUI based game.
//
// @author Rick Mercer and Cesar D. Quihuis-Romero
//
@SuppressWarnings("unused")
public class GameTree {

	// BinaryTreeNode inner class used to create new nodes in the GameTree.
	private class TreeNode {

		// Instance variables
		private String data;
		private TreeNode left;
		private TreeNode right;

		TreeNode(String theData) {
			data = theData;
			left = null;
			right = null;
		}

		// This 2nd constructor is needed in a few methods, like privste build()
		TreeNode(String theData, TreeNode leftLink, TreeNode rightLink) {
			data = theData;
			left = leftLink;
			right = rightLink;
		}

		public String getData() {
			return data;
		}
		public void setData(String newData) {
			data = newData;
		}
		public TreeNode getLeft() {
			return left;
		}

		public void setLeft(TreeNode newNode) {
			left = newNode;
		}
		@SuppressWarnings("unused")
		public TreeNode getRight() {
			return right;
		}
		public void setRight(TreeNode newNode) {
			right = newNode;
		}
	}

	// Instance variables
	private TreeNode root;
	private TreeNode currentNode;
	private Scanner scanner;

	private String fileName;

	// Constructor needed to create the game. It should open the input
	// file and call the recursive method build(). The String parameter
	// name is the name of the file from which we need to read the game
	// questions and answers from.
	//
	public GameTree(String name) {
		fileName = name;
		try {
			scanner = new Scanner(new File(fileName));
		} catch (FileNotFoundException e) {
			System.out.println("File not found: '" + fileName + "'");
		}
		root = build();
		currentNode = root;
		scanner.close();
	}

	// Build a GameTree in preorder fashion and return the root of the tree
	private TreeNode build() {
		if (!scanner.hasNext()) {
			return null;
		} else {
			String token = scanner.nextLine();
			if (!token.endsWith("?")) {
				return new TreeNode(token);
			} else {
				TreeNode leftSubtree = build();
				TreeNode rightSubtree = build();
				return new TreeNode(token, leftSubtree, rightSubtree);
			}
		}
	}

	// Method used to print out a text version of the game file
	// in a sideways order fashion
	// sideways order root, all children to root, all children to children, etc.
	@Override
	public String toString() {
		int depth = 0;
		accumulate = "\n";
		return addUp(root, depth);
	}

	String accumulate;

	private String addUp(TreeNode tree, int depth) {
//		System.out.println("currDepth: " + depth);
		if (tree != null) {
			addUp(tree.right, depth + 1);
			for (int num = 1; num <= depth; num++) {
				accumulate += "- ";
			}
			accumulate += tree.data + "\n";
			addUp(tree.left, depth + 1);
		}
		return accumulate;
	}

	// Add a new question and answer to the currentNode. If the currentNode
	// is referencing the answer "parrot", theGame.add("Does it swim?", "duck");
	// should change the GameTree on the left to the GameTree on the right:
	//
	// Feathers?                          Feathers?
	//      / \                              /      \
	// parrot horse               Does it swim?    horse
	//                                 /              \
	//                             duck           parrot
	//
	// @param newQuestion: The question to add where the old answer was.
	// @param newAnswer: The new yes answer to the new question.
	//
	// Precondition: newQuestion.endsWith("?")
	//
	public void add(String newQuestion, String newAnswer) {
		TreeNode newAnswerNode = new TreeNode(newAnswer);
		TreeNode currNode = currentNode;
		String currNodeData = currNode.getData();
		TreeNode oldAnswerNode = new TreeNode(currNodeData);
		currNode.setData(newQuestion);
		currNode.setLeft(newAnswerNode);
		currNode.setRight(oldAnswerNode);
	}

	// Return true if getCurrent() is an answer rather than a question. Return false
	// if the current node is an internal node rather than a leaf that is an answer.
	public boolean foundAnswer() {
		return !currentNode.getData().endsWith("?");
	}

	// Return the data for the current node,
	// which could be a question or an answer.
	public String getCurrent() {
		return currentNode.getData();
	}

	// Ask the game to update the current node in the tree by
	// going left for Choice.yes or right for Choice.no
	// Example code:
	// theGame.playerSelected(Choice.Yes);
	//
	public void playerSelected(Choice yesOrNo) {
		if (yesOrNo == Choice.YES) {
			currentNode = currentNode.left;
		} else {
			currentNode = currentNode.right;
		}
	}

	// Begin a game at the root of the tree. getCurrent should return the question
	// at the root of this GameTree.
	public void reStart() {
		currentNode = root;
	}

	// Overwrite the old file for this gameTree with the current state that
	// may have new questions added since the game started. Get all other
	// method workings. Complete this method last.
	public void saveGame() {
		TreeNode currNode = root;
		String outputFileName = fileName;
		FileWriter charToBytesWriter = null;
		try {
			charToBytesWriter = new FileWriter(outputFileName);
			String preOrder = this.preOrder(currNode);
			Scanner line = new Scanner(preOrder);
			//while loop???
			while (line.hasNext()) {
				charToBytesWriter.write(line.nextLine());
				charToBytesWriter.write("\n");
			}
			line.close();
			charToBytesWriter.close();
		} catch (IOException ioe){
			System.out.println("Could not create the new file: " + ioe);
		}
	
	}
	public String callToPreorder() {
		TreeNode currNode = root;
		String preOrder = this.preOrder(currNode);
		return preOrder;
		
	}
	String preOrderString = "";
	private String preOrder(TreeNode currNode) {
		if (currNode != null) {
			preOrderString += currNode.data + "\n";
			this.preOrder(currNode.left);
			this.preOrder(currNode.right);
		}
		return preOrderString;
	
	}
}