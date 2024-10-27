#include "A2.h"
#include <iostream>


Assignment::Assignment(std::map<std::string, int> assignment){
   assignment_m = assignment;
}

//adds a variable, value pair to assignment_m
void Assignment::add(std::string variable, int value){
   assignment_m.insert({variable, value});
}

//removes a variable, value pair from assignment_m
void Assignment::remove(std::string variable){
   assignment_m.erase(variable);
}

//displays all currently assigned variables and their values
void Assignment::display_assignment(){
   std::cout << std::endl;
   std::cout << "The assignment of values to variables is as follows: " << std::endl;
   std::map<std::string, int>::iterator iter;
   for (iter = assignment_m.begin(); iter != assignment_m.end(); iter++ ){
      std::cout << iter->first << ": ";
      std::cout << iter->second << " ";
      std::cout << std::endl;
   }
   std::cout << std::endl;
}

//returns true if assignment_m is empty
bool Assignment::is_empty(){
   return assignment_m.empty();
}

Problem::Problem(){

   int number_of_variables;
   int number_of_constraints;
   std::string variable;

   std::cout << "CYCLIC ORDERING PROBLEM" << std::endl;
   std::cout << "Given a finite set of symbols and a collection of constraints,"<< std::endl;
   std::cout << "agent will find a one-to-one mapping function pairing variables " << std::endl;
   std::cout << "with values so that no constraint is violated. " << std::endl;
   std::cout << std::endl;

   //ask for number of variables, loop that amount, get list of variables
   std::cout << "How many variables are there?" << std::endl;
   std::cin >> number_of_variables;

   for (int i = 0; i < number_of_variables; i++){
      std::cout << "Please enter variable " << i + 1 << ":" << std::endl;
      std::cin >> variable;
      variables_m.push_back(variable);
   }

   //ask for number of constraints, loop that amount, get list of constraints
   std::cout << "How many constraints are there?" << std::endl;
   std::cin >> number_of_constraints;

   for (int i = 0; i < number_of_constraints; i++){
      
      std::cout << "For constraint number " << i + 1 << ": " << std::endl;
      std::vector<std::string> constraint;
      for (int j = 0; j < 3; j++){
         std::cout << "Please enter variable " << j + 1 << ":" << std::endl;
         std::cin >> variable;
         constraint.push_back(variable);
      }
      constraints_m.push_back(constraint);
   }
      
   //the number of values is the number of variables (not including 0)
   std::vector<int> temp_values;
   for (int j = 0; j < number_of_variables; j++){
      temp_values.push_back(j + 1);
   }
   //create and fill in available_values, all variables have all values available currently
   for (int i = 0; i < number_of_variables; i++){
      std::string temp_var = variables_m[i];
      available_values_m.emplace(temp_var, temp_values);
   }
}

Problem::Problem(std::vector<std::string> variables,
            std::vector<std::vector <std::string>> constraints,
               std::map<std::string, std::vector<int>> available_values){
   variables_m = variables;
   constraints_m = constraints;
   available_values_m = available_values;
}

//checks current assignment to see if all variables are present in assignment
bool Problem::is_complete(Assignment assignment){

   return (variables_m.size() == assignment.assignment_m.size());
}

