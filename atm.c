/*
* Dinu Daniel - 314CB
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structures.h"
#include "allocations.h"
#include "administrators.h"
#include "clients.h"

int cards_digits_sum(char *card_number){
    int S = 0;
    char *i;
    for(i = card_number; *i; i++){
        S += (int)(*i) - 48;
    }
    return S;
}

int main(){
    FILE *input_file;
    input_file = fopen("input.in", "r");

    if(!input_file){
        fprintf(stderr, "%s", "Can't open input file!\n");
        return(-1);
    }

    FILE *output_file;
    output_file = fopen("output.out", "w");

    if(!output_file){
        fprintf(stderr, "%s", "Can't open output file!\n");
        return(-1);
    }

    /*
    * In variabila input voi retine fiecare linie din fisierul de input
    * iar in variabila buffer voi retine operatiile care vor fi
    * efectuate asupra unui card pentru istoric.
    */

    char *input;
    input = calloc(80, sizeof(char));

    if(!input){
        return(-1);
    }

    /*
    * In variabila pos retin pozitia din submultime unde voi pune
    * cardul respectiv, iar prin variabila last_pos voi retine
    * pos ul precedent, astfel incat voi putea adauga submultimi
    * de tip LSC cat este nevoie, ci nu in plus.
    */

    int pos, last_pos = 0;
    int max_number_of_cards,
        number_of_cards = 0;
    fgets(input, 10, input_file);
    max_number_of_cards = atoi(input);

    LSCList LSC = NULL;

    /*
    * Voi lua pe rand fiecare linie si o voi diviza in "cuvinte"
    * pentru a retine fiecare parametru al fiecarei operatii, cat
    * si operatia in sine.
    */

    while(fgets(input, 80, input_file) != NULL){

        /*
        * Pentru a nu pierde informatiile, voi dubla prin variabila
        * input2 fiecare linie din fisierul de input. Variabila
        * buffer o voi folosi pentru a lua liniile pe rand, iar 
        * variabila line o voi folosi pentru a impartii liniile
        * in cuvinte.
        */

        char *line = (char *)calloc(80, sizeof(char));
        char *input2 = (char *)calloc(80, sizeof(char));
        memcpy(input2, input, strlen(input));
        line = strtok(input2, "\n");

        char *buffer;
        buffer = (char *)calloc(80, sizeof(char));
        buffer = strtok(input, "\n");

        while(buffer){
            char *p = strtok(line, " ");

            /*
            * Din fiecare linie in parte, voi retine primul
            * grup de caractere pentru a afla ce functie trebuie
            * sa apelez. Pentru fiecare functie in parte, voi
            * retine informatiile de dupa, informatii ce vor
            * fi necesare in apelarea functiilor respective.
            */

            if(strcmp(p, "add_card") == 0){
                p = strtok(NULL, " ");
                char *card_number;
                card_number = (char*)calloc(strlen(p), sizeof(char));
                if(!card_number){
                    return(-1);
                }
                card_number = strdup(p);

                p = strtok(NULL, " ");
                char *pin;
                pin = (char*)calloc(strlen(p), sizeof(char));
                if(!pin){
                    return(-1);
                }
                pin = strdup(p);

                p = strtok(NULL, " ");
                char *expiry_date;
                expiry_date = (char*)calloc(strlen(p), sizeof(char));
                if(!expiry_date){
                    return(-1);
                }
                expiry_date = strdup(p);

                p = strtok(NULL, " ");
                char *cvv;
                cvv = (char*)calloc(strlen(p), sizeof(char));
                if(!cvv){
                    return(-1);
                }
                cvv = strdup(p);

                /*
                * Variabila card_exists este folosita pentru a verifica
                * daca respectivul card exista deja in baza de date. In
                * cazul in care cardul exista deja, voi afisa mesajul
                * corespunzator. Altfel, voi adauga cardul in baza de
                * date.
                */

                int card_exists = 0;

                /*
                * Parcug lista cu o copie pentru a nu pierde inceputul
                * acesteia pentru a verifica existenta cardului ce
                * ar trebui adaugat, in cazul in care baza de date
                * nu este vida.
                */

                if(LSC != NULL){
                    LSCList LSC_copy = LSC;

                    while(LSC_copy != NULL){
                        LCList LC_copy = LSC_copy->card_list;
                        while(LC_copy != NULL){
                            if(strcmp(LC_copy->card_number, card_number) == 0){
                                card_exists = 1;
                                break;
                            }
                            LC_copy = LC_copy->next_card;
                        }
                        LSC_copy = LSC_copy->next_lsc;
                    }
                    free(LSC_copy);
                }

                if(card_exists == 0){
                    pos = cards_digits_sum(card_number) % max_number_of_cards;
                    add_card(&LSC, card_number, pin, 
                        expiry_date, cvv, pos, &last_pos);

                    number_of_cards++;
                }
                else{
                    fprintf(output_file, "The card already exists\n");
                }

                if(number_of_cards == max_number_of_cards){
                    max_number_of_cards *= 2;
                }

                free(card_number);
                free(pin);
                free(expiry_date);
                free(cvv);
            }
            else if(strcmp(p, "delete_card") == 0){

                p = strtok(NULL, " ");
                char *card_number;
                card_number = (char*)calloc(strlen(p), sizeof(char));
                if(!card_number){
                    return(-1);
                }
                card_number = strdup(p);

                delete_card(&LSC, card_number);

                free(card_number);
            }
            else if(strcmp(p, "insert_card") == 0){

                p = strtok(NULL, " ");
                char *card_number;
                card_number = (char*)calloc(strlen(p), sizeof(char));
                if(!card_number){
                    return(-1);
                }
                card_number = strdup(p);

                p = strtok(NULL, " ");
                char *pin;
                pin = (char*)calloc(strlen(p), sizeof(char));
                if(!pin){
                    return(-1);
                }
                pin = strdup(p);

                insert_card(LSC, card_number, pin, buffer, output_file);

                free(card_number);
                free(pin);
            }
            else if(strcmp(p, "show") == 0){

                p = strtok(NULL, " ");
                char *card_number;
                card_number = (char*)malloc(sizeof(char));

                if(!card_number){
                    return(-1);
                }

                if(!p){
                    card_number = NULL;
                }
                else{
                    card_number = strdup(p);
                }
                
                show(LSC, card_number, output_file);
                free(card_number);
            }
            else if(strcmp(p, "cancel") == 0){
                p = strtok(NULL, " ");
                char *card_number;
                card_number = (char*)malloc(sizeof(char));

                if(!card_number){
                    return(-1);
                }
                card_number = strdup(p);

                cancel(LSC, card_number, buffer, output_file);
                free(card_number);
            }
            else if(strcmp(p, "unblock_card") == 0){
                p = strtok(NULL, " ");
                char *card_number;
                card_number = (char*)malloc(sizeof(char));

                if(!card_number){
                    return(-1);
                }
                card_number = strdup(p);

                unblock_card(LSC, card_number);
                free(card_number);
            }
            else if(strcmp(p, "pin_change") == 0){
                p = strtok(NULL, " ");
                char *card_number;
                card_number = (char*)malloc(sizeof(char));

                if(!card_number){
                    return(-1);
                }
                card_number = strdup(p);

                p = strtok(NULL, " ");
                char *new_pin;
                new_pin = (char*)calloc(strlen(p), sizeof(char));
                if(!new_pin){
                    return(-1);
                }
                new_pin = strdup(p);

                pin_change(LSC, card_number, new_pin, buffer, output_file);
                free(card_number);
                free(new_pin);            
            }
            else if(strcmp(p, "balance_inquiry") == 0){
                p = strtok(NULL, " ");
                char *card_number;
                card_number = (char*)malloc(sizeof(char));

                if(!card_number){
                    return(-1);
                }
                card_number = strdup(p);

                balance_inquiry(LSC, card_number, buffer, output_file);
                free(card_number);
            }
            else if(strcmp(p, "recharge") == 0){
                p = strtok(NULL, " ");
                char *card_number;
                card_number = (char*)malloc(sizeof(char));

                if(!card_number){
                    return(-1);
                }
                card_number = strdup(p);

                p = strtok(NULL, " ");
                int recharge_amount;
                recharge_amount = atoi(p);

                recharge(LSC, card_number, recharge_amount, buffer, output_file);
                free(card_number);
            }
            else if(strcmp(p, "cash_withdrawal") == 0){
                p = strtok(NULL, " ");
                char *card_number;
                card_number = (char*)malloc(sizeof(char));

                if(!card_number){
                    return(-1);
                }
                card_number = strdup(p);

                p = strtok(NULL, " ");
                int cash;
                cash = atoi(p);

                cash_withdrawal(LSC, card_number, cash, buffer, output_file);
                free(card_number);
            }
            else if(strcmp(p, "transfer_funds") == 0){
                p = strtok(NULL, " ");
                char *card_number_source;
                card_number_source = (char*)malloc(sizeof(char));

                if(!card_number_source){
                    return(-1);
                }
                card_number_source = strdup(p);

                p = strtok(NULL, " ");
                char *card_number_dest;
                card_number_dest = (char*)malloc(sizeof(char));

                if(!card_number_dest){
                    return(-1);
                }
                card_number_dest = strdup(p);

                p = strtok(NULL, " ");
                int cash;
                cash = atoi(p);

                transfer_funds(LSC, card_number_source,
                card_number_dest, cash, buffer, output_file);

                free(card_number_source);
                free(card_number_dest);
            }
            else if(strcmp(p, "reverse_transaction") == 0){
                p = strtok(NULL, " ");
                char *card_number_source;
                card_number_source = (char*)malloc(sizeof(char));

                if(!card_number_source){
                    return(-1);
                }
                card_number_source = strdup(p);

                p = strtok(NULL, " ");
                char *card_number_dest;
                card_number_dest = (char*)malloc(sizeof(char));

                if(!card_number_dest){
                    return(-1);
                }
                card_number_dest = strdup(p);

                p = strtok(NULL, " ");
                int cash;
                cash = atoi(p);

                reverse_transaction(LSC, card_number_source, 
                card_number_dest, cash, buffer, output_file);

                free(card_number_source);
                free(card_number_dest);
            }
            buffer = strtok(NULL, "\n");
        }
        free(buffer);
    }
    free(input);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
