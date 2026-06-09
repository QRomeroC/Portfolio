// Choice is an enum to be used in class GameTree as an argument to a method
//
//   public void playerSelected(Choice yesOrNo) 
// 
// This helps avoid problems when a string or char are used with issues like
// case sensitivity. // Sample code:
// 
//  Choice userSelection = Choice.Yes;
//
//  if(yesOrNo == Choice.YES) {
//     // This is true here
//  }
// 
// @author Rick Mercer
//
package model;


public enum Choice {
  YES, NO
};
