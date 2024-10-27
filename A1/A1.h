#pragma once
#include <iostream>


const bool debug_message = false;

//lab 1
/*
Ball sorting game is a fun puzzle game. For each game, it starts with 2 to 10 tubes each 
filled with 4 balls and 1 to 3 empty tubes. Each tube can hold maximum 4 balls. Each group 
of 4 balls are coloured the same, but all the groups are coloured differently.

The goal of the game is to sort these coloured balls until all the same coloured balls (4 
of them) are placed in the same tube.

The rule of the game is simple. In each step, you can only pick up one ball at the top of 
a tube, and place it either into an empty tube or on top of another ball if both of them 
have the same colour and the tube to hold the ball has enough space.

Ball sorting game has many levels, starting from the simplest 2-coloured balls and 3 tubes.


*/

class State{

public:
   char** state_m;
   int number_tubes_m;

   State* master_list_states_next;

   State(char** state, int number_tubes){
      state_m = state;
      number_tubes_m = number_tubes;
   }
   void display_state();
};

class Problem{

   State* init_m;
   int number_tubes_m;
   int number_colors_m;

   //used to delete all states made
   State* master_list_states_head;
   State* master_list_states_tail;

public:   

   Problem();
   ~Problem();
   void displayProblem(State* state);
   State* get_initial_state();

   bool goalTest(State* maybe_goal);
   struct Action* actions_available(struct Node* curr_state);
   bool confirm_valid_initial_state(char** initial_state, int number_of_filled_tubes);
   float heuristics(State* state);

};

struct Action{

   //filled in by Problem class in actions_available func
   State* from_state;
   State* to_state;

   int from_tube_number;
   char ball_color;
   int to_tube_number;

   Action* next_action;

   Action* master_action_list_next;


   //filled in my Agent class when nodes are created in expand function
   struct Node* from_Node; //state begin from
   struct Node* to_Node; //resulting state

};


struct Node{
   State* state;
   float path_cost;
   float priority;

   Action* action; //step taken from parent node to get here
   
   //link will be used to chart path from start to goal when solution is found
   Node* parent_node;

   //link to be used by container class while sorting nodes by priority in priority queue
   Node* next_sort_link;

   //chain used to delete all nodes at end of program
   Node* master_chain_next_link;
   int depth; 
};

class Astar{

private:
   Node* head_m;

public:
   Astar();
   void insert(Node* new_node);
   Node* remove_front();
   Node* peek_top();
   bool is_Empty();
   void insert_all(Node* list_of_nodes_to_insert);
   Node* find(State* locate_me);


};

class Agent{

private:
   Problem* problem_m;
   Astar* container_m;
  
   Node* master_chain_head_m;  //used to keep track of all nodes for deleting
   Node* master_chain_tail_m;  //used to keep track of all nodes for deleting

public: 
   Agent(Problem* problem);
   ~Agent();
   
   Node* find_solution();
   Node* make_new_node(Node* parent_node, Action* move_to_take);
   float calculate_priority(Node* curr_node);
   void display_solution(Node* end_node);
   Node* expand(Node* from);

};



