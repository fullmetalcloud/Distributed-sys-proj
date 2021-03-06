/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "place.h"
#include "prefixmatch.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <algorithm>

using namespace std;

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

/*
Trie Tree/Prefix Match Initialization
*/
trie::trie() 
{ 
    root = new_node(0);
}
trie trie::instance;
node* trie::new_node(int data) 
{   
    node *Q = new node; 
    Q->data = data;
    Q->isLeaf = false;
    Q->isCity = false;
    Q->count = 0;
    return Q; 
}
void trie::removeSpace(char *str) {
  char *p1 = str, *p2 = str;
  do 
    while (*p2 == ' ')
      p2++;
  while (*p1++ = *p2++);
}
void trie::uppercase(char* str) {
    char c;
    int i =0;
    while(str[i]){
        str[i] = toupper(str[i]);
        i++;
    }
}

void trie::add(char* S, PlacesInfo info) 
{ 
    //add to trie tree
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
    //cout<<S<<endl;
}

int trie::check(node *root, char *city, char* state, PlacesInfo& info) 
{ 
    //search trie tree for prefix matching based on word city
    //status => 0 = found, 1 = not found, 2 = ambiguous city name
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
    //if perfect match found for city
    if(cur->isCity)
    {
        //check if given state matches with city
        while(stateCount <= cur->count){
            if(strncmp(state,cur->info[stateCount].state,2)==0)
            {
                status = 0;
                info.lat = cur->info[stateCount].lat;
                info.lon = cur->info[stateCount].lon;
                strncpy(info.state,cur->info[stateCount].state,2);
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
        //check if there exists a SINGLE city that could match with city
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
            //check if state exists with found city
            while(stateCount <= cur->count){
                if(strncmp(state,cur->info[stateCount].state,2)==0)
                {
                    status = 0;
                    info.lat = cur->info[stateCount].lat;
                    info.lon = cur->info[stateCount].lon;
                    strncpy(info.state,cur->info[stateCount].state,2);
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
            //ambiguous city name
            status = 2;
    }
    return status;
}

int trie::checkroot(char* input, char* state, PlacesInfo& info) 
{ 
    //check if trie tree exists and input is a word
    if (root && sizeof(input) > 0 && input[0] >= 'A') 
        return check(root, input, state, info); 
    else 
        return 1; 
}
void
startup(trie* ptr)
{
    //initialize trie tree with places2k.txt file
    trie dict = *ptr;
    
    FILE *pFile = fopen("places2k.txt","r");
    char city[CITYBUFFER];
    char line [BUFFERSIZE];
    char temp[10];
    PlacesInfo city_details;
    int j = 0;
    while(fgets(line, BUFFERSIZE, pFile)!=NULL)
    {
        strncpy(city, line+9, CITYBUFFER);
        dict.removeSpace(city);
        strcpy(city_details.city,city);
        strncpy(city_details.state, line, 2);
        memset(temp,' ',10);
        dict.uppercase(city);
        strncpy(temp, line+143, 10);
        city_details.lat = atof(temp);
        memset(temp,' ',10);
        strncpy(temp, line+153, 11);
        city_details.lon = atof(temp);         
        dict.add(city, city_details);
        memset(city,' ',CITYBUFFER);
        memset(temp,' ',10);
        
    }
    fclose(pFile);
}
static void
places_dirprog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		placename findplace_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case FINDPLACE:
		_xdr_argument = (xdrproc_t) xdr_placename;
		_xdr_result = (xdrproc_t) xdr_findplace_ret;
		local = (char *(*)(char *, struct svc_req *)) findplace_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

    //start and save trie tree with singleton instance
	trie *prefixTree = trie::getInstance();
	startup(prefixTree);
    
	pmap_unset (PLACES_DIRPROG, PLACES_DIR_VERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, PLACES_DIRPROG, PLACES_DIR_VERS, places_dirprog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (PLACES_DIRPROG, PLACES_DIR_VERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, PLACES_DIRPROG, PLACES_DIR_VERS, places_dirprog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (PLACES_DIRPROG, PLACES_DIR_VERS, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
