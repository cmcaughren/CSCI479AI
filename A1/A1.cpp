#include "A1.h"
#include <string> 
#include <cstring>



void State::display_state(){
      std::cout << "This is the state: " << std::endl;
      std::cout << "There are " << number_tubes_m << " tubes." << std::endl;
      std::cout << "'0' means an empty spot." << std::endl;
      for (int i = 0; i < number_tubes_m; i++){
         std::cout << "Tube " << i+1 << ":" << std::endl;
         std::cout << std::endl;
         for (int j = 3; j >=0 ; j--){
            std::cout <<  "|" << state_m[i][j] << "|" << std::endl;
         }
         std::cout << "‾‾‾" << std::endl;
      }
}

Problem::Problem(){

   //get user to input the number of full tubes and empty tubes
   //calls state class initializer and makes a new state
   int number_of_filled_tubes = 0;
   int number_of_empty_tubes = 0;

   std::cout << "New Problem Creation:" << std::endl;
      
   std::cout << "How many full tubes does the initial State have? Enter a number from 2 to 10:" << std::endl;
   std::cin >> number_of_filled_tubes;
   std::cout << "How many empty tubes does the initial State have? Enter a number from 1 to 3:" << std::endl;
   std::cin >> number_of_empty_tubes;

   number_tubes_m = number_of_filled_tubes + number_of_empty_tubes;
   std::cout << "There are " << number_of_filled_tubes << " filled tubes. So, there will";
   std::cout << " be " << number_of_filled_tubes << " colors of balls." << std::endl;
   std::cout << std::endl;

   //create and initialize the vector char array
   char** initial_state = new char*[number_tubes_m];
   for (int i = 0; i < number_tubes_m; i++){
      initial_state[i] = new char[5];
      for (int j = 0; j < 5; j++){
         initial_state[i][j] = '0';
      }
      initial_state[i][5] == '\0';
   }

   std::cout << "There are initially " << number_of_filled_tubes << " filled tubes." << std::endl;
   std::cout << "There will be 4 balls per filled tube." << std::endl;
   std::cout << "Enter 4 characters with no spaces to represent the colors of the balls in each tube." << std::endl;
   std::cout << "One character represents one ball." << std::endl;
   std::cout << "Let the bottom ball be the first character entered and the top ball be last character entered." << std::endl;
   for (int i = 0; i < number_of_filled_tubes; i++){
      std::string temp;
      std::cout << "For Tube " << i << ":" << std::endl;
      std::cin >> temp;
      
      for (int j = 0 ; j < 4; j ++){
         initial_state[i][j] = temp[j];
      }
   }
   init_m = new State(initial_state, number_tubes_m);
   master_list_states_head = master_list_states_tail = init_m;

}

Problem::~Problem() {

   State* deleter = master_list_states_head;
   
   int num_states_made = 0;

   while (deleter != NULL){
      master_list_states_head = master_list_states_head->master_list_states_next;
      for (int i = 0; i < number_tubes_m; i++){
         delete [] deleter->state_m[i];
      }
      delete [] deleter->state_m;
      delete deleter;
      num_states_made++;
      deleter = master_list_states_head;
   }
   num_states_made++;
   delete deleter;

   //debugging output
   std::cout << "Number of States deleted: " << num_states_made << std::endl;


}

State* Problem::get_initial_state(){
   return  init_m;
}

void Problem::displayProblem(State* state){
   
   state->display_state(); 

}

bool Problem::goalTest(State* maybe_goal){
   
   bool all_ok = true;
   for (int i = 0; i < number_tubes_m; i++){
      for (int j = 0; j < 3; j++){
            
         int k = j + 1;
         
         if (maybe_goal->state_m[i][j] != maybe_goal->state_m[i][k]){
            all_ok = false;
            break;
         }
      }
   }
   return all_ok;
}

float Problem::heuristics(State* state){
   //we look at the bottom ball color in each tube
   //if there is a ball of the wrong color on top of the bottom ball, this is +1
   //additional +1 for each ball on top of that wrong color ball
   //this is addmissable as a heuristics, because if we think of each +1 as one "move"
   //needed before reaching goal state, this is lower than or exactly the actually number of moves 
   //needed.
   //lower heurisitic value = closer to goal state
   
   float heuristic = 0;

   for (int i = 0; i < number_tubes_m; i++){
      for (int k = 1; k < 4; k++){
         if (state->state_m[i][0] != state->state_m[i][k]){
            heuristic = heuristic + (4 - k); 
            //we have assumed the tube is full, but there should not be an
            //increase in heuristic for empty spots in a tube. Deduct those.
            for (k; k < 4; k++){
               if (state->state_m[i][k] == '0'){
                  heuristic = heuristic - 1;
               }
            }
            break; //we are done with this tube, move to next
         }
      }
   }
   return heuristic;
}


