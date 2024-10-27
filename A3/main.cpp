#include <iomanip>
#include "A3.h"

void AI_versus_AI();
void AI_versus_RAND();
void AI_versus_HUMAN();

int main(){
   
   srand(time(0));

   std::cout << "=== CONNECT FOUR ===" << std::endl;
   std::cout << "This application is an Intelligent Game Playing Agent that can " << std::endl;
   std::cout << "play the game 'Connect Four'. It can play against another AI Player," << std::endl;
   std::cout << "a RANDOM Player (selects moves at random, no AI), or against a human." << std::endl;
   std::cout << "Enter 'a' for the AI to play against another AI .... " << std::endl;
   std::cout << "Enter 'b' for the AI to play a RANDOM Player .... " << std::endl;
   std::cout << "Enter 'c' for the AI to play against you.... " << std::endl;


   std::string user_input = " ";
   while (user_input == " "){
      std::getline (std::cin,user_input);
      if ((user_input != "a") && (user_input != "b") && (user_input != "c")){
         std::cout << "That was an invalid entry please try again." << std::endl;
         user_input = " ";
      }
   }

   if (user_input == "a"){
      std::cout << "== AI VERSUS AI ==" << std::endl;
      AI_versus_AI();
   }
   else if (user_input == "b"){
      std::cout << "== AI VERSUS RANPLAYER ==" << std::endl;
      AI_versus_RAND();
   }
   else if (user_input == "c" ){
      std::cout << "== AI VERSUS YOU ==" << std::endl;
      AI_versus_HUMAN();
   }
  
}

void AI_versus_AI(){
   
   Player* player_A = new Player;
   player_A->name = "A";
   player_A->index = 0;
   player_A->myMarble = playerMarble[0];
   Player* player_B = new Player;
   player_B->name = "B";
   player_B->index = 1;
   player_B->myMarble = playerMarble[1];

   Game game(player_A, player_B);

   while (game.is_gameover(game.get_state()) == -1){
      if (game.is_Terminal(game.get_state())){
         std::cout << "GAME OVER: Tie Game!" << std::endl;
         break;
      }
      std::cout << "AI Player " << game.to_Move()->name << " moves: " << std::endl;
      int column = game.Alpha_Beta_Search(game.get_state());
      game.Update_Game_State(column, game.to_Move()->myMarble);
      game.Next_Player();
      game.displayBoard();
      std::cout << std::endl;
   }

   //-1 indicates a tie, output already given. else winner will be player index
   int winner = game.is_gameover(game.get_state());
   if (winner != -1){
      if (game.to_Move()->index == winner){
         std::cout << "GAME OVER: AI Player " << game.to_Move()->name << " wins!" << std::endl;
         std::cout << "Marble: " << game.to_Move()->myMarble << std::endl;
      }
      else {
         game.Next_Player();
         std::cout << "GAME OVER: AI Player " << game.to_Move()->name << " wins!" << std::endl; 
         std::cout << "Marble: " << game.to_Move()->myMarble << std::endl;
      }
   }

   delete player_A;
   delete player_B;

}

void AI_versus_RAND(){

   Player* player_A = new Player;
   player_A->name = "A";
   player_A->index = 0;
   player_A->myMarble = playerMarble[0];
   Player* player_B = new Player;
   player_B->name = "B";
   player_B->index = 1;
   player_B->myMarble = playerMarble[1];

   Game game(player_A, player_B);

   while (game.is_gameover(game.get_state()) == -1){
      
      if (game.is_Terminal(game.get_state())){
         game.displayBoard();
         std::cout << "GAME OVER: Tie Game!" << std::endl;
         break;
      }

      std::cout << "AI Player moves: " << std::endl;
      int column = game.Alpha_Beta_Search(game.get_state());
      game.Update_Game_State(column, game.to_Move()->myMarble);
      game.Next_Player();
      game.displayBoard();
      std::cout << std::endl;


      if (game.is_gameover(game.get_state()) != -1){
         break;
      }
      if (game.is_Terminal(game.get_state())){
         game.displayBoard();
         std::cout << "GAME OVER: Tie Game!" << std::endl;
         break;
      }

      std::cout << "RANDOM Player moves: " << std::endl;
      column = game.RAN_move(game.get_state());
      game.Update_Game_State(column, game.to_Move()->myMarble);
      game.Next_Player();
      game.displayBoard();
      std::cout << std::endl;
   }

   int winner = game.is_gameover(game.get_state());
   if (winner == 0){
      std::cout << "GAME OVER: AI Player wins! Marble 'X'" << std::endl;
   }
   else {
      std::cout << "GAME OVER: RAND Player wins! Marble 'O'" << std::endl;
   }

   delete player_A;
   delete player_B;

}

void AI_versus_HUMAN(){
  
   Player* player_A = new Player;
   player_A->name = "A";
   player_A->index = 0;
   player_A->myMarble = playerMarble[0];

   //only used to instantiate Game, second player not used
   Player* player_B = new Player;
   player_B->name = "B";
   player_B->index = 1;
   player_B->myMarble = playerMarble[1];

   char humanMarble = playerMarble[1];

   Game game(player_A, player_B);

   while (game.is_gameover(game.get_state()) == -1){
      
      if (game.is_Terminal(game.get_state())){
         game.displayBoard();
         std::cout << "Tie Game!" << std::endl;
         break;
      }

      std::cout << "AI Player moves: " << std::endl;
      int column = game.Alpha_Beta_Search(game.get_state());
      game.Update_Game_State(column, game.to_Move()->myMarble);
      game.displayBoard();


      if (game.is_gameover(game.get_state()) != -1){
         break;
      }
      if (game.is_Terminal(game.get_state())){
         game.displayBoard();
         std::cout << "Tie Game!" << std::endl;
         break;
      }

      std::cout << "Your Move, enter a Column number, 1-7:" << std::endl;

      int user_input = -1;
      while (user_input == -1){
         std::cin >> user_input;
         if ((user_input < 1) || (user_input > 7)){
            std::cout << "That was an invalid entry please try again." << std::endl;
            user_input = -1;
         }
         if ((game.get_state()->top[user_input - 1]) == Rows){
            std::cout << "That Column is full, please try again." << std::endl;
            user_input = -1;
         }
      }

      game.Update_Game_State((user_input - 1), humanMarble);
      game.displayBoard();
   }

   int winner = game.is_gameover(game.get_state());
   if (winner != -1){
      if (winner == 0){
         std::cout << "GAME OVER: AI Player wins!" << std::endl;
         std::cout << "Marble: " << player_A->myMarble << std::endl;
      }
      else {
         game.Next_Player();
         std::cout << "GAME OVER: YOU WIN!!" << std::endl; 
         std::cout << "Marble: " << humanMarble << std::endl;
      }
   }
   delete player_A;

}
