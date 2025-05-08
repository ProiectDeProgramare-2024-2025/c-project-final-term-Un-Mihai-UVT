/*Sistem pentru food delivery
Aplicația permite utilizatorului selectarea unui restaurant dintr-o listă. După ce acesta
a fost selectat, utilizatorului îi sunt prezentate comenzile ce le poate plasa, sau i se va
oferi posibilitatea de a își personaliza o pizza, introducând ingredientele dorite și dimensiunea
acesteia. La finalul comenzii, utilizatorului i se va cere să introducă locația unde acesta
vrea livrarea comenzii. Aplicația îi permite utilizatorului să vadă istoricul comenziilor (ce a
comandat, de la ce restaurant a comandat și la ce locație a fost livrată comanda).*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include<unistd.h>

#define NR_MAX_RESTAURANTE          100
#define NR_MAX_COMENZI              100
#define LUNGIME_NUME_RESTAURANTE    256
#define LUNGIME_NUME_COMENZI        256
#define LUNGIME_INGREDIENTE         300
#define LUNGIME_INPUT_ADRESA        300
#define LUNGIME_NUMAR_ADRESA        4
#define LUNGIME_DIMENSIUNE_PIZZA    2

#define ROSU    "\033[0;31m"
#define VERDE   "\033[0;32m"
#define GALBEN  "\033[0;33m"
#define MAGENTA "\033[0;35m"
#define TURCOAZ "\033[0;36m"
#define ALB     "\033[0;37m"

typedef struct Adresa {
    char judet[LUNGIME_INPUT_ADRESA];
    char localitate[LUNGIME_INPUT_ADRESA];
    char strada[LUNGIME_INPUT_ADRESA];
    int Numar;
}Adresa;

typedef struct Comanda {
    char nume[LUNGIME_NUME_COMENZI];
    int pret;
}Comanda;

typedef struct Pizza {
    int dimensiune;
    char ingrediente[LUNGIME_INGREDIENTE];
}Pizza;

typedef struct Restaurant {
    char nume[LUNGIME_NUME_RESTAURANTE];
    int numarComenzi;
    Comanda comenzi[NR_MAX_COMENZI];
}Restaurant;

struct MeniulPrincipal {
    int nrRestaurante;
    Restaurant restaurante[NR_MAX_RESTAURANTE];
}MeniulPrincipal;

void incarcaMeniuri();

int indicatorPosition(int, int);

void printMeniulPrincipal();
void printMeniuRestaurant(Restaurant);
void printIstoricComenzi();

void customizarePizza(Restaurant, int);
void confirmareComanda(Restaurant, int, Pizza, Adresa);
void procesareComanda(Restaurant, int, Pizza);
void salveazaComanda(Restaurant, int, Pizza, Adresa);

void removeNewLine(char*);
int validareText(char*);
int validareNumar(const char*, int);
void citesteInputCuValidare(const char*, char*, char*, int (*)(const char*, int), int);

void afisareCursor();
void ascundereCursor();




int main() {
    incarcaMeniuri();
    ascundereCursor();
    printMeniulPrincipal();
}




void incarcaMeniuri() {
    FILE *fisierMeniuri = fopen("meniuri.txt", "r");

    fscanf(fisierMeniuri,"%d\n", &MeniulPrincipal.nrRestaurante);

    for (int i = 0; i < MeniulPrincipal.nrRestaurante; i++) {

        fgets(MeniulPrincipal.restaurante[i].nume, sizeof(MeniulPrincipal.restaurante[i].nume), fisierMeniuri);
        removeNewLine(MeniulPrincipal.restaurante[i].nume);

        fscanf(fisierMeniuri, "%d\n", &MeniulPrincipal.restaurante[i].numarComenzi);

        for (int j = 0; j < MeniulPrincipal.restaurante[i].numarComenzi; j++) {

            fgets(MeniulPrincipal.restaurante[i].comenzi[j].nume, sizeof(MeniulPrincipal.restaurante[i].comenzi[j].nume), fisierMeniuri);
            removeNewLine(MeniulPrincipal.restaurante[i].comenzi[j].nume);

            fscanf(fisierMeniuri, "%d\n", &MeniulPrincipal.restaurante[i].comenzi[j].pret);
        }
    }

    fclose(fisierMeniuri);
}

int indicatorPosition(int position, int posMax) {
    int c = getch();
    if (c == 224) { //caracter dat de sageti
        c = getch();
        switch (c) {
            case 72: position--; break; //sageata sus
            case 80: position++; break; //sageata jos
        }
        if (position < 0)
            position = 0;
        else if (position > posMax)
            position = posMax;
        return position;
    }
    if (c == 13) { //enter
        position = posMax + 1;
    }
    return position;
}

int validareText(char *text) {
    removeNewLine(text);

    if (!strlen(text)) {
        return 0;
    }

    for (int i = 0; text[i]; i++) {
        if (!(text[i] == ' ' || (text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z') || text[i] == '-')) {
            return 0;
        }
    }

    return 1;
}

int validareNumar(const char *text, int limita) {
    if (strlen(text) == 0 || strlen(text) > limita) {
        return 0;
    }

    for (int i = 0; text[i]; i++) {
        if (text[i] < '0' || text[i] > '9') {
            return 0;
        }
    }

    if (atoi(text) <= 0) {
        return 0;
    }

    return 1;
}

void citesteInputCuValidare(const char *restrictii, char *mesaj, char *destinatie, int (*functieValidare)(const char *, int), int limita) {
    int primaIncercare = 1;
    do {
        system("cls");
        printf("----Adresa de Livrare----\n\n");

        printf(GALBEN "%s\n" ALB, restrictii);

        if (!primaIncercare) {
            printf(ROSU "Te rugam sa respecti formatul cerut.\n\n" ALB);
        }

        printf("%s ", mesaj);
        fgets(destinatie, LUNGIME_INPUT_ADRESA, stdin);
        removeNewLine(destinatie);

        primaIncercare = 0;

    } while (!functieValidare(destinatie, limita));
}




void printMeniulPrincipal() {
    int prevPosition = 0, indPosition = 0, posMax = MeniulPrincipal.nrRestaurante + 1;
    while (1) {
        system("cls");
        printf("----Meniul Principal----\n");
        for (int i = 0; i <= posMax; i++) {
            if (i == indPosition)
                printf(">>");
            else {
                printf("  ");
            }
            if (i < posMax - 1) {
                printf(ROSU "%s\n" ALB, MeniulPrincipal.restaurante[i].nume);
            }
            else {
                if (i == posMax - 1)
                    printf(MAGENTA "Istoric Comenzi\n" ALB);
                else
                    printf(MAGENTA "Iesire\n" ALB);
            }
        }
        prevPosition = indPosition;
        indPosition = indicatorPosition(indPosition, posMax);
        if (indPosition > posMax) {
            if (prevPosition == posMax)
                exit(0);
            if (prevPosition == posMax - 1) {
                printIstoricComenzi();
            }else {
                printMeniuRestaurant(MeniulPrincipal.restaurante[prevPosition]);
            }
        }
    }
}

void printMeniuRestaurant(Restaurant res) {
    int prevPosition = 0, indPosition = 0, posMax = res.numarComenzi;

    while (1) {
        system("cls");
        printf("----Meniu %s----\n", res.nume);
        for (int i = 0; i <= posMax; i++) {
            if (i == indPosition)
                printf(">>");
            else {
                printf("  ");
            }
            if (i < posMax) {
                printf(GALBEN "%s " ALB, res.comenzi[i].nume);
                printf(VERDE "%d Lei\n" ALB, res.comenzi[i].pret);
            }
            else {
                printf(MAGENTA "Inapoi" ALB);
            }
        }
        prevPosition = indPosition;
        indPosition = indicatorPosition(indPosition, posMax);
        if (indPosition > posMax) {
            if (prevPosition == posMax)
                printMeniulPrincipal();
            else {
                if (strcmp(res.comenzi[prevPosition].nume, "Customizare Pizza") == 0)
                    customizarePizza(res, prevPosition);
                else {
                    Pizza pizza = {0};
                    procesareComanda(res, prevPosition, pizza);
                }
            }
        }
    }
}

void printIstoricComenzi() {
    system("cls");
    printf("----Istoric Comenzi----\n");

    FILE *istoricComenzi = fopen("istoricComenzi.txt", "r");

    char nume_restaurant[LUNGIME_NUME_RESTAURANTE], comanda[LUNGIME_NUME_COMENZI];
    Pizza pizza;
    Adresa adresa;
    int pret;

    while (fgets(nume_restaurant, LUNGIME_NUME_RESTAURANTE, istoricComenzi) != NULL) {
        removeNewLine(nume_restaurant);
        printf("\nRestaurant: " ROSU "%s" ALB " | ", nume_restaurant);

        fgets(comanda, LUNGIME_NUME_COMENZI, istoricComenzi);
        removeNewLine(comanda);
        printf("Comanda: " GALBEN "%s" ALB " | ", comanda);

        if (strcmp(comanda, "Customizare Pizza") == 0) {
            fscanf(istoricComenzi, "%d\n", &pizza.dimensiune);
            fgets(pizza.ingrediente, LUNGIME_INGREDIENTE, istoricComenzi);

            removeNewLine(pizza.ingrediente);
            printf("Dimensiune: " GALBEN "%d cm" ALB " | ", pizza.dimensiune);
            printf("Ingrediente: " GALBEN "%s" ALB " | ", pizza.ingrediente);
        }

        fscanf(istoricComenzi, "%d\n", &pret);
        printf("Pret: " VERDE "%d Lei" ALB " | ", pret);

        fgets(adresa.judet, LUNGIME_INPUT_ADRESA, istoricComenzi);
        fgets(adresa.localitate, LUNGIME_INPUT_ADRESA, istoricComenzi);
        fgets(adresa.strada, LUNGIME_INPUT_ADRESA, istoricComenzi);
        fscanf(istoricComenzi, "%d\n", &adresa.Numar);

        removeNewLine(adresa.judet);
        removeNewLine(adresa.localitate);
        removeNewLine(adresa.strada);

        printf("Adresa de livrare: " TURCOAZ "%s, %s, %s, %d\n" ALB, adresa.judet, adresa.localitate, adresa.strada, adresa.Numar);
    }

    printf("\n");
    printf(">>" MAGENTA "Inapoi" ALB);

    int c = getch();
    while (c != 13) { //enter
        c = getch();
    }

    fclose(istoricComenzi);

    printMeniulPrincipal();
}

Pizza citestePizza() {

}

void customizarePizza(Restaurant res, int pos) {
    system("cls");
    afisareCursor();

    Pizza pizza = {0};

    printf("----Customizare Pizza----\n");

    char numar[LUNGIME_DIMENSIUNE_PIZZA];
    citesteInputCuValidare("<Se permit doar numere de maxim 2 cifre>", "Introduceti dimensiunea dorita <diametru in centimetrii>: ",
        numar, validareNumar, LUNGIME_DIMENSIUNE_PIZZA);
    pizza.dimensiune = atoi(numar);

    citesteInputCuValidare("<Se permit doar litere mari/mici, cratime si spatii>", "Introduceti ingredientele dorite: ",
        pizza.ingrediente, validareText, LUNGIME_INGREDIENTE);
    removeNewLine(pizza.ingrediente);

    ascundereCursor();
    procesareComanda(res, pos, pizza);
}

void procesareComanda(Restaurant res, int pos, Pizza pizza) {
    system("cls");
    afisareCursor();

    Adresa adresa = {0};

    citesteInputCuValidare("<Se permit doar litere mari/mici, cratime si spatii>", "Introduceti Judetul:",
        adresa.judet, validareText, LUNGIME_INPUT_ADRESA);

    citesteInputCuValidare("<Se permit doar litere mari/mici, cratime si spatii>", "Introduceti Localitatea:",
        adresa.localitate, validareText, LUNGIME_INPUT_ADRESA);

    citesteInputCuValidare("<Se permit doar litere mari/mici, cratime si spatii>", "Introduceti Strada:",
        adresa.strada, validareText, LUNGIME_INPUT_ADRESA);

    char numar[LUNGIME_NUMAR_ADRESA + 1] = {0};
    citesteInputCuValidare("<Se permit doar numere de maxim 4 cifre>", "Introduceti Numarul:",
        numar, validareNumar, LUNGIME_NUMAR_ADRESA);
    adresa.Numar = atoi(numar);


    ascundereCursor();
    confirmareComanda(res, pos, pizza, adresa);
}

void confirmareComanda(Restaurant res, int pos, Pizza pizza, Adresa adresa) {

    char c = 'a';
    while (c != 'y' && c != 'Y' && c != 'n' && c != 'N') {
        system("cls");

        printf("----Detaliile Comenzii----\n\n");

        printf("Restaurant: " ROSU "%s" ALB " | ", res.nume);
        printf("Comanda: " GALBEN "%s" ALB " | ", res.comenzi[pos].nume);

        if (pizza.dimensiune > 0) {
            printf("Dimensiunea: " GALBEN "%d cm" ALB " | ", pizza.dimensiune);
            printf("Ingrediente: " GALBEN "%s" ALB " | ", pizza.ingrediente);
        }

        printf("Pret: " VERDE "%d Lei" ALB " | ", res.comenzi[pos].pret);
        printf("Adresa de livrare: " TURCOAZ "%s, %s, %s, %d" ALB, adresa.judet, adresa.localitate, adresa.strada, adresa.Numar);

        printf("\n\nConfirmi comanda? y/n\n");
        c = getch();
    }

    if (c == 'y' || c == 'Y') {
        salveazaComanda(res, pos, pizza, adresa);

        system("cls");
        printf("Multumim pentru achizitie");
        sleep(2);

        printMeniulPrincipal();
    }
    else {
        printMeniuRestaurant(res);
    }
}

void salveazaComanda(Restaurant res, int pos, Pizza pizza, Adresa adresa) {
    FILE* istoricComenzi = fopen("istoricComenzi.txt", "a");

    fprintf(istoricComenzi, "%s\n", res.nume);
    fprintf(istoricComenzi, "%s\n", res.comenzi[pos].nume);

    if (pizza.dimensiune > 0) {
        fprintf(istoricComenzi, "%d\n", pizza.dimensiune);
        fprintf(istoricComenzi, "%s\n", pizza.ingrediente);
    }

    fprintf(istoricComenzi, "%d\n", res.comenzi[pos].pret);
    fprintf(istoricComenzi, "%s\n%s\n%s\n%d\n", adresa.judet, adresa.localitate, adresa.strada, adresa.Numar);

    fclose(istoricComenzi);
}

void removeNewLine(char *s) {
    if (s[strlen(s) - 1] == '\n') {
        s[strlen(s) - 1] = '\0';
    }
}

void ascundereCursor() {
    printf("\e[?25l");
}

void afisareCursor() {
    printf("\e[?25h");
}