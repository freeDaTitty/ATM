# ATM
An ATM machine

Am impartit codul intr-un fisier de baza "atm.c" si in mai 
multe headere pentru ca e un cod foarte mare. Cu chin si cu durere 
pentru ca arata acum destul de urat, am reusit sa il structurez pentru 
a nu depasi 80 de caractere pe linie. Am 80 maxim, promit.

	Pe scurt, tot ce-am facut in cod..:
- Am implementat functii de alocare pentru celulele necesare.
Baza de date de tip LSC, lista de carduri de tip LC si istoricul
de tip H.

- Am implementat functiile din cerinta problemei care fac, in mare,
ce ni s-a spus ca trebuie sa faca. Adauga un card in baza de date,
il sterge, il insereaza in bancomat, il elimina, depune bani pe el,
extrage bani de pe el, depune bani de pe un card pe altul, anuleaza
tranzactia daca a facut-o din greseala si tot asa.

- Exista anumite functii ajutatoare:
1. free_History
2. free_LC

Cu ajutorul acestor functii aveam de gand sa eliberez memoria, dar
deja am intarziat cu o zi si oricum nu am maxim, deci.. nu le mai folosesc
in cod.

3. add_History

	Functie ce adauga in istoric continutul necesar.

	Si cam atat. Restul este scris in cod, n-am avut prea mare inspiratie in
ceea ce priveste acest README si comentariile din cod.
