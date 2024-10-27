#include <iostream>
#include "A1.h"

int main()
{
   std::cout << "Ball Tube Sorting Game" << std::endl;
   std::cout << "Given between 2-10 tubes filled with 4 colored balls each," << std::endl;
   std::cout << "and 1-3 empty tubes, I will sort the balls until each tube contains" << std::endl;
   std::cout << "only balls of matching color. The number of ball colors must equal the." << std::endl;
   std::cout << "initial number of filled tubes." << std::endl;
   std::cout << std::endl;

   Problem problem;
   
   Agent agent(&problem);

/*
   Node* catchnode = NULL;
   catchnode = agent.find_solution();
   if (catchnode != NULL) {
      std::cout << "Found it" << std::endl;
   }
*/   


   //actual call will be:
   agent.display_solution(agent.find_solution());
   return 0;
}
