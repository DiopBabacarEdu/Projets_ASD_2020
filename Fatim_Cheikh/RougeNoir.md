# Listes chaînées : fonctions de base
Ci-dessous l'implémentation des fonctions de base sur les listes chaînées.

```c

#include "stdio.h"
#include "stdlib.h"

# define  ROUGE  0
# define  NOIR  1

struct noeud {
	int cle;
    int  couleur;
	struct noeud * parent;
	struct noeud * gauche;
	struct noeud * droite;
};

struct noeud * ROOT;
struct noeud * NILL;


void  left_rotate ( struct noeud * x);
void  right_rotate ( struct noeud  * x);
void  tree_print ( struct noeud * x);
void  red_black_insert ( int cle );
void  red_black_insert_fixup ( struct noeud * z);
struct noeud * tree_search (int cle );
struct noeud * tree_minimum ( struct noeud* x);
void  red_black_transplant ( struct noeud * u, struct noeud  * v);
void  red_black_delete ( struct noeud * z);
void  red_black_delete_fixup ( struct noeud * x);

int  main () {
	NILL = malloc ( sizeof ( struct noeud  ));
	NILL-> couleur = NOIR;

	ROOT = NILL;

        int cle;
        int choix;
        printf("Creation d'un arbre rouge noir \n");
        while (1) {
                printf("1.Inserer un nombre\n");
                printf("2.Supprimer un nombre\n");
                printf("3.Rechercher un nombre\n");
                printf("4.Afficher l'arbre\n");
                printf("5.Quitter\n");
                printf("Veuillez entrer votre choix : ");
                scanf("%d", &choix);

                switch (choix) {
                case 1:
                        printf ( "  Donnez la cle: " );
                        scanf ( " % d " , & cle);
                        red_black_insert (cle);

                        break;
                case 2:
                        printf ( " Entrer le nombre a  supprimer: " );
                        scanf ( " % d " , & cle);
                        red_black_delete (cle);
                        break;
                case 3:
                        printf ( " Entrez le nombe que vous cherchez: " );
                        scanf ( " % d " , &cle);
                        tree_search ( cle);
                        break;
                case 4:
                        tree_print (ROOT);
                        printf ( " \ n " );
                        break;
                case 5:
                        exit(1);

                default:
                        printf("Ce choix n'exixte pas\n");
                        break;
                }
        }
	return  0 ;
}
void  tree_print (struct noeud*Arbre){
if(Arbre != NULL){
    tree_print (Arbre->gauche);
	printf("%d->",Arbre->cle);
	 tree_print (Arbre->droite);
}
}

struct noeud * tree_search ( int cle ) {
	struct noeud  * x;

	x = ROOT;
	while (x != NILL && x-> cle != cle) {
		if (cle <x-> cle ) {
			x = x-> gauche ;
		}
		else {
			x = x-> droite ;
		}
	}

	return x;
}


void  red_black_insert ( int cle  ) {
	 struct noeud * z, * x, * y;
	z = malloc ( sizeof (  struct noeud ));

	z-> cle = cle;
	z-> couleur = ROUGE;
	z-> gauche = NILL;
	z-> droite = NILL;

	x = ROOT;
	y = NILL;


	while ( x != NILL) {
		y = x;
		if ( z->cle  <= x->cle  ) {
			x = x-> gauche ;
		}
		else {
			x = x-> droite ;
		}
	}

	if (y == NILL) {
		ROOT = z;
	}
	else  if ( z->cle  <= y->cle  ) {
		y-> gauche = z;
	}
	else {
		y-> droite = z;
	}

	z-> parent = y;

	red_black_insert_fixup (z);
}

struct noeud * tree_minimum (struct noeud* x) {
	while (x-> gauche != NILL) {
		x = x-> gauche ;
	}
	return x;
}


void  red_black_insert_fixup ( struct noeud * z) {
	while (z-> parent -> couleur == ROUGE) {

		if (z-> parent == z-> parent -> parent -> gauche ) {

			if (z-> parent -> parent -> droite -> couleur == ROUGE) {
				z-> parent -> couleur = NOIR;
				z-> parent -> parent -> droite -> couleur = NOIR;
				z-> parent -> parent -> couleur =ROUGE;
				z = z-> parent -> parent ;
			}

			else {

				if (z == z-> parent -> droite ) {
					z = z-> parent ;
					left_rotate (z);
				}

				z-> parent -> couleur = NOIR;
				z-> parent -> parent -> couleur = ROUGE;
				right_rotate (z-> parent -> parent );
			}
		}

		else {

			if (z-> parent -> parent -> gauche -> couleur == ROUGE) {
				z-> parent -> couleur = NOIR;
				z-> parent -> parent -> gauche -> couleur = NOIR;
				z-> parent -> parent -> couleur = ROUGE;
				z = z-> parent -> parent ;
			}

			else {

				if (z == z-> parent -> gauche ) {
					z = z-> parent ;
					right_rotate (z);
				}

				z-> parent -> couleur = NOIR;
				z-> parent -> parent -> couleur = ROUGE;
				left_rotate (z-> parent -> parent );
			}
		}
	}

	ROOT-> couleur = NOIR;
}
void  left_rotate ( struct noeud * x) {
	struct noeud * y;

	y = x-> droite ;
	x-> droite = y-> gauche ;
	if (y-> gauche != NILL) {
		y-> gauche  = x-> parent;
	}

	y-> parent = x-> parent ;
	if (y->parent == NILL) {
		ROOT = y;
	}
	else  if (x == x-> parent -> gauche ) {
		x-> parent -> gauche = y;
	}
	else {
		x-> parent -> droite = y;
	}

	y-> gauche = x;
	x-> parent = y;
}
void  right_rotate ( struct noeud * x) {
	struct noeud * y;

	y = x-> gauche ;
	x-> gauche = y-> droite ;
	if (y-> droite != NILL) {
		y-> droite -> parent = x;
	}

	y-> parent = x-> parent ;
	if (y-> parent == NILL) {
		ROOT = y;
	}
	else  if (x == x-> parent -> gauche ) {
		x-> parent -> gauche = y;
	}
	else {
		x-> parent -> droite = y;
	}

	y-> droite = x;
	x-> parent = y;
}
void  red_black_delete ( struct noeud * z) {
	 struct noeud * y, * x;
	int yOriginalcouleur;

	y = z;
	yOriginalcouleur = y-> couleur ;

	if (z-> gauche == NILL) {
		x = z-> droite ;
		red_black_transplant (z, z-> droite );
	}
	else if (z-> droite == NILL) {
		x = z-> gauche ;
		red_black_transplant (z, z-> gauche );
	}
	else {
		y = tree_minimum (z-> droite );
		yOriginalcouleur = y-> couleur ;

		x = y-> droite ;

		if (y-> parent == z) {
			x-> parent = y;
		}
		else {
			red_black_transplant (y, y-> droite );
			y-> droite = z-> droite ;
			y-> droite -> parent = y;
		}

		red_black_transplant (z, y);
		y-> gauche = z-> gauche ;
		y-> gauche -> parent = y;
		y-> couleur = z-> couleur ;
	}

	if (yOriginalcouleur == NOIR) {
		red_black_delete_fixup (x);
	}
}
void  red_black_delete_fixup ( struct noeud * x) {
	struct noeud * w;

	while (x != ROOT && x-> couleur == NOIR) {

		if (x == x-> parent -> gauche ) {
			w = x-> parent -> droite ;

			if (w-> couleur == ROUGE) {
				w-> couleur = NOIR;
				x-> parent -> couleur = ROUGE;
				left_rotate (x-> parent );
				w = x-> parent -> droite ;
			}

			if (w-> gauche -> couleur == NOIR && w-> droite -> couleur == NOIR) {
				w-> couleur = ROUGE;
				x-> parent -> couleur = NOIR;
				x = x-> parent ;
			}
			else {

				if (w-> droite -> couleur == NOIR) {
					w-> couleur = ROUGE;
					w-> gauche -> couleur = NOIR;
					right_rotate (w);
					w = x-> parent -> droite ;
				}

				w-> couleur = x-> parent -> couleur ;
				x-> parent -> couleur = NOIR;
				x-> droite -> couleur = NOIR;
				left_rotate (x-> parent );
				x = ROOT;

			}

		}
else {
			w = x-> parent -> gauche ;

			if (w-> couleur == ROUGE) {
				w-> couleur = NOIR;
				x-> parent -> couleur = NOIR;
				right_rotate (x-> parent );
				w = x-> parent -> gauche ;
			}

			if (w-> gauche -> couleur == NOIR && w-> droite -> couleur == NOIR) {
				w-> couleur = ROUGE;
				x-> parent -> couleur = NOIR;
				x = x-> parent ;
			}
			else {

				if (w-> gauche -> couleur == NOIR) {
					w-> couleur = ROUGE;
					w-> droite -> couleur = NOIR;
					left_rotate (w);
					w = x-> parent -> gauche ;
				}

				w-> couleur = x-> parent -> couleur ;
				x-> parent -> couleur = NOIR;
				w-> gauche -> couleur = NOIR;
				right_rotate (x-> parent );
				x = ROOT;

			}
		}

	}

	x-> couleur = NOIR;
}

void  red_black_transplant ( struct  noeud* u, struct  noeud * v) {
	if (u-> parent == NILL) {
		ROOT = v;
	}
	else  if (u == u-> parent -> gauche ) {
		u-> parent -> gauche = v;
	}
	else {
		u-> parent -> droite = v;
	}

	v-> parent = u-> parent ;
}
