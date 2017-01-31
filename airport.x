const MAXLENGTH = 255;
typedef string namelen<MAXLENGTH>;
typedef struct airport* airportlist;
/* a node in the airport list */
struct airport{
 namelen city;
 namelen state;
 namelen code;
 int distance;
 airportlist next;
};
struct airportlocation{
 int latitude;
 int longitude;
};

/*result of a findairport operation*/
union findairport_ret switch (int airporterr) {
 case 0: /*no error, return airport listing and location*/
  airportlist list;
 default:
 void; /*error occured, nothing returned*/
};

/*IDL program*/
program AIRPORT_DIRPROG {
 version AIRPORT_DIR_VERS {
 findairport_ret FINDAIRPORT(airportlocation) = 1;
 } = 1;
} = 0x34758657; 