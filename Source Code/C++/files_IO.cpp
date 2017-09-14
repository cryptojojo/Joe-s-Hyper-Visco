/**
 * \file files_IO.cpp
 * \author Miguel Oliveira (Dassault Systemes, Simulia Corp.)
 * \date 10 August 2016
 * \brief This file contains the implementation of the functions responsible for read,write and manipulate the I/O files.
 *
 *
 */

#include "files_IO.h"
// #include <sys/file.h>
#include "hyper_Visco.h"


using namespace std;

files_IO::files_IO() // Constructor to initialize variables
{
    
}

files_IO::~files_IO() // Destructor
{
}





/** \brief Function to parse the information inside the input file for all the supported models.
 * \details This function uses the regex method to parse given information and stores it in an instance of the structure @files_IO. This function expects to receive data from the input file.
 * \param filename string contains the data from the input file
 * \return void
 *
 */
void files_IO::read_parse(string filename)
{
    vector <string> Prony_v;
    vector<double> Prony_v_d;
    vector <string> OGDEN_v;
    vector <double> OGDEN_v_d;
    vector<string> C_Param_v;
    vector <double> C_Param_v_d;
    string s=filename;
    smatch m;

    regex m_ID("\\*(MATERIAL|MAT)(\\,\\s|\\,|\\s)") ;

    files_IO file;


    if(regex_search(s,m,m_ID))
        {
																																					//cout<<"Material is defined"<<endl;
            m_ID.assign("((\\,\\s|\\,|\\s)(NAME)\\=)");

            // Get the name of the material
            if(regex_search(s,m,m_ID))
                {
                    data_IN.Mat_ID= m.suffix().str();
                   																																	// cout<<"Nome do Material:"<<data_IN.Mat_ID<<endl;
                    m_ID.assign("\\b(\\w*)(\\b)");
                    regex_search(data_IN.Mat_ID,m,m_ID);
                    data_IN.Mat_ID = m.str();
                    																																cout<<"Nome do Material:"<<data_IN.Mat_ID<<'\n'<<endl;
                }
        }
	// Look for the hyperelastic keyword
    m_ID.assign("(\\*(Hyperelastic|Hyper|HYPERELASTIC)(\\,\\s|\\,|\\s))");

    if(regex_search(s,m,m_ID))
        {
																																					//cout<<"Hyperelastic!"<<endl;

            data_IN.Hyper_Mod= m.suffix().str();
            m_ID.assign("\\b(\\w*)(\\b)");
            regex_search(data_IN.Hyper_Mod,m,m_ID);
            data_IN.Hyper_Mod = m.str(); //get the hyperelastic mode
            																																		cout<<"Hyperelastic Calibration Mode:"<<data_IN.Hyper_Mod<<'\n'<<endl;

            //Get the parameters for the Yeoh and Neo model
            if(data_IN.Hyper_Mod=="Yeoh"||data_IN.Hyper_Mod=="YEOH"||data_IN.Hyper_Mod=="NEO"||data_IN.Hyper_Mod=="Neo")
                {
                    m_ID.assign("\\b(Moduli=)(\\s|\\b)");

                    if(regex_search(s,m,m_ID))
                        {
                            data_IN.Moduli= m.suffix().str();
                            m_ID.assign("\\b(\\w*)(\\b)");
                            regex_search(data_IN.Moduli,m,m_ID);
                            data_IN.Moduli = m.str();
                           																															 cout<<"Moduli= "<<data_IN.Moduli<<'\n'<<endl;
                        }

                    m_ID.assign("\\b(Instantaneous)(\\s|\\b)");

                    if(regex_search(s,m,m_ID))
                        {	// Get the Cs parameters
                            string cparam= m.suffix().str();
                            m_ID.assign("\\b[^,]([\\d-+,.eE\\s])+(?=[,])");
                            regex_search(cparam,m,m_ID);
                            cparam= m.str();
                            C_Param_v=split(cparam,',');

                            for(auto &s : C_Param_v) // convert from string to double
                                {
                                    std::stringstream parser(s);
                                    double x = 0.0;
                                    parser >> x;
                                    data_IN.C_Param.push_back(x);
                                    parser.str(" ");
                                }
                        }

																																					cout<<"C Parameters:"<<'\n'<<data_IN.C_Param[0]<<" "<<data_IN.C_Param[1]<<" "<< data_IN.C_Param[2]<<'\n'<<endl;
                }
            else
                if(data_IN.Hyper_Mod=="OGDEN"||data_IN.Hyper_Mod=="Ogden") //get Ogden parameters
				{
                        m_ID.assign("\\b(N=)\\b");

                        if(regex_search(s,m,m_ID))
                            {
                                string a;
                                m_ID.assign("(\\d)+(?=[,])");

                                if(regex_search(s,m,m_ID))
                                    {
                                        string b= m.suffix().str();
                                        a=m.str();
                                        data_IN.OG_N=stof(a); // Get the Ogden Order
                                      																													  cout<<"N value "<<data_IN.OG_N<<endl;
                                        m_ID.assign("\\b[\\w^]([\\d-+,.eE\\s])+(?=[,])");
                                        regex_search(b,m,m_ID);
                                        string odg=m.str();
																																						cout<<"Odg value "<<odg<<endl;
                                        OGDEN_v=split(odg,','); // Get the Ogden parameters values

                                        for(auto &s : OGDEN_v) // convert string to double
                                            {
                                                std::stringstream parser(s);
                                                double x = 0.0;
                                                parser >> x;
                                                OGDEN_v_d.push_back(x);
                                                parser.str(" ");
                                            }
                                    }

                                for(int i = 0; i < int(OGDEN_v_d.size()-data_IN.OG_N); i+= 2) // Save the values to the structure
                                    {
                                        data_IN.OG_mu.push_back(OGDEN_v_d[i]);
                                        data_IN.OG_alpha.push_back(OGDEN_v_d[i+1]);
                                    }

                               																								 for(int i=0; i<int(data_IN.OG_mu.size()); i++)
                               																								 	cout<<"Mu values: "<<data_IN.OG_mu[i]<<" Alpha values: "<<data_IN.OG_alpha[i]<<endl;
                            }
				}
			else
				if(data_IN.Hyper_Mod=="VAN"||data_IN.Hyper_Mod=="vanderwaals") // get the Van Der Waals parameters
				{
                            vector<string> VDW_v;
                            vector <double> VDW_v_d;
                            string b= m.suffix().str();
                            m_ID.assign("\\b[\\w^]([\\d-+,.eE\\s])+(?=[,])");
                            regex_search(b,m,m_ID);
                            string vdw=m.str();

                            VDW_v=split(vdw,','); // get the VDW parameters values

                            for(auto &s : VDW_v) // Convert values from string to double
                                {
                                    std::stringstream parser(s);
                                    double x = 0.0;
                                    parser >> x;
                                    data_IN.VDW_Param.push_back(x);
                                    parser.str(" ");
                                }

                            																									cout<<"Mu values: "<<data_IN.VDW_Param[0]<<" lamb m: "<<data_IN.VDW_Param[1]<<" Alpha : "<<data_IN.VDW_Param[2]<<" beta values: "<<data_IN.VDW_Param[3]<<endl;
				}
			else
				if(data_IN.Hyper_Mod=="ARRUDA"||data_IN.Hyper_Mod=="arruda") // Get the Arruda Boyce parameters
				{
                                vector<string> Arruda_v;
                                string b= m.suffix().str();
                                m_ID.assign("\\b[\\w^]([\\d-+,.eE\\s])+(?=[,])");
                                regex_search(b,m,m_ID);
                                string arruda=m.str();
                                Arruda_v=split(arruda,','); //get all the arruda parameters (mus and alphas)


                                for(auto &s : Arruda_v)
                                    {
                                        std::stringstream parser(s);
                                        double x = 0.0;
                                        parser >> x;
                                        data_IN.Arruda_Param.push_back(x);
                                        parser.str(" ");
                                    }

																																	cout<<"Mu values: "<<data_IN.Arruda_Param[0]<<" lamb m: "<<data_IN.Arruda_Param[1]<<endl;
				}

            else
				if(data_IN.Hyper_Mod=="POLYNOMIAL"||data_IN.Hyper_Mod=="polynomial") //get Ogden parameters
				{
                        m_ID.assign("\\b(N=)\\b");

                        if(regex_search(s,m,m_ID))
                            {
                                string a;
                                m_ID.assign("(\\d)+(?=[,])");

                                if(regex_search(s,m,m_ID))
                                    {
                                        string b= m.suffix().str();
                                        a=m.str();
                                        data_IN.Poly_N=stof(a); // Get the Ogden Order
                                      																													  cout<<"N Poly value: "<<data_IN.Poly_N<<endl;
                                        m_ID.assign("\\b[\\w^]([\\d-+,.eE\\s])+(?=[,])");
                                        regex_search(b,m,m_ID);
                                        string poly=m.str();
																																						cout<<"POLY value "<<poly<<endl;
                                        OGDEN_v=split(poly,','); // Get the Ogden parameters values

                                        for(auto &s : OGDEN_v) // convert string to double
                                            {
                                                std::stringstream parser(s);
                                                double x = 0.0;
                                                parser >> x;
                                                data_IN.Mooney_R_Param.push_back(x);
                                                parser.str(" ");
                                            }
                                    }




                               																								 	cout<<"C10 value: "<<data_IN.Mooney_R_Param[0]<<" C01 value: "<<data_IN.Mooney_R_Param[1]<<endl;
                            }
				}




            m_ID.assign("(\\*(Viscoelastic|VISCOELASTIC|visco)(\\,\\s|\\,|\\s))");



            if(regex_search(s,m,m_ID))
                {
                    //cout<<"VISCO!"<<endl;
                    m_ID.assign("\\b(Time=|time=)(\\s|\\b)");  //Prony

                    if(regex_search(s,m,m_ID))
                        {
                            if(regex_search(s,m,m_ID))
                                {
                                    data_IN.Visco_time= m.suffix().str();
                                    m_ID.assign("\\b(\\w*)(\\b)");
                                    regex_search(data_IN.Visco_time,m,m_ID);
                                    data_IN.Visco_time = m.str();
                                   // cout<<"Time= "<<data_IN.Visco_time<<endl;
                                }
                        }

                    //-----------------------------------------------GET PRONY VALUES-------------------
                    m_ID.assign("\\b(PRONY|Prony)(\\s|\\b)");

                    if(regex_search(s,m,m_ID))
                        {
                            string prony_out= m.suffix().str();
                            m_ID.assign("\\b[^,]([\\d-+,.eE\\s])+(?=[,])");
                            regex_search(prony_out,m,m_ID);
                            prony_out= m.str();
                            Prony_v=split(prony_out,',');
                         																										   				//cout<<s<<endl;
																																				//cout<<prony_out<<endl;

                            for(auto &s : Prony_v)
                                {
                                    std::stringstream parser(s);
                                    double x = 0.0;
                                    parser >> x;
                                    Prony_v_d.push_back(x);
                                    parser.str(" ");
                                }
                        }
                }

            for(int i = 0; i < int(Prony_v_d.size()); i+= 3)
                {
                    data_IN.gi.push_back(Prony_v_d[i]);
                    data_IN.ki.push_back(Prony_v_d[i+1]);
                    data_IN.ti.push_back(Prony_v_d[i+2]);
                }
																																		cout<<"Prony Series parameters: "<<endl;
																																		for(int i=0; i<int(data_IN.gi.size()); i++)

																																			cout<<"Gi values: "<<data_IN.gi[i]<<" Ki values: "<<data_IN.ki[i]<<" ti values: "<<data_IN.ti[i]<<endl;
        }
}

