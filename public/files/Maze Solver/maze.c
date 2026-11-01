#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
/*
This code reads the maze files and finds the location of the start and end points 
it then creates space in the memory for the maze, the cells, and the elements in the cell 
the program then finds the start and end of the maze 

this program also frees up the memory when we destroy a maze by first freeing the elements inside each cell, then the cell, then the maze itself as well

It also prints the maze which just reads the elements in each cell and just prints it out

The recursion of DFS first checks to see which turns are valid by checking left, up, right then down. It marks cells that it has visited to make sure that it does not travel twice. 
This program has a found flag where it returns 1 if END has been found. else it returns 0. 
If the program goes throgh all possible options and FOUND is still 0 it goes back to the previous visited cell and checks any other paths. it keeps on going until it reaches a cell that has a different path. 
Keeps on going until it either finds the END or returns that there is no viable solution. 

When visiting other cells it makes sure that the path travelled does not go through S twice. The program acts as if S was a wall that it cannot pass through. 

Any backtracking replaces the * (marking that it has been travelled) to a ~ (meaning that it is an open path)

*/

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    FILE * test = fopen(fileName, "r"); // open the file 
    maze_t * maze = malloc(sizeof(maze_t)); // create memory space for maze 
    fscanf(test, "%d %d", &maze->width ,&maze->height); // get the size of the maze 
    fgetc(test);  // ignores new line 

    maze->cells = malloc(sizeof(char *) * maze->height); // create the size of each cell 

    for (int i = 0; i < maze->height; i++){
        maze->cells[i] = malloc(sizeof(char) * maze->width); // create teh size of the element inside each cell 

    }



    for (int r = 0; r < maze->height; r++){ // row counter
        for (int c = 0; c < maze->width; c++){ // col counter 
            maze->cells[r][c] = fgetc(test); // reads the character inside the file 
            if (maze->cells[r][c] == START){ // checks to see if cell is START
                maze->startRow = r; // saves the location of the cell
                maze->startColumn = c;
            }
            if (maze->cells[r][c] == END){ // checks to see if cell is END
                maze->endRow = r; // saves the location of the cell
                maze->endColumn = c;
            }
        }
        fgetc(test); // new row so we skip to a new line 
    }
    fclose(test); // close the file 
    // Your code here. Make sure to replace following line with your own code.
    return maze;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    for (int r = 0; r < maze->height; r++){
        free(maze->cells[r]); // free all elements in the cells inside maze
    }
    free(maze->cells); // frees all the cells 
    free(maze); // frees the maze 
    // Your code here.
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze) // we just print the maze here
{
    for (int r = 0 ; r < maze->height; r++){ // row counter
        for (int c = 0 ; c < maze->width; c++){ // column counter 
            printf("%c", maze->cells[r][c]); // print the character in side the cell
        }
        printf("\n"); // new line if we reach the end of a column 
    }
    // Your code here.
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{
    // The provided gold files recursively visit neighbors in the order
    // left, up, right, down. It is okay to deviate from this if you'd 
    // like, but your outputs will be different.

    // we are outside the maze
    if (col < 0 || row < 0 || row > maze->height - 1 || col > maze->width - 1)
    {
        return 0;
    }

    // we reached the end
    if (maze->cells[row][col] == END){
        return 1;
    }

    // Base case: wall, path, or visited cell (S is allowed through)
    if (maze->cells[row][col] != EMPTY && maze->cells[row][col] != START){
        return 0;
    }

    // Mark this cell as part of the path (this also blocks neighbors from stepping back onto it, including S)
    maze->cells[row][col] = PATH;

    int found = 0; // setting a found flag for recursion 

    // Try left, up, right, down; stops at the first success
    if (solveMazeDFS(maze, col-1, row)){ // we see if left is open if it is open then it keeps on going until it reaches the end or a deadend, then all the path up to it will return a 0 until there is a cell that has a different option in which it will take
        found = 1;
    } 
    else if (solveMazeDFS(maze, col, row-1)){ // we see if up is open if it is open then it keeps on going until it reaches the end or a deadend, then all the path up to it will return a 0 until there is a cell that has a different option in which it will take
        found = 1;
    }
    else if (solveMazeDFS(maze, col+1, row)){ // we see if right is open if it is open then it keeps on going until it reaches the end or a deadend, then all the path up to it will return a 0 until there is a cell that has a different option in which it will take
        found = 1;
    }
    else if (solveMazeDFS(maze, col, row+1)){ // we see if down is open if it is open then it keeps on going until it reaches the end or a deadend, then all the path up to it will return a 0 until there is a cell that has a different option in which it will take
        found = 1;
    }

    // Dead end: backtrack and mark as visited
    if (!found){
        maze->cells[row][col] = VISITED;
    }

    // Put S back if this is the start cell to make sure that we do not go back to S
    if (row == maze->startRow && col == maze->startColumn){
        maze->cells[row][col] = START;
    }

    return found;
}