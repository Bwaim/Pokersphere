#ifndef REQUETES_H
#define REQUETES_H

//#ifdef LOCAL 
//static const char *URL __attribute__ ((unused)) = "http://localhost/Fabien/passerelle.php";
//#else
//#ifdef FREE
//static const char *URL __attribute__ ((unused)) = "http://arkanepoker.free.fr/pokersphere_re7/logiciel/passerelle.php";
//#else
//#ifdef OVH
//static const char *URL __attribute__ ((unused)) = "http://poker-sphere.fr/logiciel/passerelle.php";
//#endif
//#endif
//#endif

static const char * SELECT __attribute__ ((unused)) = "SELECT";
static const char * UPDATE __attribute__ ((unused)) = "UPDATE";
static const char * INSERT __attribute__ ((unused)) = "INSERT";
static const char * DELETE_REQ __attribute__ ((unused)) = "DELETE";
static const char * GET_FILE __attribute__ ((unused)) = "GET_FILE";
static const char * SET_FILE __attribute__ ((unused)) = "SET_FILE";
static const char * NO_PHOTO __attribute__ ((unused)) = "NO_PHOTO";
static const char * ARRIVE __attribute__ ((unused)) = "ARRIVE";

static const char * HEADER_SEP __attribute__ ((unused)) = "<ENTETE>";
static const char * LINE_SEP __attribute__ ((unused)) = "<LIGNE>";
static const char * SEPARATOR __attribute__ ((unused)) = "<SEP>";
static const char * DATE_SEP __attribute__ ((unused)) = "/";
static const char * TIME_SEP __attribute__ ((unused)) = ":";

