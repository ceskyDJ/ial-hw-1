
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    list->activeElement = NULL;
    list->firstElement = NULL;
    list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    DLLElementPtr nextElement = NULL;
    for (DLLElementPtr element = list->firstElement; element != NULL; element = nextElement) {
        // Backup next element
        nextElement = element->nextElement;

        free(element);
    }

    list->activeElement = NULL;
    list->firstElement = NULL;
    list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
    // Try to allocate memory for new element
    DLLElementPtr newElement;
    if ((newElement = malloc(sizeof(struct DLLElement))) == NULL) {
        DLL_Error();
        return;
    }

    // Set up new element
    newElement->data = data;
    newElement->nextElement = list->firstElement;
    newElement->previousElement = NULL;

    // Set up next element (current first)
    if (list->firstElement != NULL) {
        list->firstElement->previousElement = newElement;
    }

    // Update list
    list->firstElement = newElement;
    if (list->lastElement == NULL) {
        // Adding element into empty list means the new element is first but last, too
        list->lastElement = newElement;
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    // Try to allocate memory for new element
    DLLElementPtr newElement;
    if ((newElement = malloc(sizeof(struct DLLElement))) == NULL) {
        DLL_Error();
        return;
    }

    // Set up new element
    newElement->data = data;
    newElement->nextElement = NULL;
    newElement->previousElement = list->lastElement;

    // Set up previous element (current last)
    if (list->lastElement != NULL) {
        list->lastElement->nextElement = newElement;
    }

    // Update list
    list->lastElement = newElement;
    if (list->firstElement == NULL) {
        // Adding element into empty list means the element is last but first, too
        list->firstElement = newElement;
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
    // Check for the list is empty
    if (list->firstElement == NULL) {
        DLL_Error();
        return;
    }

    *dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
    // Check for the list is empty
    if (list->firstElement == NULL) {
        DLL_Error();
        return;
    }

    *dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    // There is no job for this function when the list is empty
    if (list->firstElement == NULL) {
        return;
    }

    // Cancel activity when the first element is active
    if (list->activeElement == list->firstElement) {
        list->activeElement = NULL;
    }

    // Update next element (current second one)
    DLLElementPtr nextElement = list->firstElement->nextElement;
    if (nextElement != NULL) {
        nextElement->previousElement = NULL;
    }

    // Delete element
    free(list->firstElement);

    // Update list
    list->firstElement = nextElement;
    if (list->firstElement == NULL) {
        // List is empty now, so it must be one-element before, which means the last element was the first
        // element, too. So after deletion the last element doesn't exist (it has been deleted)
        list->lastElement = NULL;
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
    // There is no job for this function when the list is empty
    if (list->firstElement == NULL) {
        return;
    }

    // Cancel activity when the last element is active
    if (list->activeElement == list->lastElement) {
        list->activeElement = NULL;
    }

    // Update previous element (current penultimate one)
    DLLElementPtr previousElement = list->lastElement->previousElement;
    if (previousElement != NULL) {
        previousElement->nextElement = NULL;
    }

    // Delete element
    free(list->lastElement);

    // Update list
    list->lastElement = previousElement;
    if (list->lastElement == NULL) {
        // List is empty now, so it must be one-element before, which means the first element was the last
        // element, too. So after deletion the first element doesn't exist (it has been deleted)
        list->firstElement = NULL;
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    // There is nothing to do if no element is active, or it's the last one
    if (list->activeElement == NULL || list->activeElement == list->lastElement) {
        return;
    }

    // Get element
    DLLElementPtr elementForDeletion = list->activeElement->nextElement;

    // Update previous element
    elementForDeletion->previousElement->nextElement = elementForDeletion->nextElement;

    // Update next element (it could be the last one)
    if (elementForDeletion->nextElement != NULL) {
        elementForDeletion->nextElement->previousElement = elementForDeletion->previousElement;
    }

    // Update list (when deleting the last element)
    if (elementForDeletion == list->lastElement) {
        list->lastElement = elementForDeletion->previousElement;
    }

    // Delete element
    free(elementForDeletion);
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    // There is nothing to do if no element is active, or it's the first one
    if (list->activeElement == NULL || list->activeElement == list->firstElement) {
        return;
    }

    // Get element
    DLLElementPtr elementForDeletion = list->activeElement->previousElement;

    // Update previous element (it could be the first one)
    if (elementForDeletion->previousElement != NULL) {
        elementForDeletion->previousElement->nextElement = elementForDeletion->nextElement;
    }

    // Update next element
    elementForDeletion->nextElement->previousElement = elementForDeletion->previousElement;

    // Update list (when deleting the first element)
    if (elementForDeletion == list->firstElement) {
        list->firstElement = elementForDeletion->nextElement;
    }

    // Delete element
    free(elementForDeletion);
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
    // There is nothing to do if no element is active
    if (list->activeElement == NULL) {
        return;
    }

    // Try to allocate memory for new element
    DLLElementPtr newElement;
    if ((newElement = malloc(sizeof(struct DLLElement))) == NULL) {
        DLL_Error();
        return;
    }

    // Set up new element
    newElement->data = data;
    newElement->nextElement = list->activeElement->nextElement;
    newElement->previousElement = list->activeElement;

    // Set up previous element
    newElement->previousElement->nextElement = newElement;

    // Set up next element
    // It doesn't exist when the active element is the last one
    if (newElement->nextElement != NULL) {
        newElement->nextElement->previousElement = newElement;
    }

    // Update list (if we are adding at the end of it)
    if (list->activeElement == list->lastElement) {
        list->lastElement = newElement;
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
    // There is nothing to do if no element is active
    if (list->activeElement == NULL) {
        return;
    }

    // Try to allocate memory for new element
    DLLElementPtr newElement;
    if ((newElement = malloc(sizeof(struct DLLElement))) == NULL) {
        DLL_Error();
        return;
    }

    // Set up new element
    newElement->data = data;
    newElement->nextElement = list->activeElement;
    newElement->previousElement = list->activeElement->previousElement;

    // Set up previous element
    if (newElement->previousElement != NULL) {
        newElement->previousElement->nextElement = newElement;
    }

    // Set up next element
    newElement->nextElement->previousElement = newElement;

    // Update list (if we are adding at the beginning of it)
    if (list->firstElement == list->activeElement) {
        list->firstElement = newElement;
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
    // Check for list isn't active
    if (list->activeElement == NULL) {
        DLL_Error();
        return;
    }

    *dataPtr = list->activeElement->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
    // There is nothing to do, because no element is active
    if (list->activeElement == NULL) {
        return;
    }

    list->activeElement->data = data;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
    // There is nothing to do, because no element is active
    if (list->activeElement == NULL) {
        return;
    }

    list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    // There is nothing to do, because no element is active
    if (list->activeElement == NULL) {
        return;
    }

    list->activeElement = list->activeElement->previousElement;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    return list->activeElement != NULL;
}

/* Konec c206.c */
