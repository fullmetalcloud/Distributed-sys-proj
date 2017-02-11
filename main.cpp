#include <iostream>
#include <vector>
#include <stdio.h>
#include <sstream>      // std::istringstream
#include <fstream>
#include <map>
using namespace std;

bool isStateValid(string state){

    const int nbr_states = 51; // including DC

    string states [nbr_states] = {"AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE",
                        "DC", "FL", "GA", "HI", "ID", "IL", "IN", "IA", "KS",
                        "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS", "MO",
                        "MT", "NE", "NV", "NH", "NJ", "NM", "NY", "NC", "ND",
                        "OH", "OK", "OR", "PA", "RI", "SC", "SD", "TN", "TX",
                        "UT", "VT", "VA", "WA", "WV", "WI", "WY"};

    for (int i=0; i < nbr_states; ++i) {
        if (states[i] == state)
            return true;
    }
    return false;
}
int main (int argc, char *argv[])
{
    string code;
    struct AirportInfo{
        double lat;
        double lon;
        string city;
        string state;
        double distance;
    };

    struct PlacesInfo{
        double lat;
        double lon;
        string state;
    };

    string state = argv[2];
    map<string, AirportInfo> airport_list;

    if (argc < 2) {
        printf ("usage: %s server_host\n", argv[0]);
        exit (1);
    }

    cout<<isStateValid(state)<<endl;

    ifstream airport_file("airport-locations.txt");
    ifstream places_file("places2k.txt");

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
                state = city_state.substr(city_state.length()-2);
                airport.city = temp_city;
                airport.state = state;
                airport_list[code] = airport;
            }
        }
    }

    for (const auto &c: airport_list){
        cout << c.first <<" "<< c.second.city <<" "<< c.second.state
             <<" "<< c.second.lat <<" "<< c.second.lon <<endl;

    }
    cout <<"test"<<endl;
    auto temp = airport_list.find("[DAB]")->second.city;
    cout << temp << endl;

    if(places_file){

        string line, word, city, skip, temp;

        while(getline(places_file,line)){
            if(!line.empty()){
                PlacesInfo city_details;
                istringstream record(line);
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

                cout << city <<" "<< city_details.state <<" "<< city_details
                        .lat <<" "<<
                city_details.lon<<endl;
            }
            else{cout<<"empty line"<<endl;}
        }
    }

    airport_file.close();
    places_file.close();
    exit (0);
}