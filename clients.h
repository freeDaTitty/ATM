/*
* Functia respectiva imi va adauga in istoricul unui card
* continutul necesar.
*/

void add_History(HAList H, char *message, char *buffer){
    if(*H == NULL){
        *H = AllotH_Cell(message, buffer);
    }
    else{
        HList H_aux;
        H_aux = AllotH_Cell(message, buffer);
        H_aux->next_history = (*H);
        *H = H_aux;
    }
}

/*
* Daca voi introduce un card, voi cauta in baza de date sa vad daca
* respectivul card exista. Daca va avea pinul neschimbat, ii voi
* afisa un mesaj potrivit si, in functie de statusul cardului
* si de pinul acestuia, voi adauga in istoric ce trebuie.
*/

void insert_card(LSCList LSC, char *card_number, char *pin, 
char *buffer, FILE *output_file){

    LSCList LSC_copy = LSC;

    if(!LSC_copy){
        return;
    }

    while(LSC_copy != NULL){
        LCList LC_copy = LSC_copy->card_list;
        while(LC_copy != NULL){
            if(strcmp(LC_copy->card_number, card_number) == 0){
                HList H_copy = LC_copy->history;
                if(strcmp(LC_copy->pin, pin) == 0){
                    if(strcmp(LC_copy->status, "NEW") == 0){
                        fprintf(output_file, "You must change your PIN.\n");
                        add_History(&H_copy, "SUCCESS", buffer);
                        LC_copy->failed_pin = 0;
                        LC_copy->history = H_copy;
                    }
                    else if(strcmp(LC_copy->status, "LOCKED") == 0){

                        fprintf(output_file, 
                "The card is blocked. Please contact the administrator.\n");

                        add_History(&H_copy, "FAIL", buffer);
                        LC_copy->history = H_copy;
                    }
                    else{
                        add_History(&H_copy, "SUCCESS", buffer);
                        LC_copy->history = H_copy;
                    }
                }
                else{
                    fprintf(output_file, "Invalid PIN\n");
                    add_History(&H_copy, "FAIL", buffer);
                    LC_copy->history = H_copy;
                    LC_copy->failed_pin++;
                    if(LC_copy->failed_pin == 3){

                        fprintf(output_file, 
                    "The card is blocked. Please contact the administrator.\n");

                        LC_copy->status = strdup("LOCKED");
                    }
                }
            }
            LC_copy = LC_copy->next_card;
        }
        LSC_copy = LSC_copy->next_lsc;
    }
}

/*
* La apelarea functiei, voi adauga in istoriul cardului, pe care-l
* deconectez de la bancomat, textul corespunzator.
*/

void cancel(LSCList LSC, char *card_number, char *buffer, FILE *output_file){
    LSCList LSC_copy = LSC;
    int failed_pin = 0;

    if(!LSC_copy){
        return;
    }

    while(LSC_copy != NULL){
        LCList LC_copy = LSC_copy->card_list;
        while(LC_copy != NULL){
            if(strcmp(LC_copy->card_number, card_number) == 0){
                HList H_copy = LC_copy->history;
                add_History(&H_copy, "SUCCESS", buffer);
                LC_copy->history = H_copy;
            }
            LC_copy = LC_copy->next_card;
        }
        LSC_copy = LSC_copy->next_lsc;
    }
}

/*
* Voi parcurge baza de date in cautarea cardului si ii voi
* schimba pinul numai daca acesta este valid. La final, voi adauga
* in istoric starea de decurge a procesului.
*/

void pin_change(LSCList LSC, char *card_number, char *new_pin, 
char *buffer, FILE *output_file){

    LSCList LSC_copy = LSC;

    if(!LSC_copy){
        return;
    }

    while(LSC_copy != NULL){
        LCList LC_copy = LSC_copy->card_list;
        while(LC_copy != NULL){
            if(strcmp(LC_copy->card_number, card_number) == 0){
                HList H_copy = LC_copy->history;
                if(strlen(new_pin) == 4){
                    int i, nr = 0;
                    for(i = 0; i < 4; i++){
                        if(isdigit(*(new_pin + i)) != 0){
                            nr++;
                        }
                    }
                    if(nr == 4){
                        strcpy(LC_copy->pin, new_pin);
                        add_History(&H_copy, "SUCCESS", buffer);
                        strcpy(LC_copy->status, "ACTIVE");
                        LC_copy->history = H_copy;
                    }
                    else{
                        fprintf(output_file, "Invalid PIN\n");
                        add_History(&H_copy, "FAIL", buffer);
                        LC_copy->history = H_copy;
                    }
                }
                else{
                    fprintf(output_file, "Invalid PIN\n");
                    add_History(&H_copy, "FAIL", buffer);
                    LC_copy->history = H_copy;
                }
            }
            LC_copy = LC_copy->next_card;
        }
        LSC_copy = LSC_copy->next_lsc;
    }
}

