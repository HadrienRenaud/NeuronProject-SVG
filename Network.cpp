#include "Network.h"
using namespace std;

Network::Network()
{
	Network(MAXIMAL_DISTANCE);
}
Network::Network(double maximal_distance) :
	m_firstLayer(0),
	m_lastLayer(0),
	m_totalBindingsNumber(0),
	m_initialized(false),
	m_gradientInitialized(false),
	m_maximal_distance(MAXIMAL_DISTANCE),
	m_maxLimitLoop(MAX_LIMIT_LOOP),
	m_numberLayer(0),
	m_momentum(ALPHA)
{
	char m_nameFile[MAX_LENGTH_NAME_FILE];
}
Network::Network(string geometry, double maximal_distance) :
	m_geometry(geometry),
	m_firstLayer(0),
	m_lastLayer(0),
	m_totalBindingsNumber(0),
	m_initialized(false),
	m_gradientInitialized(false),
	m_maximal_distance(MAXIMAL_DISTANCE),
	m_maxLimitLoop(MAX_LIMIT_LOOP),
	m_numberLayer(0),
	m_momentum(ALPHA)
{
	char			m_nameFile[MAX_LENGTH_NAME_FILE];

	LayerFirst*		previousLayer(0);
	istringstream	dim(geometry);
	int				nb_neurones;

	dim >> m_numberLayer;
	if (m_numberLayer < 2)
		cout << "Géométrie invalide, arret de la construction" << endl;
	else
		setLayers(dim);
}



Network::~Network()
{
	delete m_firstLayer;	//supprime TOUTES les couches
}

bool Network::recuperateur()
{
	ifstream	file(m_nameFile);	// On ouvre le fichier
	int			numberLayer;

	file >> numberLayer;
	if (numberLayer < 2)
	{
		cout << "Géométrie invalide, arret de la récupération" << endl;
		return false;
	}
	m_numberLayer = numberLayer;
	setLayers(file);

	m_firstLayer->getNextLayer()->setWeight(file);
	return true;
}

void Network::setLayers(istream &geometry)
{
	cout << "setLayers" << endl;
	int			nb_neurones;
	LayerFirst* layer;

	if (m_firstLayer == 0)
		delete m_firstLayer;

	geometry >> nb_neurones;
	m_firstLayer	= new LayerFirst(this, nb_neurones, 0, sigmo );
	layer			= m_firstLayer;

	for (int i = 0; i < m_numberLayer - 2; ++i)
	{
		geometry >> nb_neurones;
		layer = new LayerHidden(this, nb_neurones, layer, 0, sigmo);
	}

	geometry >> nb_neurones;
	m_lastLayer = new LayerLast(this, nb_neurones, layer, sigmo);
}

void Network::save()
{
	// On trouve le nom du fichier
	time_t			t = time(0);							// l'heure
	ostringstream	temps;									//flux pour traitement

	temps << asctime(localtime(&t));						//on récupère le flux correspondant au temps
	string			str_nom_fichier(temps.str());			// on crée un string correspondant au flux
	for (int i(0); i < (int)str_nom_fichier.size(); i++)	// on parcourt le nom du fichier
		if (!isalnum(str_nom_fichier[i]))					// et on remplace tout ce qui ne va pas dans un nom de fichier par '_'
			str_nom_fichier[i] = '_';
	str_nom_fichier = string(DOSSIER_SVG) + str_nom_fichier + string(EXTENSION_SVG);
	// adresse complete
	strcpy(m_nameFile, str_nom_fichier.c_str());

	//on écrit dans le fichier
	ofstream file(m_nameFile);				// flux sortant dans le fichier
	file << getGeometry() << ' ';			// on entre le nombre total de couches

	getFirstLayer()->getNextLayer()->saveWeight(file);

	// on sauvegarde le dernier fichier enregistré dans mostRecent.txt :
	string		mostRecent;
	mostRecent = DOSSIER_SVG + string(NOM_SVG) + string(EXTENSION_SVG);
	ofstream	fichier_recent(mostRecent.c_str());
	fichier_recent << m_nameFile;
}

void Network::setFirstLayer(LayerFirst* layer)
{
	if (layer)								//Si ce n'est pas un pointeur vide
		m_firstLayer = layer;
	else									//si c'est un pointeur vide
		m_firstLayer = new LayerFirst();	// on crée une nouvelle couche
}

