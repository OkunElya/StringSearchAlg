#include <stdlib.h>
#include <string>
#include <RyabinKarp.h>
#include <AhoKorasik.h>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <queue>

alg::AhoKorasik::AhoKorasik(std::vector<std::string> patterns)
{
    buildTrie(patterns);
    completeTerminals(start);
    // debugWalk(start);
}

alg::AhoKorasik::~AhoKorasik(){
    clearTrie(start);
}

void alg::AhoKorasik::clearTrie(node *temp)
{
    if (!temp)
    {
        return;
    }
    if(!temp->children.empty())
        for (auto child : temp->children)
        {
            clearTrie(child.second);
        }

    if (temp == start)
    {
        start = nullptr;
    }
    delete temp;

    return;
}

void alg::AhoKorasik::createSuffLink(node *temp, node *now)
{

    if (!now->parent)
    {
        temp->suffLink = start; // root sitation
        // std::cout << "Creating suff link to " << temp->patTemp << "-> root\n";
        return;
    }
    if (now->parent == start)
    {
        temp->suffLink = start;
        // std::cout << "Creating suff link to " << temp->patTemp << "- root 1\n";
        return;
    }
    now = now->parent;

    now = now->suffLink;
    if (now->children.contains(temp->toThis))
    {
        temp->suffLink = now->children[temp->toThis];
        // std::cout << "Creating suff link to " << temp->patTemp << "->" << now->children[temp->toThis]->patTemp << "\n";

        return;
    }
    createSuffLink(temp, now);
}

void alg::AhoKorasik::walk(node *temp)
{

    createSuffLink(temp, temp);
    // std::cout << temp->isEnd << " ";
    for (auto child : temp->children)
    {
        walk(child.second);
    }
}
// void alg::AhoKorasik::debugWalk(node *temp)
// {
//     std::string out;
//     for (size_t num : (temp->patternNumbers))
//     {
//         out += (std::to_string(num)) + " ";
//     }
//     // std::cout<<temp->toThis<<"->"<<temp->suffLink->toThis<<" pnn:" << out <<"\n";
//     std::cout << temp->patTemp << " pnn:" << out << "\n";
//     for (auto child : temp->children)
//     {
//         debugWalk(child.second);
//     }
// }

void alg::AhoKorasik::buildTrie(std::vector<std::string> patterns)
{
    patternLengths.clear();

    if (start)
    { // need to clear the trie if it's not empty
        clearTrie(start);
    }
    start = new node;
    start->suffLink = start; // first node pioints to itself.
    size_t patternCounter = 0;
    for (auto pattern : patterns)
    {
        patternLengths.push_back(pattern.length());
        node *temp = start;
        std::string temp_;
        for (auto sym : pattern)
        {
            temp_ += sym;
            if (!temp->children.contains(sym))
            {
                node *newNode = new node;
                newNode->parent = temp;
                newNode->toThis = sym;
                // newNode->patTemp = temp_;
                temp->children.insert(std::pair(sym, newNode));
                temp = newNode;
            }
            else
            {
                temp = temp->children[sym];
            }
        }
        temp->isEnd = true;
        temp->patternNumbers.push_back(patternCounter);
        patternCounter++;
    }
    walk(start);

    // now need to create suffix links
}

void alg::AhoKorasik::completeTerminals(node *temp)
{
    std::queue<node *> q;
    q.push(temp);

    while (!q.empty())
    {
        auto v = q.front();
        q.pop();
        for (auto u : v->children)
        {
            if (!u.second->isVisited)
            {
                q.push(u.second);
                // std::cout<<u.second->patTemp<<" ";
                if (u.second->suffLink->isEnd)
                {
                    for (auto endNum : u.second->suffLink->patternNumbers)
                    {
                        u.second->patternNumbers.push_back(endNum); // load numbers of suffix links to the node list
                    }
                    u.second->isEnd = true;
                }
                u.second->isVisited = true;
            }
        }
    }
}

std::vector<std::pair<size_t, size_t>> alg::AhoKorasik::searh(std::string input)
{
    std::vector<std::pair<size_t, size_t>> out;
    node *temp = start;
    size_t i = 0;
    for (auto s : input)
    {
        do{
            if (temp->children.contains(s))
            {
                temp = temp->children[s];
                if (temp->isEnd)
                {
                    for (size_t patternNum : temp->patternNumbers)
                    {
                        out.push_back(std::pair<size_t, size_t>(patternNum, (i - patternLengths[patternNum] + 1)));
                    }
                }
                break;
            }
            else
            {
                temp = temp->suffLink;
            }
        } while ((temp != start)||(temp->children.contains(s)));
        i++;
    }
    return out;
}