/*
* Se va parcurge baza de date, se va cauta cardul, iar apoi
* voi afisa cati bani are in el si voi adauga in istoric
* ce trebuie.
*/

void balance_inquiry(LSCList LSC, char *card_number, char *buffer, 
                                                FILE *output_file){

    LSCList LSC_copy = LSC;

    if(!LSC_copy){
        return;
    }

    while(LSC_copy != NULL){
        LCList LC_copy = LSC_copy->card_list;
        while(LC_copy != NULL){
            if(strcmp(LC_copy->card_number, card_number) == 0){
                HList H_copy = LC_copy->history;
                fprintf(output_file, "%d\n", LC_copy->balance);
                add_History(&H_copy, "SUCCESS", buffer);
                LC_copy->history = H_copy;
            }
            LC_copy = LC_copy->next_card;
        }
        LSC_copy = LSC_copy->next_lsc;
    }
}

/*
* Se va adauga in contul cardului respectiv o anumita
* cantitate de bani. Daca valoarea ceruta nu va
* indeplini conditiile necesare, se va afisa
* un mesaj aferent. Se va adauga apoi totul in
* istoricul cardului respectiv.
*/

void recharge(LSCList LSC, char *card_number, int recharge_amount, 
                                char *buffer, FILE *output_file){

    LSCList LSC_copy = LSC;

    if(!LSC_copy){
        return;
    }

    while(LSC_copy != NULL){
        LCList LC_copy = LSC_copy->card_list;
        while(LC_copy != NULL){
            if(strcmp(LC_copy->card_number, card_number) == 0){
                HList H_copy = LC_copy->history;
                if(recharge_amount % 10 == 0){
                    LC_copy->balance += recharge_amount;
                    fprintf(output_file, "%d\n", LC_copy->balance);
                    add_History(&H_copy, "SUCCESS", buffer);
                    LC_copy->history = H_copy;
                }
                else{
                    fprintf(output_file, 
                    "The added amount must be multiple of 10\n");
                    add_History(&H_copy, "FAIL", buffer);
                    LC_copy->history = H_copy;
                }
            }
            LC_copy = LC_copy->next_card;
        }
        LSC_copy = LSC_copy->next_lsc;
    }
}

/*
* Se va extrage din contul bancar o cantitate de bani. Daca
* aceasta cantitate nu respecta anumite conditii, se va afisa
* un anume mesaj, iar apoi se va adauga in istoric tot ce tine
* de cum a decurs acest proces. Pentru a gasi cardul, se vor parcurge
* toata baza de date si toate cardurile din fiecare celula. Asa cum
* se va face si in restul functiilor. Nu mai scriu asta, e prea repetitiv..
*/

void cash_withdrawal(LSCList LSC, char *card_number, int cash, 
                                char *buffer, FILE *output_file){

    LSCList LSC_copy = LSC;

    if(!LSC_copy){
        return;
    }

    while(LSC_copy != NULL){
        LCList LC_copy = LSC_copy->card_list;
        while(LC_copy != NULL){
            if(strcmp(LC_copy->card_number, card_number) == 0){
                HList H_copy = LC_copy->history;
                if(cash % 10 == 0){
                    if(LC_copy->balance >= cash){
                        LC_copy->balance -= cash;
                        fprintf(output_file, "%d\n", LC_copy->balance);
                        add_History(&H_copy, "SUCCESS", buffer);
                        LC_copy->history = H_copy;
                    }
                    else{
                        fprintf(output_file, "Insufficient funds\n");
                        add_History(&H_copy, "FAIL", buffer);
                        LC_copy->history = H_copy;
                    }
                }
                else{
                    fprintf(output_file, 
                    "The requested amount must be multiple of 10\n");

                    add_History(&H_copy, "FAIL", buffer);
                    LC_copy->history = H_copy;
                }
                break;
            }
            LC_copy = LC_copy->next_card;
        }
        LSC_copy = LSC_copy->next_lsc;
    }
}

/*
* Se vor transfera bani de pe un card pe altul. Vor fi afisate
* anumite mesaje in cazul nerespectarii cerintelor. Totul va fi
* adaugat in istoric mai apoi.
*/

