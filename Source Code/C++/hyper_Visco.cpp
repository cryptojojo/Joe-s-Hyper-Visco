/**
 * \file hyper_Visco.cpp
 * \author Miguel Oliveira (Dassault Systemes, Simulia Corp.)
 * \date 10 August 2016
 * \brief This file contains the implementation of the functions to obtain the material calibration.
 *
 */

#include "hyper_Visco.h"
using namespace std;


hyper_Visco::hyper_Visco()
{

}


hyper_Visco::~hyper_Visco()
{
  
}

/** \brief This function implements the Hyperelastic Mooney-Rivlin model.
 *
 * \param strain vector<double>& strain data from the input data files

 * \param deform_type string information about the mode used to create the test data: Uniaxial ("UN"), Biaxial ("EB") or Planar ("PT")
 * \param Mon_R_Param vector<double>& C10,C01 parameters
 * \param calc_stress vector<double>& Instantaneous stress calculated using the Yeoh Model (output variable)
 * \param stretch_data vector <double>& calculated stretch data using the strain data (output variable)
 * \param TS vector <double>& calculated variable used for the planar viscoelastic calibration (output variable)
 * \return void
 *
 */
void hyper_Visco:: Poly_N1(vector<double> &strain,string deform_type, vector<double> &Mon_R_Param,vector<double> &calc_stress, vector <double> &stretch_data,vector <double> &Ts){


    double stretch=0.0;
    double Inst_stress=0.0;
    double Inst_stress2=0.0;



    //for(int i=0;i<VDW_Param.size();i++) cout<<"VDW PARAM: "<<i<<" "<<VDW_Param[i]<<endl; PARAM Checked

    if(deform_type=="UN")
        {
            for(int i=0; i<int(strain.size()); i++) //Calc stress is OK -Confirmed
                {
                    stretch=strain[i]+1.0;

                    Inst_stress=2.0*(1.0-(1.0/pow(stretch,3.0)))*(Mon_R_Param[0]*stretch+Mon_R_Param[1]);
                   // Inst_stress=2.0*(1.0-(1.0/pow(stretch,3.0)))*(Mon_R_Param[0]*stretch);
                    calc_stress.push_back(Inst_stress);
                    stretch_data.push_back(stretch);



                }
        }
    else
        if(deform_type=="EB")
            {
                //
                for(int i=0; i<int(strain.size()); i++)
                    {

						stretch=strain[i]+1.0;

						Inst_stress=2.0*(stretch-(1.0/pow(stretch,5.0)))*(Mon_R_Param[0]+Mon_R_Param[1]*pow(stretch,2.0));
						calc_stress.push_back(Inst_stress);
						stretch_data.push_back(stretch);



                    }
            }
        else
            if(deform_type=="PT")
                {
                    for(int i=0; i<int(strain.size()); i++)
                        {
							stretch=strain[i]+1.0;

							Inst_stress=2.0*(stretch-(1.0/pow(stretch,3.0)))*(Mon_R_Param[0]+Mon_R_Param[1]);
							Inst_stress2=2.0*((pow(stretch,2.0)-1)/pow(stretch,3.0))*(Mon_R_Param[0]+Mon_R_Param[1]*pow(stretch,2.0));
							Ts.push_back(Inst_stress2);
							calc_stress.push_back(Inst_stress);
                    		stretch_data.push_back(stretch);

                        }
                }
            else
                cout<<"Error on the test type definition"<<endl;





}



/** \brief This function implements the Hyperelastic Yeoh model.
 *
 * \param strain vector<double>& strain data from the input data files
 * \param Param vector<double>& C10,C20 and C30 parameters
 * \param deform_type string information about the mode used to create the test data: Uniaxial ("UN"), Biaxial ("EB") or Planar ("PT")
 * \param calc_stress vector<double>& Instantaneous stress calculated using the Yeoh Model (output variable)
 * \param stretch_data vector <double>& calculated stretch data using the strain data (output variable)
 * \param TS vector <double>& calculated variable used for the planar viscoelastic calibration (output variable)
 * \return void
 *
 */
