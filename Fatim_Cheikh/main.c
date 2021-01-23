#include "stdio.h"
#include "stdlib.h"
#define M 3

typedef struct _noeud {
        int    n; /* n < M  le nombre de noeud sera toujours inferieur a l'ordre du B-arbre */
        int  cles[M - 1]; /*tableau de nombres*/
        struct _noeud *p[M]; /* (n+1 pointeurs seront utilises ) */
} noeud;

noeud *root = NULL;

//Definir le statut du noeud en prenant ses valeurs dans cette liste
typedef enum StatutCle {
        Duplique,
        RechercheEchouee,
        Reussie,
        InsererLe,
        Perdu,
} StatutCle;

StatutCle ins(noeud *ptr, int cle, int *upcle, noeud **nouveau) {
        noeud *newPtr, *lastPtr;
        int pos, i, n, splitPos;
        int newcle, lastcle;
        StatutCle value;
        if (ptr == NULL) {
                *nouveau = NULL;
                *upcle = cle;
                return InsererLe;
        }
        n = ptr->n;
        pos = ChercherPos(cle, ptr->cles, n);
        if (pos < n && cle == ptr->cles[pos])
                return Duplique;
        value = ins(ptr->p[pos], cle, &newcle, &newPtr);
        if (value != InsererLe)
                return value;

        /*Si le nombre de cles du noeud est inferieur a M-1 avec M l'ordre du B-arbre*/
        if (n < M - 1) {
                pos = ChercherPos(newcle, ptr->cles, n);

                /*Deplacement de la cle et de son pointeur vers la droite pour inserer la nouvelle cle*/
                for (i = n; i>pos; i--) {
                        ptr->cles[i] = ptr->cles[i - 1];
                        ptr->p[i + 1] = ptr->p[i];
                }

                /*la cle est inseree a l'emplacement exact*/
                ptr->cles[pos] = newcle;
                ptr->p[pos + 1] = newPtr;
                ++ptr->n; /*pour incrementer le nombre de cles dans le noeud*/
                return Reussie;

        }

         /*Si les cles dans les noeuds sont au maximum et la position du noeud a inserer  est la derniere*/
        if (pos == M - 1) {
                lastcle = newcle;
                lastPtr = newPtr;
        }
        else { /*Si les cles dans les noeuds sont au maximum et la position du noeud a inserer  n'est pas la derniere*/
                lastcle = ptr->cles[M - 2];
                lastPtr = ptr->p[M - 1];
                for (i = M - 2; i>pos; i--) {
                        ptr->cles[i] = ptr->cles[i - 1];
                        ptr->p[i + 1] = ptr->p[i];
                }
                ptr->cles[pos] = newcle;
                ptr->p[pos + 1] = newPtr;
        }
        splitPos = (M - 1) / 2;
        (*upcle) = ptr->cles[splitPos];
        (*nouveau) = (noeud*)malloc(sizeof(noeud));/*Noeud droit apres la separationt*/
        ptr->n = splitPos; /*le nombre de cles pour le noeud gauche separe*/
        (*nouveau)->n = M - 1 - splitPos;/*le nombre de cles pour le noeud gauche separe*/

        for (i = 0; i < (*nouveau)->n; i++) {
                (*nouveau)->p[i] = ptr->p[i + splitPos + 1];
                if (i < (*nouveau)->n - 1)
                        (*nouveau)->cles[i] = ptr->cles[i + splitPos + 1];
                else
                        (*nouveau)->cles[i] = lastcle;
        }
        (*nouveau)->p[(*nouveau)->n] = lastPtr;
        return InsererLe;
}

void inserer(int cle) {
        noeud *nouveau;
        int upcle;
        StatutCle value;
        value = ins(root, cle, &upcle, &nouveau);
        if (value == Duplique)
                printf("Cette valeur existe dans l'arbre\n");
        if (value == InsererLe) {
                noeud *uproot = root;
                root = (noeud*)malloc(sizeof(noeud));
                root->n = 1;
                root->cles[0] = upcle;
                root->p[0] = uproot;
                root->p[1] = nouveau;
        }
}

void display(noeud *ptr, int blanks) {
        if (ptr) {
                int i;
                for (i = 1; i <= blanks; i++)
                        printf(" ");
                for (i = 0; i < ptr->n; i++)
                        printf("%d ", ptr->cles[i]);
                printf("\n");
                for (i = 0; i <= ptr->n; i++)
                        display(ptr->p[i], blanks + 10);
        }
}

