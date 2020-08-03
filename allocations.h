/*
* In acest header am definit functiile de alocare pentru celulele
* de tip baza de date(LSC), lista de carduri(LC) si istoric(H).
*/

LSCList AllotLSC_Cell(LCList LC){
    LSCList aux;
    aux = malloc(sizeof(LSCCell));

    if(!aux){
        return NULL;
    }

    aux->card_list = LC;
    aux->next_lsc = NULL;

    return aux;
}

LCList AllotLC_Cell(char *card_number, char *pin, 
                char *expiry_date, char *cvv){
    LCList aux;
    aux = malloc(sizeof(LCCell));

    if(!aux){
        return NULL;
    }

    aux->card_number = strdup(card_number);
    aux->pin = strdup(pin);
    aux->expiry_date = strdup(expiry_date);
    aux->cvv = strdup(cvv);
    aux->balance = 0;
    aux->status = strdup("NEW");
    aux->failed_pin = 0;
    aux->history = NULL;
    aux->next_card = NULL;

    return aux;
}

HList AllotH_Cell(char *message, char *buffer){
    HList aux;
    aux = malloc(sizeof(HCell));

    if(!aux){
        return NULL;
    }

    char *content = malloc(80 * sizeof(char));
    strcat(content, message);
    strcat(content, ", ");
    strcat(content, buffer);

    aux->history = strdup(content);
    aux->next_history = NULL;

    free(content);

    return aux;
}