#ifndef MODEL_H
#define MODEL_H

#include <string>
using namespace std;


class Model
{
private:

	string modelType;

public:

	void identifyModel();			// identifies the model name and sets it to modelType
	void identifyHyperData(string);
	string getModel();

};

#endif