void hyper_Visco::Yeoh_Model(vector<double> &strain,vector<double>&Param,string deform_type, vector<double> &calc_stress, vector <double> &stretch_data,vector <double> &TS)
{
    //Calculate Instantaneous stress;
    // Function COnfirmed comparing to Abaqus

    double I=0.0;
    double stretch=0.0;
    double Inst_stress=0.0;
    double prony_TS=0.0;
    double c10=Param[0];
    double c20=Param[1];
    double c30=Param[2];

    if(deform_type=="UN")
        {
            for(int i=0; i<int(strain.size()); i++) //Calc stress is OK -Confirmed
                {

                    stretch=strain[i]+1.0;
                    I =pow(stretch,2.0)+(2.0/stretch);
                    Inst_stress=2.0*(stretch-(1.0/ pow(stretch,2.0)))*(c10+(2.0*c20*(I-3.0))+(3.0*c30*pow((I-3.0),2.0)));
                 																											 //  cout<<setprecision(12)<<"strain: "<<strain[i]<<"stretch: "<<stretch<<"I: "<<I<<" UN: "<<Inst_stress<<endl;
                    calc_stress.push_back(Inst_stress);
                    stretch_data.push_back(stretch);

                }
        }
    else
        if(deform_type=="EB")
            {

                for(int i=0; i<int(strain.size()); i++)
                    {
                        stretch=strain[i]+1.0;
                        I =2.0*pow(stretch,2.0)+(1.0/pow(stretch,4));
                        Inst_stress=2.0*(stretch-(1.0/ pow(stretch,5.0)))*(c10+(2.0*c20*(I-3.0))+(3.0*c30*pow((I-3.0),2.0)));
                        calc_stress.push_back(Inst_stress);
                        stretch_data.push_back(stretch);
                    }
            }
        else
            if(deform_type=="PT")
                {


                    for(int i=0; i<int(strain.size()); i++)
                        {
                            stretch=strain[i]+1.0;
                            I =pow(stretch,2.0)+(1.0/pow(stretch,2))+1;
                            Inst_stress=2.0*(stretch-(1.0/ pow(stretch,3.0)))*(c10+(2.0*c20*(I-3.0))+(3.0*c30*pow((I-3.0),2.0)));
                            prony_TS=2.0*((pow(stretch,2.0)-1.0)/pow(stretch,3.0))*(c10+(2.0*c20*(I-3.0))+(3.0*c30*pow((I-3.0),2.0)));

                            calc_stress.push_back(Inst_stress);
                            TS.push_back(prony_TS);
                            stretch_data.push_back(stretch);
                        }
                }
            else
                 cout<<"Tension mode readed: "<< deform_type<<"\n Please verify the name of the test data file."<<endl;
}


/** \brief This function implements the Hyperelastic Neo model.
 *
 * \param strain vector<double>& strain data from the input data files
 * \param Param vector<double>& C10 parameter
 * \param deform_type string information about the mode used to create the test data: Uniaxial ("UN"), Biaxial ("EB") or Planar ("PT")
 * \param calc_stress vector<double>& Instantaneous stress calculated using the Neo Model (output variable)
 * \param stretch_data vector <double>& calculated stretch data using the strain data (output variable)
 * \param TS vector <double>& calculated variable used for the planar viscoelastic calibration (output variable)
 * \return void
 *
 */
