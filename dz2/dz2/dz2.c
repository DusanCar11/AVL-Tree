#include <stdio.h>

typedef struct Elem {
	int broj;
	int visina;
	int balans;
	struct Elem *levi;
	struct Elem *desni;
} elem;

typedef struct Nizic {
	elem *stek[100];
}nizic;

int veca(int v1, int v2) {
	
	if (v1 >= v2)
		return v1;
	else
		return v2;
}


void printGivenLevel(elem* root, int level);
int height(elem* node);


void printLevel(elem *root, int level)
{
	if (root == NULL)
		return;
	if (level == 1) {
		printf("%d ", root->broj);
	}
	else if (level > 1)
	{
		printLevel(root->levi, level - 1);
		printLevel(root->desni, level - 1);
		
	}
}


void printLevelOrder(elem *root)
{
	int h = height(root);
	int i;
	for (i = 1; i <= h; i++)
		printLevel(root, i);
}

int height(elem *node)
{
	if (node == NULL)
		return 0;
	else
	{
		int lheight = height(node->levi);
		int rheight = height(node->desni);

		if (lheight > rheight)
			return(lheight + 1);
		else
			return(rheight + 1);
	}
}

elem* desna_rotacija(elem *x) {
	elem *y, *temp;

	y = x->levi;
	temp = y->desni;
	y->desni = x;
	x->levi = temp;
}

elem* leva_rotacija(elem *x) {
	elem *y, *temp;

	y = x->desni;
	temp = y->levi;
	y->levi = x;
	x->desni = temp;
}

elem* pretraga(elem *root, int key) {
	elem *temp;

	temp = root;
	while ((temp != NULL) && (temp->broj != key)) {
		if (temp->broj < key)
			temp = temp->levi;
		else
			temp = temp->desni;
	}
	return temp;
}

elem* umetanje(elem *root, int key, elem *stek[100], int *k) {
	elem *novi, *pom1, *pom2;

	novi = malloc(1 * sizeof(elem));
	novi->broj = key;
	novi->balans = 0;
	novi->visina = -1;
	novi->levi = NULL;
	novi->desni = NULL;
	if (root == NULL)
		root = novi;
	else {
		pom1 = root;
		while (1) {
			if (key < pom1->broj) {
				if (pom1->levi != NULL) {
					pom1->visina = -1;
					stek[(*k)++] = pom1;
					pom1 = pom1->levi;
				}
				else {
					pom1->levi = novi;
					pom1->visina = -1;
					break;
				}
			}
			else if (key > pom1->broj) {
				if (pom1->desni != NULL) {
					pom1->visina = -1;
					stek[(*k)++] = pom1;
					pom1 = pom1->desni;
				}
				else {
					pom1->desni = novi;
					pom1->visina = -1;
					break;
				}
			}
			else 
			{
				printf("Kljuc vec postoji u stablu!\n");
				return root;
			}
		}
	}
	return root;
}

elem *avl_insert(elem *root, int k) {
	elem *fp = NULL, *fx = NULL, *p, *x, *q, *y;
	int imbal;

	p = root;
	x = p;
	while (p != NULL) {
		if (k == p->broj)
			return root;
		if (k < p->broj)
			q = p->levi;
		else
			q = p->desni;

		if (q != NULL) {
			if (q->balans != 0) {
				fx = p;
				x = q;
			}
		}
		fp = p;
		p = q;
	}
	q = malloc(1 * sizeof(elem));
	q->broj = k;
	q->balans = 0;
	q->visina = 0;
	q->levi = NULL;
	q->desni = NULL;
	if (!root) {
		root = q;
		return root;
	}
	if (k < fp->broj)
		fp->levi = q;
	else
		fp->desni = q;

	if (k < x->broj)
		p = x->levi;
	else
		p = x->desni;

	y = p;

	while (p != q) {
		if (k < p->broj) {
			p->balans = 1;
			p = p->levi;
		}
		else {
			p->balans = -1;
			p = p->desni;
		}
	}

	if (k < x->broj)
		imbal = 1;
	else
		imbal = -1;

	if (x->balans == 0) {
		x->balans = imbal;
		return root;
	}

	if (x->balans != imbal) {
		x->balans = 0;
		return root;
	}

	if (y->balans == imbal) {
		p = y;
		if (imbal == 1)
			desna_rotacija(x);
		else
			leva_rotacija(x);
		x->balans = y->balans = 0;
	}
	else {
		if (imbal == 1) {
			p = y->desni;
			leva_rotacija(y);
			x->levi = p;
			desna_rotacija(x);
		}
		else {
			p = y->levi;
			x->desni = p;
			desna_rotacija(y);
			leva_rotacija(x);
		}
		if (p->balans == 0)
			x->balans = y->balans = 0;
		else {
			if (p->balans == imbal) {
				x->balans = -imbal;
				y->balans = 0;
			}
			else {
				x->balans = 0;
				y->balans = imbal;
			}
			p->balans = 0;
		}
	}
if (fx == NULL) 
	root = p;
else {
	if (x == fx->desni)
		fx->desni = p;
	else
		fx->levi = p;
}
return root;

}