static const char * OK __attribute__ ((unused)) = "OK";
static const char * CREDIT_MENSUEL __attribute__ ((unused)) = "Credit Mensuel";
static const char * MOIS_DEJA_CREDITE __attribute__ ((unused)) = "Ce mois a dejà été crédité !!!";
static const char * ZERO __attribute__ ((unused)) = "0";
static const char * UN __attribute__ ((unused)) = "1";
static const char * AUTRE __attribute__ ((unused)) = "Autres...";
static const char * WEBCAM __attribute__ ((unused)) = "Webcam";
static const char * PB_WEBCAM __attribute__ ((unused)) = "Problème avec la webcam";
static const char * JPG __attribute__ ((unused)) = "JPG";
static const char * INFOS_TEMPLATE_TOURNOI __attribute__ ((unused)) = "Informations template tournoi";
static const char * CREATION_TEMPLATE_TOURNOI __attribute__ ((unused)) = "Création template tournoi";
static const char * NOM_TEMPLATE_EXISTANT __attribute__ ((unused)) = "Ce nom de template existe déjà !";
static const char * INFOS_TEMPLATE_TOURNOI_PHRASE __attribute__ ((unused)) = "Veuillez sélectionner un template !";
static const char * SUPP_TEMPLATE_TOURNOI __attribute__ ((unused)) = "Supprimer le template de tournoi";
static const char * SUPP_TEMPLATE_TOURNOI_PHRASE __attribute__ ((unused)) = "Etes vous sûr de vouloir supprimer le template ";
static const char * EXPORT __attribute__ ((unused)) = "Export";
static const char * PROBLEME_CREATION_FICHIER __attribute__ ((unused)) = "Problème lors de la création du fichier !!!";
static const char * EXPORT_SUCCES __attribute__ ((unused)) = "Export effectué avec succès.";
static const char * ENREGISTRER_FICHIER __attribute__ ((unused)) = "Enregistrer fichier";
static const char * OUI __attribute__ ((unused)) = "Oui";
static const char * NON __attribute__ ((unused)) = "Non";
static const char * SUPPRIMER_OPERATION __attribute__ ((unused)) = "Supprimer l'opération";
static const char * SUPPRIMER_OPERATION_PHRASE __attribute__ ((unused)) = "Etes vous sûr de vouloir supprimer cette opération ?";
static const char * ADHERENT_SANS_ABONNEMENT __attribute__ ((unused)) = "L'adhérent n'a pas d'abonnement !";
static const char * ECHEANCE_PHRASE __attribute__ ((unused)) = "Echéance au : ";
static const char * INFOS __attribute__ ((unused)) = "Informations";
static const char * PLUS_KOSMOS_DISPO __attribute__ ((unused)) = "Ce membre n'a plus de kosmos disponible !";
static const char * PLUS_KOSMOS_DISPO_ENCHERES __attribute__ ((unused)) = "Ce membre n'a plus de kosmos disponible pour des enchères !";
static const char * MESSAGE_FREE __attribute__ ((unused)) = "Attention, vous êtes connecté à la base Free. C'est une base de test.";
static const char * SUPP_ADHESION __attribute__ ((unused)) = "Supprimer l'adhésion";
static const char * SUPP_ADHESION_PHRASE __attribute__ ((unused)) = "Etes vous sûr de vouloir supprimer l'adhésion du ";
static const char * RC_PHRASE __attribute__ ((unused)) = "R.C";
static const char * SUPP_ADHERENT __attribute__ ((unused)) = "Supprimer l'adhérent";
static const char * SUPP_ADHERENT_PHRASE __attribute__ ((unused)) = "Etes vous sûr de vouloir supprimer l'adhérent ";
static const char * DESINSCRIRE __attribute__ ((unused)) = "Désinscrire";
static const char * DESINSCRIRE_PHRASE __attribute__ ((unused)) = "Etes vous sûr de vouloir désinscrire ";
static const char * PRESENTS __attribute__ ((unused)) = "Presents : ";
static const char * DESINSCRIRE_PHRASE_2 __attribute__ ((unused)) = "Vous ne pouvez pas désinscrire ce membre, le tournoi a déjà été validé !";
static const char * INSCRIRE __attribute__ ((unused)) = "Inscrire";
static const char * INSCRIRE_PHRASE __attribute__ ((unused)) = "Vous ne pouvez pas inscrire un membre à ce tournoi, il a déjà été validé !";
static const char * ANNULER __attribute__ ((unused)) = "Annuler";
static const char * ANNULER_PHRASE __attribute__ ((unused)) = "Vous ne pouvez pas annuler le tournoi, il y a déjà des sortants !";
static const char * ELIMINATION __attribute__ ((unused)) = "Elimination";
static const char * ELIMINATION_PHRASE __attribute__ ((unused)) = "Etes vous sûr de vouloir annuler la sortie de ce membre ?";
static const char * ELIMINATION_PHRASE_2 __attribute__ ((unused)) = "Vous ne pouvez pas sortir un membre de ce tournoi, il n'a pas débuté !";
static const char * ELIMINATION_PHRASE_3 __attribute__ ((unused)) = "Cette place est déjà prise !";
static const char * ATTENTE __attribute__ ((unused)) = "attente";
static const char * ATTENTE_PHRASE __attribute__ ((unused)) = "Veuillez patienter, traitement en cours.";
static const char * MODIFIER_TOURNOI __attribute__ ((unused)) = "Modifier le tournoi";
static const char * MODIFIER_TOURNOI_PHRASE __attribute__ ((unused)) = "Vous ne pouvez pas modifier ce tournoi, il a déjà été validé !";
static const char * INFOS_TOURNOI __attribute__ ((unused)) = "Informations tournoi";
static const char * INFOS_TOURNOI_PHRASE __attribute__ ((unused)) = "Veuillez sélectionner un tournoi !";
static const char * SUPP_TOURNOI __attribute__ ((unused)) = "Supprimer le tournoi";
static const char * SUPP_TOURNOI_PHRASE __attribute__ ((unused)) = "Etes vous sûr de vouloir supprimer le tournoi ";
static const char * SET_PRESENT_NON_DISPO __attribute__ ((unused)) = "L'arrivée de cette personne n'est pas prise en compte, décocher et recocher.";
static const char * IMPORT_MULTIPLE_SUCCES __attribute__ ((unused)) = "Inscription en masse terminée avec succès !";
static const char * INFOS_CLUBS __attribute__ ((unused)) = "Informations clubs";
static const char * CLUB_EXISTANT __attribute__ ((unused)) = "Ce nom de club existe déjà !";
static const char * INFOS_CLUBS_PHRASE __attribute__ ((unused)) = "Veuillez sélectionner un club !";
static const char * SUPP_CLUB __attribute__ ((unused)) = "Supprimer le club";
static const char * SUPP_CLUB_PHRASE __attribute__ ((unused)) = "Etes vous sûr de vouloir supprimer le club ";
static const char * CREATION_CLUB __attribute__ ((unused)) = "Création d'un club";
static const char * NOM_CLUB_EXISTANT __attribute__ ((unused)) = "Ce nom de club existe déjà !";
static const char * CREATION_FID_STATUT __attribute__ ((unused)) = "Création d'un statut de fidélisation";
static const char * NOM_FID_STATUT_EXISTANT __attribute__ ((unused)) = "Ce statut existe déjà !";
static const char * INFOS_FID_STATUT __attribute__ ((unused)) = "Informations statut de fidélisation";
static const char * INFOS_FID_STATUT_PHRASE __attribute__ ((unused)) = "Veuillez sélectionner un statut !";
static const char * SUPP_FID_STATUT __attribute__ ((unused)) = "Supprimer le statut";
static const char * SUPP_FID_STATUT_PHRASE __attribute__ ((unused)) = "Etes vous sûr de vouloir supprimer le statut ";
static const char * INFOS_MEMBRES __attribute__ ((unused)) = "Gestion des membres";
static const char * INFOS_AJOUT_CLUB_PHRASE __attribute__ ((unused)) = "Veuillez sélectionner une franchise !";
const int DOTATION_TYPE __attribute__ ((unused)) = 0;
const int DOTATION_DESCRIPTION __attribute__ ((unused)) = 1;
static const char * CREATION_DOTATION_TYPE __attribute__ ((unused)) = "Création d'un type de dotation";
static const char * DOTATION_TYPE_EXISTANT __attribute__ ((unused)) = "Ce type existe déjà !";
static const char * CREATION_DOTATION_DESCRPITION __attribute__ ((unused)) = "Création d'une description de dotation";
static const char * DOTATION_DESCRIPTION_EXISTANT __attribute__ ((unused)) = "Cette description existe déjà !";
static const char * INFOS_DOTATION __attribute__ ((unused)) = "Informations dotation";
static const char * INFOS_DOTATION_PHRASE __attribute__ ((unused)) = "Veuillez sélectionner une dotation !";
static const char * SUPP_DOTATION __attribute__ ((unused)) = "Supprimer la dotation";
static const char * SUPP_DOTATION_PHRASE __attribute__ ((unused)) = "Etes vous sûr de vouloir supprimer la dotation ";

