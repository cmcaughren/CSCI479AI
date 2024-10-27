#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <utility>
#include <time.h>
#include "A3.h"

Game::Game(Player* A, Player* B){
    to_move_m = A;
    players_m[A->index] = A;
    players_m[B->index] = B;
    Goal_m = Goal;
    Rows_m = Rows;
    Columns_m = Columns;

    state_m = new State;
    state_m->board = new char*[Rows];
    for (int i = 0; i < Rows; i++){
        state_m->board[i] = new char[Columns];
        for (int j = 0; j < Columns; j++){
            state_m->board[i][j] = Filling;
        }
    }
    state_m->top = new int[Columns];
    for (int i = 0; i < Columns_m; i++){
        state_m->top[i] = 0;
    }
    state_m->to_move_next = A;
    displayBoard();

}

Game::~Game(){
   for (int i = 0; i < Rows; i++){
      delete[] state_m->board[i];
   }
   delete[] state_m->board;
   delete[] state_m->top;
   delete state_m;

}

void Game::displayBoard(){

    std::cout << std::endl;
    for(int i = Rows_m-1; i >= 0; i--) {
        std::cout << "-";
        for(int j = 0; j < Columns_m; j++)
            std::cout << "----";
        std::cout << std::endl;
        std::cout << "|";
        for(int j = 0; j < Columns_m; j++)
            std::cout << " " << state_m->board[i][j] << " |";
        std::cout << std::endl;
    }
    std::cout << "-";
    for(int j = 0; j < Columns_m; j++)
        std::cout << "----" ;
    std::cout << std::endl;
    std::cout << " ";
    for(int j = 0; j < Columns_m; j++)
        std::cout << std::setw(2) << j + 1 << "  ";
    std::cout << std::endl;

}

//returns a pointer to the player whose turn it is currently
Player* Game::to_Move(){
    return to_move_m;
}

//rotate to_move to the next player
void Game::Next_Player(){
    to_move_m = players_m[((to_move_m->index + 1) % Players)];
}

//updates game state, adds marble symbol to column
void Game::Update_Game_State(int column, char Marble){
    int position = state_m->top[column];
    state_m->board[position][column] = Marble;
    state_m->top[column]++;
}

//returns an int representing the column that to_move Player should drop their marble to
int Game::Alpha_Beta_Search(State* state){

    int depth = 0;

    std::pair<long int, long int> move = Max_Value(state, depth, NEG_INFINITY, POS_INFINITY);

    return move.first;
}

std::pair<long int, long int> Game::Max_Value(State* state, int depth, long int alpha, long int beta){
    if ( (depth == max_Search_Depth) || is_Terminal(state) ){
        long int utility = Utility(state);
        std::pair<long int, long int> cut_off;
        cut_off.first = -1;
        cut_off.second = utility;
        return cut_off;
    }
    
    std::pair<long int, long int> best_move = {NEG_INFINITY,NEG_INFINITY};

    std::vector<int>* actions = Actions(state);
    for ( int i = 0; i < actions->size(); i++){

        State* resulting_state = Result(state, (*actions)[i]);

        std::pair<long int, long int> temp = Min_Value(resulting_state, depth+1, alpha, beta);

        if (temp.second > best_move.second){
            best_move.first = (*actions)[i];
            best_move.second = temp.second;
            
            if (alpha < best_move.second){
                alpha = best_move.second;
            }
        }
        if (best_move.second >= beta){
            return best_move;
        }
        //delete the temporary state used
        for (int i = 0; i < Rows; i++){
            delete[] resulting_state->board[i];
        }
        delete[] resulting_state->board;
        delete[] resulting_state->top;
        delete resulting_state;
    }

    //delete actions vector
    actions->erase(actions->begin(), actions->end());
    actions->shrink_to_fit();

    return best_move;

}

