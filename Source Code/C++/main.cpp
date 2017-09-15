

#include <iostream>

#include "Data.h"
#include "HyperVisco.h"
#include "Model.h"
#include "Output.h"
#include "ModelStructs.h"


using namespace std;


int main() {

	Model info;

	info.identifyModel();
	

	if (info.getModel() == "arruda-boyce")
	{


	}

	if (info.getModel() == "neo")
	{


	}


	if (info.getModel() == "vanderwaals")
	{


	}

	if (info.getModel() == "polynomial")					// Moony Rivlin
	{


	}

	if (info.getModel() == "odgen3")
	{



	}
	if (info.getModel() == "odgen6")
	{


	}
	if (info.getModel() == "yeoh")
	{


	}





	cin.get();









}