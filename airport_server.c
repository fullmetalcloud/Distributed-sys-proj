#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <math.h>
#include "airport.h"
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;

//global variables
#define pi 3.14159265358979323846
const int k = 2; // 2 dimension kd-tree
const int n = 5; // 5 nearest neighbors
// end globals

struct AirportInfo{
    float lat;
    float lon;
    string city;
    string state;
    float distance;
};

struct near_candidates{
    string code;
    float distance;
};
//vector<near_candidates> nc;

bool operator <(const near_candidates &lhs, const near_candidates &rhs){
    return (lhs.distance < rhs.distance);
}

struct Node{
    float point [k]; //array to store lat and long
    string airport_code;
    Node* left;
    Node* right;
};

// distance calc
float deg2rad(float deg) {
    return (deg * pi / 180);
}

float rad2deg(float rad) {
    return (rad * 180 / pi);
}

float distance(float lat1, float lon1, float lat2, float lon2) {
    float theta, dist;
    theta = lon1 - lon2;
    dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) *
                                                     cos(deg2rad(lat2)) * cos(deg2rad(theta)); dist = acos(dist);
    dist = rad2deg(dist);
    return (dist * 60 * 1.1515);
}

// kd tree node
struct Node* newNode(float lat, float lon, string airport_code){
    struct Node* temp = new Node;
    float coordinates[k]={lat,lon};

    for (int i=0; i<k;++i){
        temp->point[i]= coordinates[i];
    }
    temp->airport_code=airport_code;
    temp->left=NULL;
    temp->right=NULL;

    return temp;
}

// knn search
void nearestAirSearch(vector<near_candidates> &nc, map<string, struct
        AirportInfo>& airport_list, Node* root, float user_lat, float
                      user_lon, int depth){

    if(root==NULL){
        return;
    }

    int curr_dim = depth % k; // current tree dimension for select of x or y
    float coordinates[k]={user_lat,user_lon};

    // compute dist and update array
    float node_lat, node_lon;
    node_lat = airport_list.find(root->airport_code)->second.lat;
    node_lon = airport_list.find(root->airport_code)->second.lon;
    float dist = distance(user_lat,user_lon, node_lat,node_lon);

    airport_list.find(root->airport_code)->second.distance = dist;

    if (nc.size()!=5){
        nc.push_back({root->airport_code,dist});

        if(nc.size()==5){
            make_heap(nc.begin(), nc.end());
        }
    }
    else {
        if (nc[0].distance > dist) {
            pop_heap(nc.begin(),nc.end());
            nc[n-1] = {root->airport_code, dist};
            push_heap(nc.begin(),nc.end());
        }
    }

    bool is_left = (coordinates[curr_dim] < root->point[curr_dim]);

    if(is_left){
        nearestAirSearch(nc, airport_list, root->left,
                                user_lat,user_lon, depth+1);
    }
    else{
        nearestAirSearch(nc, airport_list, root->right,
                                user_lat,user_lon, depth+1);
    }

    float temp_coord;
    if(curr_dim==0){
        temp_coord = airport_list.find(nc[0].code)->second.lat;
    }
    else{
    temp_coord = airport_list.find(nc[0].code)->second.lon;}

    if (nc.size()!=5 || fabs(coordinates[curr_dim] - root->point[curr_dim])
                        < nc[0].distance){

        if(is_left){
            nearestAirSearch(nc, airport_list, root->right,
                             user_lat,user_lon, depth+1);
        }
        else{
            nearestAirSearch(nc, airport_list, root->left,
                             user_lat,user_lon, depth+1);
        }
    }
}

// helper to insert node in kd tree
Node* insertHelper(Node* root, float lat, float lon, string airport_code, int
depth){
    // check if kd-tree empty
    if(root==NULL){
        return newNode(lat, lon, airport_code);
    }

    int curr_dim = depth % k; // current tree dimension for select of x or y
    float coordinates[k]={lat,lon};

    //insert left
    if(coordinates[curr_dim]<root->point[curr_dim]){
        root->left=insertHelper(root->left,lat,lon,airport_code,depth+1);
    }
    // insert right
    else{
        root->right=insertHelper(root->right,lat,lon,airport_code,depth+1);
    }

    return root;
}

