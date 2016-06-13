#include "config.h"
#include <iostream>
using namespace std;

Neuron::Neuron() :
	m_input(0),
	m_output(0),
	m_trsf(0),
	m_layer(0),
	m_indexInLayer(0),
	m_gradient(0),
	m_prevStepWeight({}),
	m_weight({}),
	m_prevNeuron({})
{
}									//constructeur par défaut inutile
Neuron::Neuron(const Neuron& neuron) :
	m_input(0),
	m_output(0),
	m_trsf(neuron.m_trsf),
	m_layer(neuron.m_layer),
	m_indexInLayer(neuron.m_indexInLayer),
	m_gradient(0),
	m_prevNeuron(neuron.m_prevNeuron),
	m_weight(neuron.m_weight),
	m_prevStepWeight(neuron.m_prevStepWeight)
{
}														//jamais utilisé
Neuron::Neuron(Layer* layer, transfert trsf) :
	m_input(0),
	m_output(0),
	m_trsf(trsf),
	m_layer(layer),
	m_indexInLayer(layer->getSize()),
	m_gradient(0),
	m_prevStepWeight({}),
	m_weight({}),
	m_prevNeuron({})
{
	if (trsf == 0)
		m_trsf = sigmo;
}	//utlisé lors de la création d'une couche, il ne faut pas invoquer ce constructeur lorsque'on veut ajouter un neurone à une couche après avoir créé la couche
//il faut utiliser la fonction addNeuron() sur la couche concernée.

Neuron::~Neuron()	//destructeur, inintéressant
{
}

void Neuron::setTransfert(double (*trsf)(double))
{
	m_trsf = trsf;
}
transfert Neuron::getTransfert() const
{
	return m_trsf;
}
Layer* Neuron::getLayer() const
{
	return m_layer;
}
int Neuron::getBindingsNumber() const
{
	return m_prevNeuron.size();
}

void Neuron::receive()	//le neurone calcule son input en récupérant la somme pondérée de neurones d'avant
{
	for (int i = 0; i < m_prevNeuron.size(); i++)
		m_input += (m_prevNeuron[i]->getOutput()) * (m_prevStepWeight[i]);
	//enfin, calcul de l'output avec la fonction de transfert
	m_output = m_trsf(m_input);
}

void Neuron::sendGradient()	//rétrop propager le gradient aux neurones des couches d'avant
{
	for (int i = 0; i < m_prevNeuron.size(); i++)
		m_prevNeuron[i]->addGradient((m_gradient * (m_prevStepWeight[i]) * sigmo1(m_prevNeuron[i]->getInput())));
	// gradien_du_neurone_précédent+=(gradient_actuel*pods_de_la_liaison*dérivée_de_sigmo_en(m_input du neurone précédent));
}

double Neuron::getInput() const
{
	return m_input;
}

double Neuron::getOutput() const
{
	return m_output;
}

double Neuron::getGradient() const
{
	return m_gradient;
}

bool Neuron::initNeuron(double input)	//on autorise de regler l'input si et seulement si il s'agit d'un neurone de la première couche
{
	if (getLayer()->getNetwork()->getFirstLayer() == getLayer() || input == 0)
	{
		m_input	= input;
		m_output = m_trsf(m_input);
		return true;
	}
	else	//si echec
		return false;
}

bool Neuron::initNeuronGradient(double expectedOutput)
{
	//on initialise le gradient du neurone à une valeur différente de 0, seulement si le neurone est dans la dernière couche.
	if (getLayer()->getNetwork()->getLastLayer() == getLayer())
	{
		m_gradient = 2 * sigmo1(m_input) * (expectedOutput - m_output);	//le x2 est dans la poly mais pas sur wiki
		return true;
	}
	//on peut initialiser le gradient de n'impote quel neurone à 0.
	else if (expectedOutput == 0)
	{
		m_gradient = 0;
		return true;
	}
	else	//si echec, le retour n'est jamais utilisé
		return false;
}

int Neuron::getIndexInLayer() const
{
	return m_indexInLayer;
}

void Neuron::learn()	//Le neuron recalcule le poids de toutes ces liaisons avec les neurones précédents
{						//la fonction utilise MU, le gradient du neuron présent et l'output du neuron qui est à l'autre bout de la laison
	double temp;

	for (int i = 0; i < m_prevNeuron.size(); i++)
	{
		temp = m_weight[i];
		m_weight[i]+=((m_gradient) * MU * (m_prevNeuron[i]->m_output) + getLayer()->getNetwork()->getMomentum() * m_prevStepWeight[i]);
		m_prevStepWeight[i]=m_weight[i]-temp;
	}
}

void Neuron::addPrevNeuron(Neuron* neurone)
{
	m_prevStepWeight.push_back(0);
	m_weight.push_back( (rand() % 1000 - 500) / 500 );
	m_prevNeuron.push_back(neurone);
}

void Neuron::clearBindings()
{
	m_prevNeuron.clear();
	m_weight.clear();
	m_prevStepWeight.clear();
}

void Neuron::setWeight(int i, double w)
{
	if (i < m_prevNeuron.size())
	{
		m_weight[i]=w;
		m_prevStepWeight[i]=0;
	}
}

double Neuron::getWeight(int i)
{
	if (i < m_prevNeuron.size())
		return m_weight[i];
	else
		return 0;
}

void Neuron::addGradient(double grad)
{
	m_gradient += grad;
}

//////////////HORS NEURON////////////////////////////
double threshold(double input)
{
	if (input > THRESHOLD)
		return 1;
	return -1;
}
double threshold1(double input)	//dérivée de threshold
{
	return 0;
}

inline double sigmo(double input)
{
	double result((exp(PENTE * input) - 1) / (exp(PENTE * input) + 1));

	if (result != result)
		cout << "sigmo probleme avec input = " << input << endl;
	return result;
}
inline double sigmo1(double input)	//dérivée de sigmo
{
	return 2 * PENTE * exp(PENTE * input) / ((exp(PENTE * input) + 1) * (exp(PENTE * input) + 1));
}
