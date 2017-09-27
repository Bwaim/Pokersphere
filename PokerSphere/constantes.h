#ifndef CONSTANTES_H
#define CONSTANTES_H


/*#define HOSTNAME  "localhost"
#define DATABASE_NAME "poker_sphere"
#define SQL_USER_NAME "root"
#define SQL_PASSWORD ""
*/
//#define HOSTNAME  "phpmyadmin.free.fr"
/*#define HOSTNAME  "212.27.63.202"
#define DATABASE_NAME "arkanepoker"
#define SQL_USER_NAME "arkanepoker"
#define SQL_PASSWORD "asroi33"*/


static const char *MISSING_LOGIN       __attribute__ ((unused)) = "Renseigner un nom utilisateur";
static const char * MISSING_PASSWORD   __attribute__ ((unused)) = "Renseigner un mot de passe";
static const char * WRONG_LOGIN		__attribute__ ((unused)) = "Mauvais login ou mot de passe";

const int TAILLE_TYPE_PARTIE __attribute__ ((unused)) = 3;

static const char *TYPE_PARTIE[TAILLE_TYPE_PARTIE] __attribute__ ((unused)) = {"Tournoi MTT", "Sit & Go", "Partie Libre"};

const int TAILLE_TYPE_PARTIE_DEPOT __attribute__ ((unused)) = 4;

static const char *TYPE_PARTIE_DEPOT[TAILLE_TYPE_PARTIE_DEPOT] __attribute__ ((unused)) = {"Tournoi MTT", "Sit & Go", "Partie Libre", "Bonus"};


const int TAILLE_TYPE_PARTIE_COMPLET __attribute__ ((unused)) = 6;

static const char *TYPE_PARTIE_COMPLET[TAILLE_TYPE_PARTIE_COMPLET] __attribute__ ((unused)) = {"Tournoi MTT", "Sit & Go", "Partie Libre", "Enchères", "Adhésion", "Bonus"};

const int TAILLE_VARIANTE __attribute__ ((unused)) = 4;

static const char *TYPE_VARIANTE[TAILLE_VARIANTE] __attribute__ ((unused)) = {"Holdem NL", "Holdem PL", "Omaha PL", "Autre"};

static const char * CREATION_INFORMATIONS_MISSING __attribute__ ((unused)) = "Vous devez renseigner un nom, un prénom et une adresse mail !";

static const char * FILTER_FRANCHISE_SIZE __attribute__ ((unused)) = "Vous devez entrer au moins 4 caractères pour la recherche.";

const int TAILLE_FID_TYPES __attribute__ ((unused)) = 6;

static const char *FID_TYPES[TAILLE_FID_TYPES] __attribute__ ((unused)) = {"Mois Abonnement", "Nombre de tournois", "Chargement Euros", "Parrainage", "Points Fidélité", "Achat avec Points de Fidélité"};
static const char *FID_TYPES_OPERATIONS[TAILLE_FID_TYPES] __attribute__ ((unused)) = {"Mois Abonnement", "Nombre de tournois", "Chargement Euros", "Parrainage", "Gain de tournoi", "Achat avec Points de Fidélité"};

#endif // CONSTANTES_H
