#ifndef MODEL_H
#define MODEL_H

#include <string>
#include "ModelStructs.h"
using namespace std;


class Model
{

public:
	string modelType;








	void identifyModel();							// identifies the model name and sets it to modelType
	
	arruda identifyHyperDataArruda();
	neo identifyHyperDataNeo();
	van identifyHyperDataVan();
	moon identifyHyperDataMoon();
	ogden3 identifyHyperDataOgden3();
	ogden6 identifyHyperDataOgden6();
	yeoh identifyHyperDataYeoh();
	
	string getModel();
};

#endif