#include<unordered_map>
#include<map>
#include<vector>
#include<string>

#ifndef AHO_KORASIK_HEADER  
#define AHO_KORASIK_HEADER 
  
namespace alg{
    class AhoKorasik{
        struct node{
            std::map<char,node*> children;
            node* suffLink=nullptr;
            node* parent=nullptr;
            bool isEnd=false;
            std::vector<size_t> patternNumbers;
            char toThis=0;
            // std::string patTemp;
            bool isVisited=false;//
        
        };
        node* start=nullptr;
        std::vector<size_t> patternLengths;

        
        void clearTrie(node* temp);

        void buildTrie(std::vector<std::string> patterns);

        void walk(node* temp);

        void debugWalk(node* temp);

        void createSuffLink(node* temp, node* now);
        public:
        AhoKorasik(std::vector<std::string> patterns);
        ~AhoKorasik();
        void completeTerminals(node* temp);
        std::vector<std::pair<size_t,size_t>>searh(std::string input);
    };
}
#endif