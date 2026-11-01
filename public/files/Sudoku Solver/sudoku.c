#include "sudoku.h"
/*

this is a sudoku solving algorithm that checks to see if an input on the board is valid or not. If it is then it continues on, if not then it backtracks to the next value and checks again. 
It firsts see if there are any squares that need an input(any value of 0 or null) and then makes a guess starting from 1 then goes up to 9. If there is a guess, the program checks to see if there are any duplicates of values in the row, column, or 3x3 sector.
If so, then the program will reject the input and will move onto the next guess. 
If there are no valid guess for the current square, it undos the guesses of the current square and calls back to the previous square as a failure. In doing so the previous square gets a new number guess to try and solve the sudoku puzzle 

*/
//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) { // this checks to  see if the input value already exists in the current row  

  assert(i>=0 && i<9);

  // BEG TODO
  int j;
  for (j = 0; j < 9; j++){ // loop that iterates in the row 
    if (sudoku[i][j] == val){ // compares the current square with val 
      return 1; // if there is a match returns 1 
    }
  }
  return 0;
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);
  
  // BEG TODO
  int i; 
  for (i = 0; i < 9; i++){ // loop that iterates in the col 
    if (sudoku[i][j] == val){// compares the current square with val 
      return 1;// if there is a match returns 1 
    }
  }
  return 0;
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
   
  assert(i>=0 && i<9);
  // BEG TODO
  int rowcent = ((i/3)*3)+1; // rowcent and colcent is there to find the center of each 3x3 group the decimal points are sheared off and we add one for center offset
  int colcent = ((j/3)*3)+1;
  int r,c; 
  for (r = rowcent - 1; r <= rowcent + 1; r++){ // checks the surrounding cells 
    for (c = colcent - 1; c <= colcent + 1; c++){
      if(sudoku[r][c] == val){ // if there is a match
        return 1; // returns 1 meaning we have a match
      }
    }
  }
  // END TODO
  return 0;
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);
  // BEG TODO
  return !is_val_in_3x3_zone(val, i, j, sudoku) && !is_val_in_col(val, j, sudoku) && !is_val_in_row(val, i, sudoku); // because each function call returns 1 if there is a match then if all returns 1 and we not then we get 0s which would mean that we would return a 0
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) { // find the first empty cell, then try 1 to 9 in it, recursing on each valid guess.
  int i, j; // initializing counters and variables 
  int found = 0;
  int num; 
  // BEG TODO.
  for (i = 0; i < 9; i++){ // checks the cells on the board 
    for (j = 0; j<9; j++){
      if (sudoku[i][j] == 0){ // checks to see if a cell is 0 
        found = 1; // found flag to show that there is a 0 
        break; 
      }
    }
    if (found) {
      break; // get out of the for loop and continue to the next part of the program of the solver and valid checker 
    }
  }
  if (found == 0){
    return 1; // all cells are filled with valid guesses and means that the board is solved 
  }


  for (num = 1; num <= 9; num++){ // checks the valid inputs 
    if (is_val_valid(num, i, j, sudoku)){ // if the guess is valid 
      sudoku[i][j] = num;  // set the value inside the cell 
      if (solve_sudoku(sudoku)){ // see if the guess is valid 
        return 1; // if so we move on to the next 0 square 
      }
      sudoku[i][j] = 0;  // if not we set the current square to 0, there was no valid guess so that means that the previous entry must be wrong 
    }
  }
  return 0; // no number fits here: report failure so the caller backtracksgit add
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}