// Read the input of the file
/** \brief Function to open and read the input file

 *
 * \param file string is the name of the input file
 * \return string is the saved data
 *
 */
string files_IO::read_IF(string file)
{
    ifstream inFile;
    files_IO files;
    inFile.open(file); // Define the file path
    string line;
    string save;
    string item;
    int counter=0;

    if(inFile.good())
        {
            // Number lines
            while(inFile>>line)
                {
                    counter++;
                  	if(line.back()!=',')line=line+","; //add a comma to the end of each line;
                    save+=line;

                }
        }
    else  //Check for error
        {
            cerr << "Error open input file with parameters data " << endl;
            cerr << "Error code: " << strerror(errno)<<endl;
            exit(1);
        }

    inFile.close();
    return save;
}

// This two functions are used in function string_parse
vector<string> &files_IO::split(const string &s, char delim, vector<string> &elems)
{
    stringstream ss(s);
    string item;

    while(std::getline(ss, item, delim))
        {
            if(item.empty())item="0.0";

            elems.push_back(item);
        }

    return elems;
}


/** \brief Function to split the data inside a string considering a specified delimiter
 *
 * \param s const string& is the data
 * \param delim char is the delimiter
 * \return vector<string> is the returned parsed data in a vector format
 *
 */
vector<string> files_IO::split(const string &s, char delim)
{
    vector<string> elems;
    files_IO files;
    files.split(s, delim, elems);
    return elems;
}