static const char * ERREUR __attribute__ ((unused)) = "Erreur";
static const char * ERREUR_MSG1 __attribute__ ((unused)) = "Erreur lors de la récupération d'informations. Vérifiez votre connexion internet ou réessayez plus tard <br /><br /> Code de l'erreur : <br /><em>";

static const int SELECT_LOGIN __attribute__ ((unused)) = 1;
/* select u.ID, u.LOGIN, u.PASSWORD, g.ID AS ID_GROUP, g.NAME
   from TB01_USERS u, TB02_GROUP g
   WHERE u.LOGIN __attribute__ ((unused)) = '?0?'
   AND u.PASSWORD __attribute__ ((unused)) = SHA1('?1?')
*/

static const char * ID_REQ __attribute__ ((unused)) = "ID";
static const char * LOGIN __attribute__ ((unused)) = "LOGIN";
static const char * PASSWORD __attribute__ ((unused)) = "PASSWORD";
static const char * ID_GROUP __attribute__ ((unused)) = "ID_GROUP";
static const char * NAME __attribute__ ((unused)) = "NAME";

static const int SELECT_ALL_MEMBERS __attribute__ ((unused)) = 2;
/* select m.ID, m.NAME Nom, m.FIRSTNAME Prénom, m.Pseudo, DATE_FORMAT(m.BIRTH_DATE,'%d/%m/%Y') `Date de Naissance`
   , m.ADRESS Adresse
   , m.ZIP_CODE `Code Postal`, m.CITY Ville, m.PHONE Portable, m.EMAIL Mail, DATE_FORMAT(a2.ECHEANCE,'%d/%m/%Y') Echénace
   , DATE_FORMAT(m.CREATION_DATE,'%d/%m/%Y') Ancienneté, m.Kosmos, m.CLUB, a.VALUE RC
   from TB03_MEMBER m
		LEFT OUTER JOIN
		(select a.ID_MEMBER, a.DISABLE_DATE ECHEANCE,
		 CASE WHEN CURDATE() BETWEEN a.ENABLE_DATE AND a.DISABLE_DATE THEN a.VALUE ELSE 0 END VALUE
		 from TB04_ADHESION a
         where a.DISABLE_DATE __attribute__ ((unused)) = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER __attribute__ ((unused)) = a.ID_MEMBER)) a
        ON m.ID __attribute__ ((unused)) = a.ID_MEMBER
   WHERE m.DELETED __attribute__ ((unused)) = 'N'
   ORDER BY m.NAME, m.FIRSTNAME
*/