Action* Problem::actions_available(Node* curr_state){
   
   Action* head_list_of_actions = NULL;
   Action* trav = NULL;
   
   //We locate a top ball in a tube to move
   for (int tube_from = 0; tube_from < number_tubes_m; tube_from++){
      
      for (int space_from = 3; space_from >= 0 ; space_from--){
         //if the top space in a tube is empty, look below to see if a ball is lower
         if (curr_state->state->state_m[tube_from][space_from] == '0'){
            continue;
         }
         else{
            //Now we loof for an empty space in a different tube where we can move the ball to
            for (int tube_to = 0; tube_to < number_tubes_m; tube_to++){
               
               if (tube_from == tube_to) {
                  continue; 
               }
               //skip a tube if it is full
               if (curr_state->state->state_m[tube_to][3] == '0'){
                  //we have an empty space, need to iterate to the lowest open space
                  int lowest_space_in_tube = 0;
                  for (int i = 2; i >= 0; i--){
                        if  (curr_state->state->state_m[tube_to][i] != '0'){
                           lowest_space_in_tube = i +1;
                           break;
                        }
                  }
                  //we have a possible Action: move char from [tube_from][space_from] -> [tube_to][lowest_space_in_tube]
                  //step 1: make the new state

                  //we create a new state and copy all values from the previous state
                  char** new_state_array = new char*[number_tubes_m];
                  for (int i = 0; i < number_tubes_m; i++){
                     new_state_array[i] = new char[5];
                     for (int j = 0; j < 4; j++){
                        new_state_array[i][j] = curr_state->state->state_m[i][j];
                     }
                     new_state_array[i][5] == '\0';
                  }
                  //we capture the ball color which we have identified to move in this action
                  char ball_char = new_state_array[tube_from][space_from];
                  //move this ball to its new location in the new state
                  new_state_array[tube_from][space_from] = '0';
                  new_state_array[tube_to][lowest_space_in_tube] = ball_char;

                  //create the new state
                  State* new_state = new State(new_state_array, number_tubes_m);
                  master_list_states_tail->master_list_states_next = new_state;
                  master_list_states_tail = new_state;

                  //step 2). make new action, using the new state as the "to" state
                  //add this action to head_list_of_actions list to return 
                  if (head_list_of_actions == NULL){
                     head_list_of_actions = new Action;
                     head_list_of_actions->from_state = curr_state->state;
                     head_list_of_actions->to_state = new_state;
                     head_list_of_actions->next_action = NULL;
                     trav = head_list_of_actions;
                     trav->from_tube_number = tube_from;
                     trav->to_tube_number = tube_to;
                     trav->ball_color = ball_char;
                     
                  } 
                  else {
                     trav->next_action = new Action;
                     trav = trav->next_action;
                     trav->from_state = curr_state->state;
                     trav->to_state = new_state;
                     trav->next_action = NULL;
                     trav->from_tube_number = tube_from;
                     trav->to_tube_number = tube_to;
                     trav->ball_color = ball_char;
                  }
               }
            }
         break;
         }
      }
   }
   return head_list_of_actions;
}


Agent::Agent(Problem* problem){ 
      problem_m = problem;
      master_chain_head_m = NULL;
      master_chain_tail_m = NULL;

}

Agent::~Agent(){

   //delete chain of nodes
   Node* deleter = master_chain_head_m;

   int num_nodes_made = 0;
   while (deleter->master_chain_next_link != NULL){

      master_chain_head_m = master_chain_head_m->master_chain_next_link;
      delete deleter->action;
      delete deleter;      
      deleter = master_chain_head_m;
      num_nodes_made++;

   }
   delete deleter->action;
   delete deleter;
   num_nodes_made++;

   std::cout << "Number of Nodes deleted: " << num_nodes_made << std::endl;
   delete container_m;
} 

