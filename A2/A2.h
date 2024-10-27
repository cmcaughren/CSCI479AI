#pragma once
#include <map>
#include <vector>
#include <queue>


typedef std::priority_queue<std::pair<int, int>,std::vector<std::pair<int, int>>, class Compare> priority_q;

class Assignment{

private:

   //Map holding all assigned variables mapped to their value
   std::map<std::string, int> assignment_m;

public:
   Assignment(std::map<std::string, int> assignment); 

   //adds a variable, value pair to assignment_m
   void add(std::string variable, int value);

   //removes a variable, value pair from assignment_m
   void remove(std::string variable);

   //displays all currently assigned variables and their values
   void display_assignment();

   //returns true if assignment_m is empty
   bool is_empty();

   friend class Problem;
};


class Problem{

private:
   //Vector holding all variables available in the problem
   std::vector<std::string> variables_m;

   //Vector of Vectors, holding the constraints for the problem
   std::vector<std::vector <std::string>> constraints_m;

   //Map of unassigned variables paired with a vector holding its remaining available values
   std::map<std::string, std::vector<int>> available_values_m; 

public:
   Problem();
   Problem(std::vector<std::string> variables_m,
            std::vector<std::vector <std::string>> constraints_m,
               std::map<std::string, std::vector<int>> available_values_m );

   //checks current assignment to see if all variables are present in assignment
   bool is_complete(Assignment assignment);

   //Heuristic functionality: "least constraining value"
   //creates a priority queue for the var in question: queue will hold int pairs, 
   //first in will be a potential "value" that var could take on, and the second int
   //will be the "count" of how many values would be remaining in "available_values" were var 
   //to take on that value. Priority queue will sort these pairs, putting those with higher 
   //"count" at the top of the queue.
   priority_q* available_variables(std::string var, Assignment assignment);

   //count the number of values listed in the available_values map
   int count_available_values();

   //Checks current assignment to see what variables have already been assigned, and
   //updates the available_values_m 
   void update_available_values(Assignment assignment);

   //helper function for update_available_values: remove any key's from available_values 
   //which are found already assigned in assignment
   void remove_used_variables(Assignment assignment);

   //helper function for update_available_values: removes any values in the Vector of values 
   //for any Key in available_values, which are already assigned to a variable in assignment
   void remove_used_values(Assignment assignment);

   //helper function for update_available_values: removes any values in the Vector of values 
   //for any Key in available_values, which now violate constraints
   void remove_constraint_violations(Assignment assignment);
   
   //displays the current problem variables, constraints, and remaining 
   //unassigned variables and the values which are available to them 
   void display_problem();

   //displays unassigned variables and the values which are available to them 
   void display_available_values();

   friend class Agent;

};

class Agent {
   
private:   
   Problem* problem_m;

public:

   Agent(Problem problem);
   ~Agent();

   //Returns either a consistent, complete assignment, or an empty assignment indicating 
   //no solution is available 
   Assignment* Backtracking_Search(Problem problem);
   

   //Returns either a consistent, complete assignment, or an empty assignment indicating 
   //no solution is available:
   //Calls select_unassigned_variable to choose the best variable "var "to assign next, using
   //"minimum remaining value" and "most constricted variable" heuristics. 
   //Calls available_variables to get a prioriy_q of sorted values "val" which are 
   //available to var sorted by "least constraining value" heuristics. 
   //Recursively assign values to variables, checking each new assignment for completeness
   //until either a solution is found or all options have been tried (no solution)
   Assignment* Recursive_Backtrack(Assignment* assignment, Problem problem);

   
   //Heuristic functionality: "minimum remaining value" and "most constrained variable"
   //Consider each variable "var" remaining in available_variables
   //count the number of potential values available to each var
   //return the variable with the least remaining values ie minimum remaining value
   //In case of tie for lowest:
   //consider each var which tied for lowest remaining value. Count how many times each of
   //these vars appear in a constraint. Return the var which appears in the constraints
   //most often ie most constrained variable
   std::string select_unassigned_variable(std::map<std::string, std::vector<int>> available_values);


   //Helper function to select_unassigned_variable
   //consider each var in tied_variables. Counts how many times each of
   //these vars appear in a constraint. Returns the var which appears in the constraints
   //most often ie most constrained variable
   std::string most_constrained_variable(std::vector<std::string> tied_variables); 

};

//Custom compare function for use with priority_q:
//priority_q stores and sorts all potential values which a variable could be assigned
//We wish to use "least constraining value", so we need the value at the top of the heap
//to be the value which gives us the highest total potential values remaining  
class Compare{
   public:
      bool operator() (std::pair<int, int> a, std::pair<int, int> b) {
         
         return (a.second < b.second);
      }    

};

