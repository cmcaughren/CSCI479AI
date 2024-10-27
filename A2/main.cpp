/*
Assignment 2
Carolyn McAughren, Student Number: 558 755 906
CSCI 379, Professor Huizhu Liu
*/
#include <iostream>
#include "A2.h"

int main(){

   Problem problem;
   Agent agent(problem);
   Assignment* final_assignment = agent.Backtracking_Search(problem);

   if (final_assignment->is_empty()){
      std::cout << std::endl;
      std::cout << "There is no solution to this problem." << std::endl;
   }
   else {
      std::cout << std::endl;
      std::cout << "A solution has been found: " << std::endl;
      final_assignment->display_assignment();
   }
   delete final_assignment;

}  