LayerFirst* Network::getFirstLayer() const
{
	return m_firstLayer;
}

LayerLast* Network::getLastLayer() const
{
	return m_lastLayer;
}

int Network::getTotalLayerNumber()
{
	return m_numberLayer;
}

int Network::getFirstLayerSize() const
{
	return m_firstLayer->getSize();
}

vd Network::getOutput(vd inputs)
{
	vd outputs;

	// initialisation des entrees
	getFirstLayer()->resetNeurons(inputs);

	//calcul des sorties
	getFirstLayer()->getNextLayer()->calculate();

	//maintenant on récupère la sortie dans un tableau
	for (int i = 0; i < m_lastLayer->getSize(); i++)
		outputs.push_back(m_lastLayer->getNeuron(i)->getOutput());

	m_initialized = false;	//le réseau n'est plus utilisable pour l'instant

	return outputs;
}

void Network::retropropagation(vd expectedOutputs)
{
	getLastLayer()->resetNeuronsGradient(expectedOutputs);	//initialisation des gradients

	getLastLayer()->calculateGradient();	// calcul par rétropropagation des gradients

	getFirstLayer()->getNextLayer()->learn();				// apprentissage
}

double Network::getMomentum()
{
	return m_momentum;
}
void Network::setMomentum(double momentum)
{
	m_momentum = momentum;
}

void Network::getMostRecent()
{
	//initialisations
	string	str_nom_fichier;
	string	nom_db;

	//on reconstitue le nom du fichier :
	nom_db = string(DOSSIER_SVG) + NOM_SVG + string(EXTENSION_SVG);

	ifstream file(nom_db.c_str());	//on ouvre le fichier getMostRecent.txt
	file >> str_nom_fichier;		//on lit son contenu
	strcpy(m_nameFile, str_nom_fichier.c_str());
	cout << "Recuperation de : " << str_nom_fichier << endl;
	recuperateur();	// On récupère le réseau stocké dans le fichier de svg le plus récent
}

double Network::getMaximalDistance()
{
	return m_maximal_distance;
}
void Network::setMaximalDistance(double maximal_distance)
{
	m_maximal_distance = maximal_distance;
}

int Network::getMaxLimitLoop()
{
	return m_maxLimitLoop;
}
void Network::setMaxLimitLoop(int maxLimitLoop)
{
	m_maxLimitLoop = maxLimitLoop;
}

string Network::getGeometry() const
{
	return m_geometry;
}