//Heuristic functionality: "least constraining value"
//creates a priority queue for the var in question: queue will hold int pairs, 
//'first' will be a potential "value" that var could take on, and the 'second' int
//will be the "count" of how many values would be remaining in "available_values" were var 
//to take on that value. Priority queue will sort these pairs, putting those with higher 
//"count" at the top of the queue.
priority_q* Problem::available_variables(std::string var, Assignment assignment){
   
   //iterators 
   std::map<std::string, std::vector<int>>::iterator avail_val_iter;
   std::vector<int>::iterator vec_iter;

   //priority queue to store one pair of ints for each value 'v' that var could take on. 
   //The first int will be "v", second int will be "count" of the total number of values left
   //available in available_values if var were to take on value "v". Priority_q will sort 
   //these pairs by the "count", placing higher counts at the top of the queue.  
   priority_q* values_queue = new priority_q;

   //find key which equals var in available_values 
   avail_val_iter = available_values_m.find(var);
   if (avail_val_iter != available_values_m.end()){
      
      //iterate through the vector of values that var could take on
      std::vector<int> vec_values = avail_val_iter->second;
      for(vec_iter = vec_values.begin(); vec_iter != vec_values.end(); vec_iter++){
         
         int counter = 0;

         //make a copy of the current assignment, add in var = current val
         Assignment assignment_copy = assignment;
         assignment_copy.add(var, *vec_iter);

         //create a temporary copy of problem
         std::vector<std::string> variables_copy = variables_m;
         std::vector<std::vector <std::string>> constraints_copy = constraints_m;
         std::map<std::string, std::vector<int>> available_values_copy = available_values_m;
         Problem problem_copy(variables_copy, constraints_copy, available_values_copy);

         //update problem_copy based on assignment_copy with this addtional var = value
         problem_copy.update_available_values(assignment_copy);
         //count how many value options are left to the other vars now that we
         //assigned var = value?
         counter = problem_copy.count_available_values();
         
         //add this value, counter pair to the priority q
         std::pair<int, int> value_count = std::make_pair(*vec_iter, counter);
         values_queue->push(value_count);

      }
   }
   //return the priority_q full of all value, counter pairs which var could take on,
   //sorted with higher counts on the top
   return values_queue;
}

//count the number of values listed in the available_values map
int Problem::count_available_values(){
   
   //iterator
   std::map<std::string, std::vector<int>>::iterator iter;

   int count = 0;

   for (iter = available_values_m.begin(); iter != available_values_m.end(); iter++){
      count = count + iter->second.size();
   }
   return count;
}


//Checks current assignment to see what variables have already been assigned, and
//updates the available_values_m 
void Problem::update_available_values(Assignment assignment){
   //STEP 1) remove any K's from available_values which are found already assigned in assignment
   remove_used_variables(assignment);

   //STEP 2) remove any values in V (for any K in available_values) which are already
   //assigned to a variable in assignment
   remove_used_values(assignment);
  
   //STEP 3) remove any values in V (for any K in available_values) which now 
   //violate constraints
   remove_constraint_violations(assignment);
}

//helper function for update_available_values: remove any key's from available_values 
//which are found already assigned in assignment
void Problem::remove_used_variables(Assignment assignment){
   
   //iterators 
   std::map<std::string, std::vector<int>>::iterator avail_val_iter;
   std::map<std::string, int>::iterator assignm_iter;
   
   //cycle through all keys (variables) in available_values
   for (avail_val_iter = available_values_m.begin(); avail_val_iter != available_values_m.end(); 
         avail_val_iter++){
      
      //check each k in available _values to see if it is a k in assignment.assignment_m
      std::string key = avail_val_iter->first;
      
      //assignm_iter will == end() if temp was not found in the map
      assignm_iter = assignment.assignment_m.find(key);
      if (assignm_iter != assignment.assignment_m.end()){
         //remove temp from available_values, it is already assigned
         available_values_m.erase(key);
      }

   }
}

//helper function for update_available_values: removes any values in the Vector of values 
//for any Key in available_values, which are already assigned to a variable in assignment
void Problem::remove_used_values(Assignment assignment){
   
   //iterators
   std::map<std::string, std::vector<int>>::iterator avail_val_iter;
   std::map<std::string, int>::iterator assignm_iter;
   std::vector<int>::iterator vec_iter;
   
   //iterate through every k in assignment_m
   for (assignm_iter = assignment.assignment_m.begin(); 
         assignm_iter != assignment.assignment_m.end(); assignm_iter++){
      
      //check the v associated with this k, we will look for it and remove it 
      //from available_values
      int val = assignm_iter->second;
      
      //iterate though each k in available_values map
      for (avail_val_iter = available_values_m.begin(); 
            avail_val_iter != available_values_m.end(); avail_val_iter++){
         
         //each k in available_values has a vector of available values
         //check each of these values and remove them if they == val 
         for (int i = 0; i < avail_val_iter->second.size(); i++){
            
            if (avail_val_iter->second[i] == val){
               vec_iter = avail_val_iter->second.begin() + i;
               avail_val_iter->second.erase(vec_iter);
               break;
            }
         }
      }
   }   
}

