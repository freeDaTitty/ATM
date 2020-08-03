/*
* Primele 2 functii sunt pentru a elibera continutul
* din listele de carduri, luand separat o functie
* ce dezaloca memoria din istoric.
*/

void free_History(HAList H){
    HList H_aux;

    while(*H != NULL){
        H_aux = *H;
        *H = (*H)->next_history;
        free(H_aux->history);
        free(H_aux);
    }
}

void free_LC(LCAList LC){
    LCList LC_aux = NULL;
    while(*LC != NULL){
        LC_aux = *LC;
        *LC = (*LC)->next_card;
        HList H = LC_aux->history;

        free_History(&H);

        free(LC_aux->card_number);
        free(LC_aux->pin);
        free(LC_aux->expiry_date);
        free(LC_aux->cvv);
        free(LC_aux->status);
    }
}

/*
* In functia de add_card voi crea celule noi de tip corespunzator.
* In cazul in care baza de date este goala, voi constitui celule
* de tip baza de date pana cand voi ajunge in pozitia in care
* trebuie sa adaug un card. Altfel, voi parcurge baza de date pana
* cand va fi nevoie sa adaug un card. Am retinut prin variabila
* last_pos numarul de celule de tip baza de date pentru a nu adauga
* in plus sau pentru a nu le pierde pe cele vechi.
*/

void add_card(LSCAList ALSC, char *card_number, char *pin,
        char *expiry_date, char *cvv, int pos, int *last_pos){
    LCList LC;
    LC = AllotLC_Cell(card_number, pin, expiry_date, cvv);

    if(!LC){
        exit(EXIT_FAILURE);
    }

    if((*ALSC) == NULL){
        LSCList LSC_aux;
        LSC_aux = AllotLSC_Cell(LC);

        if(!LSC_aux){
            exit(EXIT_FAILURE);
        }
        
        while(pos > (*last_pos)){
            LSCList LSC;
            LSC = AllotLSC_Cell(NULL);
            if(!LSC){
                exit(EXIT_FAILURE);
            }

            LSC->next_lsc = LSC_aux;
            LSC_aux = LSC;
            (*last_pos)++;
        }
        (*ALSC) = LSC_aux;
    }
    else{

        /*
        * Acest caz observa daca am de adaugat doar o singura
        * celula
        */

        if(pos - (*last_pos) == 1){
            LSCList ALSC_copy = (*ALSC);
            while(ALSC_copy->next_lsc != NULL){
                ALSC_copy = ALSC_copy->next_lsc;
            }

            LSCList LSC_aux;
            LSC_aux = AllotLSC_Cell(LC);

            if(!LSC_aux){
                exit(EXIT_FAILURE);
            }

            ALSC_copy->next_lsc = LSC_aux;
            (*last_pos)++;
        }

        /*
        * In acest caz verific daca am de adaugat mai mult de
        * o celule, caz in care voi adauga celule goale pana la ultima,
        * iar la final voi adauga celula cu continutul corespunzator.
        */

        else if(pos - (*last_pos) > 1){
            LSCList ALSC_copy = (*ALSC);
            while(ALSC_copy->next_lsc != NULL){
                ALSC_copy = ALSC_copy->next_lsc;
            }

            while(pos != (*last_pos) + 1){
                LSCList LSC_aux;
                LSC_aux = AllotLSC_Cell(NULL);

                if(!LSC_aux){
                    exit(EXIT_FAILURE);
                }

                ALSC_copy->next_lsc = LSC_aux;
                ALSC_copy = LSC_aux;
                (*last_pos)++;
            }

            LSCList LSC;
            LSC = AllotLSC_Cell(LC);

            if(!LSC){
                exit(EXIT_FAILURE);
            }

            ALSC_copy->next_lsc = LSC;
        }

        /*
        * Aici verific daca am de adaugat un card intr-o celula
        * deja alocata. Daca da, parcurg lista pana la celula
        * unde trebuie sa ma opresc si adaug cardul.
        */

        else if(pos < (*last_pos)){
            LSCList ALSC_copy = (*ALSC);

            int pos_copy = pos;
            while(pos_copy != 0){
                ALSC_copy = ALSC_copy->next_lsc;
                pos_copy--;
            }

            if(ALSC_copy->card_list != NULL){
                LC->next_card = ALSC_copy->card_list;
                ALSC_copy->card_list = LC;
            }
            else{
                ALSC_copy->card_list = LC;
            }
        }

        /*
        * Daca am de adaugat o celula fix unde m-am oprit
        * data trecuta sau pe ultima pozitie creata, voi
        * parcurge baza de date pana la final si voi
        * adauga cardul.
        */

        else if(pos == (*last_pos)){
            LSCList ALSC_copy = (*ALSC);

            int pos_copy = pos;
            while(pos_copy != 0){
                ALSC_copy = ALSC_copy->next_lsc;
                pos_copy--;
            }

            if(ALSC_copy->card_list != NULL){
                LC->next_card = ALSC_copy->card_list;
                ALSC_copy->card_list = LC;
            }
            else{
                ALSC_copy->card_list = LC;
            }
        }
    }
}