void hyper_Visco::Neo_Model(vector<double> &strain,vector<double>&Param,string deform_type, vector<double> &calc_stress, vector <double> &stretch_data,vector <double> &TS)
{
    double I=0.0;
    double stretch=0.0;
    double Inst_stress=0.0;
    double prony_TS=0.0;
    double c10=Param[0];

    if(deform_type=="UN")
        {
            for(int i=0; i<int(strain.size()); i++)
                {
                    stretch=strain[i]+1.0;
                    I=pow(stretch,2.0)+(2.0/stretch);
                    Inst_stress=2.0*(stretch-(1.0/ pow(stretch,2.0)))*c10;
                    calc_stress.push_back(Inst_stress);
                    stretch_data.push_back(stretch);
                }
        }
    else
        if(deform_type=="EB")
            {
                //
                for(int i=0; i<int(strain.size()); i++)
                    {
                        stretch=strain[i]+1.0;
                        I =2.0*pow(stretch,2.0)+(1.0/pow(stretch,4));
                        Inst_stress=2.0*(stretch-(1.0/ pow(stretch,5.0)))*c10;
                        calc_stress.push_back(Inst_stress);
                        stretch_data.push_back(stretch);
                    }
            }
        else
            if(deform_type=="PT")
                {
                    cout<<"PT"<<endl;

                    for(int i=0; i<int(strain.size()); i++)
                        {
                            stretch=strain[i]+1.0;
                            I =pow(stretch,2.0)+(1.0/pow(stretch,2))+1;
                            Inst_stress=2.0*(stretch-(1.0/ pow(stretch,3.0)))*c10;
                            prony_TS=2.0*((pow(stretch,2.0)-1.0)/pow(stretch,3.0))*c10;
                            calc_stress.push_back(Inst_stress);
                            TS.push_back(prony_TS);
                            stretch_data.push_back(stretch);
                        }
                }
            else
                cout<<"Tension mode readed: "<< deform_type<<"\n Please verify the name of the test data file."<<endl;
}


/** \brief This function implements the Hyperelastic Ogden model.
 *
 * \param strain vector<double>& strain data from the input data files
 * \param deform_type string information about the mode used to create the test data: Uniaxial ("UN"), Biaxial ("EB") or Planar ("PT")
 * \param mu  vector<double> & Ogden model Mu parameters
 * \param alpha  vector<double> & Ogden Model alpha parameters
 * \param calc_stress vector<double>& Instantaneous stress calculated using the Ogden Model (output variable)
 * \param stretch_data vector <double>& calculated stretch data using the strain data (output variable)
 * \param TS_visco vector <double>& calculated variable used for the planar viscoelastic calibration (output variable)
 * \return void
 *
 */
void hyper_Visco::Ogden(vector<double> &strain,string deform_type, vector<double> &mu, vector<double>&alpha,vector<double> &calc_stress, vector <double> &stretch_data, vector <double> &TS_visco)
{
    int N= int(alpha.size()); //Ogden Order
  																																						  cout<<"N Ogden: "<<N<<endl;
    double stretch=0.0;
    double Inst_stress=0.0;
    double TS=0.0;
    double sigm22=0.0;

    if(deform_type=="UN")
        {
            for(int i=0; i<int(strain.size()); i++)
                {
                    stretch=strain[i]+1.0;

                    for(int j=0; j<N; j++)
                        Inst_stress=Inst_stress+(2.0*(mu[j]/alpha[j])*(pow(stretch,alpha[j]-1.0)-pow(stretch,-0.5*alpha[j]-1.0)));
																																//	cout<<setprecision(15)<<"TU_Eva: "<<Inst_stress<<endl;
                    calc_stress.push_back(Inst_stress);
                    stretch_data.push_back(stretch);
                    Inst_stress=0.0; //Re-Initialize the variable;
                }
        }
    else
        if(deform_type=="EB")
            {
                //
                for(int i=0; i<int(strain.size()); i++)
                    {
                        stretch=strain[i]+1.0;

                        for(int j=0; j<N; j++)
                            Inst_stress=Inst_stress+(2.0*(mu[j]/alpha[j])*(pow(stretch,alpha[j]-1.0)-pow(stretch,-2.0*alpha[j]-1.0)));

                        calc_stress.push_back(Inst_stress);
                        stretch_data.push_back(stretch);
                        Inst_stress=0.0; //Re-Initialize the variable;
                    }
            }
        else
            if(deform_type=="PT")
                {
                    //cout<<"PT"<<endl;
                    for(int i=0; i<int(strain.size()); i++)
                        {
                            stretch=strain[i]+1.0;

                            for(int j=0; j<N; j++)
                                {
                                    Inst_stress=Inst_stress+(2.0*(mu[j]/alpha[j])*(pow(stretch,alpha[j]-1.0)-pow(stretch,-alpha[j]-1.0)));

                                    sigm22=sigm22+(2.0*(mu[j]/alpha[j])*(1-(1/pow(stretch,alpha[j]))));
                                }

                            TS=sigm22/stretch;
                            TS_visco.push_back(TS);
                            calc_stress.push_back(Inst_stress);
                            stretch_data.push_back(stretch);
                            Inst_stress=0.0; //Re-Initialize the variable;

                            sigm22=0.0;
                        }
                }
            else
                cout<<"Work more, this is incomplete"<<endl;
}

