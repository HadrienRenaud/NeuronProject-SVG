#ifndef NETWORK_H
#define NETWORK_H

#include "config.h"

class Layer;//on dit qu'on va utiliser la classe layer
class LayerFirst;
class LayerLast;
class LayerHidden;
//! Classe Reseau de neurones
/*! Contient sa premiere couche, les methodes et tous les parametres necessaire */
class Network {
public:
	//! Constructeur par defaut (vide)
	Network();
	//! Constructeur utilise
	/*!
	   initialise tous les attributs :
	   \param lettre_testee lettre dont s'occupera le reseau
	   \param nom_fichier string qui servira a stocke le nom du fichier pendant les recuperations, sauvegardes
	   \param maximal_distance initialisation de l'attribut [m_maximal_distance](@maximal_distance)
	 */
	Network(char lettre_testee, string nom_fichier = "", double maximal_distance = MAXIMAL_DISTANCE);
	//! Constructeur initialisateur
	/*!
	   initialise tous les attributs :
	   \param lettre_testee lettre dont s'occupera le reseau
	   \param nom_fichier string qui servira a stocke le nom du fichier pendant les recuperations, sauvegardes
	   \param maximal_distance initialisation de l'attribut [m_maximal_distance](@maximal_distance)
	 */
	Network(char lettre_testee, string geometry, string nom_fichier = "", double maximal_distance = MAXIMAL_DISTANCE);

	//! Destructeur
	~Network();


	//! Changer la première couche
	void		setFirstLayer(LayerFirst* layer = 0);

	//! Retourne la premiere couche du reseau
	LayerFirst*	getFirstLayer() const;

	//! Retourne la derniere couche du reseau
	LayerLast*	getLastLayer() const;

	//! Retourne la taille de la premiere couche = taille de l'entree a fournir
	int			getFirstLayerSize() const;

	//! Nombre de liaisons dans le réseau
	int			getTotalBindingsNumber() const;

	//! Nombre de couches dans le réseau
	int			getTotalLayerNumber();

	//! Augmenter le nombre de bindings de n (par defaut de 1)
	void		increaseTotalBindingsNumber(int n = 1);

	//! Tester si le réseau boucle sur lui mm
	bool		isALoop() const;

	//! Lancer la propagation
	void getOutput(double* inputs, double output[]);

	//! Algorithme d'apprentissage (tout initialise)
	void	retropropagation(double* expectedOutputs);

	//! Sauver l'etat du réseau
	void	save();

	//! Fixe le moment d'inertie à momentum
	void	setMomentum(double momentum);

	//! Retourne le moment d'inertie du reseau
	double	getMomentum();

	//! Fonction appelée pour récupérer à partir d'un fichier
	bool	recuperateur();

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
	void learnNetwork(const int nb_exemples, char** tabloFichiers, double** inputs);


	//! Retourne la lettre testee par le reseau
	char	getLettreTestee();

	//! Fixe la lettre testee par le reseau
	void	setLettreTestee(char lettre_testee);

	//! Retourne la distance maximale en fin d'apprentissage productif
	double	getMaximalDistance();

	//! Fixe la distance maximale en fin d'apprentissage productif
	void	setMaximalDistance(double maximal_distance);

	//! Retourne le nombre maximal de boucles d'apprentissage a effectuer par le reseau
	int		getMaxLimitLoop();

	//! Fixe le nombre maximal de boucles d'apprentissage a effectuer par le reseau
	void	setMaxLimitLoop(int maxLimitLoop);

	//! Retourne la geometrie du réseau
	string	getGeometry() const;

private:

	//! Pointeur vers la premiere couche
	/*! Seule la premiere couche suffit, le reseau fonctionne comme une liste chainee */
	LayerFirst* m_firstLayer;

	//! Pointeur vers la derniere couche
	LayerLast* m_lastLayer;

//! Nombre total de liaison dans le reseau
	int	m_totalBindingsNumber;

//! On procede à la propagation seulement si m_initialized est vrai
	bool m_initialized;

//! On procede à la retropropagation seulement si m_gradientInitialized est vrai
	bool m_gradientInitialized;

//! Facteur d'inertie, par defaut define -> ALPHA
	double m_momentum;

//! Distance maximale en fin d'apprentissage productif
	double m_maximal_distance;

//! Nombre maximal de boucles d'apprentissage a effectuer
	int	m_maxLimitLoop;

//! Lettre testee par le neurone
	char m_testedLetter;

	//! Nom du fichier dans lequel le reseau est sauvegarde/ recupere
	char* m_nameFile;

	//! Géométrie du réseau
	string m_geometry;

	//! Nombre de couches du réseau
	int m_numberLayer;

	//! Fonction créant les couches
	void setLayers(istream &geometry);
};

template <class T>

//! affichage d'un tableau
void	displayArray(T* data, int length);

//! Calcul d'écart
double	distance(double* data1, double* data2, int length);

//! Calcul d'écart, modulo 26 : la casse n'est pas prise en compte
double	distanceMod(double* data1, double* data2, int length);


#endif