StatutCle suppr(noeud *ptr, int cle) {
        int pos, i, pivot, n, min;
        int *cle_arr;
        StatutCle value;
        noeud **p, *lptr, *rptr;
        if (ptr == NULL)
                return RechercheEchouee;
        /*Affecte les valeurs du noeud*/
        n = ptr->n;
        cle_arr = ptr->cles;
        p = ptr->p;
        min = (M - 1) / 2;/*nombre de cles minimum*/
        //Recherche de la cle a supprimer
        pos = ChercherPos(cle, cle_arr, n);
        // si p est une feuille
        if (p[0] == NULL) {
                if (pos == n || cle < cle_arr[pos])
                        return RechercheEchouee;
        /*Deplacement de la cle et du pointeur gauche*/
                for (i = pos + 1; i < n; i++)
                {
                        cle_arr[i - 1] = cle_arr[i];
                        p[i] = p[i + 1];
                }
                return --ptr->n >= (ptr == root ? 1 : min) ? Reussie : Perdu;
        }

         //si la cle est trouve mais p n'est pas une feuille
        if (pos < n && cle == cle_arr[pos]) {
                noeud *qp = p[pos], *qp1;
                int ncle;
                while (1) {
                        ncle = qp->n;
                        qp1 = qp->p[ncle];
                        if (qp1 == NULL)
                                break;
                        qp = qp1;
                }
                cle_arr[pos] = qp->cles[ncle - 1];
                qp->cles[ncle - 1] = cle;
        }
        value = suppr(p[pos], cle);
        if (value != Perdu)
                return value;
        if (pos > 0 && p[pos - 1]->n > min) {
                pivot = pos - 1;/*pivoter pour les noeuds gauche et droit */
                lptr = p[pivot];
                rptr = p[pos];
                /*Attribution des valeurs pour le noeud droit*/
                rptr->p[rptr->n + 1] = rptr->p[rptr->n];
                for (i = rptr->n; i>0; i--) {
                        rptr->cles[i] = rptr->cles[i - 1];
                        rptr->p[i] = rptr->p[i - 1];
                }
                rptr->n++;
                rptr->cles[0] = cle_arr[pivot];
                rptr->p[0] = lptr->p[lptr->n];
                cle_arr[pivot] = lptr->cles[--lptr->n];
                return Reussie;
        }

         //si (posn > min)
        if (pos < n && p[pos + 1]->n > min) {
                pivot = pos; /*pivoter pour les noeuds gauche et droit*/
                lptr = p[pivot];
                rptr = p[pivot + 1];
                /*Attribution des valeurs pour le noeud gauche*/
                lptr->cles[lptr->n] = cle_arr[pivot];
                lptr->p[lptr->n + 1] = rptr->p[0];
                cle_arr[pivot] = rptr->cles[0];
                lptr->n++;
                rptr->n--;
                for (i = 0; i < rptr->n; i++) {
                        rptr->cles[i] = rptr->cles[i + 1];
                        rptr->p[i] = rptr->p[i + 1];
                }
                rptr->p[rptr->n] = rptr->p[rptr->n + 1];
                return Reussie;
        }

        if (pos == n)
                pivot = pos - 1;
        else
                pivot = pos;
        lptr = p[pivot];
        rptr = p[pivot + 1];
        /*fusionner le noeud droit avec le noeud gauche*/
        lptr->cles[lptr->n] = cle_arr[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
        for (i = 0; i < rptr->n; i++) {
                lptr->cles[lptr->n + 1 + i] = rptr->cles[i];
                lptr->p[lptr->n + 2 + i] = rptr->p[i + 1];
        }
        lptr->n = lptr->n + rptr->n + 1;
        free(rptr); /*Supprimer le noeud droit*/
        for (i = pos + 1; i < n; i++) {
                cle_arr[i - 1] = cle_arr[i];
                p[i] = p[i + 1];
        }
        return --ptr->n >= (ptr == root ? 1 : min) ? Reussie : Perdu;
}

void Supprimer(int cle) {
        noeud *uproot;
        StatutCle value;
        value = suppr(root, cle);
        switch (value) {
        case RechercheEchouee:
                printf("La cle %d  est introuvable\n", cle);
                break;
        case Perdu:
                uproot = root;
                root = root->p[0];
                free(uproot);
                break;
        default:
                return;
        }

}

int ChercherPos(int cle, int *cle_arr, int n) {
        int pos = 0;
        while (pos < n && cle > cle_arr[pos])
                pos++;
        return pos;
}

void Chercher(int cle) {
        int pos, i, n;
        noeud *ptr = root;
        printf("Quelle est la cle que vous cherchez:\n");
        while (ptr) {
                n = ptr->n;
                for (i = 0; i < ptr->n; i++)
                        printf(" %d", ptr->cles[i]);
                printf("\n");
                pos = ChercherPos(cle, ptr->cles, n);
                if (pos < n && cle == ptr->cles[pos]) {
                        printf("La cle %d est trouvee a la position  %d du dernier affichage de noeud \n", cle, i);
                        return;
                }
                ptr = ptr->p[pos];
        }
        printf("La cle %d est introuvable\n", cle);
}

void eatline(void) {
        char c;
        while ((c = getchar()) != '\n');
}

void inorder(noeud *ptr) {
        if (ptr) {
                if (ptr->n >= 1) {
                        inorder(ptr->p[0]);
                        printf("%d ", ptr->cles[0]);
                        inorder(ptr->p[1]);
                        if (ptr->n == 2) {
                                printf("%d ", ptr->cles[1]);
                                inorder(ptr->p[2]);
                        }
                }
        }
}

int main() {
        int cle;
        int choix;
        printf("Creation d'un B-arbre pour M=%d\n", M);
        while (1) {
                printf("1.Inserer un nombre\n");
                printf("2.Supprimer un nombre\n");
                printf("3.Rechercher un nombre\n");
                printf("4.Afficher l'arbre\n");
                printf("5.Quitter\n");
                printf("Veuillez entrer votre choix : ");
                scanf("%d", &choix); eatline();

                switch (choix) {
                case 1:
                        printf("Entrer le nombre : ");
                        scanf("%d", &cle); eatline();
                        inserer(cle);
                        break;
                case 2:
                        printf("Entrer le nombre :  ");
                        scanf("%d", &cle); eatline();
                        Supprimer(cle);
                        break;
                case 3:
                        printf("Entrer le nombre :  ");
                        scanf("%d", &cle); eatline();
                        Chercher(cle);
                        break;
                case 4:
                        printf("L'arbre B obtenu apres modificaton est:\n");
                        display(root, 0);
                        break;
                case 5:
                        exit(1);

                default:
                        printf("Ce choix n'exixte pas\n");
                        break;
                }
        }
        return 0;
}
