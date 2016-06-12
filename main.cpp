
#include "config.h"



using namespace std;

int main(int argc, char** argv)
{
	Network* net = new Network("2 2 1", 0.5);

	net->setMaxLimitLoop(100);

	v2d inputs = {{0,0},{0,1},{1,0},{1,1}};
	v2d outputs;
	v2d expectedOutputs = {{0},{0},{0},{1}};

	for (int i = 0; i < 4; ++i)
	{
		outputs.push_back(net->getOutput(inputs[i]));
		cout << "Sortie :" << outputs[i][0] << endl;
	}

	net->learn(4, expectedOutputs, inputs );


	for (int i = 0; i < 4; ++i)
	{
		outputs.push_back(net->getOutput(inputs[i]));
		cout << "Sortie :" << outputs[i][0] << endl;
	}

	// delete net;

	return 0;
}