std::pair<long int, long int> Game::Min_Value(State* state, int depth, long int alpha, long int beta){
    if ( (depth == max_Search_Depth) || is_Terminal(state) ){
        long int utility = Utility(state);
        std::pair<long int, long int> cut_off;
        cut_off.first = -1;
        cut_off.second = utility;
        return cut_off;
    }
    
    std::pair<long int, long int> best_move = {POS_INFINITY,POS_INFINITY};

    std::vector<int>* actions = Actions(state);
    for ( int i = 0; i < actions->size(); i++){

        State* resulting_state = Result(state, (*actions)[i]);

        std::pair<long int, long int> temp = Max_Value(resulting_state, depth+1, alpha, beta);

        if (temp.second < best_move.second){
            best_move.first = (*actions)[i];
            best_move.second = temp.second;
            
            if (beta > best_move.second){
                beta = best_move.second;
            }
        }
        if (best_move.second <= alpha){
            return best_move;
        }
        //delete the temporary state used
        for (int i = 0; i < Rows; i++){
            delete[] resulting_state->board[i];
        }
        delete[] resulting_state->board;
        delete[] resulting_state->top;
        delete resulting_state;
    }

    //delete actions vector
    actions->erase(actions->begin(), actions->end());
    actions->shrink_to_fit();
    
    return best_move;

}
//returns true if the board in state is completely full
bool Game::is_Terminal(State* state){
    bool full = true;
    for (int i = 0; i < Columns; i++){
         if (state->top[i] != Rows) {
            full = false;
         }
    }
    return full;
}
//returns a numeric value score of this state, for player to_move
//higher value for lines of to_move player
//lower value for lines of symbols not to_move players
//no change in score for lines of mixed symbols 
//operates as a "eval" function when called on a non-terminal state
//see report for more details about the values awarded for what rows 
long int Game::Utility(State* state){

    char marble = state->to_move_next->myMarble;
    long int total_score = 0;

    //calculate the score of each row in the board
    for (int r = 0; r < Rows; r ++){
        for (int c = 0; c < Columns - 3 ; c++){
            total_score += row_score_check(state, r, c, marble);
        }
    }

    //calculate the score of each column in the board
    for (int c = 0; c < Columns; c ++){
        for (int r = 0; r < Rows - 3; r++){
            total_score += col_score_check(state, r, c, marble);
        }
    }

    //calculate the score of each diagonal in the board 
    for (int r = 0; r < 3; r++){
        for (int c = 0; c < 4; c++){
            total_score += diagonal_down_score_check(state, r, c, marble);
        }
    }

    for (int r = 3; r < Rows; r++){
        for (int c = 0; c < 4; c++){
            total_score += diagonal_up_score_check(state, r, c, marble);
        }
    }
    return total_score;
}

long int Game::row_score_check(State* state, int row, int col, char marble){
    
    long int row_score = 0;
    bool break_in_segment = false;
    
    int my_pieces = 0;
    int op_pieces = 0;
    int open_spots = 0;

    //count how many of my pieces, opponents pieces, empty spots are in this 4 square section
    for (int j = col; j < col + 4; j++ ){
        
        if ((row != 0) && ((state->top[j]) < row)){
            break_in_segment = true;
            break;
        }
        else if(state->board[row][j] == marble){
            my_pieces++;
        }
        else if(state->board[row][j] == Filling){
            open_spots++;
        }
        else { //its the opponents symbol
            op_pieces++;
        }
    }
    if (!break_in_segment){
        //calculate score based on the number of each type of symbol
        row_score += calculate_score(my_pieces, op_pieces, open_spots);
        //else, there is a hole preventing this row segment from being completed, it is worth 0
    }
    
    return row_score;
}

long int Game::col_score_check(State* state, int row, int col, char marble){
    long int col_score = 0;
   
    int my_pieces = 0;
    int op_pieces = 0;
    int open_spots = 0;
    
    //count how many of my pieces, opponents pieces, empty spots are in this 4 square section
    for (int j = row; j < row + 4; j++ ){
            
        if(state->board[j][col] == marble){
            my_pieces++;
        }
        else if(state->board[j][col] == Filling){
            open_spots++;
        }
        else { //its the opponents symbol
            op_pieces++;
        }
    }
    //calculate score based on the number of each type of symbol
    col_score += calculate_score(my_pieces, op_pieces, open_spots);
    
    return col_score;
}

long int Game::diagonal_down_score_check(State* state, int row, int col, char marble){
    
    bool break_in_segment = false;
    int my_pieces = 0;
    int op_pieces = 0;
    int open_spots = 0;

    //count how many of my pieces, opponents pieces, empty spots are in this 4 square section
    for (int i = 0; i < Goal; i++){

        if (((row + i) != 0) && (state->top[col + i] < (row + i))){
            break_in_segment = true;
                break;
        }
        else if(state->board[row + i][col + i] == marble){
            my_pieces++;
        }
        else if(state->board[row + i][col + i] == Filling){
            open_spots++;
        }
        else { //its the oppoents symbol
            op_pieces++;
        }
    } 
    if (!break_in_segment){    
        //calculate score based on the number of each type of symbol
        return calculate_score(my_pieces, op_pieces, open_spots);
            
    }
    //else, there is a hole preventing this diagonal segment from being completed, it is worth 0
    return 0;
    
}

