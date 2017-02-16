/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "place.h"
#include "errno.h"
#include <string>
#include <iostream>

using namespace std;

const int nbr_states = 52; // including DC and Puerto Rico
//function
bool
isStateValid(string state){
    string states [nbr_states] = {"AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE",
                        "DC", "FL", "GA", "HI", "ID", "IL", "IN", "IA", "KS",
                        "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS", "MO",
                        "MT", "NE", "NV", "NH", "NJ", "NM", "NY", "NC", "ND",
                        "OH", "OK", "OR", "PA", "RI", "SC", "SD", "TN", "TX",
                        "UT", "VT", "VA", "WA", "WV", "WI", "WY", "PR"};

    for (int i=0; i < nbr_states; ++i) {
        if (states[i] == state){
        	return true;
        }
            
    }
    return false;
}

void
places_dirprog_1(char *host, char *city, char *state)
{
	cout<<city<<endl;
	CLIENT *clnt;
	findplace_ret  *result_1;
	placename  findplace_1_arg;
	placelist curr;
	findplace_1_arg.city = city;
	findplace_1_arg.state = state;
#ifndef	DEBUG
	clnt = clnt_create (host, PLACES_DIRPROG, PLACES_DIR_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = findplace_1(&findplace_1_arg, clnt);
	if (result_1 == (findplace_ret *) NULL) {
		clnt_perror (clnt, "call failed");
	} else {
		// successful result from server
		if(result_1->placeerr != 0) {
			errno = result_1->placeerr;
			perror("FAILED");
			exit(1);
		}
		// print directories
		if (result_1->findplace_ret_u.ans.status==1){
			printf("%s, %s NOT FOUND.\n",city,state);
		}
		else if(result_1->findplace_ret_u.ans.status==2){
			printf("AMBIGUOUS CITY NAME: %s. BE MORE SPECIFIC.\n",city);
		}else {
			printf("%s, %s: %f %f \n", result_1->findplace_ret_u.ans.city, result_1->findplace_ret_u.ans.state
				, result_1->findplace_ret_u.ans.latitude, result_1->findplace_ret_u.ans.longitude);
			curr = result_1->findplace_ret_u.ans.list;
			while(curr) {
				printf("code=%s, name=%s, state=%s, distance: %u \n",curr->code, curr->city, curr->state, curr->distance);
				curr = curr->next;
			}
		}
	}
#ifndef	DEBUG
	xdr_free((xdrproc_t) xdr_findplace_ret,  (char *) &result_1);
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 4) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	string state = argv[3];
	if(!isStateValid(state)){
		printf("INVALID STATE. PLEASE RE-ENTER YOUR DETAILS WITH VALID STATE.\n");
		exit(1);
	}
	places_dirprog_1 (host, argv[2],argv[3]);
	exit (0);
}