// Function to create the output files
/** \brief Function to write the files with the results
 *
 * \param File_name string is the name of the file used for the calibration
 * \param time vector<double>& is the time data
 * \param data_strain vector<double>& is the strain data
 * \param data_stress vector<double>& is the stress data
 * \param calc_stress vector<double>& is the calculated stress data
 * \param error vector<double>& is the calculated error data
 * \param write_err int is a flag used to avoid the file to overwrite
 *
 *
 */
void files_IO::write_F(string File_name,vector<double> &time,vector<double> &data_strain,vector<double> &data_stress,vector<double> &calc_stress,vector<double> &error,int write_err)
{
    files_IO files;
    vector<string> a = files.split(File_name,'.');  //parse all the elements by ','
    string name=a[0];
    name=name+"_sim.txt";
    cout<<name<<"\n"<<endl;
    ofstream myfile(name);
  //  cout.setprecision(10);

    if(myfile.good())
	{

						myfile<<"Time"<<","<<"Strain"<<","<<"Stress"<<","<<"Calc Stress"<<","<<"Error"<<endl;
						for(int it=0; it<int(data_strain.size()); it++){
							myfile<<time[it]<<","<<data_strain[it]<<","<<data_stress[it]<<","<<calc_stress[it]<<","<<error[it]<<endl;
						//cout<<setprecision(10)<<"Strain: "<<data_strain[it]<<" Data Stress: "<<data_stress[it]<<" Calc Stress: "<<calc_stress[it]<<" Error: "<<error[it]<<endl;


				}


            //  myfile<<"C10: "<< c10<<","<<"C20: "<< c20<<"C30: "<< c30<<","<<","<<","<<","<<","<<"Error SUM: "<<error.back()<<endl;
            //flock(fd, LOCK_UN);
            myfile.close();
    }
    else cout << "Unable to create Data File";

    ofstream errT;

    if(write_err==1)
        {
            ofstream errT("Data_Suite_Error.txt");
            write_err=0;
        }

    //int counter=0;
    errT.open("Data_Suite_Error.txt", std::ofstream::out | std::ofstream::app);

    if(errT.good())
        {
            errT<<name<<" "<<error.back()<<endl;
            errT.close();
        }

}


