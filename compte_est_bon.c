#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define MAX 6

/* Structure pour memoriser une etape de calcul de l'IA */
typedef struct {
    char left[256];
    char right[256];
    char op;
    long long result;
} Step;

/* Variables globales */
long long target;
long long best_value;
char best_expr[256];
Step best_steps[32];
int best_steps_len;

/* --------- Generation des nombres de depart */
void generateNombres(long long nums[]) {
    int grands[] = {25, 50, 75, 100};
    int nb_grand = rand() % 3; /* 0, 1 ou 2 grands nombres */

    /* Tirage sans remise des grands nombres */
    int deja[4] = {0, 0, 0, 0};
    for (int i = 0; i < nb_grand; i++) {
        int r;
        do {
            r = rand() % 4;
        } while (deja[r]);
        deja[r] = 1;
        nums[i] = grands[r];
    }

    for (int i = nb_grand; i < MAX; i++) {
        nums[i] = (rand() % 10) + 1; /* 1 a 10 */
    }
}

/* --------- Afficher les regles */
void afficherRegles() {
    printf("\n=== REGLES DU JEU : LE COMPTE EST BON ===\n");
    printf("1. Une cible entre 100 et 999 est generee au hasard.\n");
    printf("2. Vous disposez de 6 nombres (1 a 10 et parfois 25, 50, 75, 100).\n");
    printf("3. Votre objectif est d atteindre la cible en combinant ces nombres\n");
    printf("   avec les operations +, -, * et /.\n");
    printf("\n--- Utilisation des nombres \n");
    printf("4. A chaque etape, vous choisissez deux nombres disponibles et une operation :\n");
    printf("   a op b\n");
    printf("   Exemple : 5 * 10, 8 + 3, 100 - 25\n");
    printf("5. Les deux nombres utilises disparaissent de la liste et sont remplaces\n");
    printf("   par le resultat de l operation.\n");
    printf("6. Ce resultat devient un nouveau nombre disponible que vous pouvez\n");
    printf("   reutiliser dans les etapes suivantes.\n");
    printf("\n--- Contraintes sur les operations \n");
    printf("7. Soustraction : le resultat doit etre strictement positif.\n");
    printf("8. Division : la division doit etre exacte (pas de reste) et le denominateur\n");
    printf("   doit etre non nul.\n");
    printf("\n--- Fin de votre calcul \n");
    printf("9. Vous continuez jusqu a ce qu il ne reste plus qu un seul nombre :\n");
    printf("   ce sera votre resultat final.\n");
    printf("10. Le programme calcule ensuite :\n");
    printf("    diff = cible - resultat_final\n");
    printf("    - Si diff = 0 : vous avez trouve exactement la cible -> victoire !\n");
    printf("    - Sinon : le programme indique a quel point vous etiez proche.\n");
    printf("\n--- Abandon et IA \n");
    printf("11. A tout moment, vous pouvez abandonner en tapant : 0 0 0\n");
    printf("    (cela signifie : a = 0, op = '0', b = 0 dans la saisie).\n");
    printf("12. En cas d abandon ou apres votre essai, l IA cherche une solution\n");
    printf("    en utilisant les memes nombres et les memes regles que vous :\n");
    printf("    - si elle peut atteindre exactement la cible, elle le fait ;\n");
    printf("    - sinon, elle donne le meilleur resultat possible.\n");
    printf("13. L IA affiche les etapes de calcul en detail (operation par operation).\n");
    printf("============================================\n");
}