/*
* In functia delete_card voi parcurge baza de date pana cand
* intalnesc cardul pe care trebuie sa l elimin.
*/

void delete_card(LSCAList ALSC, char *card_number){
    LSCList ALSC_copy = (*ALSC);
    LCList previous = NULL;

    while(ALSC_copy != NULL){
        LCList LC_copy;
        if(ALSC_copy->card_list != NULL){
            LC_copy = ALSC_copy->card_list;
        }
        else{
            LC_copy = NULL;
        }

        /*
        * Aici verific daca este chiar primul card cel care trebuie
        * eliminat. Daca da, il elimin si ies din functie. Altfel,
        * voi parcurge lista de carduri din fiecare baza de date.
        */

        if(LC_copy != NULL && strcmp(LC_copy->card_number, card_number) == 0){
            ALSC_copy->card_list = LC_copy->next_card;
            return;
        }

        while(LC_copy != NULL && strcmp(LC_copy->card_number, card_number) != 0){
            previous = LC_copy;
            LC_copy = LC_copy->next_card;
        }

        if(LC_copy != NULL){
            previous->next_card = LC_copy->next_card;
            LC_copy->next_card = NULL;
        }
        ALSC_copy = ALSC_copy->next_lsc;
    }
}

/*
* Daca trebuie sa afisez un anume card, voi parcurge toata baza de date
* in cautarea acestuia, urmand apoi sa afisez continul sau. Altfel, voi
* parcurge toata baza de date si voi afisa fiecare card din fiecare celula
* de tip baza de date. Am folosit fprintf-urile asa pentru a nu depasi
* 80 de caractere pe linie.
*/

void show(LSCList LSC, char *card_number, FILE *output_file){
    
    if(!card_number){
        int i = 0;
        LSCList LSC_copy = LSC;
        while(LSC_copy != NULL){
            LCList LC_copy = LSC_copy->card_list;
            if(LC_copy != NULL){
                fprintf(output_file, "pos%d: [\n", i);
                
                while(LC_copy != NULL){

                    fprintf(output_file, "(card number: %s, PIN: %s, ",
                    LC_copy->card_number, LC_copy->pin);

                    fprintf(output_file, "expiry date: %s, CVV: %s, ",
                    LC_copy->expiry_date, LC_copy->cvv);

                    fprintf(output_file, "balance: %d, status: %s, ",
                    LC_copy->balance, LC_copy->status);

                    fprintf(output_file, "history: [");

                    HList H_copy = LC_copy->history;
                    
                    while(H_copy != NULL){
                        fprintf(output_file, "(%s", H_copy->history);
                        
                        if(H_copy->next_history != NULL){
                            fprintf(output_file, "), ");
                        }
                        else{
                            fprintf(output_file, ")");
                        }
                        
                        H_copy = H_copy->next_history;
                    }
                    fprintf(output_file, "])\n");
                    LC_copy = LC_copy->next_card;
                }
            fprintf(output_file, "]\n");
            }
            else{
                fprintf(output_file, "pos%d: []\n", i);
            }
            
            i++;
            LSC_copy = LSC_copy->next_lsc;
        }
    }
    else{
        LSCList LSC_copy = LSC;
        while(LSC_copy != NULL){
            LCList LC_copy = LSC_copy->card_list;
 
            if(LC_copy != NULL){
                
                while(LC_copy != NULL){
                    if(strcmp(LC_copy->card_number, card_number) == 0){

                        fprintf(output_file, "(card number: %s, PIN: %s, ",
                        LC_copy->card_number, LC_copy->pin);

                        fprintf(output_file, "expiry date: %s, CVV: %s, ",
                        LC_copy->expiry_date, LC_copy->cvv);

                        fprintf(output_file, "balance: %d, status: %s, ",
                        LC_copy->balance, LC_copy->status);

                        fprintf(output_file, "history: [");

                        HList H_copy = LC_copy->history;
                        
                        while(H_copy != NULL){
                            fprintf(output_file, "(%s", H_copy->history);
                            
                            if(H_copy->next_history != NULL){
                                fprintf(output_file, "), ");
                            }
                            else{
                                fprintf(output_file, ")");
                            }
                            
                            H_copy = H_copy->next_history;
                        }
                        fprintf(output_file, "])\n");
                    }
                    LC_copy = LC_copy->next_card;
                }
            }
            LSC_copy = LSC_copy->next_lsc;
        }
    }
}

/*
* In cazul in care va trebuie sa deblochez un card, voi parcurge
* baza de date pana unde trebuie, ii voi schimba statusul si
* totodata numar de incercari ale pinului asupra cardului
* respectiv.
*/

void unblock_card(LSCList LSC, char *card_number){
    LSCList LSC_copy = LSC;

    if(!LSC_copy){
        return;
    }

    while(LSC_copy != NULL){
        LCList LC_copy = LSC_copy->card_list;
        while(LC_copy != NULL){
            if(strcmp(LC_copy->card_number, card_number) == 0){
                    strcpy(LC_copy->status, "ACTIVE");
                LC_copy->failed_pin = 0;

            }
            LC_copy = LC_copy->next_card;
        }
        LSC_copy = LSC_copy->next_lsc;
    }
}