/** \brief This function implements the Hyperelastic Van Der Waals model.
 *
 * \param strain vector<double>& strain data from the input data files
 * \param deform_type string information about the mode used to create the test data: Uniaxial ("UN"), Biaxial ("EB") or Planar ("PT")
 * \param VDW_Param  vector<double> & Van Der Waals parameters extracted from the input file
 * \param calc_stress  vector<double>& Instantaneous stress calculated using the Van Der Waals Model (output variable)
 * \param stretch_data vector <double>& calculated stretch data using the strain data (output variable)
 * \return void
 *
 */

void hyper_Visco::VanDWaals(vector<double> &strain,string deform_type, vector<double> &VDW_Param,vector<double> &calc_stress, vector <double> &stretch_data,vector <double> &Ts)
{
    // equations Theory manual 4.6.1
    // Confirmed results
    //Isight: Param Lambda_m must be >sqrt(3.0)
    double I1=0.0;
    double I2=0.0;
    double I=0.0;
    double stretch=0.0;
    double Inst_stress=0.0;
    double Inst_stress2=0.0;
    double dU_I1=0.0;
    double dU_I2=0.0;

    double eta=0.0;
    //for(int i=0;i<VDW_Param.size();i++) cout<<"VDW PARAM: "<<i<<" "<<VDW_Param[i]<<endl; PARAM Checked

    if(deform_type=="UN")
        {
            for(int i=0; i<int(strain.size()); i++) //Calc stress is OK -Confirmed
                {
                    stretch=strain[i]+1.0;
                    I1 = pow(stretch,2.0)+(2.0/stretch);
                    I2 = (1/pow(stretch,2.0))+2.0*stretch;
                    I=(1-VDW_Param[3])*I1+VDW_Param[3]*I2;
                    eta=sqrt((I-3.0)/(pow(VDW_Param[1],2.0)-3.0));

                    Inst_stress= VDW_Param[0]*(1.0-(1.0/pow(stretch,3.0)))*((1.0/(1.0-eta))-VDW_Param[2]*sqrt((I-3.0)/2.0))*(stretch*(1.0-VDW_Param[3])+VDW_Param[3]);

                    calc_stress.push_back(Inst_stress);
                    stretch_data.push_back(stretch);

                }
        }
    else
        if(deform_type=="EB")
            {
                //
                for(int i=0; i<int(strain.size()); i++)
                    {
                        stretch=strain[i]+1.0;
                        I1 =2.0*pow(stretch,2.0)+(1.0/pow(stretch,4.0));
                        I2 = 2.0*(1/pow(stretch,2.0))+pow(stretch,4.0);
                        I=(1-VDW_Param[3])*I1+VDW_Param[3]*I2;
                        eta=sqrt((I-3.0)/(pow(VDW_Param[1],2.0)-3.0));
                        Inst_stress= VDW_Param[0]*(stretch-(1.0/pow(stretch,5.0)))*((1.0/(1.0-eta))-VDW_Param[2]*sqrt((I-3.0)/2.0))*(1.0-VDW_Param[3]+VDW_Param[3]*pow(stretch,2.0));
                        calc_stress.push_back(Inst_stress);
                        stretch_data.push_back(stretch);
                      //   cout<<"eta: "<<eta<<" I1: "<<I1<<" I2: "<<I2<<" I: "<<I<<" Inst Stress: "<<Inst_stress<<endl;
                    }
            }
        else
            if(deform_type=="PT")
                {
                    for(int i=0; i<int(strain.size()); i++)
                        {
                            stretch=strain[i]+1.0;
                            I1= pow(stretch,2.0)+(1.0/pow(stretch,2.0))+1;
                            I2=I1;
                            I=(1-VDW_Param[3])*I1+VDW_Param[3]*I2;
                            eta=sqrt((I-3.0)/(pow(VDW_Param[1],2.0)-3.0));
                            Inst_stress= VDW_Param[0]*(stretch-(1.0/pow(stretch,3.0)))*((1.0/(1.0-eta))-VDW_Param[2]*sqrt((I-3.0)/2.0));
							dU_I1=(VDW_Param[0]*(VDW_Param[3]-1.0)*(((sqrt(2.0)*VDW_Param[2]*eta*sqrt(I-3.0))/2.0)-((sqrt(2.0)*VDW_Param[2]*sqrt(I-3.0))/(2.0))+1.0))/(2.0*eta-2.0);
							dU_I2=-(((VDW_Param[0]*VDW_Param[3])/2.0*(eta-1.0))+(VDW_Param[2]*VDW_Param[0]*VDW_Param[3]*sqrt((I-3)/2)/2.0));
							Inst_stress2=2.0*((pow(stretch,2.0)-1)/pow(stretch,3.0))*(dU_I1+pow(stretch,2.0)*dU_I2);

                   			//  if(i==1)  cout<<setprecision(10)<<"Strecth: "<<stretch<<"DU_I1: "<<dU_I1<<" DU_I2: "<<dU_I2<<" TS: "<<Inst_stress2<<endl;
                            //Inst_stress2=Inst_stress/(stretch/(pow(stretch,2.0)-1));

                         //   Inst_stress2=2.0*

                            calc_stress.push_back(Inst_stress);
                            Ts.push_back(Inst_stress2);
                            stretch_data.push_back(stretch);

                        //  cout<<setprecision(15)<<"eta: "<<eta<<" I1: "<<I1<<" I2: "<<I2<<" I: "<<I<<" Inst Stress: "<<Inst_stress<<endl;
                        }
                }
            else
                cout<<"Work more, this is incomplete"<<endl;
}

