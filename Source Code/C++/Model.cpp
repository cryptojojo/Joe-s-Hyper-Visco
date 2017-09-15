#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <algorithm>

#include "ModelStructs.h"
#include "Model.h"

using namespace std;



void Model::identifyModel()
{
	string waste, model;
	ifstream ModelFile("Hyper_Prony_Material_Model.txt");

	getline(ModelFile, waste);															// skip top line
	getline(ModelFile, waste, ',');														// skip "*Hyperelastic"
	getline(ModelFile, model, ',');														// get the model name (e.g. Yeoh, Ogden, etc)

	ModelFile.close();

	model.erase(remove_if(model.begin(), model.end(), isspace), model.end());			// take spaces from the model name
	transform(model.begin(), model.end(), model.begin(), ::tolower);					// make model name all lower case

	modelType = model;

}

arruda Model::identifyHyperDataArruda()
{
	arruda arrudaV;

	string waste, muS, lambdaS;									// to get from text file

	ifstream ModelFile("Hyper_Prony_Material_Model.txt");
	getline(ModelFile, waste);									// ignore line 1
	getline(ModelFile, waste);									// ignore line 2
	getline(ModelFile, muS, ',');								// get mu
	getline(ModelFile, lambdaS, ',');							// get lambda

	arrudaV.mu = stod(muS);
	arrudaV.lambda = stod(lambdaS);

	return arrudaV;
}

neo Model::identifyHyperDataNeo()
{
	neo neoV;

	return neoV;
}

van Model::identifyHyperDataVan()
{
	van vanV;

	return vanV;
}

moon Model::identifyHyperDataMoon()
{
	moon moonV;

	return moonV;
}

ogden3 Model::identifyHyperDataOgden3()
{
	ogden3 ogden3V;

	return ogden3V;
}

ogden6 Model::identifyHyperDataOgden6()
{
	ogden6 ogden6V;

	return ogden6V;
}

yeoh Model::identifyHyperDataYeoh()
{
	yeoh yeohV;

	return yeohV;
}





string Model::getModel()
{
	return modelType;
}