static const char * NOM __attribute__ ((unused)) = "Nom";
static const char * FIRSTNAME __attribute__ ((unused)) = "Prenom";
static const char * PSEUDO __attribute__ ((unused)) = "Pseudo";
static const char * BIRTH_DATE __attribute__ ((unused)) = "Date de Naissance";
static const char * ADRESS __attribute__ ((unused)) = "Adresse";
static const char * ZIP_CODE __attribute__ ((unused)) = "Code Postal";
static const char * CITY __attribute__ ((unused)) = "Ville";
static const char * EMAIL __attribute__ ((unused)) = "Mail";
static const char * PHONE __attribute__ ((unused)) = "Portable";
static const char * KOSMOS __attribute__ ((unused)) = "Kosmos";
static const char * CLUB __attribute__ ((unused)) = "CLUB";
static const char * ANCIENNETE __attribute__ ((unused)) = "Anciennete";
static const char * ECHEANCE __attribute__ ((unused)) = "Echeance";
static const char * RC __attribute__ ((unused)) = "RC";
static const char * CARD_NUMBER __attribute__ ((unused)) = "N° de Carte";
static const char * ID_UTILISATEUR __attribute__ ((unused)) = "Id Utilisateur";
static const char * SPONSOR_NAME __attribute__ ((unused)) = "Nom Parrain";
static const char * SPONSOR_FIRSTNAME __attribute__ ((unused)) = "Prenom Parrain";
static const char * PLAQUE_IMMATRICULATION __attribute__ ((unused)) = "Plaque d'immatriculation";
static const char * ID_PMU __attribute__ ((unused)) = "Id PMU";

static const int SELECT_MEMBERSHIP_MEMBER __attribute__ ((unused)) = 3;
/* select a.ID, a.VALUE `Type d'abonnement`, DATE_FORMAT(a.OPERATION_DATE,'%d/%m/%Y') `Date de l'opération`
   , DATE_FORMAT(a.ENABLE_DATE,'%d/%m/%Y') `Début de l'abonnement`, DATE_FORMAT(a.DISABLE_DATE,'%d/%m/%Y') `Fin de l'abonnement`
   from TB04_ADHESION a
   WHERE a.ID_MEMBER __attribute__ ((unused)) = ?0?
   ORDER BY a.OPERATION_DATE
*/

static const char * VALUE __attribute__ ((unused)) = "Type d'abonnement";
static const char * OPERATION_DATE __attribute__ ((unused)) = "Date de l'operation";
static const char * ENABLE_DATE __attribute__ ((unused)) = "Debut de l'abonnement";
static const char * DISABLE_DATE __attribute__ ((unused)) = "Fin de l'abonnement";
static const char * PRIX __attribute__ ((unused)) = "Prix";

static const int SELECT_ALL_MEMBERS_KOSMOS __attribute__ ((unused)) = 4;
/* select m.ID, m.NAME Nom, m.FIRSTNAME Prénom, m.Pseudo, m.Kosmos
   , CASE WHEN m.KOSMOS < ROUND((m.KOSMOS - IFNULL(k1.netResult,0)) / 100 * 5) + IFNULL(k1.netResult,0) 
		  THEN m.KOSMOS
		  ELSE ROUND((m.KOSMOS - IFNULL(k1.netResult,0)) / 100 * 5) + IFNULL(k1.netResult,0)
		  END `Kosmos Disponible`
   , m.KOSMOS - IFNULL(k2.KOSMOS_ADHESION,0) `Kosmos aux Enchères`
   , m.KOSMOS + IFNULL(k3.KOSMOS_RESULTAT_NET,0) `Résultat Net`
   , IFNULL(k4.KOSMOS_RESULTAT_MOIS_PREC,0) `Résultat Mois précédent`
   , IFNULL(k5.KOSMOS_RESULTAT_MOIS_COURANT,0) `Résultat Mois en cours`
   , DATE_FORMAT(a.ECHEANCE,'%d/%m/%Y') Echéance
*/

static const char * KOSMOS_DISPONIBLE __attribute__ ((unused)) = "Kosmos Disponible";
static const char * KOSMOS_DISPONIBLE_ENCHERE __attribute__ ((unused)) = "Kosmos aux Enchères";
static const char * KOSMOS_RESULTAT_NET __attribute__ ((unused)) = "Resultat Net";
static const char * KOSMOS_RESULTAT_MOIS_PREC __attribute__ ((unused)) = "Resultat Mois precedent";
static const char * KOSMOS_RESULTAT_MOIS_COURANT __attribute__ ((unused)) = "Resultat Mois en cours";