/* --------- IA : solveur recursif classique du Compte est Bon */
/*
   nums[]     : valeurs disponibles
   n          : nombre de valeurs
   exprs      : expression texte associee a chaque valeur
   steps      : etapes effectuees jusque-la
   step_count : nombre d etapes deja enregistrees
*/
void solveIA(long long nums[], int n, char exprs[][256], Step steps[], int step_count) {
    if (best_value == target) {
        /* Une solution exacte a deja ete trouvee */
        return;
    }

    if (n == 1) {
        long long val = nums[0];
        long long current_diff = llabs(val - target);
        long long best_diff = llabs(best_value - target);

        if (current_diff < best_diff) {
            best_value = val;
            strcpy(best_expr, exprs[0]);
            best_steps_len = step_count;
            for (int i = 0; i < step_count; i++) {
                best_steps[i] = steps[i];
            }
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long a = nums[i], b = nums[j];
            char exprA[256], exprB[256];
            strcpy(exprA, exprs[i]);
            strcpy(exprB, exprs[j]);

            long long next_nums[MAX];
            char next_exprs[MAX][256];
            int m = 0;

            /* recopier les autres valeurs */
            for (int k = 0; k < n; k++) {
                if (k != i && k != j) {
                    next_nums[m] = nums[k];
                    strcpy(next_exprs[m], exprs[k]);
                    m++;
                }
            }

            /* 1) Addition : a + b */
            {
                long long res = a + b;
                Step s;
                strcpy(s.left, exprA);
                strcpy(s.right, exprB);
                s.op = '+';
                s.result = res;
                next_nums[m] = res;
                sprintf(next_exprs[m], "(%s + %s)", exprA, exprB);
                steps[step_count] = s;
                solveIA(next_nums, m + 1, next_exprs, steps, step_count + 1);
                if (best_value == target) return;
            }

            /* 2) Multiplication : a * b */
            {
                long long res = a * b;
                Step s;
                strcpy(s.left, exprA);
                strcpy(s.right, exprB);
                s.op = '*';
                s.result = res;
                next_nums[m] = res;
                sprintf(next_exprs[m], "(%s * %s)", exprA, exprB);
                steps[step_count] = s;
                solveIA(next_nums, m + 1, next_exprs, steps, step_count + 1);
                if (best_value == target) return;
            }

            /* 3) Soustraction : a - b, resultat strictement positif */
            if (a - b > 0) {
                long long res = a - b;
                Step s;
                strcpy(s.left, exprA);
                strcpy(s.right, exprB);
                s.op = '-';
                s.result = res;
                next_nums[m] = res;
                sprintf(next_exprs[m], "(%s - %s)", exprA, exprB);
                steps[step_count] = s;
                solveIA(next_nums, m + 1, next_exprs, steps, step_count + 1);
                if (best_value == target) return;
            }

            /* 4) Soustraction inverse : b - a, resultat strictement positif */
            if (b - a > 0) {
                long long res = b - a;
                Step s;
                strcpy(s.left, exprB);
                strcpy(s.right, exprA);
                s.op = '-';
                s.result = res;
                next_nums[m] = res;
                sprintf(next_exprs[m], "(%s - %s)", exprB, exprA);
                steps[step_count] = s;
                solveIA(next_nums, m + 1, next_exprs, steps, step_count + 1);
                if (best_value == target) return;
            }

            /* 5) Division : a / b si exacte */
            if (b != 0 && a % b == 0) {
                long long res = a / b;
                Step s;
                strcpy(s.left, exprA);
                strcpy(s.right, exprB);
                s.op = '/';
                s.result = res;
                next_nums[m] = res;
                sprintf(next_exprs[m], "(%s / %s)", exprA, exprB);
                steps[step_count] = s;
                solveIA(next_nums, m + 1, next_exprs, steps, step_count + 1);
                if (best_value == target) return;
            }

            /* 6) Division inverse : b / a si exacte */
            if (a != 0 && b % a == 0) {
                long long res = b / a;
                Step s;
                strcpy(s.left, exprB);
                strcpy(s.right, exprA);
                s.op = '/';
                s.result = res;
                next_nums[m] = res;
                sprintf(next_exprs[m], "(%s / %s)", exprB, exprA);
                steps[step_count] = s;
                solveIA(next_nums, m + 1, next_exprs, steps, step_count + 1);
                if (best_value == target) return;
            }
        }
    }
}

/* --------- IA : lance la resolution et affiche les etapes */
void modeIA(long long original_nums[]) {
    char exprs[MAX][256];
    for (int i = 0; i < MAX; i++) {
        sprintf(exprs[i], "%lld", original_nums[i]);
    }

    best_value = LLONG_MAX / 4;
    best_expr[0] = '\0';
    best_steps_len = 0;

    Step steps[32];
    solveIA(original_nums, MAX, exprs, steps, 0);

    printf("\n--- Solution proposee par l IA ---\n");
    printf("Cible : %lld\n", target);
    printf("Nombres de depart : ");
    for (int i = 0; i < MAX; i++) {
        printf("%lld ", original_nums[i]);
    }
    printf("\n");

    if (best_expr[0] == '\0') {
        printf("L IA n a trouve aucune combinaison (ce qui est tres improbable).\n");
        return;
    }

    long long diff = target - best_value;
    long long ecart = llabs(diff);

    if (ecart == 0) {
        printf("L IA a trouve une solution exacte.\n");
    } else if (ecart <= 5) {
        printf("L IA a trouve une solution tres proche de la cible.\n");
    } else if (ecart <= 20) {
        printf("L IA a trouve une solution raisonnablement proche.\n");
    } else {
        printf("L IA est assez loin de la cible, mais c est le meilleur resultat possible.\n");
    }

    printf("\nExpression finale de l IA : %s = %lld\n", best_expr, best_value);
    printf("cible - resultat = %lld (ecart absolu : %lld)\n", diff, ecart);

    printf("\nDetails des etapes de l IA :\n");
    for (int i = 0; i < best_steps_len; i++) {
        printf("  Etape %d : %s %c %s = %lld\n", i + 1,
               best_steps[i].left, best_steps[i].op, best_steps[i].right,
               best_steps[i].result);
    }
    printf("\n");
}

/* --------- Afficher les nombres disponibles pour le joueur */
void afficherNombresCourants(long long nums[], int n) {
    printf("\nNombres disponibles : ");
    for (int i = 0; i < n; i++) {
        printf("%lld ", nums[i]);
    }
    printf("\n");
}

