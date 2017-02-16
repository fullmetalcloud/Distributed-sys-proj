const MAXLEN = 255;
typedef string nametype<MAXLEN>;
typedef struct place* placelist;
/* a node in the airport list */
struct place{
 nametype city;
 nametype state;
 nametype code;
 int distance;
 placelist next;
};
struct placename{
 nametype city;
 nametype state;
};
struct answer{
 placelist list;
 nametype city;
 nametype state;
 float latitude;
 float longitude;
 int status;
};
/*result of a findplace operation*/
union findplace_ret switch (int placeerr) {
 case 0: /*no error, return airport listing and location*/
  answer ans;
 default:
 void; /*error occured, nothing returned*/
};

/*IDL program*/
program PLACES_DIRPROG {
 version PLACES_DIR_VERS {
  findplace_ret FINDPLACE(placename) = 1;
 } = 1;
} = 0x34758651; 