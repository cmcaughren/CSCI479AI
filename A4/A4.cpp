#include "A4.h"

//inserts a rule into the KB
void HF_KB::tell(std::string premise, std::string conclusion){

   KB.insert(std::pair<std::string, std::string>(premise,conclusion));

}

//displays the current KB
void HF_KB::display(){

   std::cout << "\n\tKnowledge Base:" << std::endl; 

   std::multimap<std::string, std::string>::iterator it;

   for(it = KB.begin(); it != KB.end(); it++){
      
      if (it->first == ""){
         std::cout << it->second << std::endl;
      }
      else {
         std::cout << it->first << " -> " << it->second << std::endl;
      }

   }
}
//returns true if the KB entails symbol
//uses forward-chaining algorithm: begins with known facts (positive literals)
//Checks each premise in each implication, and if they are all found to be true, 
//the conclusion is added to the set of known facts. Continue until the no further
//inferences can be made, or until the query symbol is located.
//Linear runtime
bool HF_KB::ask(std::string symbol){

   //Symbols_q keeps track of symbols known to be true, but not yet processed
   //Count keeps track of how many premises of each implication are not yet proven
   //Count will be decremented by 1 each time a symbol from that implication is proven
   //If count reached 0, the conclusion of that implication is added to symbols_q (known true)
   //Inferred tracks those symbols which have been processed, so we do not add it to symbols_q
   //again. 
   std::map <std::string, bool>* inferred = new std::map <std::string, bool>;
   std::queue <std::string>* symbols_q = new std::queue <std::string>;
   std::map <std::string, int>* count = new std::map <std::string, int>; 
   std::multimap<std::string, std::string>::iterator it;
   
   //sets up inferred, symbols_q, and count
   ask_set_up(inferred, symbols_q, count);

   //for each symbol known to be true in the KB
   while(symbols_q->size() != 0){
      std::string curr_symbol = symbols_q->front();
      symbols_q->pop();

      //KB entails symbol
      if (curr_symbol == symbol) {
         return true;
      }

      //if it has not yet been processed
      if (inferred->find(curr_symbol)->second == false){
         //mark it as processed
         (*inferred)[curr_symbol] = true;
         
         //for each rule in the KB
         for( it = KB.begin(); it != KB.end(); it++){
            
            std::string curr_premise = it->first;

            if (curr_premise == ""){
               continue;
            }
            //parsing the implication into seperate symbols by ^ as a seperator
            std::string parsed_symbol;
            std::stringstream premise_stream(curr_premise);

            while(getline(premise_stream, parsed_symbol, '^')){

               //if the curr_symbol from symbols_q (known to be true) is in this implication
               //decrease the count for this implication 
               if (parsed_symbol == curr_symbol){
                  (*count)[curr_premise] = (*count)[curr_premise] - 1;
               }
            }

            //if count is 0 for the current implication we are considering in the KB, 
            //we know all premises in the implication are true, so the conclusion is true,
            //add the conclusion to the symbols_q for processing
            if ((*count)[curr_premise] == 0 ){
               symbols_q->push(it->second);
            }
         }
      }
   }
   delete inferred;
   delete count;
   delete symbols_q;

   //the KB does not entail symbol
   return false;
}

//helper function for ask():
//Goes through KB, parsing all implications into premise symbols, and counting the number
//of symbols in each implication. This implication is added to count as key, with the number
//of symbols in it as the value. Each parsed symbol from all implications is added to inferred,
//set to false. All implications which are empty "", we add the conclusion to symbols_q as
//a known truth. 
void HF_KB::ask_set_up(std::map<std::string, bool>* inferred, std::queue<std::string>* symbols_q, 
                        std::map <std::string, int>* count){

   std::multimap<std::string, std::string>::iterator it;
   int symbol_count;
   for( it = KB.begin(); it != KB.end(); it++){
      
      symbol_count = 0;
      std::string curr_implication = it->first;
      std::string curr_symbol;
      std::stringstream premise_stream(curr_implication);
      
      if (curr_implication == ""){
         //if "" is the implication, push the conclusion into symbols_q as known truth
         symbols_q->push(it->second);
      }
      else {
         //parsing the implication into symbols using ^ as the seperator
         while(getline(premise_stream, curr_symbol, '^')){
            symbol_count++;
            //add each symbols to inferred as false
            inferred->insert(std::pair<std::string, bool>(curr_symbol, false));
         }
         //add each implication to count as a key, with the number of symbols in it as the 
         //value
         count->insert(std::pair<std::string, int>(curr_implication, symbol_count));
      }
   }
}