/* --------- Mode joueur : operations avec reutilisation des resultats */
void modeJoueur(long long original_nums[]) {
    long long nums[MAX];
    int n = MAX;

    for (int i = 0; i < MAX; i++) {
        nums[i] = original_nums[i];
    }

    printf("\nCible a atteindre : %lld\n", target);

    while (n > 1) {
        afficherNombresCourants(nums, n);
        printf("Entrez une operation sous la forme : a op b\n");
        printf("Exemples : 5 * 10    8 + 3    100 - 25\n");
        printf("Pour abandonner et laisser l IA jouer, tapez : 0 0 0\n");
        printf("Votre saisie : ");

        int a_input, b_input;
        char op;

        if (scanf("%d %c %d", &a_input, &op, &b_input) != 3) {
            printf("Entree invalide. Veuillez reessayer.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        /* Abandon : 0 0 0 */
        if (a_input == 0 && b_input == 0) {
            printf("\nVous avez choisi d abandonner. L IA va maintenant chercher une solution.\n");
            modeIA(original_nums);
            return;
        }

        /* Chercher les indices des valeurs a_input et b_input dans nums[] */
        int idxA = -1, idxB = -1;
        for (int i = 0; i < n; i++) {
            if (nums[i] == a_input) {
                idxA = i;
                break;
            }
        }
        for (int i = 0; i < n; i++) {
            if (i != idxA && nums[i] == b_input) {
                idxB = i;
                break;
            }
        }

        if (idxA == -1 || idxB == -1) {
            printf("Ces nombres ne sont pas disponibles (ou pas disponibles ensemble).\n");
            continue;
        }

        long long a = nums[idxA];
        long long b = nums[idxB];
        long long result = 0;
        int valide = 1;

        /* accepter x ou X comme multiplication */
        if (op == 'x' || op == 'X') op = '*';

        switch (op) {
            case '+':
                result = a + b;
                break;
            case '-':
                if (a - b <= 0) {
                    printf("Soustraction non valide : le resultat doit etre strictement positif.\n");
                    valide = 0;
                } else {
                    result = a - b;
                }
                break;
            case '*':
                result = a * b;
                break;
            case '/':
                if (b == 0 || a % b != 0) {
                    printf("Division non valide : elle doit etre exacte et le denominateur non nul.\n");
                    valide = 0;
                } else {
                    result = a / b;
                }
                break;
            default:
                printf("Operation inconnue. Utilisez +, -, *, /.\n");
                valide = 0;
                break;
        }

        if (!valide) {
            continue;
        }

        printf("Resultat de %lld %c %lld = %lld\n", a, op, b, result);

        /*
           Mise a jour des nombres :
           - on remplace nums[idxA] par result
           - on supprime nums[idxB]
        */
        if (idxB < idxA) {
            int tmp = idxA;
            idxA = idxB;
            idxB = tmp;
        }
        nums[idxA] = result;
        for (int i = idxB; i < n - 1; i++) {
            nums[i] = nums[i + 1];
        }
        n--;

        if (n == 1) {
            break;
        }
    }

    long long final_result = nums[0];
    long long diff = target - final_result;
    long long ecart = llabs(diff);

    printf("\n=== Fin de votre calcul ===\n");
    printf("Resultat final obtenu : %lld\n", final_result);
    printf("cible - resultat = %lld (ecart absolu : %lld)\n", diff, ecart);

    if (ecart == 0) {
        printf("Bravo ! Vous avez exactement atteint la cible, parfait !\n");
    } else if (ecart <= 5) {
        printf("Tres bien joue, vous etes vraiment tres proche de la cible.\n");
    } else if (ecart <= 15) {
        printf("Pas mal du tout, vous etes assez proche de la cible.\n");
    } else if (ecart <= 30) {
        printf("Vous etiez a peu pres au milieu, il manquait encore un peu de precision.\n");
    } else {
        printf("Vous etes assez loin de la cible cette fois, mais ce n est pas grave !\n");
    }

    printf("\nMaintenant, voyons ce que l IA peut faire avec les memes nombres...\n");
    modeIA(original_nums);
}

/* --------- Menu principal */
void afficherMenu() {
    printf("\n=== LE COMPTE EST BON ===\n");
    printf("1. Jouer\n");
    printf("2. Regles du jeu\n");
    printf("3. Quitter\n");
    printf("Votre choix : ");
}

/* MAIN */
int main() {
    srand((unsigned)time(NULL));
    int choix;

    do {
        afficherMenu();

        if (scanf("%d", &choix) != 1) {
            printf("Entree invalide.\n");
            return 1;
        }

        if (choix == 1) {
            long long original_nums[MAX];
            generateNombres(original_nums);
            target = (rand() % 900) + 100; /* cible entre 100 et 999 */

            printf("\nNombres disponibles au depart : ");
            for (int i = 0; i < MAX; i++) {
                printf("%lld ", original_nums[i]);
            }
            printf("\nCible : %lld\n", target);

            modeJoueur(original_nums);

        } else if (choix == 2) {
            afficherRegles();

        } else if (choix != 3) {
            printf("Choix invalide.\n");
        }

    } while (choix != 3);

    printf("Merci d avoir joue ! A bientot.\n");
    return 0;
}