//helper function for update_available_values: removes any values in the Vector of values 
//for any Key in available_values, which now violate constraints
void Problem::remove_constraint_violations(Assignment assignment){
   
   //iterators
   std::map<std::string, std::vector<int>>::iterator avail_val_iter;
   std::map<std::string, int>::iterator assignm_iter;
   
   //for each constraint check: are there 2 variables assigned, and one not?
   for (int i = 0; i < constraints_m.size(); i++){

      int counter = 0;
      std::string var_1 = constraints_m[i][0];
      int var_1_value = -1;
      std::string var_2 = constraints_m[i][1];
      int var_2_value = -1;
      std::string var_3 = constraints_m[i][2];
      int var_3_value = -1;
      std::string unassigned_var;
      int* unassigned_var_value;

      //check if var_1 is in assignment
      assignm_iter = assignment.assignment_m.find(var_1); 
      //assignm_iter will == end() if temp was not found in the map
      if (assignm_iter != assignment.assignment_m.end()){
         //var_1 is assigned already, save the value given to var_1
         var_1_value = assignm_iter->second;
         counter ++;
      }
      else{
         //var_1 is not assigned yet
         unassigned_var = var_1;
         unassigned_var_value = &var_1_value;
      }
      
      //check if var_2 is in assignment
      assignm_iter = assignment.assignment_m.find(var_2); 
      //assignm_iter will == end() if temp was not found in the map
      if (assignm_iter != assignment.assignment_m.end()){
         //var_2 is assigned already

         //save the value given to var_2
         var_2_value = assignm_iter->second;
         counter ++;
      }
      else{
         //var_2 is not assigned yet
         unassigned_var = var_2;
         unassigned_var_value = &var_2_value;
      }
      
      //check if var_3 is in assignment
      assignm_iter = assignment.assignment_m.find(var_3); 
      //assignm_iter will == end() if temp was not found in the map
      if (assignm_iter != assignment.assignment_m.end()){
         //var_3 is assigned already

         //save the value given to var_3
         var_3_value = assignm_iter->second;
         counter ++;
      }
      else{
         //var_3 is not assigned yet
         unassigned_var = var_3;
         unassigned_var_value = &var_3_value;
      }    
      
      //if two variables in a constraint are set but one is not: let us check this unassigned
      //variable in values_available, and remove any value from its list of values_available
      //which violate the constraint 
      if (counter == 2){
         //locate the vector of possible values associated with the unassigned variable
         //in available_values 
         avail_val_iter = available_values_m.find(unassigned_var);

         //for each potential value in the vector of values available for this unassigned var
         for (int i = 0; i < avail_val_iter->second.size(); i++){
            
            bool satisfied = false;

            *unassigned_var_value = avail_val_iter->second[i];

            if ( (var_1_value < var_2_value) && (var_2_value < var_3_value)){
               satisfied = true;
            }
            else if ( (var_2_value < var_3_value) && (var_3_value < var_1_value) ) {
               satisfied = true;
            }
            else if ( (var_3_value < var_1_value) && (var_1_value < var_2_value)){
               satisfied = true;
            }
            if (satisfied == false){
               //this value for var does not satisfy our constraints, remove it from 
               //available_values value vector for var
               avail_val_iter->second.erase(avail_val_iter->second.begin() + i);
               //we have removed a value, so size is one less
               // back up one space in iteration 
               i--; 
            }
         }
      }
   }
}

