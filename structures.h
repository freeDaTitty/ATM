/*
* In header-ul acesta am realizat definirea
* fiecarei structuri in parte.
*/

typedef struct History{
    char *history;
    struct History *next_history;
}HCell, *HList, **HAList;

typedef struct Cards_List{
    char *card_number;
    char *pin;
    char *expiry_date;
    char *cvv;
    int balance;
    char *status;
    int failed_pin;
    HList history;
    struct Cards_List *next_card;
}LCCell, *LCList, **LCAList;

typedef struct SCards_List LSCCell;
typedef struct SCards_List *LSCList;
typedef struct SCards_List **LSCAList;

struct SCards_List{
    LCList card_list;
    LSCList next_lsc;
};