void ubaci_leve(elem *root, elem **stek, int *top) {
	elem *p;
	
	p = root;
	while (p) {
		(*top) += 1;
		stek[(*top)] = p;
		p = p->levi;
	}
}

int inorder(elem *root) {
	elem *current, *stek[100];
	int top=0;

	current = root;
	ubaci_leve(current, stek, &top);

	while (current) {
		if (top)
			current = stek[top--];
		else
			current = NULL;
		if (current) {
			printf("%d ", current->broj);
			ubaci_leve(current->desni, stek, &top);
		}
	}
}

int obrisi(elem *root) {
	elem *current, *stek[100];
	int top = 0;

	current = root;
	ubaci_leve(current, stek, &top);

	while (current) {
		if (top)
			current = stek[top--];
		else
			current = NULL;
		if (current) {
			ubaci_leve(current->desni, stek, &top);
			free(current);
		}
	}
}

elem* razlika(elem *root1, elem *root2) {
	elem *p, *q, *stek1[100], *stek2[100], *root3=NULL;
	int top1 = 0, top2 = 0;

	p = root1;
	q = root2;

	ubaci_leve(p, stek1, &top1);
	ubaci_leve(q, stek2, &top2);

	if (top1)
		p = stek1[top1--];
	else
		p = NULL;

	if (top2)
		q = stek2[top2--];
	else
		q = NULL;

	while (p) {
		if (!q) {
			root3 = avl_insert(root3, p->broj);
			ubaci_leve(p->desni, stek1, &top1);
			if (top1)
				p = stek1[top1--];
			else
				p = NULL;
			continue;
		}
		if (p->broj < q->broj) {
			root3= avl_insert(root3, p->broj);
			ubaci_leve(p->desni, stek1, &top1);
			if (top1)
				p = stek1[top1--];
			else
				p = NULL;
		}
		else if (p->broj == q->broj) {
			ubaci_leve(p->desni, stek1, &top1);
			ubaci_leve(q->desni, stek2, &top2);
			if (top1)
				p = stek1[top1--];
			else
				p = NULL;

			if (top2)
				q = stek2[top2--];
			else
				q = NULL;
		}
		else {
			ubaci_leve(q->desni, stek2, &top2);
			if (top2)
				q = stek2[top2--];
			else
				q = NULL;
		}
	}
	inorder(root3);
	obrisi(root3);
}

void main() {
	int izbor, kljuc, lazni_int;
	elem *glava1 = NULL, *temp, *niz[100] = { NULL }, *glava2 = NULL;
	char odgovor;

	printf("\nIzaberite operaciju koju zelite da izvrsite biranjem broja ispred zeljene operacije\n");
	printf("1. Formiranje AVL stabla\n");
	printf("2. Pretrazivanje stabla na odredjeni kljuc\n");
	printf("3. Umetanje kljuca u stablo\n");
	printf("4. Ispis stabla\n");
	printf("5. Razlika dva AVL stabla\n");
	printf("6. Prekid izvrsavanja\n");

	scanf("%d", &izbor);

	while (izbor) {
		switch (izbor) {
		case 1: {
			printf("Unosite kljuceve\n");
			scanf("%d", &kljuc);
			while (kljuc != 0) {
				glava1 = avl_insert(glava1, kljuc);
				scanf("%d", &kljuc);
			}
			
		} break;
		case 2: {
			printf("Unesite kljuc koji zelite da trazite\n");
			scanf("%d", &kljuc);
			temp = pretraga(glava1, kljuc);
			if (temp == NULL)
				printf("Kljuc ne postoji u stablu\n");
			else
				printf("Vas kljuc je pronadjen\n");
		} break;
		case 3: {
			printf("Unesite kljuc koji zelite da umetnete u stablo\n");
			scanf("%d", &kljuc);
			glava1 = avl_insert(glava1, kljuc);
		} break;
		case 4: {
			if (glava1 == NULL)
				printf("nema idiote\n");
			inorder(glava1);}
				break;
		case 5: {
			printf("Unosite kljuceve drugog\n");
			scanf("%d", &kljuc);
			while (kljuc != 0) {
				glava2 = avl_insert(glava2, kljuc);
				scanf("%d", &kljuc);
			}
			razlika(glava1, glava2);
		} break;
		case 6: { obrisi(glava1);
			obrisi(glava2);
			exit(1);}
		}

		printf("\nDa li zelite da izvrsite jos neku operaciju (Y/N)\n");
		getchar();
		scanf("%c", &odgovor);

		if (odgovor != 'Y') break;

		printf("\nIzaberite operaciju koju zelite da izvrsite biranjem broja ispred zeljene operacije\n");
		printf("1. Formiranje AVL stabla\n");
		printf("2. Pretrazivanje stabla na odredjeni kljuc\n");
		printf("3. Umetanje kljuca u stablo\n");
		printf("4. Ispis stabla\n");
		printf("5. Razlika dva AVL stabla\n");
		printf("6. Prekid izvrsavanja\n");

		scanf("%d", &izbor);

	}
}