// Function for read the Data Suite file content
/** \brief Function to read the Data Suite file
 *
 * \param File string is the file name
 * \return vector<string> is the vector with name of the test data files
 *
 */
vector<string> files_IO::read_dataF(string File)
{
    ifstream inFile;
    inFile.open(File); // Define the file path
    string line;
    vector<string> elems;

    if(inFile.good())
        {
            while(inFile>>line)
                elems.push_back(line);
        }
    else  //Check for error
        {
            cerr << "Error open Data Suite file" << endl;
            exit(1);
        }

    inFile.close();
    return elems;
}


/** \brief Function to read data from the test data files
 *
 * \param File string is the name of the data file
 * \param data_time vector<double>& Extracted time data
 * \param data_strain vector<double>& Extracted strain data
 * \param data_stress vector<double>& Extracted stress data
 *
 *
 */
void files_IO::read_data(string File,vector<double> &data_time,vector<double> &data_strain,vector<double> &data_stress)
{
    double time,strain,stress;
    																																		cout<<"File Name: "<<File<<endl;
    string line;
    string linez;
    ifstream inFile;
    inFile.open(File);

    if(inFile.good())
        {
            getline(inFile, linez);//just skip the line contents if you do not want header
            getline(inFile, linez);
            getline(inFile, linez);

            while(inFile >> time >> strain>> stress)
                {
                    data_time.push_back(time);
                    data_strain.push_back(strain);
                    data_stress.push_back(stress);

                }
        }
    else
        {
            cout<<"Could not open the Test Data file"<<endl;
            cerr << "Error code: " << strerror(errno)<<endl;
            exit(1);
        }
}
