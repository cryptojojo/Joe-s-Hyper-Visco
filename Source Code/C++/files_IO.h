/**
 * \file files_IO.h
 * \author Miguel Oliveira (Dassault Systemes, Simulia Corp.)
 * \date 10 August 2016
 * \brief This file contains a class with all the functions responsible for read,write and manipulate the I/O files.
 *
 *
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

#ifndef FILES_IO_H
#define FILES_IO_H

/** \class files_IO
 * \brief Include all the functions to read and write to the files.
 * \details  This files_IO class includes all the functions needed to read from the input file and test data files and create the output files with the final results.
 *
 */

class files_IO
{
    public:
        //Constructor
        files_IO();

        //Destructor
        ~files_IO();

        //Accessor Functions (access variables)
        string read_IF(string); //returns the file input
        string string_parse(string);
        vector<string> &split(const string &, char, vector<string> &);
        vector<string> split(const string &, char);
        //string extract(const string &, char);
        //int getIntValue(const string&);
        void write_F(string,vector<double> &,vector<double> &,vector<double> &,vector<double> &,vector<double> &,int);
        void read_data(string File,vector<double> &,vector<double> &,vector<double> &);
        vector<string> read_dataF(string);
        void read_parse(string);
        void temp_file(string ,vector<double> & ,vector<double> & );

	/** @struct inputFile
	 *  @brief This structure stores all the information parsed from the input file
	 *  @var inputFile::Mat_ID
	 *  Member 'Mat_ID' stores the material name
	 *  @var inputFile::Mat_Def
	 *  Member 'Mat_Def' stores the material definition
	 *  @var inputFile::Hyper_Mod
	 *  Member 'Hyper_Mod' stores the Hyperelastic mode defined
	 *  @var inputFile::Moduli
	 *  Member 'Moduli' stores the Moduli definition
	 *  @var inputFile::Visco_time
	 *  Member 'Visco_time' stores the 'Time' definition
	 *  @var inputFile::C_Param
	 *  Member 'C_Param' stores the C's param for the Yeoh and Neo Model
	 *  @var inputFile::gi
	 *  Member 'gi' stores the Prony g values
	 *  @var inputFile::ki
	 *  Member 'ki' stores the Prony k values
	 *  @var inputFile::ti
	 *  Member 'ti' stores the Prony tau values
	 *  @var inputFile::OG_N
	 *  Member 'OG_N' stores the defined Ogden model order
	  *  @var inputFile::Poly_N
	 *  Member 'Poly_N' stores the defined Polynomial model order
	 *  @var inputFile::OG_mu
	 *  Member 'OG_mu' stores the mu values for the Ogden model
	 *  @var inputFile::OG_alpha
	 *  Member 'OG_alpha' stores the alpha values for the Ogden model
	 *  @var inputFile::VDW_Param
	 *  Member 'VDW_Param' stores all the parameters for the Van Der Waals model
	 *  @var inputFile::Arruda_Param;
	 *  Member 'Arruda_Param' stores all the parameters for the Arruda Boyce model
	 *  @var inputFile::Mooney_R_Param;
	 *  Member 'Mooney_R_Param' stores all the parameters for the Mooney-Rivlin model
	 */

        struct inputFile
        {
            string Mat_ID = " ";
            string Mat_Def=" ";
            string Hyper_Mod=" ";
            string Moduli=" ";
            string Visco_time=" ";
            vector <double> C_Param;
            vector <double> gi;
            vector <double> ki;
            vector <double> ti;
            int OG_N=0;
			int Poly_N=0;
            vector <double> OG_mu;
            vector <double> OG_alpha;
            vector <double> VDW_Param;
            vector <double> Arruda_Param;
            vector <double> Mooney_R_Param;

        } data_IN;

    private: // Variables

};

#endif // FILES_IO_H