static const int SELECT_KOSMOS_MEMBER __attribute__ ((unused)) = 5;
/* select k.ID, k.VALUE `Valeur`, DATE_FORMAT(k.OPERATION_DATE,'%d/%m/%Y %H:%i') `Date de l'opération`
   , k.REASON `Type d'opération`, k.COMMENT `Infos Additionnelles`, k.CREDIT
   from TB05_KOSMOS k
   WHERE k.ID_MEMBER __attribute__ ((unused)) = ?0?
   ORDER BY k.OPERATION_DATE
*/

static const char * VALEUR __attribute__ ((unused)) = "Valeur";
static const char * REASON __attribute__ ((unused)) = "Type d'operation";
static const char * COMMENT __attribute__ ((unused)) = "Infos Additionnelles";
static const char * CREDIT __attribute__ ((unused)) = "CREDIT";

static const int SELECT_TOURNAMENT __attribute__ ((unused)) = 6;
/* select t.ID, t.NAME `Tournoi`, DATE_FORMAT(t.BEGIN_DATE,'%d/%m/%Y %H:%i') `Début du tournoi`
				, DATE_FORMAT(t.END_REGISTRATION_DATE,'%d/%m/%Y %H:%i') `Fin des inscriptions`, t.BUY_IN `Buy-in`
				, t.NB_PLAYERS `Nombre de joueurs`, t.Variante
				, (select count('x') 
				   from TB07_TOURNAMENT_REGISTRATION
                   where ID_TOURNAMENT __attribute__ ((unused)) = t.ID) `Nombre d'inscrits`
				, t.VALIDATED
				from TB06_TOURNAMENT t
				WHERE t.BEGIN_DATE BETWEEN str_to_date('?0?','%d/%m/%Y') AND str_to_date('?1?','%d/%m/%Y')
				ORDER BY t.BEGIN_DATE
*/

static const char * TOURNAMENT __attribute__ ((unused)) = "Tournoi";
static const char * BEGIN_DATE_TOURNAMENT __attribute__ ((unused)) = "Debut du tournoi";
static const char * END_REGISTRATION_TOURNAMENT __attribute__ ((unused)) = "Fin des inscriptions";
static const char * BUY_IN __attribute__ ((unused)) = "Buy-in";
static const char * NB_PLAYERS __attribute__ ((unused)) = "Nombre de joueurs";
static const char * VARIANTE __attribute__ ((unused)) = "Variante";
static const char * NB_SUBSCRIBES __attribute__ ((unused)) = "Nombre d'inscrits";
static const char * VALIDATED __attribute__ ((unused)) = "VALIDATED";
static const char * COMMENT_TOURNAMENT __attribute__ ((unused)) = "Commentaire";
static const char * PRIVATE __attribute__ ((unused)) = "Prive";
static const char * SENT_TO_RANKINGHERO __attribute__ ((unused)) = "SENT_TO_RANKINGHERO";

static const int SELECT_SUBSCRIBES_MEMBERS __attribute__ ((unused)) = 7;
/* select m.ID, m.NAME Nom, m.FIRSTNAME Prénom, m.Pseudo, m.Kosmos
   , DATE_FORMAT(a.ECHEANCE,'%d/%m/%Y') Echéance, tr.IS_LATE
   from TB03_MEMBER m
		LEFT OUTER JOIN
		(select a.ID_MEMBER, a.DISABLE_DATE ECHEANCE, 
		 CASE WHEN CURDATE() BETWEEN a.ENABLE_DATE AND a.DISABLE_DATE THEN a.VALUE ELSE 0 END VALUE
		 from TB04_ADHESION a
         where a.DISABLE_DATE __attribute__ ((unused)) = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER __attribute__ ((unused)) = a.ID_MEMBER)) a
        ON m.ID __attribute__ ((unused)) = a.ID_MEMBER
		, TB07_TOURNAMENT_REGISTRATION tr
    WHERE m.DELETED __attribute__ ((unused)) = 'N' and tr.ID_MEMBER __attribute__ ((unused)) = m.id and tr.ID_TOURNAMENT __attribute__ ((unused)) = ?0?
	ORDER BY m.NAME, m.FIRSTNAME
*/

static const char * IS_LATE __attribute__ ((unused)) = "IS_LATE";