//displays the current problem variables, constraints, and remaining 
//unassigned variables and the values which are available to them 
void Problem::display_problem(){

   std::cout << std::endl;
   std::cout << "PROBLEM INSTANCE DISPLAY" << std::endl;
   std::cout << std::endl;
   std::cout << "The Variables are: " << std::endl;
   for (int i = 0; i < variables_m.size(); i++){
      std::cout << variables_m[i] << " ";
   }
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << "The Constraints are: " << std::endl;
   for (int i = 0; i < constraints_m.size(); i++){
      for (int j = 0; j < constraints_m[i].size(); j++){
         std::cout << constraints_m[i][j] << " ";
      }
      std::cout << std::endl;   
   }
   std::cout << std::endl;
   std::cout << "The values available to each variable are: " << std::endl;
   std::map<std::string, std::vector<int>>::iterator iter;
   for (iter = available_values_m.begin(); iter != available_values_m.end(); iter++ ){
      std::cout << iter->first << ": ";
      for (int j = 0; j < iter->second.size(); j++){
         std::cout << iter->second[j] << " ";
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
}

//displays unassigned variables and the values which are available to them 
void Problem::display_available_values(){

   std::cout << std::endl;
   std::cout << "The values available to each variable are: " << std::endl;
   std::map<std::string, std::vector<int>>::iterator iter;
   for (iter = available_values_m.begin(); iter != available_values_m.end(); iter++ ){
      std::cout << iter->first << ": ";
      for (int j = 0; j < iter->second.size(); j++){
         std::cout << iter->second[j] << " ";
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;

}

Agent::Agent(Problem problem){
   
   std::vector<std::string> variables_copy = problem.variables_m;
   std::vector<std::vector <std::string>> constraints_copy = problem.constraints_m;
   std::map<std::string, std::vector<int>> available_values_copy = problem.available_values_m;
   
   problem_m = new Problem(variables_copy, constraints_copy, available_values_copy);
   
}

Agent::~Agent(){
   delete problem_m;
}

//Returns either a consistent, complete assignment, or an empty assignment indicating 
//no solution is available 
Assignment* Agent::Backtracking_Search(Problem problem){
   
   std::map<std::string, int> assignment_map;
   Assignment* assignment = new Assignment(assignment_map);
   
   return Recursive_Backtrack(assignment, problem);

}

//Returns either a consistent, complete assignment, or an empty assignment indicating 
//no solution is available:
//Calls select_unassigned_variable to choose the best variable "var "to assign next, using
//"minimum remaining value" and "most constricted variable" heuristics. 
//Calls available_variables to get a prioriy_q of sorted values "val" which are 
//available to var sorted by "least constraining value" heuristics. 
//Recursively assign values to variables, checking each new assignment for completeness
//until either a solution is found or all options have been tried (no solution)
Assignment* Agent::Recursive_Backtrack(Assignment* assignment, Problem problem){

   //check if current assignment is complete, return it if so
   if (problem.is_complete(*assignment)) {
      return assignment;
   }
   
   //select best variable "var" to assign next, according to "minimum remaining value" and 
   //"most constricted variable" heuristics. 
   std::string var = select_unassigned_variable(problem.available_values_m);

   //obtain a priority_q of all values available to var, sorted according to
   //"least constraining value" heuristics
   priority_q* pq = problem.available_variables(var, *assignment);

   //we consider each value in pq
   while (!pq->empty()){ 

      //collect the top value, count pair from pq
      std::pair<int, int> top_val = pq->top();
      pq->pop();
      
      //add var = value pair to assignment
      assignment->add(var, top_val.first);

      //create a copy of our problem to restore later if needed
      std::vector<std::string> variables_copy = problem.variables_m;
      std::vector<std::vector <std::string>> constraints_copy = problem.constraints_m;
      std::map<std::string, std::vector<int>> available_values_copy = problem.available_values_m;
      Problem original_problem(variables_copy, constraints_copy, available_values_copy);

      //update available_values based on assignment which now has var = value
      problem.update_available_values(*assignment);

      //recursively call self to check this new problem, assignment for a solution
      Assignment* result = Recursive_Backtrack(assignment, problem);

      //if we have found a solution, delete the priority_q and return the result  
      if (problem.is_complete(*result)){
         delete pq;
         return result;
      }

      //no solution was found down this path for val = val, remove var from assignment 
      //so we can loop again and assign it a different value  
      assignment->remove(var);

      //restore problem
      problem = original_problem;

   }
   //No solution is possible, all var = value assignments have been tested
   //delete priority_q and return empty assignment to signal no solution possible
   delete pq;
   return assignment;
}


//Heuristic functionality: "minimum remaining value" and "most constrained variable"
//Consider each variable "var" remaining in available_variables
//count the number of potential values available to each var
//return the variable with the least remaining values ie minimum remaining value
//In case of tie for lowest:
//consider each var which tied for lowest remaining value. Count how many times each of
//these vars appear in a constraint. Return the var which appears in the constraints
//most often ie most constrained variable
std::string Agent::select_unassigned_variable(std::map<std::string, std::vector<int>> available_values){

   //iterator 
   std::map<std::string, std::vector<int>>::iterator avail_val_iter;

   //variable to hold the name of the var which has the minimum values available to it
   std::string var_min_values;
   //variable to hold the minimum amount of values available to any var
   int var_min_values_count = problem_m->variables_m.size();

   //find the lowest number of values for any variable
   //iterate throgh all the variables in available_values and compare the size of their 
   //value vectors to find the smallest
   for (avail_val_iter = available_values.begin(); 
                  avail_val_iter != available_values.end(); avail_val_iter++){
      
      int count = avail_val_iter->second.size();
      if (count < var_min_values_count){
         var_min_values = avail_val_iter->first;
         var_min_values_count = count;
      }
   }
   
   //check if there are multiple variables with this same lowest count of values available,
   //collect any variables which tie into a vector 
   std::vector<std::string> tied_variables;
   int ties_counter = 1;

   //iterate throgh all the variables in available_values and compare the size of their 
   //value vectors to the size of the known smallest, save var name in tied_variables if same
   //keep track of the number of variables which tie for smallest
   for (avail_val_iter = available_values.begin(); 
            avail_val_iter != available_values.end(); avail_val_iter++){
     
      int count = avail_val_iter->second.size();
      if (count == var_min_values_count){

         if (avail_val_iter->first != var_min_values){
            ties_counter ++;
            tied_variables.push_back(avail_val_iter->first);
         }
      }
   }

   //if more than one variable had the smallest number of values available, 
   //we use "most constrained variable" heuristics to break tie
   //Count how many times each of the tied vars appear in a constraint. 
   //Return the var which appears most often in the constraints
   if (ties_counter > 1){
    var_min_values = most_constrained_variable(tied_variables);
   }
   //return the variable with the "minimum remaining values" and/or "most constrained variable"
   return var_min_values;
}


//Helper function to select_unassigned_variable
//consider each var in tied_variables. Counts how many times each of
//these vars appear in a constraint. Returns the var which appears in the constraints
//most often ie most constrained variable
std::string Agent::most_constrained_variable(std::vector<std::string> tied_variables){

   //iterators
   std::vector<std::string>::iterator tied_variables_iter;
   std::vector<std::vector <std::string>>::iterator constraint_iter;
   std::vector<std::string>::iterator constraints_var_iter;
   
   std::string most_seen;
   int most_seen_count = 0;

   //consider each var in tied_variables
   for (tied_variables_iter = tied_variables.begin(); 
         tied_variables_iter != tied_variables.end(); tied_variables_iter++){
      
      std::string curr_var = *tied_variables_iter;
      int current_var_count = 0;
      
      //iterate through all constraints
      for (constraint_iter = problem_m->constraints_m.begin(); 
               constraint_iter != problem_m->constraints_m.end();
                  constraint_iter ++) {

         //iterate through each variable within the constraint
         //count the number of times var is seen
         std::vector<std::string> constraint_var_ptr = *constraint_iter;
         for (constraints_var_iter = constraint_var_ptr.begin(); constraints_var_iter != constraint_var_ptr.end(); constraints_var_iter++){
            if (*constraints_var_iter == curr_var){
               current_var_count++;
            }
         }
      }
      //compare the number of times var was seen with the current "most_seen_count"
      //if this var was seen more, let it be the new most_seen
      if (current_var_count > most_seen_count){
         most_seen_count = current_var_count;
         most_seen = curr_var;
      } 
   }
   //return the most constrained variable
   return most_seen;
}

