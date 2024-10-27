#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <queue>

class HF_KB{
   
   private:
      //KB is a collection of premise -> conclusion pairs
      std::multimap<std::string, std::string> KB;

   public:
      //inserts a rule into the KB
      void tell(std::string premise, std::string conclusion);
      //displays the current KB
      void display();
      //returns true if the KB entails symbol
      bool ask(std::string symbol);
      //helper function for ask
      void ask_set_up(std::map<std::string, bool>* inferred, std::queue<std::string>* symbols_q, 
            std::map <std::string, int>* count);
      
};