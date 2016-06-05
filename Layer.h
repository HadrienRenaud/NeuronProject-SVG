#ifndef LAYERJK_H
#define LAYERJK_H

#include "config.h"

typedef double (*transfert)(double);

class Neuron;	//on va utliser la classe neurone
class Network;	//network

class Layer
{

public:
	//! Constructeur par défaut (vide)
	Layer();

	//! Vrai constructeur
	Layer(Network* network, int neurons = 0, transfert trsf = 0);

	//! Destructeur
	~Layer();

	//! nombre de neurones dans la couche
	int				getSize() const;

	//! S'agit-il de la couche d'entrée ?
	virtual bool	isFirst() const = 0;

	//! S'agit-il de la couche de sortie ?
	virtual bool	isLast() const = 0;

	//! On récupère le n-ième neurone
	Neuron*			getNeuron(int n) const;

	//! Réseau parent
	Network*		getNetwork() const;

	//! Préparer les neurones à propager
	void			resetNeurons() const;

	//! Modifier les inputs
	void			setInput(double *inputs);

protected:

	//! Le réseau auquel la couche appartient
	Network* m_network;

	//! La taille de la couche
	const int m_size;

	//! L'ensemble de ses neurones
	std::vector<Neuron*> m_neurons;
};

class LayerLast;

class LayerFirst :
	public Layer
{
public:
	//! Constructeur par défaut
	LayerFirst();

	//! Constructeur réellement utilisé
	LayerFirst(Network* network, int neurons, LayerLast* nextLayer, transfert trsf);

	//! Destructeur
	~LayerFirst();

	//! Retourne la couche suivante
	LayerLast*		getNextLayer() const;

	//! Fixe la couche suivante
	bool			setNextLayer(LayerLast* layer);

	//! S'agit-il de la couche de sortie ?
	virtual bool	isLast() const;

	//! S'agit-il de la couche d'entrée ?
	virtual bool	isFirst() const;

protected:

	//! Couche suivante
	LayerLast* m_nextLayer;

	//! L'ensemble de ses neurones
	std::vector<NeuronFirst*> m_neurons;

};

class LayerLast :
	public Layer
{
public:

	//! Constructeur par défaut
	LayerLast();

	//! Constructeur réellement utilisé
	LayerLast(Network* network, int neurons, LayerFirst* previousLayer, transfert trsf);

	//! Destructeur
	~LayerLast();

	//! Retourne la couche precedente
	LayerFirst*		getPreviousLayer() const;

	//! Fixe la couche precedente
	bool			setPreviousLayer(LayerFirst* layer);

	//! S'agit-il de la couche d'entrée ?
	virtual bool	isFirst() const;

	//! S'agit-il de la couche d sortie ?
	virtual bool	isLast() const;

	//! Demander à chaque neurone de calculer sa sortie
	void			calculate() const;

	//! Demander à chaque neurone d'envoyer le gradient
	void			calculateGradient() const;

	//! Dit a chaque neurone de Neuron::learn()
	bool			learn();

	//! Préparer les neurones à rétropropager
	void			resetNeuronsGradient() const;

protected:
	//! Couche precedente
	LayerFirst* m_previousLayer;

	//! L'ensemble de ses neurones
	std::vector<NeuronLast*> m_neurons;

};

class LayerHidden :
	public LayerLast,
	public LayerFirst
{
public:
	//! constructeur par défaut
	LayerHidden();

	//! constructeur réellement utilisé
	LayerHidden(Network* network, int neurons, LayerFirst* previousLayer, LayerLast* nextLayer, transfert trsf);

	//! Destructeur
	~LayerHidden();

	//! S'agit-il de la couche d'entrée ?
	virtual bool	isFirst() const;

	//! S'agit-il de la couche d sortie ?
	virtual bool	isLast() const;

protected:
	//! L'ensemble de ses neurones
	std::vector<NeuronHidden*> m_neurons;

};

#endif