long int Game::diagonal_up_score_check(State* state, int row, int col, char marble){
    
    bool break_in_segment = false;
    int my_pieces = 0;
    int op_pieces = 0;
    int open_spots = 0;

    //count how many of my pieces, opponents pieces, empty spots are in this 4 square section
    for (int i = 0; i < Goal; i++){

        if (((row - i) != 0) && (state->top[col + i] < (row - i))){
            break_in_segment = true;
            break;
        }
        if(state->board[row - i][col + i] == marble){
            my_pieces++;
        }
        else if(state->board[row - i][col + i] == Filling){
            open_spots++;
        }
        else { //its the oppoents symbol
            op_pieces++;
        }
    } 
    if (!break_in_segment){    
        //calculate score based on the number of each type of symbol
        return calculate_score(my_pieces, op_pieces, open_spots);
    }
    //else, there is a hole preventing this diagonal segment from being completed, it is worth 0
    return 0;
    
}

long int Game::calculate_score(int my_pieces, int op_pieces, int open_spots){
    //calculate score based on the number of each type of symbol
    if (my_pieces == 4) {
        return 1000001;
    }
    else if (op_pieces == 4) {
        return -1000000;
    }
    else if (my_pieces == 3 && open_spots == 1){
        return 10000;
    }
    else if (op_pieces == 3 && open_spots == 1){
        return -10001;
    }
    else if (my_pieces == 2 && open_spots == 2){
        return 100;
    }
    else if (op_pieces == 2 && open_spots == 2){
        return -101;
    }
    return 0;

}
//returns a vector of ints, all columns with an open space in state's board
std::vector<int>* Game::Actions(State* state){
    
    std::vector<int>* actions = new std::vector<int>;
    for (int i = 0; i < Columns; i++){
         if (state->top[i] < Rows ){
            actions->push_back(i);
         }
    }
    return actions;

}
//returns a pointer to a dynamically allocated new state, which is made from
//prev_state but with an additional symbol (that of player to_move's) in column number Action
State* Game::Result(State* prev_state, int action){
    
    //create a copy of the prev_state board
    State* new_state = new State;
    new_state->board = new char*[Rows];
    for (int i = 0; i < Rows; i++){
        new_state->board[i] = new char[Columns];
        for (int j = 0; j < Columns; j++){
            new_state->board[i][j] = prev_state->board[i][j];
        }
    }
    //create a copy of the prev_states top
    new_state->top = new int[Columns];
    for(int i = 0; i < Columns_m; i++){
        new_state->top[i] = prev_state->top[i];
    }
    new_state->to_move_next = prev_state->to_move_next;

    //add the additional marble to the column indicated by action
    int position = new_state->top[action];
    new_state->board[position][action] = new_state->to_move_next->myMarble;
    new_state->top[action]++;

    //rotate whose turn it is to play next for this new_state
    int index = new_state->to_move_next->index;
    index = (index + 1) % Players; 
    new_state->to_move_next = players_m[index];

    return new_state;

}
//select a random column from available columns for player to place their symbol
int Game::RAN_move(State* state){

    
    int count = 0;
    for (int i = 0; i < Columns; i++){
         if (state->top[i] < Rows -1 ) count ++;
    }

    int index = rand() % count;
    for (int col = 0; col < Columns; col ++) {
        if (state->top[col] < Rows) index--;
        
        if (index < 0) {
             return col;
          }
    }
    return -1;
}

State* Game::get_state(){
    return state_m;
}

int Game::is_gameover(State* state){
 
    int player_id = to_move_m->index;
    int opponent_id = players_m[((player_id + 1) % Players)]->index;

    long int winning_score_player = 1000001;
    long int winning_score_opponent = -1000000;
    long int segment_score = 0;

    //calculate the score of each row in the board
    for (int r = 0; r < Rows; r ++){
        for (int c = 0; c < Columns - 3 ; c++){
            
            segment_score = row_score_check(state, r, c, to_move_m->myMarble);
            if (segment_score == winning_score_player){
                return player_id;
            }
            else if (segment_score == winning_score_opponent){
                return opponent_id;
            }
        }
    }

    //calculate the score of each column in the board
    for (int c = 0; c < Columns; c ++){
        for (int r = 0; r < Rows - 3; r++){
            segment_score = col_score_check(state, r, c, to_move_m->myMarble);
            if (segment_score == winning_score_player){
                return player_id;
            }
            else if (segment_score == winning_score_opponent){
                return opponent_id;
            }
        }
    }

    //calculate the score of each diagonal in the board 
    for (int r = 0; r < 3; r++){
        for (int c = 0; c < 4; c++){
            segment_score = diagonal_down_score_check(state, r, c, to_move_m->myMarble);
            if (segment_score == winning_score_player){
                return player_id;
            }
            else if (segment_score == winning_score_opponent){
                return opponent_id;
            }
        }
    }

    for (int r = 3; r < Rows; r++){
        for (int c = 0; c < 4; c++){
            segment_score = diagonal_up_score_check(state, r, c, to_move_m->myMarble);
            if (segment_score == winning_score_player){
                return player_id;
            }
            else if (segment_score == winning_score_opponent){
                return opponent_id;
            }
        }
    }
    //not game over
    return -1;

}