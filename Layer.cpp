#include "Layer.h"

using namespace std;

///////////// LAYER //////////////////

Layer::Layer() :
	m_size(0)
{
}

Layer::Layer(Network* network, int neurons, transfert trsf) :
	m_network(network),
	m_size(neurons)
{
	for (int i = 0; i < m_size; ++i)
		m_neurons.push_back(0);
}

Layer::~Layer()			//destructeur, inintéressant
{
	for (int i = 0; i < getSize(); i++)
	{
		delete m_neurons[i];
		m_neurons.pop_back();
	}
}

int Layer::getSize() const
{
	return m_size;
}

Neuron* Layer::getNeuron(int n) const
{
	if (n >= 0 && n < getSize())
		return m_neurons[n];
	return 0;
}

Network* Layer::getNetwork() const
{
	return m_network;
}

void Layer::resetNeurons() const
{
	for (unsigned int i = 0; i < m_neurons.size(); i++)
		m_neurons[i]->initNeuron();
}

//////////// LAYER FIRST ///////////////////

LayerFirst::LayerFirst() :
	Layer(),
	m_nextLayer(0)
{
}
LayerFirst::LayerFirst(Network* network, int neurons, LayerLast* nextLayer, transfert trsf) :
	Layer(network, neurons, trsf),
	m_nextLayer(nextLayer)
{
	if (network)
	{
		if (network->getFirstLayer() == 0)
		{
			network->setFirstLayer(this);			//on vérifie une première fois que la
			//fisrtlayer du réseau n'ait pas changé, puis une 2nde juste
			//en-dessous dans les fonctions setNextLayer/setPreviousLayer
		}
	}
	setNextLayer(nextLayer);			//on fait le lien dans les deux sens
	nextLayer->setPreviousLayer(this);
	for (int i = 0; i < neurons; ++i)
	{
		delete m_neurons[i];
		m_neurons[i] = new NeuronFirst(this, trsf);
	}
}

LayerFirst::~LayerFirst()
{
	delete m_nextLayer;
}

LayerLast* LayerFirst::getNextLayer() const
{
	return m_nextLayer;
}

bool LayerFirst::setNextLayer(LayerLast* layer)
{
	if (!layer || layer->getNetwork() != getNetwork())
		return false;
	m_nextLayer = layer;
	return true;
}

///////// LAYER LAST ///////////////////////

LayerLast::LayerLast() :
	Layer(),
	m_previousLayer(0)
{
}
LayerLast::LayerLast(Network* network, int neurons, LayerFirst* previousLayer, transfert trsf) :
	Layer(network, neurons, trsf),
	m_previousLayer(previousLayer)
{
	setPreviousLayer(previousLayer);
	previousLayer->setNextLayer(this);
	for (int i = 0; i < neurons; ++i)
	{
		delete m_neurons[i];
		m_neurons[i] = new NeuronLast(this, trsf);
	}
}
LayerLast::~LayerLast()
{
}

LayerFirst* LayerLast::getPreviousLayer() const
{
	return m_previousLayer;
}

bool LayerLast::setPreviousLayer(LayerFirst* layer)
{
	if (!layer || layer->getNetwork() != getNetwork())
		return false;
	m_previousLayer	= layer;
	return true;
}

bool LayerLast::isLast() const
{
	return true;
}

bool LayerLast::isFirst() const
{
	return false;
}

void LayerLast::calculate() const			//propagation normale
{
	for (unsigned int i = 0; i < m_neurons.size(); i++)
	{
		m_neurons[i]->receive();		//chauqe neurone va chercher l'output de tous
		//les neurones de la couche précédente, en faire la somme pondérée par
		//les poids, puis calculer son propre output avec la fonction de transfert
	}
}

void LayerLast::calculateGradient() const			//rétropropasgation du gradient
{
	for (unsigned int i = 0; i < m_neurons.size(); i++)
		m_neurons[i]->sendGradient();
}

bool LayerLast::learn()
{
	for (int i = 0; i < getSize(); i++)
		m_neurons[i]->learn();
	return true;
}

void LayerLast::resetNeuronsGradient() const
{
	for (unsigned int i = 0; i < m_neurons.size(); i++)
		m_neurons[i]->initNeuronGradient(0);
}

///////////// LAYER HIDDEN ////////////////////////

LayerHidden::LayerHidden()
{
}

LayerHidden::LayerHidden(Network* network, int neurons, LayerFirst* previousLayer, LayerLast* nextLayer, transfert trsf) :
	LayerFirst(network, neurons, nextLayer, trsf),
	LayerLast(network, neurons, previousLayer, trsf)
{
	setNextLayer(nextLayer);					//on fait le lien dans les deux sens
	setPreviousLayer(previousLayer);			//idem
	nextLayer->setPreviousLayer(this);
	previousLayer->setNextLayer(this);
	for (int i = 0; i < neurons; ++i)
	{
		delete m_neurons[i];
		m_neurons[i] = new NeuronHidden(this, trsf);
	}
}

LayerHidden::~LayerHidden()
{
}

bool Layer::isLast() const
{
	return false;
}

bool Layer::isFirst() const
{
	return false;
}
