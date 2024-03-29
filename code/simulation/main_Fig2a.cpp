//23/01/2020 CP: This code specifically uses the parameters for the Fig2a in this paper

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "basic_particle.h"
#include <random>
#include <vector>
#include <array>
#include <fstream>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>

gsl_rng *rgslbis2 = gsl_rng_alloc(gsl_rng_mt19937);

//Define constant
extern const double pi=3.14159265;
extern const double Delta=pow(10,-3); //diffusion
extern const double Lmax=1.0; //size of the grid
extern const double area=Lmax*Lmax; //area
extern const double Utot=0.1; //advection, corresponds to U\tau/2 in Young et al. 2001
extern const double k=2*pi/Lmax; 
extern const int size_pop=20000; //initial number of particles
extern const int tmax=30; //length of the simulation
extern const double proba_death=0.; //Death and birth probability
extern const double proba_repro=0.;

using namespace std;

//Basic functions to compute the distance between two particles
double distance(basic_particle p1, basic_particle p2)
{
	double x1,x2,y1,y2,dist;
	x1=p1.get_x();
	x2=p2.get_x();
	y1=p1.get_y();
	y2=p2.get_y();
	dist=pow(x1-x2,2.0)+pow(y1-y2,2.0);
	return dist;
}

//Compute the pair density.
double PairDens(double xi, double dxi, std::vector<basic_particle> Part_table)
{
double d2,dt2,dtt2;
double area=pow(Lmax,2);
int ki,kj;
basic_particle temp, current;
int iter=0;
int p1=0,p2=0;
    
    for(p1=0;p1<Part_table.size();p1++)
{   
    current=Part_table.at(p1);
    // double loop (or loop on all pairs)
    for (p2=0;p2<Part_table.size();p2++)
{
        if(p1!=p2)
{               
                temp=Part_table.at(p2);
                dtt2=area;
                for (ki=-1;ki<=1;ki++) //Because of the periodic boundary conditions, we need to consider the 8 other squares surrounding the one we are studying
            { 
                    for (kj=-1;kj<=1;kj++)
                    {   
                        dt2 = pow((temp.get_x() - current.get_x()+ki*Lmax),2) + pow((temp.get_y() - current.get_y()+kj*Lmax),2);
                        d2 = std::min(dtt2,dt2);
                        dtt2 = d2;
                      }
             }
                
                // squared distance computed
                // if distance between xi and xi+dxi increment PairDens
                if((d2<pow(xi+dxi,2))&&(d2>pow(xi,2)))
                {
                        iter++;
                }
              }
        }
}
        return iter/(pi*(pow(xi+dxi,2.0) - pow(xi,2.0))*area); // number of pairs between xi and xi+dxi divided by the area of the crown
}

//Births and deaths in a community. The current vector of particles is updated with the new particles (at the end of the table), then we remove the dead ones
void branching_process(std::vector<basic_particle> &part_1,double proba_repro, double proba_death)
{
	double proba;	
	std::vector <int> id_to_remove;
	int size=part_1.size();
	basic_particle tmp_part;

	for(int j=0;j<size;j++)
	{
		proba=gsl_rng_uniform(rgslbis2); //Computes a number between 0 and 1, then compare it with the different probabilites of death, birth, or just staying alive
		if (proba<proba_repro) //new particle at the same place, and the parent is still there
		{
			tmp_part=basic_particle(part_1.at(j).get_x(),part_1.at(j).get_y(),part_1.at(j).get_yfirst(),part_1.at(j).get_firstparent());
			part_1.push_back(tmp_part);

               } else if( (proba_repro<=proba) and (proba<(proba_repro+proba_death)) )
		{
			id_to_remove.push_back(j); //Here, we just do a list of the indices we will need to remove at the end. We cannot remove them on the fly because it would modify the sequence of rows
		}
	}
	for(int j=id_to_remove.size()-1;j>=0;j--) //We remove dead individuals starting at the end of the table. Indeed, if we were to start by the beginning, removing row number 2 would shift all following rows (element 4 from the old table would be element 3, for example)
	{
			part_1.erase(part_1.begin()+id_to_remove[j]);
	}
} 

int main()
{
	int i,j,t;
	double a_x,a_y,phi,theta,a_n,xi,dxi,pow_min,pow_max,dpow,pow_i,pcf;
	std::vector<basic_particle> Part_table;
	std::ofstream f0,f1;
	double C;

	 dxi=pow(10,-8); //width of the ring around a particle in which we look for other particles
        pow_min=-1+log10(Delta);pow_max=5+log10(Delta); //These are the limits for r/Delta in the original paper, we compute r/Delta=pow(10,x) where x is between pow_min and pow_max
        dpow=0.5; //Increment between pow_min and pow_max (will determine the number of points we have for pcf=f(r/Delta))


	//Open the file in which we will have the x, y, parent of each particle
	f0.open("Spatial_distribution_particle_Fig2a.txt");
	f1.open("pcf_particle_Fig2a.txt");

	//Initialize
	for(i=0; i < size_pop; i++)
	{
		a_x=gsl_rng_uniform(rgslbis2);
		a_y=gsl_rng_uniform(rgslbis2);
		Part_table.push_back(basic_particle(a_x,a_y,a_y,i));
                f0<< 0 <<";";
                f0<< Part_table[i].get_x()  << ';';
                f0<< Part_table[i].get_y()  << ';';
                f0<< Part_table[i].get_yfirst()  << ";";
                f0<< Part_table[i].get_firstparent()  << std::endl;
	}

	//Run the simulation
	for(t=0;t<=tmax;t++)
	{
	printf("TIME=%d\n",t);
	//Compute the phase in x and y for the turbulent flow from Pierrehumbert. These phases are common to each particle as they correspond to a unique flow
	phi=gsl_rng_uniform(rgslbis2)*2*pi;
	theta=gsl_rng_uniform(rgslbis2)*2*pi;

	//First step: birth and death	
	//branching_process(Part_table,proba_repro,proba_death);

	//For each particle, diffusion, then advection
	for(j=0; j<Part_table.size(); j++)
	{
		Part_table[j].diffusion(Delta, Lmax);
		Part_table[j].pierrehumbert_flow(Utot, k, phi,theta, Lmax);
		if(t==tmax)
                {
                f0<< t <<";";
                f0<< Part_table[j].get_x()  << ';';
                f0<< Part_table[j].get_y()  << ';';
                f0<< Part_table[j].get_yfirst()  << ";";
                f0<< Part_table[j].get_firstparent()  << std::endl;
                }


	} //end j, i.e. the particle mvt
	} //end t, i.e the whole simulation

	//End of the simulation
	pow_i=pow_min;
	while (pow_i<pow_max && pow_i<0.0)
	{
		printf("pow %lg\n",pow_i);
		xi=pow(10,pow_i);
		pow_i+=dpow;
		C=Part_table.size()/area;
		pcf=PairDens(xi,dxi,Part_table)/pow(C,2);
		f1<<xi/Delta<<";";
		f1<<pcf<<std::endl;
	}

	f0.close();
	f1.close();
	return 0;
}

int main();
