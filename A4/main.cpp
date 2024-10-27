/*
Assignment 4
Carolyn McAughren, Student Number: 558 755 906
CSCI 379, Professor Huizhu Liu
November 7/2021
*/

#include "A4.h"


int main()
{
   HF_KB myKB;

   std::string number_of_rules;
   int num_rules;
   std::string RHS;
   std::string LHS;
   std::string query;

   std::cout << "\n\tHORN FORM KNOWLEDGE BASE" << std::endl;
   std::cout << "This application manages a list of definite Horn Clauses."<< std::endl;
   std::cout << "Given a list of rules to form the KB, application will query" << std::endl;
   std::cout << "the KB and determine if it entails the specified literal." << std::endl;
   std::cout << "To enter a rule in the KB, use '^' as the conjunction symbol." << std::endl;
   std::cout << "Right hand side of rule will be a conjunction of symbols, " << std::endl;
   std::cout << "where each symbol is a case sensitive string, starting with a letter." << std::endl;
   std::cout << "Left hand side of the rule will consist of a single symbol." << std::endl;
   std::cout << "The literal which will be queried for must be a single symbol also." << std::endl;
   std::cout << "Example rule: RHS 'A^B^AB' or the empty string ''. " << std::endl;
   std::cout << "LHS 'D'. Query 'F'." << std::endl;
   std::cout << "Note: When entering an empty string for some premise, press enter.\n" << std::endl;
   std::cout << std::endl;

   //ask for number of rules
   std::cout << "How many rules are in the KB initially?" << std::endl;
   getline(std::cin, number_of_rules);
   //convert to an int
   std::stringstream number(number_of_rules);
   number >> num_rules;
   
   //get the premise and conclusion of each rule
   for (int i = 0; i < num_rules; i++){

      std::cout << "Please enter RHS of rule " << i + 1 << ":" << std::endl;
      std::getline(std::cin, RHS);
      std::cout << "Please enter LHS of rule " << i + 1 << ":" << std::endl;
      std::getline(std::cin, LHS);
      //add the rule to the KB
      myKB.tell(RHS, LHS);
   }

   std::cout << "\nWhat symbol would you like to query the KB for?" << std::endl;
   std::getline(std::cin, query);

   //displays initial KB, as specified by user
   myKB.display();
   
   if (myKB.ask(query))
      std::cout << "The knowledge base entails " << query << std::endl;
   else
      std::cout << "The knowledge base does not entail " << query << std::endl;
}