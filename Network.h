#ifndef NETWORK_H
#define NETWORK_H

#include "config.h"

using namespace std;

class Layer;//on dit qu'on va utiliser la classe layer
//! Classe Reseau de neurones
/*! Contient sa premiere couche, les methodes et tous les parametres necessaire */

class Network
{
public:
	//! Constructeur par defaut (vide)
	Network();
	//! Constructeur utilise
	/*!
	   initialise tous les attributs :
	   \param geometry : geometrie du reseau
	   \param maximal_distance initialisation de l'attribut [m_maximal_distance](@maximal_distance)
	 */
	Network(string geometry, double maximal_distance = MAXIMAL_DISTANCE);

	//! Destructeur
	~Network();


	//! Changer la première couche
	// void	setFirstLayer(Layer* layer = 0);

	//! Retourne la premiere couche du reseau
	Layer*	getFirstLayer() const;

	//! Retourne la derniere couche du reseau
	Layer*	getLastLayer() const;

	//! Retourne la taille de la premiere couche = taille de l'entree a fournir
	int		getFirstLayerSize() const;

	//! Initialiser pour la propagation normale
	void	initNetwork(double* inputs);

	//! Initialiser pour la rétropropagation
	void	initNetworkGradient(double* expectedOutputs);

	//! Nombre de couches dans le réseau
	int		getTotalLayerNumber();

	//! Tester si le réseau boucle sur lui mm
	bool	isALoop() const;

	//! Lancer la propagation
	void launch(double output[]);

	//! Lancer la rétropropagation du gradient
	bool	launchGradient();

	//! Algorithme d'apprentissage (tout initialise)
	bool	learn();

	//! Retourne le string des poids
	string	getWeightStr();

	//! Actualise l'id_reseau
	void	UpdateIdReseau();

	//! Set l'id_reseau
	void	setIdReseau(int id);

	//! Sauver l'etat du réseau
	bool	save();

	//! Fixe le moment d'inertie à momentum
	void	setMomentum(double momentum);

	//! Retourne le moment d'inertie du reseau
	double	getMomentum();

	//! Actualise le réseau
	void	getMostRecent();

	//! Retourne la distance maximale en fin d'apprentissage productif
	double	getMaximalDistance();

	//! Fixe la distance maximale en fin d'apprentissage productif
	void	setMaximalDistance(double maximal_distance);

	//! Retourne le nombre maximal de boucles d'apprentissage a effectuer par le reseau
	int		getMaxLimitLoop();

	//! Fixe le nombre maximal de boucles d'apprentissage a effectuer par le reseau
	void	setMaxLimitLoop(int maxLimitLoop);

	//! Fixe les poids du réseau
	void setWeight(string weigths);

protected:

	//! Pointeur vers la premiere couche
	/*! Seule la premiere couche suffit, le reseau fonctionne comme une liste chainee */
	Layer*	m_firstLayer;

	//! Pointeur vers la dernere couche
	Layer*	m_lastLayer;

	//! Nombre total de liaison dans le reseau
	int		m_totalBindingsNumber;

	//! On procede à la propagation seulement si m_initialized est vrai
	bool	m_initialized;

	//! On procede à la retropropagation seulement si m_gradientInitialized est vrai
	bool	m_gradientInitialized;

	//! Facteur d'inertie, par defaut define -> ALPHA
	double	m_momentum;

	//! Distance maximale en fin d'apprentissage productif
	double	m_maximal_distance;

	//! Nombre maximal de boucles d'apprentissage a effectuer
	int		m_maxLimitLoop;

	//! Nom du fichier dans lequel le reseau est sauvegarde/ recupere
	char*	m_nameFile;

	//! geometrie du réseau
	string	m_geometry;

	//! Nombre de couches dans le réseau
	int		m_nbLayers;

	//! Id du reseau dans la database
	int		m_idReseau;
};

class NetworkLetter :
	public Network
{
public:
	//! Constructeur par défaut, inutile
	NetworkLetter();

	//! Constructeur réellement utilisé
	/*!
	   initialise tous les attributs :
	   \param lettre_testee : charactere reconnu par le réseau
	   \param geometry : geometrie du reseau
	   \param maximal_distance initialisation de l'attribut [m_maximal_distance](@maximal_distance)
	 */
	NetworkLetter(char lettre_testee, string geometry, double maximal_distance = MAXIMAL_DISTANCE);

	//! destructeur
	~NetworkLetter();

	//! Sauver l'etat du réseau
	bool	save();

	//! Actualise le réseau
	void	getMostRecent();

	//! Ecrit le resultat de l'apprentissage dans un fichier
	void	writeReport(bool resultat, int count, double distance_moyenne, double temps_mis, string commentaires);

	//! Apprentissage sur la base de donnee
	/*!
	   Apprend sur toute la base de donne decrite par :
	   \param nb_exemples nombre d'exemples a traiter
	   \param tabloFichiers tableau de nb_exemples noms de fichiers sur lesquels apprendre, la sortie attendue pour chaque fichier etant la premiere lettre du nom
	   \param inputs entree a donner au reseau
	 */
	void learn(const int nb_exemples, char** tabloFichiers, double** inputs);


	//! Retourne la lettre testee par le reseau
	char	getLettreTestee();

	//! Fixe la lettre testee par le reseau
	void	setLettreTestee(char lettre_testee);

protected:

	//! Lettre testee par le neurone
	char m_testedLetter;


};



template <class T>

//! affichage d'un tableau
void	displayArray(T* data, int length);

//! Calcul d'écart
double	distance(double* data1, double* data2, int length);

//! Calcul d'écart, modulo 26 : la casse n'est pas prise en compte
double	distanceMod(double* data1, double* data2, int length);

//! Fonctions de retour de la lecture de base de donnees.
static int callbin(void *data, int argc, char **argv, char **azColName);
static int callbackUpdateIdReseau(void *data, int argc, char **argv, char **azColName);
static int callbackGetMostRecent(void *data, int argc, char **argv, char **azColName);



#endif