void Network::learn(const int nbExemples, v2d expectedOutputs, v2d inputs)
{
	clock_t t0(clock());//temps de départ du programme

	cout << "apprentissage !" << endl;

	// Initialisations
	int		exemple			= 0;					//exemple actuellement selectionné pour l'apprentissage, cette variable est incrémenté à chaque fois qu'il réussi un exemple
	int		successes		= 0;					//le réseau doit enchainer nbExemples succès pour avoir fini l'apprentissage, cela ne devra pas être le cas pour les caractères manuscrits, parce qu'il y un risque de surapprentissage
	int		successesInLoop = 0;
	int		count			= 0;					//nombre de passage dans la boucle
	double	dist			= 0;					//pour stocker la moyenne des écarts quadratiques entre les valeurs attendues et les valeurs retournées
	double	totalDistance	= 0;
	double	maxDist			= 0;
	vd	output;	//valeur prototypale en sortie du réseau


	clock_t t1(clock());

	//APPRENTISSAGE
	while ((successes < nbExemples) && (count < m_maxLimitLoop * nbExemples))	//tant qu'on a pas enchaîné nbExemples succès
	{
		exemple++; //incrémentation
		exemple %= nbExemples;													//On ne dépasse pas nbExemples
		count++;

		// Affichages de temps en temps ...
		if (count % (NB_LEARNING * nbExemples) == nbExemples - 1 )	//de temps en temps, on affiche dist et un poids(ça sert à rien le poids mais bon)
		{
			//cout << "count = " << count << " soit " << count / nbExemples << " boucles : ";
			//cout << " ( " << NB_LEARNING << " boucles  en : " << ((float)(clock() - t1) / CLOCKS_PER_SEC) << " s. ) ." << endl;
			cout << "Progression " << min(int(100 * m_maximal_distance / maxDist), 100) << "% - " << count / nbExemples << " boucles." << endl;
			cout << "Distance moyenne : " << (totalDistance / nbExemples) << " - distance maximale : " << maxDist << endl;
			t1 = clock();
		}

		//Remise à zéro des paramètres de boucles
		if (count % nbExemples == 0)
		{
			successesInLoop = 0;
			totalDistance	= 0;
			maxDist			= 0;
		}

		//Calcul de la réponse du réseau
		output = getOutput(inputs[exemple]);

		//Calcul de l'écart
		if (count < nbExemples * MAX_LIMIT_CASE)
			dist = distance(output, expectedOutputs[exemple], LAST_LAYER_SIZE );		//on calcule l'écart
		else
			dist = distanceMod(output, expectedOutputs[exemple], LAST_LAYER_SIZE );		//on calcule l'écart

		//changement de maxDist et de totalDistance
		if (dist > maxDist)
			maxDist = dist;
		totalDistance += dist;

		//On apprend, ou pas en fonction du résultat
		if (dist < m_maximal_distance)	//si c'est assez petit, c'est un succès
			successes++;
		else							//sinon c'est un echec et le réseau recalcule les poids des liaisons
		{
			retropropagation(expectedOutputs[exemple]);
			successes = 0;	//on réinitialise aussi les nombre de succès enchaînés
		}
	}

	//On sauvegarde le réseau
	save();

	//Affichages ...
	cout << endl;
	if (count >= m_maxLimitLoop * nbExemples)
		cout << "Apprentissage INFRUCTUEUX sur " << count << " exemples lus.";
	else
		cout << "Apprentissage productif : " << count << " exemples lus sur " << m_maxLimitLoop * nbExemples;

	cout << " avec " << successes << " succes, effectue en " << ((float)(clock() - t0) / CLOCKS_PER_SEC) << " secondes." << endl;


	//Calcul du temps mis
	double temps_mis(((float)(clock() - t0) / CLOCKS_PER_SEC));
	cout << "Apprentissage effectue en " << temps_mis << " secondes." << endl;

	// On met à jour les données dans le fichier
	// writeReport((count < m_maxLimitLoop * nbExemples), count / nbExemples, totalDistance / nbExemples);
}

///////////////////////// NETWORKLETTER ///////////////////////////////////////

NetworkLetter::NetworkLetter(char letter, double maximal_distance) :
	Network(maximal_distance),
	m_testedLetter(letter)
{
}
NetworkLetter::NetworkLetter(char lettre_testee, string geometry, double maximal_distance) :
	Network(geometry, maximal_distance),
	m_testedLetter(lettre_testee)
{
}

NetworkLetter::~NetworkLetter()
{
};

void NetworkLetter::save()
{
	// On trouve le nom du fichier
	time_t			t = time(0);							// l'heure
	ostringstream	temps;									//flux pour traitement

	temps << asctime(localtime(&t));						//on récupère le flux correspondant au temps
	string			str_nom_fichier(temps.str());			// on crée un string correspondant au flux
	for (int i(0); i < (int)str_nom_fichier.size(); i++)	// on parcourt le nom du fichier
		if (!isalnum(str_nom_fichier[i]))					// et on remplace tout ce qui ne va pas dans un nom de fichier par '_'
			str_nom_fichier[i] = '_';
	str_nom_fichier = string(DOSSIER_SVG) + str_nom_fichier + "_" + m_testedLetter + string(EXTENSION_SVG);
	// adresse complete
	strcpy(m_nameFile, str_nom_fichier.c_str());

	//on écrit dans le fichier
	ofstream file(m_nameFile);				// flux sortant dans le fichier
	file << getGeometry() << ' ';			// on entre le nombre total de couches

	getFirstLayer()->getNextLayer()->saveWeight(file);

	// on sauvegarde le dernier fichier enregistré dans mostRecent.txt :
	string		mostRecent;
	mostRecent = DOSSIER_SVG + string(NOM_SVG) + m_testedLetter + string(EXTENSION_SVG);
	ofstream	fichier_recent(mostRecent.c_str());
	fichier_recent << m_nameFile;
}

void NetworkLetter::getMostRecent()
{
	//initialisations
	string	str_nom_fichier;
	string	nom_db;

	//on reconstitue le nom du fichier :
	nom_db = string(DOSSIER_SVG) + NOM_SVG + m_testedLetter + string(EXTENSION_SVG);

	ifstream file(nom_db.c_str());	//on ouvre le fichier getMostRecent.txt
	file >> str_nom_fichier;		//on lit son contenu
	strcpy(m_nameFile, str_nom_fichier.c_str());
	cout << "Reseau " << m_testedLetter << " - recuperation de : " << str_nom_fichier << endl;
	recuperateur();	// On récupère le réseau stocké dans le fichier de svg le plus récent
}

