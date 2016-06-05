#include "config.h"
#include <iostream>

using namespace std;

/////////////// NEURON /////////////////////

Neuron::Neuron() :
	m_input(0),
	m_output(0),
	m_trsf(0),
	m_layer(0),
	m_gradient(0)
{
}									//constructeur par défaut inutile
/*Neuron::Neuron(const Neuron& neuron) :
    m_input(0),
    m_output(0),
    m_trsf(neuron.m_trsf),
    m_layer(neuron.m_layer),
    m_gradient(0)
   {
   }*/
Neuron::Neuron(Layer* layer, transfert trsf) :
	m_input(0),
	m_output(0),
	m_trsf(trsf),
	m_layer(layer),
	m_gradient(0)
{
	if (trsf == 0)
		m_trsf = sigmo;
}	//utlisé lors de la création d'une couche, il ne faut pas invoquer ce constructeur lorsque'on veut ajouter un neurone à une couche après avoir créé la couche
//il faut utiliser la fonction addNeuron() sur la couche concernée.

Neuron::~Neuron()	//destructeur, inintéressant
{
	//for (int i = 0; i < (int)m_bindings.size(); i++)
	//{
	//	delete m_bindings[i];
	//	m_bindings.pop_back();
	//}
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

void Neuron::setGradient(double grad)
{
	m_gradient = grad;
}

bool Neuron::initNeuron()	//on autorise de regler l'input si et seulement si il s'agit d'un neurone de la première couche
{
	m_input		= 0;
	m_output	= m_trsf(m_input);
	return true;
}

bool Neuron::initNeuronGradient()
{
	m_gradient = 0;
	return true;
}

////////////// NEURON FIRST //////////////////

NeuronFirst::NeuronFirst() :
	Neuron()
{
}
NeuronFirst::NeuronFirst(LayerFirst* layer, transfert trsf) :
	Neuron(layer, trsf)
{
}


bool NeuronFirst::initNeuron(double input)
{
	m_input = input;
	return true;
}

bool NeuronFirst::isFirst()
{
	return true;
}

bool NeuronFirst::isLast()
{
	return false;
}


////////////// NEURON LAST /////////////////

NeuronLast::NeuronLast() :
	Neuron(),
	m_bindingsNumber(0)
{
}
/*NeuronLast::NeuronLast(const NeuronLast& neuron) :
    Neuron(neuron),
    m_neuronsPrev(neuron.m_neuronsPrev),
    m_weightPrev(neuron.m_weightPrev),
    m_bindingsNumber(neuron.m_bindingsNumber)
   {
   }*/
NeuronLast::NeuronLast(LayerLast* layer, transfert trsf) :
	Neuron(layer, trsf),
	m_bindingsNumber(layer->getPreviousLayer()->getSize())
{
	Neuron*	m_neuronsPrev[m_bindingsNumber];
	double	m_weightPrev[m_bindingsNumber];

	for (int i = 0; i < m_bindingsNumber; ++i)
	{
		m_neuronsPrev[i]	= layer->getPreviousLayer()->getNeuron(i);
		m_weightPrev[i]		= (rand() % 1000 - 500 ) / 500;
	}
}	//utlisé lors de la création d'une couche, il ne faut pas invoquer ce constructeur lorsque'on veut ajouter un neurone à une couche après avoir créé la couche
//il faut utiliser la fonction addNeuron() sur la couche concernée.


bool NeuronLast::initNeuronGradient(double expectedOutput)
{
	m_gradient = /* 2 * */ sigmo1(m_input) * (expectedOutput - m_output);	//le x2 est dans la poly mais pas sur wiki
	return true;
}


int NeuronLast::getBindingsNumber() const
{
	return m_bindingsNumber;
}

void NeuronLast::receive()	//le neurone calcule son input en récupérant la somme pondérée de neurones d'avant
{
	for (int i = 0; i < m_bindingsNumber; i++)
		m_input += (m_neuronsPrev[i]->getOutput()) * m_weightPrev[i];
	//enfin, calcul de l'output avec la fonction de transfert
	m_output = m_trsf(m_input);
}

void NeuronLast::sendGradient()	//rétrop propager le gradient aux neurones des couches d'avant
{
	for (int i = 0; i < m_bindingsNumber; i++)
		m_neuronsPrev[i]->setGradient(m_gradient * m_weightPrev[i] /*trsf[1]*/ * sigmo1(m_neuronsPrev[i]->getInput()));
	// gradien_du_neurone_précédent+=(gradient_actuel*pods_de_la_liaison*dérivée_de_sigmo_en(m_input));
}

void NeuronLast::learn()	//Le neuron recalcule le poids de toutes ces liaisons avec les neurones précédents
{							//la fonction utilise MU, le gradient du neuron présent et l'output du neuron qui est à l'autre bout de la laison
	for (int i = 0; i < m_bindingsNumber; i++)
		m_weightPrev[i] += ((m_gradient) * MU * (m_neuronsPrev[i]->getOutput()));
}

string NeuronLast::printWeights()
{
	ostringstream convert;

	for (int i = 0; i < m_bindingsNumber; ++i)
		convert << m_weightPrev[i] << " ";
	return convert.str();
}

void NeuronLast::setWeight(int i, double w)
{
	m_weightPrev[i] = w;
}

bool NeuronLast::isFirst()
{
	return false;
}

bool NeuronLast::isLast()
{
	return true;
}

////////////// NEURON HIDDEN ////////////////

NeuronHidden::NeuronHidden() :
	NeuronLast()
{
}
NeuronHidden::NeuronHidden(LayerHidden* layer, transfert trsf) :
	NeuronLast(layer, trsf)
{
}

bool NeuronHidden::isFirst()
{
	return false;
}

bool NeuronHidden::isLast()
{
	return false;
}

bool NeuronHidden::initNeuronGradient()
{
	return Neuron::initNeuronGradient();
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
