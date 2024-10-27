#pragma once
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <map>

const long int POS_INFINITY = 2147483647;
const long int NEG_INFINITY = -2147483648;

const char Filling = ' ';
const int Columns = 7;
const int Rows = 6;
const int Goal = 4;
const int Players = 2;
const int max_Search_Depth = 6; //ply depth

enum Status {Win, Draw, Continue, Cheating};

const char playerMarble[Players] = {'X', 'O'};

struct Player{
   std::string name; 
   int index;
   char myMarble;
};

struct State{
   char** board;
   int* top;
   Player* to_move_next;
};

class Game{

private:
   State* state_m;
   Player* to_move_m; //Player whose move it is currently
   int Rows_m;
   int Columns_m;
   int Goal_m;
   Player* players_m[Players];

public:
   //initializes Player pointer array players[], set Rows, Columns, Goal to the
   //global values, create a blank initial state using new 
   //let player whose move it is first be the first Player passed as a parameter
   Game(Player* A, Player* B);
   //delete the game state 
   ~Game();

   void displayBoard();
   //returns a pointer to the player whose turn it is currently
   Player* to_Move();
   //rotate to_move to the next player
   void Next_Player();
   //updates game state, adds marble symbol to column
   void Update_Game_State(int column, char Marble);
   //returns an int representing the column that to_move Player should drop their marble to
   int Alpha_Beta_Search(State* state);
   
   std::pair<long int, long int> Max_Value(State* state, int depth, long int alpha, long int beta);
   std::pair<long int,long int> Min_Value(State* state, int depth, long int alpha, long int beta);

   //returns true if the board in state is completely full
   bool is_Terminal(State* state);

   //returns a numeric value score of this state, for player to_move
   //higher value for lines of to_move player
   //lower value for lines of symbols not to_move players
   //no change in score for lines of mixed symbols 
   //operates as a "eval" function when called on a non-terminal state
   long int Utility(State* state);
   long int row_score_check(State* state, int row, int col, char marble);
   long int col_score_check(State* state, int row, int col, char marble);
   long int diagonal_down_score_check(State* state, int row, int col, char marble);
   long int diagonal_up_score_check(State* state, int row, int col, char marble);
   long int calculate_score(int my_pieces, int op_pieces, int open_spots);

   //returns a vector of ints, all columns with an open space in state's board
   std::vector<int>* Actions(State* state);

   //returns a pointer to a dynamically allocated new state, which is made from
   //prev_state but with an additional symbol (that of player to_move's) in column number Action
   State* Result(State* prev_state, int action);

   //select a random column from Actions for to_move player to place thier symbol
   int RAN_move(State* state);

   State* get_state();

   //returns player id of winner, or -1 if no winner (may be a tie, need to check is_Terminal also)
   int is_gameover(State* state);

};