void NetworkLetter::writeReport(bool resultat, int count, double distance_moyenne, double temps_mis, string commentaires)
{
	//initialisations
	ofstream	base_donnes;
	time_t		rawtime;
	struct tm * timeinfo;
	char		buffer [80];

	time(&rawtime);
	Layer*		temp;

	//Inscription des premières données
	base_donnes.open("Compte_rendu_test_toutes.csv", ios::out | ios::app);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%d/%m/%y", timeinfo);
	base_donnes << buffer << ',';
	strftime(buffer, 80, "%H:%M:%S", timeinfo);
	base_donnes << buffer << "," << m_testedLetter << "," << getTotalLayerNumber() + 1 << ',';

	base_donnes << m_geometry << ',';

	//fin de l'inscription des données
	base_donnes << count << ',' << distance_moyenne << ',' << m_maximal_distance << ',';
	base_donnes << resultat << ',' << temps_mis << ',' << commentaires << ',' << m_nameFile << endl;
}

void NetworkLetter::learn(const int nbExemples, vc fileArray, v2d inputs)
{
	clock_t t0(clock());//temps de départ du programme

	cout << endl << "Apprentissage de la lettre " << m_testedLetter << " ..." << endl << endl;

	v2d expectedOutputs;

	for (int i = 0; i < nbExemples; ++i)
		expectedOutputs[i][0] = (int)(fileArray[i][0] == m_testedLetter);

	Network::learn(nbExemples, inputs, expectedOutputs);

	//Calcul de la distance moyenne
	double	totalDistance = 0;
	vd	output;
	for (int exemple = 0; exemple < nbExemples; exemple++)
	{
		//Réponse du réseau
		output = getOutput(inputs[exemple]);

		//Calcul de la distance
		totalDistance += distance(output, expectedOutputs[exemple], LAST_LAYER_SIZE );
	}
	cout << "Distance moyenne sur les exemples : " << totalDistance / nbExemples << endl;

	//Calcul du temps mis
	double temps_mis(((float)(clock() - t0) / CLOCKS_PER_SEC));
	cout << "Apprentissage effectue en " << temps_mis << " secondes." << endl;

	// On met à jour les données dans le fichier
	// writeReport((count < m_maxLimitLoop * nbExemples), count / nbExemples,
	//          totalDistance / nbExemples, temps_mis);
}

char NetworkLetter::getLettreTestee()
{
	return m_testedLetter;							//récupère la lettre testee.
}

void NetworkLetter::setLettreTestee(char lettre_testee)	//change la lettre
{
	cout << "Network " << m_testedLetter << " change de lettre et devient ";
	m_testedLetter = lettre_testee;
	cout << m_testedLetter << endl;
}

////////////////////////// HORS NETWORK ///////////////////////////////////////

template <class T>
void displayArray(T* data, int length)	//afficher un tableau de valeur
{
	cout << "[";
	for ( int i = 0; i < length - 1; i++)
		cout << (data[i] >= 0 ? "+" : "") << data[i] << ",";
	cout << (data[length - 1] >= 0 ? "+" : "") << data[length - 1] << "]";
}

double distance(vd data1, vd data2, int length)			//on fait la moyenne des carrés de chaque écart entre data1 et data2
{
	double res = 0;													//initialisation

	for (int i = 0; i < length; i++)								//on parcourt la liste
		res += ((data1[i] - data2[i]) * (data1[i] - data2[i]));		//On augmente la moyenne du carré de la distance entre data1 et data2 ~ Variance
	res /= length;													// On moyenne

	return res;
}

double distanceMod(vd data1, vd data2, int length)
{
	double	res = 0;	//initialisation
	int		j	= -1;	//initialisation

	for (int i = 0; i < length; i++)
	{
		res += ((data1[i] - data2[i]) * (data1[i] - data2[i]));	//carré de la distance
		j	= (i + 26) % length;								//position de la majuscule
		res += ((data1[i] - data2[j]) * (data1[i] - data2[j]));	//carré de la distance à la majuscule
	}
	res /= length * 2;											// on moyenne

	return res;
}