/** \brief This function implements the Hyperelastic Arruda-Boyce model.
 *
 * \param strain vector<double>& strain data from the input data files
 * \param deform_type string information about the mode used to create the test data: Uniaxial ("UN"), Biaxial ("EB") or Planar ("PT")
 * \param Arruda_Param  vector<double> & Arruda-Boyce parameters extracted from the input file
 * \param calc_stress  vector<double>& Instantaneous stress calculated using the Arruda-BOyce Model (output variable)
 * \param stretch_data vector <double>& calculated stretch data using the strain data (output variable)
 * \return void
 *
 */
void hyper_Visco::A_boyce(vector<double> &strain,string deform_type, vector<double> &Arruda_Param,vector<double> &calc_stress, vector <double> &stretch_data,vector<double> &Ts)
{
    // Function COnfirmed comparing to Abaqus
    double c10=0.5;
    double c20=0.05;
    double c30=1.047619047619047619047619047619e-2;
    double c40=2.71428571428571428571428571429e-3;
    double c50=7.7031539888682745825602968460111e-4;
    vector <double> Cs{c10,c20,c30,c40,c50};
    double I1=0.0;
    double stretch=0.0;
    double Inst_stress=0.0;
     double Inst_stress2=0.0;
    double arruda_sum=0.0;

    if(deform_type=="UN")
        {
            for(int i=0; i<int(strain.size()); i++) //Calc stress is OK -Confirmed
                {
                    stretch=strain[i]+1.0;
                    I1 = pow(stretch,2.0)+(2.0/stretch);

                    for(int a=1; a<6; a++) arruda_sum=arruda_sum+(Cs[a-1]*a/pow(Arruda_Param[1],2*a-2))*pow(I1,a-1);

                    Inst_stress=2.0*Arruda_Param[0]*(stretch-(1.0/pow(stretch,2.0)))*arruda_sum;
                    //																														cout<<"Stress UN: "<<Inst_stress<<endl;


                    calc_stress.push_back(Inst_stress);
                    stretch_data.push_back(stretch);
                    arruda_sum=0.0;
                }
        }
    else
        if(deform_type=="EB")
            {
                //
                for(int i=0; i<int(strain.size()); i++)
                    {
                        stretch=strain[i]+1.0;
                        I1 =2.0*pow(stretch,2.0)+(1.0/pow(stretch,4.0));

                        for(int a=1; a<6; a++) arruda_sum=arruda_sum+(Cs[a-1]*a/pow(Arruda_Param[1],2*a-2))*pow(I1,a-1);

                        Inst_stress=2*Arruda_Param[0]*(stretch-(1/pow(stretch,5)))*arruda_sum;
                        //																														cout<<"Stress EB: "<<Inst_stress<<endl;
                        calc_stress.push_back(Inst_stress);
                        stretch_data.push_back(stretch);
                        arruda_sum=0.0;
                        //Inst_stress=0.0; //Re-Initialize the variable;
                    }
            }
        else
            if(deform_type=="PT")
                {
                    //cout<<"PT"<<endl;
                    for(int i=0; i<int(strain.size()); i++)
                        {
                            stretch=strain[i]+1.0;
                            I1= pow(stretch,2.0)+(1.0/pow(stretch,2))+1;

                            for(int a=1; a<6; a++) arruda_sum=arruda_sum+(Cs[a-1]*a/pow(Arruda_Param[1],2*a-2))*pow(I1,a-1);

                            Inst_stress=2*Arruda_Param[0]*(stretch-(1/pow(stretch,3)))*arruda_sum;
                           	Inst_stress2=2*Arruda_Param[0]*((pow(stretch,2)-1)/pow(stretch,3))*arruda_sum;

                            calc_stress.push_back(Inst_stress);
                            Ts.push_back(Inst_stress2);
                            stretch_data.push_back(stretch);
                            arruda_sum=0.0;
                        }
                }
            else
                cout<<"Work more, this is incomplete"<<endl;
}


