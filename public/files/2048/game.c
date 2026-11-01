#include "game.h"


    /*

    We initialize all game variables that are used in game.h with their respective pointers and addresses 

    We also check to see if there are any legal moves. If there is then we continue. Else the code returns a 0 and game over :(


    Move Keys: 

    premise set counter and temp variables 
    go to current column / row depending on the move W / S checks col first then row & A / D checks row first then col. 
    The loop checks the first avaliable -1 entry (empty cell) which the first non -1 entry is then replaced into it. 
    Target variable is used to find the cell where any non-empty cells can go to 
    last_merged is to make sure that we don't do multiple merges at once. 
    Before it moves it it checks to see if there are two cells that are either on the same col / row (depending on the movement) and then it makes the new element with the updated value and then make the "less" priority cell be -1 to indicate that a merge has happened
    This keeps on going until we go through all elements in either row / column 
    After all changes have been made we have a flag variable to see if there is any change at all
    If there was then the movement key returns a 1
    else it returns a 0 meaning that there have been no changes whatsoever. 

    I really don't want to type this all out for every moveset but ill make comments to highlight the differences 

    */

game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    int i;  // initializes all the variables not initialized before 
    mygame->rows = rows; // set the game rows into a variable rows and cols and score 
    mygame->cols = cols; 
    mygame->score = 0;
    for (i = 0; i < rows*cols; i++){ // make all entries -1 
        mygame->cells[i]=-1;
    }

    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	 //YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
    int i; 
    (*_cur_game_ptr)->rows = new_rows; // reinitializes the variables when we remake the game 
    (*_cur_game_ptr)->cols = new_cols;
    (*_cur_game_ptr)->score = 0;
    for (i = 0; i < new_rows*new_cols; i++){ // set all entries to -1
        (*_cur_game_ptr)->cells[i] = -1;
    }

	return;	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
    if ( row < 0 || row >= cur_game->rows || col < 0 || col >= cur_game->cols){
        return NULL;
    }
    return &cur_game->cells[row * cur_game->cols + col];
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{
    //YOUR CODE STARTS HERE
    /*

    premise set counter and temp variables 
    go to current column / row depending on the move W / S checks col first then row & A / D checks row first then col. 
    The loop checks the first avaliable -1 entry (empty cell) which the first non -1 entry is then replaced into it. 
    Target variable is used to find the cell where any non-empty cells can go to 
    last_merged is to make sure that we don't do multiple merges at once. 
    Before it moves it it checks to see if there are two cells that are either on the same col / row (depending on the movement) and then it makes the new element with the updated value and then make the "less" priority cell be -1 to indicate that a merge has happened
    This keeps on going until we go through all elements in either row / column 
    After all changes have been made we have a flag variable to see if there is any change at all
    If there was then the movement key returns a 1
    else it returns a 0 meaning that there have been no changes whatsoever. 

    I really don't want to type this all out for every moveset but ill make comments to highlight the differences 

    */
    int target;
    int r, c;
    int last_merged;
    int diditchange = 0;
    for (c = 0; c < cur_game->cols; c++){
        last_merged = -1; 
        target = 0;
        for (r = 0; r < cur_game->rows; r++) { // since pressing w makes the tiles go up we want to check from top to bottom to see if there's any empty spots and replace the empty spots with the first appearance cell 
            if ( target > 0 && last_merged != target-1 && *get_cell(cur_game, r, c) != -1 && *get_cell(cur_game, r, c) == *get_cell(cur_game, target-1, c) ) { // Conditions to first see if there has already been a merge and to see the top and bottom row of the adjacent rows are equal
                *get_cell(cur_game, target-1, c) = 2 * *get_cell(cur_game, target-1, c); // double the cell of the top most
                cur_game->score = cur_game->score + *get_cell(cur_game, target-1, c); // add to score
                *get_cell(cur_game, r, c) = -1; // make the bottom cell empty 
                last_merged = target-1; // the location of the last merge so we don't do multiple merges 
                diditchange = 1; // change flag to show that this was a valid move
            } else if ( *get_cell(cur_game, r, c) != -1 ) { // If no merges happened 
                if (r != target){
                    *get_cell(cur_game, target, c) = *get_cell(cur_game, r, c); // Move any cells that have avaliable moving capabilities to the top
                    *get_cell(cur_game, r, c) = -1;  // bottom cell gets empty
                    diditchange = 1;  // change did occur 
                }
                target++;  // new pointer to find the next empty cell
            }
        }
    }
        

    return diditchange;
};

