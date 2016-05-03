#ifndef APPRENTISSAGE_H_INCLUDED
#define APPRENTISSAGE_H_INCLUDED

#include "Network.h"

#define MAX_LENGTH_NAME_FILE 100
#define NB_APPRENTISSAGE 10
#define DISTANCE_MAXIMALE 0.1
#define MAX_LIMIT_LOOP 20
#define LENGTH_ALPHABET 52
#define BORNE_INF_DISTINCTION 0.1
#define BORNE_MAX_CASSE 10

class Network;

template <class T>
void displayArray(T* data, int length);//affichage d'un tableau
double distance(double* data1, double* data2, int length);//calcul d'écart
double distanceMod(double* data1, double* data2, int length);//calcul d'écart
bool readExemple(char* nom_fichier,double entrees[],int taille_entree);
bool readExemple(char* nom_fichier,double entrees[],int taille_entree,const char* directory);//idem
int countExemples();
int countExemples(const char* directory);//compte les exemples dans directory
void getTabloFichier(char** tabloFichier); // lit le tableau correspondant à une image
void getTabloFichier(char** tabloFichier, const char* directory); // idem
void getTabloExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples); // récupère tous les tablos d'exemples
void getTabloExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples,const char* directory); //idem
void plus_recent(Network* net);
void plus_recent(Network* net,char lettre_testee);
void write_compte_rendu(Network* net, bool resultat, int count, double distance_moyenne, double temps_mis, string commentaires,char lettre_testee, char* nom_fichier);
void apprend(Network* net,char lettre_testee, const int nb_exemples, char** tabloFichiers, double** inputs);
void recupere_apprend();
Network** apprend_nouveau();
void apprend_tous(Network** tablo_net);
Network** createurs_reseaux();
void destructeur_reseaux(Network** tablo_net);
char tester(Network** tablo_net, double input[]);
//char teste_Pierrot(char* nom_fichier, const char* directory);
//char* recupere_teste();
//char* recupere_teste(const char* directory);
#endif //APPRENTISSAGE_H_INCLUDED