/** \brief This functions applies the viscoelastic calibration using Prony series and calculates error between the stress data and the calculated stress.
 *
 * \param time vector<double>& time data from the test data file
 * \param stress vector<double>& stress data from the test data file
 * \param calc_stress vector<double>& calculated instantaneous stress without the viscoelastic calibration
 * \param deform_type string information about the tension mode
 * \param gi_data vector<double>& Prony terms gs
 * \param ti_data vector<double>& Prony terms taus
 * \param stretch_data vector <double>& calculated stretch data
 * \param error vector <double>& calculated error
 * \param Ts vector <double>& parameter for the planar mode viscoelastic calculations.
 * \return void
 *
 */
void hyper_Visco::Prony_Error_RP(vector<double> &time,vector<double> &stress,vector<double> &calc_stress,string deform_type,vector<double> &gi_data,vector<double> &ti_data,vector <double> &stretch_data,vector <double> &error,vector <double> &Ts)
{
    int nprony=int(gi_data.size()); // numero de prony members
//    cout<<nprony<<endl;
    double gamm=0.0;
    double dt=0.0;
    double alph=0.0;
    double beta=0.0;
    double errort=0.0;
    int no_of_cols = int(calc_stress.size());
    int no_of_rows = nprony;
    int initial_value = 0;
    std::vector<std::vector<double>> BH;
    BH.resize(no_of_rows, std::vector<double>(no_of_cols, initial_value));
    std::vector<std::vector<double>> BG;
    BG.resize(no_of_rows, std::vector<double>(no_of_cols, initial_value));
    std::vector<std::vector<double>> BI;
    BI.resize(no_of_rows, std::vector<double>(no_of_cols, initial_value));
    std::vector<std::vector<double>> BJ;
    BJ.resize(no_of_rows, std::vector<double>(no_of_cols, initial_value));

    for(int i=0; i<nprony; i++)
	{
		for(int j=1; j<=int(calc_stress.size()); j++)
		{
			 // number of lines of test data
                    if(j==1)
                        {
                            BH[i][j-1]=0.0;
                            BG[i][j-1]=0.0;
                            BI[i][j-1]=0.0;
                            BJ[i][j-1]=0.0;
                            dt=0.0;
                        }
                    else
                        if(j==2)
                            {
                                dt=time[j-1]-time[j-2];
                                gamm=exp(-dt/ti_data[i]);
                                alph=1.0-(ti_data[i]/dt*(1.0-gamm));
                                beta=(ti_data[i]/dt*(1.0-gamm))-gamm; // formulas ok

                                if(deform_type=="UN")
                                    {
                                        BH[i][j-1]=-2.0/3.0*gi_data[i]*alph*calc_stress[j-1];
                                        BG[i][j-1]= BH[i][j-1]/2.0;
                                    }
                                else
                                    if(deform_type=="EB")
                                        {
                                            BH[i][j-1]=-1.0/3.0*gi_data[i]*alph*calc_stress[j-1];
                                            BG[i][j-1]= 2*BH[i][j-1];
                                        }
                                    else
                                        if(deform_type=="PT") // Debuged with randy mathcad
                                            {
                                                alph=exp(-dt/ti_data[i]);
                                                beta= pow(ti_data[i],2.0)*(alph+((dt/ti_data[i])-1));
                                                gamm=ti_data[i]*(1-alph);
                                                BH[i][j-1]=-2.0/3.0*(gi_data[i]/ti_data[i])*(beta/dt*calc_stress[j-1]);
                                                BG[i][j-1]= BH[i][j-1]/2.0;
                                                BI[i][j-1]= 1.0/3.0*(gi_data[i]/ti_data[i])*(beta/dt*Ts[j-1]);
                                                BJ[i][j-1]= -BI[i][j-1];

                                            }
                                            if(isnan(BH[i][j-1])||isnan(BG[i][j-1])||isnan(BI[i][j-1])||isnan(BJ[i][j-1])){BH[i][j-1]=0.0;BG[i][j-1]=0.0;BI[i][j-1]=0.0;BJ[i][j-1]=0.0;}
                            }
                        else
                            {
                                dt=time[j-1]-time[j-2];
                                gamm=exp(-dt/ti_data[i]);
                                alph=1.0-(ti_data[i]/dt*(1.0-gamm));
                                beta=(ti_data[i]/dt*(1.0-gamm))-gamm; // formulas ok

                                if(deform_type=="UN")
                                    {
                                        BH[i][j-1]= -2.0/3.0*gi_data[i]*(alph*calc_stress[j-1]+beta*(stretch_data[j-1]/stretch_data[j-2])*calc_stress[j-2])+gamm*(stretch_data[j-1]/stretch_data[j-2])*BH[i][j-2];
                                        BG[i][j-1]= -1.0/3.0*gi_data[i]*(alph*calc_stress[j-1]+beta*pow(stretch_data[j-2]/stretch_data[j-1],2)*calc_stress[j-2])+gamm*pow(stretch_data[j-2]/stretch_data[j-1],2)*BG[i][j-2];
                                    }
                                else
                                    if(deform_type=="EB")
                                        {
                                            BH[i][j-1]= -1.0/3.0*gi_data[i]*(alph*calc_stress[j-1]+beta*(stretch_data[j-1]/stretch_data[j-2])*calc_stress[j-2])+gamm*(stretch_data[j-1]/stretch_data[j-2])*BH[i][j-2];
                                            BG[i][j-1]= -2.0/3.0*gi_data[i]*(alph*calc_stress[j-1]+beta*pow(stretch_data[j-2]/stretch_data[j-1],5)*calc_stress[j-2])+gamm*pow(stretch_data[j-2]/stretch_data[j-1],5)*BG[i][j-2];
                                        }
                                    else
                                        if(deform_type=="PT")
                                            {
                                                alph=exp(-dt/ti_data[i]);
                                                beta= pow(ti_data[i],2.0)*(alph+((dt/ti_data[i])-1));
                                                gamm=ti_data[i]*(1-alph);
                                                BH[i][j-1]= -2.0/3.0*(gi_data[i]/ti_data[i])*(stretch_data[j-1]/stretch_data[j-2])*((gamm-(beta/dt))*calc_stress[j-2]+(beta/dt)*(stretch_data[j-2]/stretch_data[j-1])*calc_stress[j-1])+(stretch_data[j-1]/stretch_data[j-2])*alph*BH[i][j-2];
                                                BG[i][j-1]= -1.0/3.0*(gi_data[i]/ti_data[i])*pow(stretch_data[j-2]/stretch_data[j-1],3.0)*((gamm-(beta/dt))*calc_stress[j-2]+(beta/dt)*pow(stretch_data[j-1]/stretch_data[j-2],3.0)*calc_stress[j-1])+pow(stretch_data[j-2]/stretch_data[j-1],3.0)*alph*BG[i][j-2];
                                                BI[i][j-1]= 1.0/3.0*(gi_data[i]/ti_data[i])*(stretch_data[j-1]/stretch_data[j-2])*((gamm-(beta/dt))*Ts[j-2]+(beta/dt)*(stretch_data[j-2]/stretch_data[j-1])*Ts[j-1])+(stretch_data[j-1]/stretch_data[j-2])*alph*BI[i][j-2];
                                                BJ[i][j-1]= -1.0/3.0*(gi_data[i]/ti_data[i])*pow(stretch_data[j-2]/stretch_data[j-1],3.0)*((gamm-(beta/dt))*Ts[j-2]+(beta/dt)*pow(stretch_data[j-1]/stretch_data[j-2],3.0)*Ts[j-1])+pow(stretch_data[j-2]/stretch_data[j-1],3.0)*alph*BJ[i][j-2];
                                            }
                                        else
                                            cout<<"Problem Prony  calc"<<endl;

									if(isnan(BH[i][j-1])||isnan(BG[i][j-1])||isnan(BI[i][j-1])||isnan(BJ[i][j-1])){BH[i][j-1]=0.0;BG[i][j-1]=0.0;BI[i][j-1]=0.0;BJ[i][j-1]=0.0;} // Duo to the formulation if one of the prony taus is defined as 0 the final sum will be corrupted (nan).
																																												//This forces  variables to 0 in case of NAN.

                            }
		}
	}


           // double mean=0.0;
          //  double abss=0.0;


    for(int i=0; i<int(calc_stress.size()); i++)
        {
            double errori=0.0;


            for(int j=0; j<nprony; j++)
                {
                    if(deform_type=="PT"){

                        calc_stress[i]=calc_stress[i]+BH[j][i]+BG[j][i]+BI[j][i]+BJ[j][i];

                    }else{
                        calc_stress[i]=calc_stress[i]+BH[j][i]+BG[j][i];


					}

                }

           			 if(stress[i]==0.0)
                        errori=pow(stress[i]-calc_stress[i],2.0);
                    else
                        {


                            errori=pow((stress[i]-calc_stress[i])/stress[i],2.0);



                        }







            error.push_back(errori);

            errort=errort+errori;

        }

			errort=errort/calc_stress.size(); // normalize by the number of points of the set
    		error.push_back(errort);


    cout<<"Error Sum: "<<error.back()<<endl;
    errort=0.0;


}