void transfer_funds(LSCList LSC, char *card_number_source, 
char *card_number_dest, int cash, char *buffer, FILE *output_file){

    LSCList LSC_copy = LSC;
    LSCList LSC_copy_dest = LSC;

    if(!LSC_copy){
        return;
    }

    if(!LSC_copy_dest){
        return;
    }

    while(LSC_copy != NULL){
        LCList LC_copy = LSC_copy->card_list;

        while(LC_copy != NULL){
            if(strcmp(LC_copy->card_number, card_number_source) == 0){
                HList H_copy = LC_copy->history;

                if(cash % 10 == 0){
                    if(LC_copy->balance >= cash){
                        LC_copy->balance -= cash;
                        fprintf(output_file, "%d\n", LC_copy->balance);

                        while(LSC_copy_dest != NULL){
                            LCList LC_copy_dest = LSC_copy_dest->card_list;

                            while(LC_copy_dest != NULL){

                                if(strcmp(LC_copy_dest->card_number, 
                                card_number_dest) == 0){

                                    HList H_copy_dest = LC_copy_dest->history;
                                    LC_copy_dest->balance += cash;

                                    add_History(&H_copy, "SUCCESS", buffer);

                                    add_History(&H_copy_dest, "SUCCESS", 
                                    buffer);

                                    LC_copy->history = H_copy;
                                    LC_copy_dest->history = H_copy_dest;
                                    break;
                                }
                                LC_copy_dest = LC_copy_dest->next_card;
                            }
                            LSC_copy_dest = LSC_copy_dest->next_lsc;
                        }
                    }
                    else{
                        fprintf(output_file, "Insufficient funds\n");
                        add_History(&H_copy, "FAIL", buffer);
                        LC_copy->history = H_copy;
                    }
                }
                else{
                    fprintf(output_file, 
                    "The transferred amount must be multiple of 10\n");

                    add_History(&H_copy, "FAIL", buffer);
                    LC_copy->history = H_copy;
                }
            }
            LC_copy = LC_copy->next_card;
        }
        LSC_copy = LSC_copy->next_lsc;
    }
}

/*
* Daca cineva, din greseala, a ales sa transfere bani pe alt cont
* (din neatentie, desigur), are o portita de scapare. Aceasta functie
* va face opusul a ceea ce face functia de transfer_funds. Va extrage
* banii din contul in care i-a depus si-i va pune inapoi pe cardul lui.
* Se va sterge din istoricul cardului ce primise banii faptul ca i-a primit,
* iar in istoricul celui care i-a extras se va pune ce-a facut.
*/

void reverse_transaction(LSCList LSC, char *card_number_source, 
char *card_number_dest, int cash, char *buffer, FILE *output_file){

    LSCList LSC_copy = LSC;
    LSCList LSC_copy_dest = LSC;

    if(!LSC_copy){
        return;
    }

    if(!LSC_copy_dest){
        return;
    }
    
    char *history_check;
    history_check = (char *)malloc(90 * sizeof(char));
    char *cash_string;
    cash_string = (char *)malloc(10 * sizeof(char));

    sprintf(cash_string, "%d", cash);

    strcat(history_check, "SUCCESS, transfer_funds ");
    strncat(history_check, card_number_source, strlen(card_number_source) + 1);
    strcat(history_check, " ");
    strncat(history_check, card_number_dest, strlen(card_number_dest) + 1);
    strcat(history_check, " ");
    strncat(history_check, cash_string, strlen(cash_string) + 1);

    while(LSC_copy != NULL){
        LCList LC_copy = LSC_copy->card_list;

        while(LC_copy != NULL){
            if(strcmp(LC_copy->card_number, card_number_source) == 0){
                HList H_copy = LC_copy->history;
                while(LSC_copy_dest != NULL){
                    LCList LC_copy_dest = LSC_copy_dest->card_list;

                    while(LC_copy_dest != NULL){
                        if(strcmp(LC_copy_dest->card_number, 
                        card_number_dest) == 0){

                            HList H_copy_dest = LC_copy_dest->history;

                            if(LC_copy_dest->balance >= cash){
                                LC_copy_dest->balance -= cash;
                                LC_copy->balance += cash;
                                add_History(&H_copy, "SUCCESS", buffer);
                                LC_copy->history = H_copy;

                                while(H_copy_dest != NULL){
                                    if(strcmp(H_copy_dest->history, 
                                    history_check) == 0){

                                        H_copy_dest = 
                                        LC_copy_dest->history->next_history;
                                        
                                        LC_copy_dest->history = H_copy_dest;

                                        free(history_check);
                                        break;
                                    }
                                    H_copy_dest = H_copy_dest->next_history;
                                }
                                LC_copy_dest->history = H_copy_dest;
                                break;
                            }
                            else{
                                fprintf(output_file, 
                                "The transaction cannot be reversed\n");

                                break;
                            }
                        }
                        LC_copy_dest = LC_copy_dest->next_card;
                    }
                    LSC_copy_dest = LSC_copy_dest->next_lsc;
                }
            }
            LC_copy = LC_copy->next_card;
        }
        LSC_copy = LSC_copy->next_lsc;
    }
}