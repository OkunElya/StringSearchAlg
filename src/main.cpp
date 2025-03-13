#include<iostream>
#include<fstream>
#include <string>
#include <vector>
#include <chrono>
#include "RyabinKarp.h"
#include "AhoKorasik.h"
struct node{
    std::unordered_map<char,node*> children;

};


size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)//lazy ahh code from stackowerflow
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

int main(){
    const size_t fieldsNum=5;
    setlocale(LC_ALL, "RU");
    std::ifstream inputFile;
    std::string filePath;
    while(!inputFile.is_open()){\
        std::cout<<"Enter input file path\n:";
       
        // std::cin>>filePath;
        filePath="./../data/inputTranslit.txt";
        std::cin.clear();//clear buffer
       
        inputFile.open(filePath);
    } 
    //open output files ( 1st for Karasik and second for Aho
	std::ofstream outFileKarp(filePath + ".found.karp.txt");
	if (!outFileKarp.is_open()) {
		std::cerr << "Unable to open output file." << std::endl;
	}

	std::ofstream outFileAho(filePath + ".foiund.karasik.txt");
	if (!outFileAho.is_open()) {
		std::cerr << "Unable to open output file." << std::endl;
	}
    std::string lineString;
    size_t lineNumber=0;
    size_t searchLineCount=500;

    std::vector<size_t> occCountKarp={1,1,1,1,1};
    std::vector<std::string> patternsKarp={"9","M","Grem","Mar","PSC"};

    std::vector<size_t> occCountKarasik={1,1,1,1,1,1};
    std::vector<std::vector<std::string>> patternsKarasik={{"4","9"},{"Ra","Luz"},{"Burse","Grem"},{"J","Arthur"},{"LA","FPO"}};

    std::vector<std::string> inpData;
    std::vector<std::vector<std::string>> inpSplitted;

    //load data
    while(std::getline(inputFile,lineString)&&(lineNumber<searchLineCount)){
        std::vector<std::string> temp;
        split(lineString,temp,' ');
        inpData.push_back(lineString);
        inpSplitted.push_back(temp);
        lineNumber++;
    }

    std::vector<std::vector<std::vector<size_t>>> searchResKarp;
    auto startTime = std::chrono::high_resolution_clock::now();
    for(auto lineArr: inpSplitted){
        std::vector<std::vector<size_t>> searchResBuf;
        for(int i =0;i<fieldsNum;i++){
            auto inpStr=lineArr[i];
            searchResBuf.push_back(alg::RyabinKarp(inpStr,patternsKarp[i]));
        }
        searchResKarp.push_back(searchResBuf);
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto timeKarp = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();

    std::vector<alg::AhoKorasik*> ahos;
    for(auto karasPattern: patternsKarasik){
        auto tempPtr= new alg::AhoKorasik(karasPattern);
        ahos.push_back(tempPtr);
    }

    std::vector<std::vector<std::vector<std::pair<size_t,size_t>>>> searchResAho;
    startTime = std::chrono::high_resolution_clock::now();
    for(auto lineArr: inpSplitted){
        std::vector<std::vector<std::pair<size_t,size_t>>> searchResBuf;
        for(int i =0;i<fieldsNum;i++){
            auto inpStr=lineArr[i];
            searchResBuf.push_back(ahos[i]->searh(inpStr));
        }
        searchResAho.push_back(searchResBuf);

    }
    endTime = std::chrono::high_resolution_clock::now();
    auto timeAho = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();


    //write out results
    //karp
    lineNumber=0;
    outFileKarp<<"Output search results for Karp search ("<<searchResKarp.size()<<" lines) \nperfomance:"<<((float)(searchLineCount)*1000/std::max(timeKarp,(int64_t)1))<<" lines per second\n";
    for(auto searchRes: searchResKarp){
        bool doMatches=true;
        for(int i = 0;i<fieldsNum;i++){
            if(searchRes[i].size()<occCountKarp[i]){
                doMatches=false;
                break;
            }
        }
        if(doMatches){
            outFileKarp<<""<<(lineNumber+1)<<".";
            outFileKarp<<inpData[lineNumber]<<"\n";
        }
        lineNumber++;
    }
    outFileKarp.close();

    //karasik
    
    lineNumber=0;
    outFileAho<<"Output search results for Aho search ("<<searchResKarp.size()<<" lines) \n"<<((float)(searchLineCount)*1000/std::max(timeKarp,(int64_t)1))<<" lines per second\n";

    for(auto searchRes: searchResAho){
        bool doMatches=true;
        for(int i = 0;i<fieldsNum;i++){
            if(searchRes[i].size()<occCountKarasik[i]){
                doMatches=false;
                break;
            }
        }
        if(doMatches){
            outFileAho<<""<<(lineNumber+1)<<".";
            outFileAho<<inpData[lineNumber]<<"\n";
        }
        lineNumber++;
    }
    outFileAho.close(); 
    return 0;
}