Node* Agent::find_solution(){
   
   Node* search_head = new Node;
   master_chain_head_m = master_chain_tail_m = search_head; //add node to list of all nodes to be deleted
   search_head->state = problem_m->get_initial_state();
   search_head->path_cost = 0;
   search_head->priority = calculate_priority(search_head);
   search_head->action = NULL;
   search_head->parent_node = NULL; 
   search_head->next_sort_link = NULL;
   search_head->master_chain_next_link = NULL;

   if (debug_message){
   std::cout << " My initial state is : " << std::endl;
   problem_m->displayProblem(search_head->state);
   std::cout << " My initial path cost is : " << search_head->path_cost << std::endl;
   std::cout << " My initial priority is : " << search_head->priority << std::endl;
   std::cout << std::endl;
   }

   //check that the initial state is not the goal state already
   if (problem_m->goalTest(search_head->state)){
      return search_head;
   }

   //create node pointer to point to the node/state we are currently on, exploring into frontier
   Node* curr = search_head;
   container_m = new Astar;
   //find all nodes we can travel too, calculate their priority and input into priority queue
   container_m->insert_all(expand(search_head));

   //set current to top priority node to explore
   curr = container_m->remove_front();
   
   //testing output message
    if (debug_message){
   std::cout << "TOP OPTION I'M GOING TO EXPLORE NEXT" << std::endl;
   problem_m->displayProblem(curr->state);
   std::cout << "Which has a priority: " << curr->priority << std::endl;
   std::cout << "(path cost part is : " << curr->path_cost << std::endl;
   std::cout << "----------------------------------" << std::endl;
    }
      
   //check current to see if it is the goal
   while (!problem_m->goalTest(curr->state)){

      //find all nodes we can travel too, calculate their priority and input into priority queue
      container_m->insert_all(expand(curr));

      //set current to top priority node to explore
      curr = container_m->remove_front();
      //testing output message
       if (debug_message){
      std::cout << "TOP OPTION I'M GOING TO EXPLORE NEXT" << std::endl;
      problem_m->displayProblem(curr->state);
      std::cout << "Which has a priority: " << curr->priority << std::endl;
      std::cout << "(path cost part is : " << curr->path_cost << std::endl;
      std::cout << "----------------------------------" << std::endl;
       }
   }
   //a solution was found
   return curr;
}

Node* Agent::make_new_node(Node* parent_node, Action* move_to_take){

   Node* new_node = new Node;
   new_node->state = move_to_take->to_state;
   new_node->path_cost = (parent_node->path_cost + 1);
   new_node->action = move_to_take;
   new_node->parent_node = parent_node;
   new_node->master_chain_next_link = NULL;
   new_node->next_sort_link = NULL;
   new_node->depth = parent_node->depth + 1;
   return new_node;
}



float Agent::calculate_priority(Node* curr_node){
   
   float priority = curr_node->path_cost + problem_m->heuristics(curr_node->state);
   return priority;
}

Node* Agent::expand(Node* from){

   //given a current node, state
   //calculate all possible states we could transition to
   //if that transition state is not already in the Priority Queue with a lower priority
   //create a node for that transition state and place it in the queue

   Node* head = NULL;
   Node* trav = NULL;

   Action* actions_available = problem_m->actions_available(from);

   //if there are no actions available from this from node
   if (actions_available == NULL)
   {
      return head;
   }

   while(actions_available != NULL){
      //check that the to_state is not already in the Queue with a lower priority
      float new_state_priority = (from->path_cost + 1) + problem_m->heuristics(actions_available->to_state);
      Node* temp = container_m->find(actions_available->to_state);
      if (temp != NULL){
         if (temp->priority < new_state_priority){
            //skip to next action
            actions_available = actions_available->next_action;
            continue;
         }
      }

      //make a new node for current action in actions_available
      if (head == NULL) {

         head = trav = make_new_node(from, actions_available);
         //update master chain tail node to include this new node
         master_chain_tail_m-> master_chain_next_link= trav;

         //update current action to include to and from destination nodes
         actions_available->from_Node = from;
         actions_available->to_Node = trav;

         //add priority
         trav->priority = calculate_priority(trav);
         
         //testing output message
          if (debug_message){
         std::cout << "A POSSIBLE OPTION I COULD EXPLORE NEXT" << std::endl;
         problem_m->displayProblem(trav->state);
         std::cout << "Priority is " << trav->priority << std::endl;
            std::cout << "(path cost part is : " << trav->path_cost << std::endl;
          }

         //increment actions_availale pointer
         actions_available = actions_available->next_action;
      }
      else {
         //make a new node for current action in actions_available
         trav->master_chain_next_link = make_new_node(from, actions_available);
         
         //update master chain tail ptr to include this new node
         trav = trav->master_chain_next_link;
         master_chain_tail_m = trav;

         //update current action to include to and from destination nodes
         actions_available->from_Node = from;
         actions_available->to_Node = trav;

         trav->priority = calculate_priority(trav);

         //testing output message
          if (debug_message){
         std::cout << "A POSSIBLE OPTION I COULD EXPLORE NEXT" << std::endl;
         problem_m->displayProblem(trav->state);
         std::cout << "Priority is " << trav->priority << std::endl;
            std::cout << "(path cost part is : " << trav->path_cost << std::endl;
          }

         //increment actions_availale pointer
         actions_available = actions_available->next_action;
      }
   }
   return head;
}

