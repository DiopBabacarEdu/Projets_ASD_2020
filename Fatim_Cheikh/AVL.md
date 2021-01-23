# Algorithmes et structurs de données
## Projet ASD 2021
### Fatime Guèye & Cheikh Aïdara

```c
#include<stdio.h>
#include<stdlib.h>

struct Noeud
{
    int val;
    struct Noeud*gauche;
    struct Noeud*droit;
    int hauteur;
};

// Une fonction permettant de connaitre le maximum entre deux entiers
int max(int a, int b);

// Une fonction permettant de determiner la hauteur d'un arbre
int hauteur(struct Noeud *N)
{
    if (N == NULL)
        return 0;
    return N->hauteur;
}

// Une fonction permettant de connaitre le maximum entre deux entiers
int max(int a, int b)
{
    return (a > b)? a : b;
}

//fonction d'assistance qui alloue un nouveau noeud avec la valeur donnee et les pointeurs NULL gauche et droit
struct Noeud* nouveauNoeud(int nouveau)
{
    struct Noeud* noeud= (struct Noeud*)malloc(sizeof(struct Noeud));
    noeud->val   = nouveau;
    noeud->gauche  = NULL;
    noeud->droit  = NULL;
    noeud->hauteur = 1;  // le nouveau noeud initialement ajoute est une feuille
    return(noeud);
}

// Une fonction permettant de faire pivoter a droite le sous-arbre enracine avec y
// voir le diagramme ci-dessous
struct Noeud *RotationDroite(struct Noeud *y)
{
    struct Noeud *x = y->gauche;
    struct Noeud *T2 = x->droit;

    // Effectuer la rotation
    x->droit = y;
    y->gauche = T2;

    // Mettre a jour les hauteurs des sous-arbres
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droit))+1;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droit))+1;

    // retourner le nouveau noeud
    return x;
}

// Une fonction permettant de faire pivoter a gauche le sous-arbre enracine avec y
// voir le diagramme ci-dessous
struct Noeud *RotationGauche(struct Noeud*x)
{
    struct Noeud *y = x->droit;
    struct Noeud *T2 = y->gauche;

    // Effectuer la rotation
    y->gauche = x;
    x->droit = T2;

    //  Maitre a jour les hauteurs des sous-arbres
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droit))+1;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droit))+1;

    // retourner le nouveau noeud
    return y;
}
// Obtenir le facteur d'equilibre du noeud N
int equilibrer(struct Noeud *N)
{
    if (N == NULL)
        return 0;
    return hauteur(N->gauche) - hauteur(N->droit);
}
// Fonction recursive pour inserer un nombre dans le sous-arbre enracine
//avec le noeud et retourne la nouvelle racine du sous-arbre

struct Noeud* inserer(struct Noeud* noeud, int val)
{
    /* 1.  Effectuer l'insertion dans un ABR */
    if (noeud == NULL)
        return(nouveauNoeud(val));

    if (val < noeud->val)
        noeud->gauche  = inserer(noeud->gauche, val);
    else if (val > noeud->val)
        noeud->droit = inserer(noeud->droit, val);
    else
        printf("Cette valeur existe deja dans l'arbre\n");// les valeurs egales ne sont pas autorisees dans un ABR
        return noeud;

    /* 2. Mettre a jour les hauteurs de ce noeud ancetre */
    noeud->hauteur = 1 + max(hauteur(noeud->gauche),
                           hauteur(noeud->droit));

    /* 3.  Obtenir le facteur d'equilibre de ce noeud ancetre pour
            verifier si ce noeud est devenu desequilibre*/
    int equilibre = equilibrer(noeud);

    // Si ce noeud devient desequilibre, alors il y a 4 cas

     // Cas gauche gauche
    if (equilibre > 1 && val < noeud->gauche->val)
        return RotationDroite(noeud);

    // Cas droite droite
    if (equilibre < -1 && val > noeud->droit->val)
        return RotationGauche(noeud);

    // Cas gauche droite
    if (equilibre > 1 && val > noeud->gauche->val)
    {
        noeud->gauche =  RotationGauche(noeud->gauche);
        return RotationDroite(noeud);
    }

    // Cas droite gauche
    if (equilibre < -1 && val < noeud->droit->val)
    {
        noeud->droit = RotationDroite(noeud->droit);
        return RotationGauche(noeud);
    }

    return noeud;
}

/*Etant donne un arbre de recherche binaire non vide,
    retourne le noeud avec la valeur minimale trouvee dans
    cet arbre. Notez que l'arborescence entiere n'a pas
    besoin d'etre recherchee*/
struct Noeud * ValeurMin(struct Noeud* noeud)
{
	struct Noeud* courant = noeud;

	/* boucle vers le bas pour trouver la feuille la plus a gauche*/
	while (courant->gauche != NULL)
		courant = courant->gauche;

	return courant;
}

// fonction recursive pour supprimer un noeud avec une
//valeur doonee du sous-arbre avec une racine donnee.
//Il renvoie la racine du sous-arbre modifie
struct Noeud* supprimerNoeud(struct Noeud* root, int aSupprimer)
{
	// ETAPE 1 : Effectuer une suppression dans un ABR standard

	if (root == NULL)
        printf("L'arbre est vide\n");
		return root;

	// Si la valeur a supprimer est plus petite que la valeur de la racine,
	//alors elle se trouve dans le sous-arbre gauche
	if ( aSupprimer < root->val )
		root->gauche = supprimerNoeud(root->gauche, aSupprimer);

	// Si la valeur a supprimer est plus grande que la valeur de la racine,
	//alors elle se trouve dans le sous-arbre droit
	else if( aSupprimer > root->val )
		{root->droit = supprimerNoeud(root->droit, aSupprimer);}

	// ,Si la valeur a supprimer est la meme que la valeur de la racine
	// alors c'est la valeur a supprimer
	else
	{
		/* noeud avec un seul enfant ou pas d'enfant*/
		if( (root->gauche == NULL) || (root->droit == NULL) )
		{
			struct Noeud *temp = root->gauche ? root->gauche : root->droit;

			// Cas ou il n'a pas d'enfant
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // Cas ou il n'a qu'un seul enfant
			*root = *temp; // Copier le contenu de l'enfant non vide
			free(temp);
		}
		else
		{
			// noeud avec deux enfants: recupere le successeur
			//inorder(le plus petit du sous-arbre)
			struct Noeud* temp = ValeurMin(root->droit);

			// Copier les donnees du successeur preordre sur ce noeud
			root->val = temp->val;

			// Supprimer le successeur inordre
			root->droit = supprimerNoeud(root->droit, temp->val);
		}
	}

	// Si l'arbre n'avait qu'un seul noeud alors retournez
	if (root == NULL)
	return root;

    //printf("Cette valeur n'est pas dans l'arbre\n");
	// ETAPE 2 : Mettre a jour la hauteur du noeud courant
	root->hauteur = 1 + max(hauteur(root->gauche),
						hauteur(root->droit));
    printf("Cette valeur n'est pas dans l'arbre\n");
	// ETAPE 3 : Obtenir le facteur d'equilibre de ce noeud (
    //verifier si ce noeud est devenu desequilibre

	int equilibre = equilibrer(root);

	// Si le noeud est devenu desequilibre, alors il y a 4 cas

	// Cas gauche gauche
	if (equilibre > 1 && equilibrer(root->gauche) >= 0)
		return RotationDroite(root);

	// Cas gauche droite
	if (equilibre > 1 && equilibrer(root->gauche) < 0)
	{
		root->gauche = RotationGauche(root->gauche);
		return RotationDroite(root);
	}

	//Cas droite droite
	if (equilibre < -1 && equilibrer(root->droit) <= 0)
		return RotationGauche(root);

	// Cas droite gauche
	if (equilibre < -1 && equilibrer(root->droit) > 0)
	{
		root->droit = RotationDroite(root->droit);
		return RotationGauche(root);
	}
	return root;
}

struct Noeud*recherche(struct Noeud*root, int aChercher ){
    if (root == NULL || root->val == aChercher){
        return root;
    }
    else{
        if(root->val < aChercher)
            return recherche(root->droit, aChercher);
        else{
        return recherche(root->gauche, aChercher);
        }
    }
};
// La fonction permettant d'afficher le parcours preordre de l'arbre
// La fonction affiche aussi la hauteur de tous les noeuds
void preOrdre(struct Noeud *root)
{
    if(root != NULL)
    {
        printf("%d =>", root->val);
        preOrdre(root->gauche);
        preOrdre(root->droit);
    }
}

//Menu de choix
void menu(){

	printf("\n--------MENU DE CHOIX-----\n");
	printf("1: Afficher l'arbre \n");
	printf("2: Insertion d'un noeud \n");
	printf("3: Suppression d'un noeud \n");
	printf("4: Recherche une valeur dans l'arbre\n");
	printf("5: Sortie\n");
}

int main()
{
	struct Noeud *root = NULL;
	int choix,nombre;
    menu();
	while(1){
		printf("\nQuelle operation voulez-vous effectuer?\n");
		scanf("%d",&choix);

		switch(choix){
			case 1:{
			    if(root==NULL){
                    printf("L'arbre est vide");
			    }
                else{
			        printf("Le parcours preordre de l'arbre AVL est \n");
				preOrdre(root);
			    }
				break;
			}
			case 2:{
				printf("Donnez le nombre a inserer\n");
				scanf("%d",&nombre);
				root = inserer(root,nombre);
				printf("Le parcours preordre de l'arbre AVL apres insertion de ce nombre est \n");
				preOrdre(root);
				break;
			}
			case 3:{
				printf("Donnez le nombre que vous voulez supprimer\n");
				scanf("%d",&nombre);
				root = supprimerNoeud(root,nombre);
				printf("\nLe parcours preordre de l'arbre AVL apres suppression est \n");
				preOrdre(root);
				break;
			}
			case 4:{
			    int aChercher;
			    printf("Donnez la valeur que vous cherchez\n");
			    scanf("%d",&aChercher);
			    root = recherche(root,aChercher);
			    break;
			}
			case 5:{exit(1);
				break;
			default: printf("Ce choix n'existe pas !\n");
			}

		}
	}
  return 0;
}

```
