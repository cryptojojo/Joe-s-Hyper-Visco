/**
 * \file hyper_Visco.h
 * \author Miguel Oliveira (Dassault Systemes, Simulia Corp.)
 * \date 10 August 2016
 * \brief This file contains the declaration of the functions to obtain the material calibration.
 *
 * In this header file is included a class with all the functions responsible for the calculation of instantaneous stress, viscoelastic
 * effect using Prony terms and the final error obtained from the fitting between the test data and the calibration method used.
 *
 */

#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include <sstream>
#include <iomanip>
#include "files_IO.h"

using namespace std;

#ifndef HYPER_VISCO_H
#define HYPER_VISCO_H


/** \class hyper_Visco
 * \brief The hyper_Visco class includes all the functions that perform all the calculations for the Hyper-Viscoelastic calibration.
 * \details  This class includes functions for the calculations of the instantaneous stress for the Hyperelastic Models Yeoh, Neo, Ogden, Van Der Waals and Arruda-boyce.
 *Includes also a function that performs the viscoelastic calibration and obtains the material calibration error for the used method.
 *
 */

class hyper_Visco
{
    public:
        //Constructor
        hyper_Visco();


        //Destructor
        ~hyper_Visco();

        //Functions for material calibration
        void Yeoh_Model(vector<double> &, vector<double> &,string, vector<double> &, vector <double> &,vector <double> &);
        void Neo_Model(vector<double> &, vector<double> &,string, vector<double> &, vector <double> &,vector <double> &);
        void Prony_Error_RP(vector<double> &,vector<double> &,vector<double> &,string,vector<double> &,vector<double> &,vector <double> &,vector <double> &,vector <double> &);
        void Ogden(vector<double> &,string, vector<double> &, vector<double>&,vector<double> &, vector <double> &,vector <double> &);
        void VanDWaals(vector<double> &,string, vector<double> &,vector<double> &, vector <double> &,vector <double> &);
        void A_boyce(vector<double> &,string, vector<double> &,vector<double> &, vector <double> &,vector<double> &);
        void Poly_N1(vector<double> &,string,vector<double> &,vector<double> &,vector<double> &,vector<double> &);

    protected:

    private:



};

#endif // HYPER_VISCO_H
