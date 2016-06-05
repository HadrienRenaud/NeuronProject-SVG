#ifndef NEURONJK_H
#define NEURONJK_H
//! fichier contenant la classe neurone et les fonctions de transfert utiles


#include "config.h"

using namespace std;

//! pointeur de fonction
/*! On défini un format de pointeur de fonction, on l'invoquera avec le mot transfert
    Ce format est destiné à contenir les fonctions de transfert des neurones */
typedef double (*transfert)(double);

class Layer;	//idem
class LayerHidden;
class LayerFirst;
class LayerLast;

class Neuron
{
public:

	//! Constructeur par défaut
	Neuron();

	//! Le constructeur de copie, associe à la copie : la meme fonction de transfert, la meme couche et les memes liaisons vers les memes neurones,
	//Neuron(const Neuron& neuron);

	//! Si aucune fonction de transfert n'est donné en paramètre, on prend une sigmoide
	Neuron(Layer* layer, transfert trsf = 0);

	//! Destructeur
	~Neuron();

	//! Changer la fonction de transfert
	void setTransfert(double (*)(double));

	//! Retourne la fonction de transfert
	transfert		getTransfert() const;

	//! Retourne la couche parente du neurone
	Layer*			getLayer() const;

	//! Retourne l'entree du neurone
	double			getInput() const;

	//! Retourne la sortie du neurone
	double			getOutput() const;

	//! Retourne le gradient du neurone
	double			getGradient() const;

	//! Change le gradient du neurone
	void			setGradient(double grad);

	//! Remet à 0 l'input.
	bool			initNeuron();

	//! Remet à zero l'output.
	bool			initNeuronGradient();

	//! Premire couche ?
	virtual bool	isFirst() = 0;

	//! Dernière couche ?
	virtual bool	isLast() = 0;

protected:

	//! Somme pondérée des entrées
	double m_input;

	//! Sortie = transfert(m_input)
	double m_output;

	//! Fonction de transfert du neuron
	transfert m_trsf;

	//! Couche à laquelle il appartient
	Layer* m_layer;

	//! Gradient du neurone
	double m_gradient;
};

class NeuronFirst :
	public Neuron
{
public:
	//! Constructeur par défaut
	NeuronFirst();

	//! Constructeur réellement utilisé
	NeuronFirst(LayerFirst* layer, transfert trsf);

	//! Recharger les neurones
	bool			initNeuron(double input);

	//! Premire couche ?
	virtual bool	isFirst();

	//! Dernière couche ?
	virtual bool	isLast();
};

class NeuronLast :
	public Neuron
{
public:
	//! Constructeur par défaut
	NeuronLast();

	//! Constructeur réellement utilisé
	NeuronLast(LayerLast* layer, transfert trsf);

	//! Premire couche ?
	virtual bool	isFirst();

	//! Dernière couche ?
	virtual bool	isLast();

	//! Donne le nombre de liaisons avec les neurones des couches PRECEDENTES
	int				getBindingsNumber() const;

	//! Le neurone envoie son propre gradient aux neurones précedents, pondérée par le poids de chaque liaison
	void			sendGradient();

	//! Algorithme d'apprentissage
	void			learn();

	//! Fonction affichant un string avec tous les poids
	string			printWeights() const;

	//! Changer le poids i en w
	void			setWeight(int i, double w);

	//! Retourne le poids de la liaison i
	double			getWeight(int i) const;

	//! Le neurone récupère la somme pondérée des outputs des neurones des couches d'avant auxquels il est connecte
	void			receive();

	//! initialise le gradient
	bool			initNeuronGradient(double expectedOutput);

protected:

	//! L'ensemble des Neurones de la couche précédente (neurones sources, et non successeurs)
	std::vector<Neuron*> m_neuronsPrev;

	//! Nombre de liaisons avec la couche précédente
	const int m_bindingsNumber;

	//! Poids des liaisons avec les neurones de la couche précédente
	double* m_weightPrev;


};

class NeuronHidden :
	public NeuronLast
{
public:
	//! Constructeur par défaut
	NeuronHidden();

	//! Si aucune fonction de transfert n'est donné en paramètre, on prend une sigmoide
	NeuronHidden(LayerHidden* layer, transfert trsf = 0);

	//! initialise le gradient (= Neuron::initNeuron)
	bool			initNeuronGradient();

	//! Remet à 0 l'input. (= Neuron::initNeuron)
	bool			initNeuron();

	//! Premire couche ?
	virtual bool	isFirst();

	//! Dernière couche ?
	virtual bool	isLast();

};

//2 couples (fonction,dérivée)
double	threshold(double input);	//! fonction de transfert 1
double	threshold1(double input);	//! fonction de transfert 2

double	sigmo(double input);		//! Derivee de la fonction de transfert 1
double	sigmo1(double input);		//! Derivee de la fonction de transfert 1

#endif
