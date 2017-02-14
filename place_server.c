/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */
#include "errno.h"
#include "place.h"
#include "airport.h"
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <algorithm>

#define BUFFER 128
#define BUFFERSIZE 255
struct PlacesInfo{
    double lat;
    double lon;
    char city [62];
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
void removeSpace(char *str) {
  char *p1 = str, *p2 = str;
  do 
    while (*p2 == ' ')
      p2++;
  while (*p1++ = *p2++);
}
void uppercase(char* str) {
    char c;
    int i =0;
    while(str[i]){
        str[i] = toupper(str[i]);
        i++;
    }
}
/* 
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
            Q->isLeaf = false;
            Q->isCity = false;
            Q->count = 0;
            return Q; 
        }
        void add(char* S, PlacesInfo info) 
        { 
            node *cur = root;
            int temp;
            int size = strlen(S);
            
            for (int i = 0; i <= size; i++)
            {
                temp = S[i] - 'A';
                if (!cur->child[temp]) {
                    cur->child[temp] = new_node(S[i]);
                }
                if(i!=size){
                    cur = cur->child[temp];
                    cur->count++;
                }
            }
            cur->isCity = true;
            temp = cur->count;
            cur->info[temp].lat = info.lat;
            cur->info[temp].lon = info.lon;
            strncpy(cur->info[temp].state, info.state,2);
            strcpy(cur->info[temp].city, S);
        }

        int check(node *root, char *city, char* state, PlacesInfo& info) 
        { 
            node *cur = root;
            node *nodeCheck;
            
            int count = 0;
            int stateCount = 0;
            int checkCount = 0;
            int temp, status = 1;
            int size = strlen(city);
            while(count < size)
            {
                temp = city[count] - 'A';
                if(cur->child[temp])
                {
                    count++;
                    cur = cur->child[temp];
                }
                else
                    return 1;  
            }

            if(cur->isCity)
            {
                while(stateCount <= cur->count){
                    if(strncmp(state,cur->info[stateCount].state,2)==0)
                    {
                        status = 0;
                        info.lat = cur->info[stateCount].lat;
                        info.lon = cur->info[stateCount].lon;
                        strcpy(info.state,cur->info[stateCount].state);
                        strcpy(info.city,cur->info[stateCount].city);
                        break;
                    }
                    else{
                        status = 1;
                        stateCount++;
                    }
                }
            } 
            else
            {
                while(cur->count==1&&!cur->isCity)
                {
                    nodeCheck = cur->child[checkCount];
                    while(nodeCheck ==0)
                    {

                        nodeCheck = cur->child[checkCount];
                        checkCount++;
                    }
                    cur = nodeCheck;
                    checkCount = 0;
                }
                if(cur->isCity)
                {
                    while(stateCount <= cur->count){
                        if(strncmp(state,cur->info[stateCount].state,2)==0)
                        {
                            status = 0;
                            info.lat = cur->info[stateCount].lat;
                            info.lon = cur->info[stateCount].lon;
                            strcpy(info.state,cur->info[stateCount].state);
                            strcpy(info.city,cur->info[stateCount].city);
                            break;
                        }
                        else{
                            status = 1;
                            stateCount++;
                        }
                    }
                }
                else
                    status = 2;
            }
            return status;
        }
 
        int checkroot(char* input, char* state, PlacesInfo& info) 
        { 
            if (root && sizeof(input) > 0 && input[0] >= 'A') 
                return check(root, input, state, info); 
            else 
                return 1; 
        }
};

trie *
startup()
{
    static trie dict;
    trie *result;
    FILE *pFile = fopen("places2k.txt","r");
    char city[62];
    char line [BUFFERSIZE];
    char temp[10];
    PlacesInfo city_details;
    int j = 0;
    while(fgets(line, BUFFERSIZE, pFile)!=NULL)
    {
        strncpy(city, line+9, 62);
        removeSpace(city);
        strcpy(city_details.city,city);
        strncpy(temp, line, 2);
        strcpy(city_details.state, temp);
        memset(temp,' ',10);
        uppercase(city);
        strncpy(temp, line+143, 10);
        city_details.lat = atof(temp);
        memset(temp,' ',10);
        strncpy(temp, line+153, 11);
        city_details.lon = atof(temp);         
        dict.add(city, city_details);
        memset(city,' ',62);
        memset(temp,' ',10);
        
    }
    fclose(pFile);

    result = &dict;
    return result;
}

findplace_ret *
findplace_1_svc(placename *argp, struct svc_req *rqstp)
{
    static findplace_ret result;
    static trie *prefixTree;
    //check if prefix tree is made
    if(prefixTree==0)
        prefixTree = startup();
    trie dict = *prefixTree;  
	CLIENT *clnt;
	char hostname[] = "cs1.seattleu.edu";
	char *host = hostname;
	placename input = *argp;
	findairport_ret  *result_1;
	airportlocation  findairport_1_arg;
	airportlist curr;
	placelist newlist;
	placelist *listptr;
	PlacesInfo info;
    int status, count =0;

    //free mem of last result
	xdr_free ((xdrproc_t) xdr_findplace_ret,  (char *) &result);

    //search tree for match
    uppercase(input.city);
    removeSpace(input.city);
    uppercase(input.state);
    status  = dict.checkroot(input.city, input.state, info);
    //store info into return value
    result.findplace_ret_u.ans.status = status;
    result.findplace_ret_u.ans.city = (char *)malloc(strlen(info.city));
	result.findplace_ret_u.ans.state = (char *)malloc(strlen(input.state));
	strcpy(result.findplace_ret_u.ans.city, info.city);
    strcpy(result.findplace_ret_u.ans.state, input.state);
	result.findplace_ret_u.ans.latitude = (float)info.lat;
	result.findplace_ret_u.ans.longitude = (float)info.lon; 

    //send lat and long input to airport server
	findairport_1_arg.latitude = (float)info.lat;
	findairport_1_arg.longitude = (float)info.lon; 
	#ifndef	DEBUG
		clnt = clnt_create (host, AIRPORT_DIRPROG, AIRPORT_DIR_VERS, "udp");
		if (clnt == NULL) {
			clnt_pcreateerror (host);
			exit (1);
		}
	#endif	

	result_1 = findairport_1(&findairport_1_arg, clnt);
	if (result_1 == (findairport_ret *) NULL) {
		clnt_perror (clnt, "call failed");
	} else {

        //transfer airport result to place result
		curr = result_1->findairport_ret_u.list;
		listptr = &result.findplace_ret_u.ans.list;
		while(curr){
			newlist = *listptr = (place *) malloc(sizeof(place));
			newlist->city = (char *)malloc(strlen(curr->city));
			newlist->state = (char *)malloc(strlen(curr->state));
			newlist->code = (char *)malloc(strlen(curr->code));
			//newlist->distance = malloc(sizeof(int));
			strcpy(newlist->city,curr->city);
			strcpy(newlist->state,curr->state);
			strcpy(newlist->code,curr->code);	
			newlist->distance = curr->distance;		
			listptr = &newlist->next;
			curr = curr->next;
		}
	*listptr = NULL;
	result.placeerr = 0;
	}

	#ifndef	DEBUG
		xdr_free ((xdrproc_t) xdr_findairport_ret,  (char *) &result_1);
		clnt_destroy (clnt);
	#endif
	return &result;
}