// insert node in kd tree
Node* insert(Node* root, float lat, float lon, string airport_code){
    return insertHelper(root, lat, lon, airport_code,0);
}

// build kd tree
Node* buildTree(map<string, struct AirportInfo>& airport_list, Node *root){
    //struct Node* root = NULL;
    map<string, struct AirportInfo>::iterator it;
    for (it = airport_list.begin(); it != airport_list.end(); it ++){
        root=insert(root,it->second.lat,it->second.lon,it->first);
    }
    return root;
}

// airport hashtable
void airportMap(map<string, struct AirportInfo>& airport_list){
    ifstream airport_file("airport-locations.txt");
    string code;

    if(airport_file){

        string line, word, city_state, temp_city, temp_state;
        getline(airport_file,line); // skip header

        while(getline(airport_file,line)){
            if(!line.empty()){
                AirportInfo airport;
                istringstream record(line);
                record>>code;
                record>>airport.lat;
                record>>airport.lon;
                getline(record,city_state);
                temp_city = city_state.substr(0,city_state.length()-3);
                temp_state = city_state.substr(city_state.length()-2);
                airport.city = temp_city;
                airport.state = temp_state;
                airport_list[code] = airport;
            }
        }
    }

    airport_file.close();

    //for (const auto &c: airport_list){
    //    cout << c.first <<" "<< c.second.city <<" "<< c.second.state
    //         <<" "<< c.second.lat <<" "<< c.second.lon <<endl;
    //}
}

void strtochar(char* c, string str)
{

}

findairport_ret *
findairport_1_svc(airportlocation *argp, struct svc_req *rqstp)
{
	static findairport_ret result;
	airportlocation args = *argp;
	airportlist newlist;
	airportlist *listptr;
	char *cityname;
	char *statename;
	char * code;
	int distance = 10;
	float user_lat = args.latitude;
	float user_lon = args.longitude;

	xdr_free ((xdrproc_t) xdr_findairport_ret, (char *) &result);

    near_candidates temp;
    AirportInfo airportTemp;
	//double user_lat = 32;
    //double user_lon = -87;
    struct Node* root = NULL;
    vector<near_candidates> nc;
    map<string, AirportInfo> airport_list;
	/*search algo and distance calculation here*/
	airportMap(airport_list);
    root = buildTree(airport_list, root);
    nearestAirSearch(nc,airport_list, root, user_lat, user_lon,0);
    sort_heap(nc.begin(),nc.end());
	/*linked list for place server*/
	listptr = &result.findairport_ret_u.list;
	for (unsigned i=0;i<n;i++){
	    temp = nc[i];
        airportTemp = airport_list[temp.code];
        cityname = new char[airportTemp.city.length()+1];
        strcpy(cityname,airportTemp.city.c_str());
        statename = new char[airportTemp.state.length()+1];
        strcpy(statename,airportTemp.state.c_str());
        code = new char[temp.code.length()+1];
        strcpy(code,temp.code.c_str());
        distance = temp.distance;
		newlist = *listptr = (airport *) malloc(sizeof(airport));
		newlist->city = (char *)malloc(strlen(cityname));
		newlist->state = (char *)malloc(strlen(statename));
		newlist->code = (char *)malloc(strlen(code));
		//newlist->distance = (int) malloc(sizeof(distance));
		strcpy(newlist->city,cityname);	
		strcpy(newlist->state,statename);	
		strcpy(newlist->code,code);		
		newlist->distance = distance;		
		listptr = &newlist->next;
        delete [] cityname;
        delete [] statename;
        delete [] code;
	}
	*listptr = NULL;
	result.airporterr = 0; 
	return &result;
}