static const int SELECT_MEMBERS_TO_SUBSCRIBE __attribute__ ((unused)) = 8;
/* select m.ID, m.NAME Nom, m.FIRSTNAME Prénom, m.Pseudo, m.Kosmos
   , DATE_FORMAT(a.ECHEANCE,'%d/%m/%Y') Echéance
   from TB03_MEMBER m
		LEFT OUTER JOIN
		(select a.ID_MEMBER, a.DISABLE_DATE ECHEANCE, 
		 CASE WHEN CURDATE() BETWEEN a.ENABLE_DATE AND a.DISABLE_DATE THEN a.VALUE ELSE 0 END VALUE
		 from TB04_ADHESION a
         where a.DISABLE_DATE __attribute__ ((unused)) = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER __attribute__ ((unused)) = a.ID_MEMBER)) a
        ON m.ID __attribute__ ((unused)) = a.ID_MEMBER
    WHERE m.DELETED __attribute__ ((unused)) = 'N' AND m.NAME like '?0?%'
	ORDER BY m.NAME, m.FIRSTNAME
*/

static const int SELECT_CHECK_MONTH_CREDIT __attribute__ ((unused)) = 9;
/* select CASE WHEN count('x') > 0 THEN 'OK' ELSE 'KO' END RESULT
				from TB05_KOSMOS k
                WHERE MONTH(DATE_SUB(k.OPERATION_DATE, INTERVAL 8 HOUR)) __attribute__ ((unused)) = MONTH(str_to_date('?0?','%d/%m/%Y'))
                AND YEAR(DATE_SUB(k.OPERATION_DATE, INTERVAL 8 HOUR)) __attribute__ ((unused)) = YEAR(str_to_date('?0?','%d/%m/%Y'))
                AND k.REASON __attribute__ ((unused)) = 4 AND k.VALUE __attribute__ ((unused)) = 1000
*/

static const char * RESULT __attribute__ ((unused)) = "RESULT";

static const int SELECT_STATISTIQUES __attribute__ ((unused)) = 10;
/* select (select count('x') from TB03_MEMBER) NB_ADHERENT,
				(select count(distinct a.ID_MEMBER) from TB04_ADHESION a
                     where a.DISABLE_DATE __attribute__ ((unused)) = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER __attribute__ ((unused)) = a.ID_MEMBER)
					 AND CURDATE() < a.DISABLE_DATE) NB_ADHERENT_A_JOUR,
				(select count(distinct a.ID_MEMBER) from TB04_ADHESION a
                     where a.DISABLE_DATE __attribute__ ((unused)) = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER __attribute__ ((unused)) = a.ID_MEMBER)
					 AND a.DISABLE_DATE BETWEEN CURDATE() AND DATE_ADD(CURDATE(), INTERVAL 7 DAY) ) NB_ADHERENT_QUI_EXPIRE,
				(select count(distinct a.ID_MEMBER) from TB04_ADHESION a
                     where a.DISABLE_DATE __attribute__ ((unused)) = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER __attribute__ ((unused)) = a.ID_MEMBER)
					 AND CURDATE() > a.DISABLE_DATE) NB_ADHERENT_EXPIRES
*/

static const char * NB_ADHERENT __attribute__ ((unused)) = "NB_ADHERENT";
static const char * NB_ADHERENT_A_JOUR __attribute__ ((unused)) = "NB_ADHERENT_A_JOUR";
static const char * NB_ADHERENT_QUI_EXPIRE __attribute__ ((unused)) = "NB_ADHERENT_QUI_EXPIRE";
static const char * NB_ADHERENT_EXPIRES __attribute__ ((unused)) = "NB_ADHERENT_EXPIRES";

static const int SELECT_TOURNAMENT_INFORMATIONS __attribute__ ((unused)) = 11;
/* select SUM(t.BUY_IN) `Prizepool`, count('x') `Nombre d'inscrits`
				from TB06_TOURNAMENT t,
				TB07_TOURNAMENT_REGISTRATION tr
                WHERE t.ID __attribute__ ((unused)) = tr.ID_TOURNAMENT
                AND t.ID __attribute__ ((unused)) = ?0?
*/

static const char * PRIZEPOOL __attribute__ ((unused)) = "Prizepool";

static const int SELECT_STATISTIQUES_MEMBERS __attribute__ ((unused)) = 12;

static const int SELECT_STATISTIQUES_MEMBERS_UPTODATE __attribute__ ((unused)) = 13;

