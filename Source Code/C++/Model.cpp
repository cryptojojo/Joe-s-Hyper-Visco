#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <algorithm>

#include "Model.h"

using namespace std;



void Model::identifyModel()
{
	string waste, model;
	ifstream ModelFile("Hyper_Prony_Material_Model.txt");

	getline(ModelFile, waste);															// skip top line
	getline(ModelFile, waste, ',');														// skip "*Hyperelastic"
	getline(ModelFile, model, ',');														// get the model name (e.g. Yeoh, Ogden, etc)

	model.erase(remove_if(model.begin(), model.end(), isspace), model.end());			// take spaces from the model name
	transform(model.begin(), model.end(), model.begin(), ::tolower);					// make model name all lower case

	modelType = model;

}

string Model::getModel()
{
	return modelType;
}
