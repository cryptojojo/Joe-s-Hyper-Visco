#include<numeric>
#include "files_IO.h"
#include "hyper_Visco.h"
#include<iostream>



using namespace std;

bool prony= true;


int main()
{
    // Read and parse the material model input file-----------------------------------------------------------------------
    string in_data=" "; //in_data: Store the data from the input file
    vector<string> line; //Store the names of the input data files

    files_IO infile; // Create a class object files_IO
    hyper_Visco HypV; // Create a class object hyper_Visco
    in_data=infile.read_IF("Hyper_Prony_Material_Model.txt"); // Read the input file and save the data of the input file
    infile.read_parse(in_data); // Parse the input file data


    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    // Obtain the name and number of the input data files inside "Data_Suite_2.txt"
    line=infile.read_dataF("Data_Suite.txt");
    int n_files=line.size(); // Number of files present
																																cout<<endl;
																																cout<<"Number of data files: "<<n_files<<endl;// Prints the number of data files
    //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    int error_flag=1; // Func write_F: flag to create a new error file

    // Main for loop to read the files, calculations and create the results file.
    for(int i=0; i<n_files; i++)
        {
            //Initialize the vectors to store the data
            vector<double> data_time; // Test data: Time
            vector<double> data_strain; // Test data: Strain
            vector<double> data_stress; // Test data: Stress
            vector<double> calc_stress; // Calculated stress data
            vector<double> stretch_data; // calculated stretch data
            vector<double> TS_visco; // calculated parameter used for the viscoelastic calibration (planar mode)
            vector<double>error; // calculated error between the test data and the calibration mode

            //Variables initialization
            double errori=0.0;
            double errort=0.0;
            string def_type=" ";

			//Obtain the experimental test type used to generate the test data
            if(line[i].substr(0,2)=="UN")
                def_type="UN";

            if(line[i].substr(0,2)=="EB")
                def_type="EB";

            if(line[i].substr(0,2)=="PT")
                def_type="PT";

           																									 cout<<"Test data type: "<<def_type<<endl; // print tension test mode information
            infile.read_data(line[i],data_time,data_strain,data_stress);


			// Perform the material calibration using the Yeoh Model (Checked)
            if(infile.data_IN.Hyper_Mod=="YEOH"||infile.data_IN.Hyper_Mod=="Yeoh")
                {
                    HypV.Yeoh_Model(data_strain,infile.data_IN.C_Param,def_type,calc_stress,stretch_data,TS_visco); //Yeoh calculations
                    HypV.Prony_Error_RP(data_time,data_stress,calc_stress,def_type,infile.data_IN.gi,infile.data_IN.ti,stretch_data,error,TS_visco);
                    infile.write_F(line[i],data_time,data_strain,data_stress,calc_stress,error,error_flag);


				}

			// Perform the material calibration using the Neo Model (Checked)
            if(infile.data_IN.Hyper_Mod=="NEO"||infile.data_IN.Hyper_Mod=="Neo")
                {

					HypV.Neo_Model(data_strain,infile.data_IN.C_Param,def_type,calc_stress,stretch_data,TS_visco); //Neo calculations

					 if(prony){ 

                    		HypV.Prony_Error_RP(data_time,data_stress,calc_stress,def_type,infile.data_IN.gi,infile.data_IN.ti,stretch_data,error,TS_visco);

					}else{
						//Error calc without visco
							for(int i=0; i<int(calc_stress.size()); i++)
								{
									if(data_stress[i]==0.0)
										errori=pow(data_stress[i]-calc_stress[i],2);
									else
										errori=pow(data_stress[i]-calc_stress[i],2)/pow(data_stress[i],2);
										error.push_back(errori);
										errort=errort+errori;
								}

							error.push_back(errort);
							cout<<"ERROR SUMi: "<<error.back()<<endl;
							errort=0.0;


					}

							infile.write_F(line[i],data_time,data_strain,data_stress,calc_stress,error,error_flag);


                }

			// Perform the material calibration using the Ogden Model (Checked)
            if(infile.data_IN.Hyper_Mod=="OGDEN"||infile.data_IN.Hyper_Mod=="Ogden")
                { 

                    HypV.Ogden(data_strain,def_type,infile.data_IN.OG_mu, infile.data_IN.OG_alpha,calc_stress,stretch_data,TS_visco);
					HypV.Prony_Error_RP(data_time,data_stress,calc_stress,def_type,infile.data_IN.gi,infile.data_IN.ti,stretch_data,error,TS_visco);
					infile.write_F(line[i],data_time,data_strain,data_stress,calc_stress,error,error_flag);




                }

			// Perform the material calibration using the Van Der Waals Model
            if(infile.data_IN.Hyper_Mod=="VAN"||infile.data_IN.Hyper_Mod=="van")
                {
                    HypV.VanDWaals(data_strain, def_type,infile.data_IN.VDW_Param,calc_stress,stretch_data,TS_visco);

                    if(prony){ 

                    		HypV.Prony_Error_RP(data_time,data_stress,calc_stress,def_type,infile.data_IN.gi,infile.data_IN.ti,stretch_data,error,TS_visco);

					}else{
						
						//Error calc without visco
							for(int i=0; i<int(calc_stress.size()); i++)
								{
									if(data_stress[i]==0.0)
										errori=pow(data_stress[i]-calc_stress[i],2);
									else
										errori=pow(data_stress[i]-calc_stress[i],2)/pow(data_stress[i],2);
										error.push_back(errori);
										errort=errort+errori;
								}

							error.push_back(errort);
							cout<<"ERROR SUMi: "<<error.back()<<endl;
							errort=0.0;


					}

							infile.write_F(line[i],data_time,data_strain,data_stress,calc_stress,error,error_flag);

                }

            // Perform the material calibration using the Arruda-Boyce Model
            if(infile.data_IN.Hyper_Mod=="ARRUDA"||infile.data_IN.Hyper_Mod=="arruda")
                {
                    HypV.A_boyce(data_strain, def_type,infile.data_IN.Arruda_Param,calc_stress,stretch_data,TS_visco);



                    if(prony){

                    		HypV.Prony_Error_RP(data_time,data_stress,calc_stress,def_type,infile.data_IN.gi,infile.data_IN.ti,stretch_data,error,TS_visco);

					}else{
						//Error calc without visco
							for(int i=0; i<int(calc_stress.size()); i++)
								{
									if(data_stress[i]==0.0)
										errori=pow(data_stress[i]-calc_stress[i],2);
									else
										errori=pow(data_stress[i]-calc_stress[i],2)/pow(data_stress[i],2);
										error.push_back(errori);
										errort=errort+errori;
								}

							error.push_back(errort);
							cout<<"ERROR SUM: "<<error.back()<<endl;
							errort=0.0;


					}

							infile.write_F(line[i],data_time,data_strain,data_stress,calc_stress,error,error_flag);




                }

            if(infile.data_IN.Hyper_Mod=="POLYNOMIAL"||infile.data_IN.Hyper_Mod=="polynomial"){

					// With switch we can use different polynomial orders. RIght Now only N=1 is defined
					switch(infile.data_IN.Poly_N){

						case 1://Mooney-Rivlin

                                HypV.Poly_N1(data_strain, def_type,infile.data_IN.Mooney_R_Param,calc_stress,stretch_data,TS_visco);

                                if(prony){ 

                    				HypV.Prony_Error_RP(data_time,data_stress,calc_stress,def_type,infile.data_IN.gi,infile.data_IN.ti,stretch_data,error,TS_visco);

								}else{

								for(int i=0; i<int(calc_stress.size()); i++)
									{
										//calc_stress[i]=calc_stress[i]+BH[j][i]+BG[j][i]+BC[j][i]+BD[j][i];
										if(data_stress[i]==0.0)
											errori=pow(data_stress[i]-calc_stress[i],2);
										else
											errori=pow(data_stress[i]-calc_stress[i],2)/pow(data_stress[i],2);
											error.push_back(errori);
											errort=errort+errori;
									}

								error.push_back(errort);
								cout<<"ERROR SUM: "<<error.back()<<endl;
								errort=0.0;


					}

								infile.write_F(line[i],data_time,data_strain,data_stress,calc_stress,error,error_flag);
								break;

						default:	cout<<"Only N=1 (Mooney-Rivlin) Polynomial model supported"<<endl;

					}





            }

            error_flag=0; // flag to avoid the created "Data_Suite_Error.txt" file to overwrite
        }

    return 0;
}