int move_s(game * cur_game) //slide down
{
    /* move_s: same as move_w, but slides down.
    Walk each column from bottom to top; target starts at the last row
    and counts down; the previous landed tile is at target+1.
    */

    //YOUR CODE STARTS HERE
    int target;
    int r, c;
    int last_merged;
    int diditchange = 0;
    for (c = 0; c < cur_game->cols; c++){
        last_merged = -1; 
        target = cur_game->rows-1;
        for (r = cur_game->rows - 1; r >= 0; r--) {
            if ( target < cur_game->rows-1 && last_merged != target+1 && *get_cell(cur_game, r, c) != -1 && *get_cell(cur_game, r, c) == *get_cell(cur_game, target+1, c) ) {
                *get_cell(cur_game, target+1, c) = 2 * *get_cell(cur_game, target+1, c);
                cur_game->score = cur_game->score + *get_cell(cur_game, target+1, c);
                *get_cell(cur_game, r, c) = -1;
                last_merged = target+1;
                diditchange = 1;
            } else if ( *get_cell(cur_game, r, c) != -1 ) {
                if (r != target){
                    *get_cell(cur_game, target, c) = *get_cell(cur_game, r, c);
                    *get_cell(cur_game, r, c) = -1; 
                    diditchange = 1; 
                }
                target--; 
            }
        }
    }
        

    return diditchange;
};

int move_a(game * cur_game) //slide left
{
    //YOUR CODE STARTS HERE
    /*
    instead of going from top to bottom or bottom to top we go left to right now
    for move_a we go from left to right; target starts at the left most row and then walks right
    */
    int target;
    int r, c;
    int last_merged;
    int diditchange = 0;
    for (r = 0; r < cur_game->rows; r++){
        last_merged = -1; 
        target = 0;
        for (c = 0; c < cur_game->cols; c++) {
            if ( target > 0 && last_merged != target-1 && *get_cell(cur_game, r, c) != -1 && *get_cell(cur_game, r, c) == *get_cell(cur_game, r, target -1) ) {
                *get_cell(cur_game, r, target -1) = 2 * *get_cell(cur_game, r, target -1);
                cur_game->score = cur_game->score + *get_cell(cur_game, r, target -1);
                *get_cell(cur_game, r, c) = -1;
                last_merged = target-1;
                diditchange = 1;
            } else if ( *get_cell(cur_game, r, c) != -1 ) {
                if (c != target){
                    *get_cell(cur_game, r, target) = *get_cell(cur_game, r, c);
                    *get_cell(cur_game, r, c) = -1; 
                    diditchange = 1; 
                }
                target++; 
            }
        }
    }
        

    return diditchange;
};

int move_d(game * cur_game)
{
    //YOUR CODE STARTS HERE
    /*
    Some thing as move_a but we go the opposite way now
    */
    int target;
    int r, c;
    int last_merged;
    int diditchange = 0;
    for (r = 0; r < cur_game->rows; r++){
        last_merged = -1; 
        target = cur_game->cols - 1;
        for (c = cur_game->cols-1; c >= 0; c--) {
            if ( target < cur_game->cols-1 && last_merged != target+1 && *get_cell(cur_game, r, c) != -1 && *get_cell(cur_game, r, c) == *get_cell(cur_game, r, target+1) ) {
                *get_cell(cur_game, r, target+1) = 2 * *get_cell(cur_game, r, target+1);
                cur_game->score = cur_game->score + *get_cell(cur_game, r, target+1);
                *get_cell(cur_game, r, c) = -1;
                last_merged = target+1;
                diditchange = 1;
            } else if ( *get_cell(cur_game, r, c) != -1 ) {
                if (c != target){
                    *get_cell(cur_game, r, target) = *get_cell(cur_game, r, c);
                    *get_cell(cur_game, r, c) = -1; 
                    diditchange = 1; 
                }
                target--; 
            }
        }
    }
        

    return diditchange;
};
int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    int r, c;
    // goes through every cell with the two for loops 
    for (r = 0; r < cur_game->rows ; r++){
        for (c = 0; c < cur_game->cols; c++){

            // we do not need 4 checks since we go through every cell as A can merge with B can be flagged as a legal move and we do not need to see the opposite interaction of if B can merge with A 
            if (*get_cell(cur_game, r, c) == -1){ // if there is an empty cell that means there is a legal move we can play 
                return 1;
            }
            cell * right = get_cell(cur_game, r, c+1); // Checks to see if the current cell at r,c and the cell to the right of it are the same, if so that's another legal move
            if (right != NULL && *right == *get_cell(cur_game, r,c)){
                return 1;
            }
            cell * below = get_cell(cur_game, r+1, c); // same thing but instead of checking to the right we check below 
            if (below != NULL && *below == *get_cell(cur_game, r,c)){
                return 1;
            }
            
        }
    }
    //YOUR CODE STARTS HERE

    return 0;
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