void Agent::display_solution(Node* end_node){
   //from the goal node found, use a counter and travel all the way up to the init state
   //declare an array of strings of that size
   //go back through the list of nodes starting from the goal node, up towards the init node
   //and fill in the array as you go
   //output the array data for the user in order from init to goal
   std::cout << "A Solution has been found." << std::endl;

   int depth = end_node->depth;

   Action** Solution_list = new Action*[depth];
   Action* trav = end_node->action;

   while (trav != NULL){
      Solution_list[trav->to_Node->depth] = trav;
      trav = trav->from_Node->action;
   }
   std::cout << "Beginning at initial state..." << std::endl;
   std::cout << std::endl;
   for (int i = 1; i <= depth; i++)
   {
      std::cout << "Step " << Solution_list[i]->to_Node->depth << ".) ";
      std::cout << "Move " << Solution_list[i]->ball_color << " ball from Tube ";
      std::cout << Solution_list[i]->from_tube_number + 1 << " to Tube ";
      std::cout << Solution_list[i]->to_tube_number + 1 << ".";
      std::cout << std::endl;
   }
   std::cout << "Goal state found." << std::endl;

   for(int i = 0; i < depth; i++){
      Solution_list[i] = NULL;
      delete Solution_list[i];
   }
   delete [] Solution_list;

}

Astar::Astar(){
   head_m = NULL;
}

void Astar::insert_all(Node* list_of_nodes_to_insert){

   while(list_of_nodes_to_insert != NULL){
      insert(list_of_nodes_to_insert);
      list_of_nodes_to_insert = list_of_nodes_to_insert->master_chain_next_link;
   }

}

void Astar::insert(Node* new_node){

   if (is_Empty()){
      head_m = new_node;
      return;
   }
   
   if (head_m->priority > new_node->priority){

      new_node->next_sort_link = head_m;
      head_m = new_node;
      return;
   }
   Node* prev = head_m;
   Node* curr = head_m->next_sort_link;

   while (curr != NULL){

      if (curr->priority > new_node->priority){
         prev->next_sort_link = new_node;
         new_node->next_sort_link = curr;
         return;
      }

      if (curr->priority == new_node->priority){
          new_node->next_sort_link = curr->next_sort_link;
          curr->next_sort_link = new_node;
         return;
      }
      if (curr->priority < new_node->priority){
         curr = curr->next_sort_link;
         prev = prev->next_sort_link;
      }
   }
   prev->next_sort_link = new_node;
   return;
}

Node* Astar::remove_front(){

   Node* temp = head_m;
   head_m = head_m->next_sort_link;
   return temp;

}

Node* Astar::peek_top(){

   return head_m;

}

Node* Astar::find(State* locate_me){
   
   Node* temp = head_m;

   //for each node in the Q
   while (temp != NULL){
      
      bool states_match = true;
      bool tubes_match = false;

      //compare each tube to the corresponding tube in the state we're looking for
      for (int i = 0; i < locate_me->number_tubes_m; i++){

         for (int j = 0; j < locate_me->number_tubes_m; j++){

            if ( std::strcmp(temp->state->state_m[i], locate_me->state_m[j]) == 0) {
               //match found, check next tube
               tubes_match = true;
                
            }
         }
         if (!tubes_match){
            states_match = false;
            break;
         } 
         
      }

      //each string has a match, these states are equivalent 
      if (states_match) { 
         return temp; 
      }   
      temp = temp->next_sort_link;
   }
   temp = NULL;
   return temp;
}

//returns true if empty
bool Astar::is_Empty(){

   if (head_m == NULL){
      return true;
   }
   return false;
}
