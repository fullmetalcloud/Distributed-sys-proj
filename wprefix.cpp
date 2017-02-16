/* 
 * C++ Program to Implement Longest Prefix Matching
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <algorithm>
using namespace std;
#define BUFFER 256
/* 
 * node Declaration
 */
struct node
{ 
    wchar_t data; 
    node *child[BUFFER];
    node()
    {
        for (int i = 0; i < BUFFER; i++)
            child[i] = NULL;
    }
};
struct PlacesInfo{
    double lat;
    double lon;
    wstring state;
};
/* 
 * trie class Declaration
 */
class trie
{ 
    private: 
        node *root;
    public: 
        trie() 
        { 
            root = new_node(0);
        }

        node *new_node(int data) 
        {   
            node *Q = new node; 
            Q->data = data; 
            return Q; 
        }
 
        void add(wstring S) 
        { 
            node *cur = root; 
            for (int i = 0; i < S.length(); i++)
            {
                int temp = S[i] - L'A';
                wcout<<temp<<" ";
                if (!cur->child[temp]) 
                    cur->child[temp] = new_node(S[i]);
                cur = cur->child[temp]; 
            }
        }
 
        void check(node *cur, wstring S, int i) 
        { 
            if (cur) 
            { 
                int temp = S[i] - L'A';
                wcout<<cur->data<<" "<<temp;
                if (i < S.length()) 
                    check(cur->child[temp], S, i + 1); 
            }
        }
 
        void checkroot(wstring input) 
        { 
            wstring S = input;
            S.erase( remove( S.begin(), S.end(), L' ' ), S.end() );
            if (root && S.length() > 0 && S[0] >= L'A') 
                check(root->child[S[0] - L'A'],S,1); 
            else 
                wcout<<"Empty root \n"; 
        }
};

/* 
 * Main
 */
int main() 
{ 
    trie dict;
    wfstream places_file("places2k.txt");
    if(places_file){

        wstring line, word, city, skip, temp;
        while(getline(places_file,line)){
            if(!line.empty()){
                PlacesInfo city_details;
                wstringstream record(line);
                record>>temp;
                city_details.state = temp.substr(0,2);
                temp = temp.erase(0,9);
                record>>city;
                city = temp+city;
                record>>skip;
                record>>skip;
                record>>skip;
                record>>skip;
                record>>skip;
                record>>skip;
                record>>city_details.lat;
                record>>city_details.lon;

                dict.add(city);
                cout<<endl;
            }
            else{cout<<"empty line"<<endl;}
        }
    }

    wstring input;

    input = L"Alabaster";
    wcout<< input << endl;
    dict.checkroot(input);
    wcout<<endl;
    input = L"Chester-Chester";
    wcout<< input << endl;
    dict.checkroot(input);
    wcout<<endl;
    input = L"Bailey's Crossroads";
    wcout<< input << endl;
    dict.checkroot(input);
    wcout<<endl;
    input = L"Río Lajas";
    wcout<< input << endl;
    dict.checkroot(input);
    wcout<<endl;
    input = L"Río Cañas Abajo";
    wcout<< input << endl;
    dict.checkroot(input);
    wcout<<endl;

    return 0;
}