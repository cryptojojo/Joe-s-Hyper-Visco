

#include <iostream>

#include "Data.h"
#include "HyperVisco.h"
#include "Model.h"
#include "Output.h"


using namespace std;


int main() {

	Model joe;

	joe.identifyModel();
	joe.identifyHyperData(joe.getModel());



	cin.get();









}