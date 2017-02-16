#define CITYBUFFER 62
#define BUFFER 128
#define BUFFERSIZE 255


struct PlacesInfo{
    double lat;
    double lon;
    char city [CITYBUFFER];
    char state [2];
};
struct node
{ 
    char data;
    bool isLeaf;
    bool isCity;
    PlacesInfo info [52];
    int count;
    node *child[BUFFER];
    node()
    {
        for (int i = 0; i < BUFFER; i++)
            child[i] = NULL;
    }
};
class trie
{ 
    private: 
        static trie instance;
        node *root;
    public: 
        trie();
        static trie* getInstance(){ return &instance; }
        node *new_node(int data);
        void removeSpace(char *str);
		void uppercase(char* str) ;
        void add(char* S, PlacesInfo info);

        int check(node *root, char *city, char* state, PlacesInfo& info);
 
        int checkroot(char* input, char* state, PlacesInfo& info);
};

