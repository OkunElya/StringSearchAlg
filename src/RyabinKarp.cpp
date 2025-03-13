#include <stdlib.h>
#include <string>
#include <vector>
#include<RyabinKarp.h>
namespace alg
{
    size_t simple_hash_1(std::string inp)
    {
        size_t out = 0;
        for (auto s : inp)
        {
            out += (int)s;
        }
        return out;
    }

    std::vector<size_t> RyabinKarp(std::string inp, std::string pattern)
    {
        std::vector<size_t> output;
        if (pattern.length() > inp.length()){
            return output;
        }
        size_t inpLen = inp.length();
        // find initial hash for search pattern
        std::size_t occurrences = 0;
        std::size_t patternHash = simple_hash_1(pattern);
        std::size_t windowHash = simple_hash_1(inp.substr(0, pattern.length()));
        for (size_t i = 0; i < (inp.length() - pattern.length()); i++)
        {
            if (windowHash == patternHash)
            {
                if (pattern == inp.substr(i, pattern.length()))
                {
                    output.push_back(i);
                }
            }

            windowHash -= (int)inp[i];
            windowHash += (int)inp[i + pattern.length()];
        }

        return output;
    }
}