static const int SELECT_STATISTIQUES_MEMBERS_EXPIRE_NEXT_WEEK __attribute__ ((unused)) = 14;

static const int SELECT_STATISTIQUES_MEMBERS_EXPIRED __attribute__ ((unused)) = 15;

static const int SELECT_EXTRACT_TOURNAMENT_RANKING __attribute__ ((unused)) = 16;

static const int SELECT_STATISTIQUES_ADHESION __attribute__ ((unused)) = 17;

static const int SELECT_TOURNAMENT_TEMPLATE __attribute__ ((unused)) = 18;

static const int SELECT_EXTRACT_KOSMOS_DATE __attribute__ ((unused)) = 19;

static const int SELECT_EXTRACT_KOSMOS __attribute__ ((unused)) = 20;

static const int SELECT_STATISTIQUES_UNIQUE_PLAYER __attribute__ ((unused)) = 21;

static const int SELECT_MEMBER_TO_ASSIGN_CARD_NUMBER __attribute__ ((unused)) = 22;

static const int SELECT_MAILS_MEMBER_TOURNAMENT __attribute__ ((unused)) = 23;

static const int SELECT_SPONSOR_MEMBER __attribute__ ((unused)) = 24;

static const char * SPONSOR_ID __attribute__ ((unused)) = "SPONSOR_ID";

static const int SELECT_SPONSOR_BONUS __attribute__ ((unused)) = 25;

static const char * LAUNCHING __attribute__ ((unused)) = "Declenchement";
static const char * TYPE_GODSON_GAIN __attribute__ ((unused)) = "Type de gain filleul";
static const char * GODSON_GAIN __attribute__ ((unused)) = "Gain filleul";
static const char * TYPE_SPONSOR_GAIN __attribute__ ((unused)) = "Type de gain parrain";
static const char * SPONSOR_GAIN __attribute__ ((unused)) = "Gain parrain";
static const char * NB_MONTH_FIRST_MEMBERSHIP __attribute__ ((unused)) = "Nb mois 1er abonnement";

static const int SELECT_FRANCHISE __attribute__ ((unused)) = 26;

static const char * FRANCHISE __attribute__ ((unused)) = "Franchise";

static const int SELECT_TOURNAMENT_WINNER __attribute__ ((unused)) = 27;

static const int SELECT_MULTIPLE_MEMBERS_TO_SUBSRIBE __attribute__ ((unused)) = 28;

static const int SELECT_TOURNAMENT_ASSOCIATED __attribute__ ((unused)) = 29;

static const int SELECT_TOURNAMENT_ASSOCIATED_WITH_DATE __attribute__ ((unused)) = 30;

static const char * ATTACHED_TOURNAMENT __attribute__ ((unused)) = "Tournoi lie";

static const int SELECT_MEMBERS_FRANCHISE_TO_SUBSCRIBE __attribute__ ((unused)) = 31;

static const int SELECT_CLUB __attribute__ ((unused)) = 32;

static const int SELECT_FID_STATUTS __attribute__ ((unused)) = 33;

static const char * STATUT __attribute__ ((unused)) = "Statut";
static const char * SEUIL __attribute__ ((unused)) = "Seuil";

static const int SELECT_FID_TYPES __attribute__ ((unused)) = 34;

static const int SELECT_FID_OPERATION_MEMBER __attribute__ ((unused)) = 35;

static const int SELECT_ALL_MEMBERS_FID_OPERATIONS __attribute__ ((unused)) = 36;

static const int SELECT_MEMBER_FID_POINTS __attribute__ ((unused)) = 37;

static const char * POINT __attribute__ ((unused)) = "Points";
static const char * POINT_FID __attribute__ ((unused)) = "Points Fidelite";

static const int SELECT_DOTATION_TYPE __attribute__ ((unused)) = 38;

static const char * TYPE __attribute__ ((unused)) = "Type";

static const int SELECT_DOTATION_DESCRIPTION __attribute__ ((unused)) = 39;

static const char * DESCRIPTION __attribute__ ((unused)) = "Description";

static const int CREATE_MEMBER __attribute__ ((unused)) = 1;
static const int CREATE_ADHESION __attribute__ ((unused)) = 2;
static const int CREATE_RETRAIT __attribute__ ((unused)) = 3;
static const int CREATE_DEPOT __attribute__ ((unused)) = 4;
static const int CREATE_ENCHERE __attribute__ ((unused)) = 5;
static const int CREATE_KOSMOS_OPERATION __attribute__ ((unused)) = 6;
static const int CREATE_TOURNAMENT __attribute__ ((unused)) = 7;
static const int CREATE_TOURNAMENT_SUBSCRIPTION __attribute__ ((unused)) = 8;
static const int CREATE_TOURNAMENT_VALIDATION __attribute__ ((unused)) = 9;
static const int CREATE_TOURNAMENT_CANCEL __attribute__ ((unused)) = 10;
static const int CREATE_MONTHLY_CREDIT __attribute__ ((unused)) = 11;
static const int CREATE_TOURNAMENT_TEMPLATE __attribute__ ((unused)) = 12;
static const int CREATE_MEMBER_CARD_NUMBER __attribute__ ((unused)) = 14;
static const int CREATE_TOURNAMENT_SUBSCRIPTION_MULTIPLE __attribute__ ((unused)) = 16;
static const int CREATE_MEMBER_BY_MULTIPLE_SUBSCRIPTION __attribute__ ((unused)) = 17;
static const int CREATE_ATTACHED_TOURNAMENT_SUBSCRIPTION __attribute__ ((unused)) = 18;
static const int CREATE_CLUB __attribute__ ((unused)) = 19;
static const int CREATE_FID_STATUT __attribute__ ((unused)) = 20;
static const int CREATE_FID_OPERATION __attribute__ ((unused)) = 21;
static const int CREATE_EUROS_LOADING __attribute__ ((unused)) = 22;
static const int CREATE_DOT_TYPE __attribute__ ((unused)) = 24;
static const int CREATE_DOT_DESCRIPTION __attribute__ ((unused)) = 25;


static const int UPDATE_MEMBER __attribute__ ((unused)) = 1;
static const int UPDATE_ADHESION __attribute__ ((unused)) = 2;
static const int UPDATE_KOSMOS_OPERATION __attribute__ ((unused)) = 3;
static const int UPDATE_TOURNAMENT __attribute__ ((unused)) = 4;
static const int UPDATE_TOURNAMENT_PRESENTS __attribute__ ((unused)) = 5;
static const int UPDATE_TOURNAMENT_OUT_WITHOUT_GAIN __attribute__ ((unused)) = 6;
static const int UPDATE_TOURNAMENT_OUT_WITH_GAIN __attribute__ ((unused)) = 7;
static const int UPDATE_TOURNAMENT_OUT_CANCEL __attribute__ ((unused)) = 8;
static const int UPDATE_TOURNAMENT_TEMPLATE __attribute__ ((unused)) = 9;
static const int UPDATE_SPONSOR_BONUS __attribute__ ((unused)) = 10;
static const int UPDATE_TOURNAMENT_SENT_RANKING_HERO __attribute__ ((unused)) = 11;
static const int UPDATE_FRANCHISE_MEMBER __attribute__ ((unused)) = 12;
static const int UPDATE_FINALISE_TOURNAMENT __attribute__ ((unused)) = 13;
static const int UPDATE_CLUB __attribute__ ((unused)) = 14;
static const int UPDATE_FID_STATUT __attribute__ ((unused)) = 15;
static const int UPDATE_FID_TYPES __attribute__ ((unused)) = 16;
static const int UPDATE_FID_OPERATION __attribute__ ((unused)) = 17;
static const int UPDATE_DOT_TYPE __attribute__ ((unused)) = 18;
static const int UPDATE_DOT_DESCRIPTION __attribute__ ((unused)) = 19;

static const int DELETE_MEMBER __attribute__ ((unused)) = 1;
static const int DELETE_ADHESION __attribute__ ((unused)) = 2;
static const int DELETE_KOSMOS_OPERATION __attribute__ ((unused)) = 3;
static const int DELETE_TOURNAMENT __attribute__ ((unused)) = 4;
static const int DELETE_UNSUBSCRIBE __attribute__ ((unused)) = 5;
static const int DELETE_TOURNAMENT_TEMPLATE __attribute__ ((unused)) = 6;
static const int DELETE_CLUB __attribute__ ((unused)) = 7;
static const int DELETE_FID_STATUT __attribute__ ((unused)) = 8;
static const int DELETE_FID_OPERATION __attribute__ ((unused)) = 9;
static const int DELETE_DOT_TYPE __attribute__ ((unused)) = 10;
static const int DELETE_DOT_DESCRIPTION __attribute__ ((unused)) = 11;

static const int PHOTO __attribute__ ((unused)) = 1;

#endif // REQUETES_H
