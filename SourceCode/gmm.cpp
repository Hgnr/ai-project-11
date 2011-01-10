// Gaussian mixture model class
// Hand writing recognition Januari project, MSc AI, University of Amsterdam
// Thijs Kooi, 2011

//To do: optimise, work on arrays rather than vectors

#include "gmm.h"

double PI = 4.0*atan(1.0);

int main()
{
	vector<vector<double> > A;
	vector<double> b;
	vector<double> c;
// 	double test[3][3] = {{2.0,3.0,4.0},{3.0,4.0,5.0},{2.0,5.0,6.0}};
	double test[2][2] = {{1.0,2.0},{3.0,4.0}};

	GMM testGMM;
// 	double test1[3] = {2.0,4.0,3.0};
// 	double test2[3] = {4.0,3.0,2.0};
	
// 	for(size_t i = 0; i < 3; ++i)
// 	{
// 		b.push_back(test1[i]);
// 		c.push_back(test2[i]);
// 	}
		
	for(size_t i = 0; i < 2; ++i)
	{
		b.clear();
		for(size_t j = 0; j < 2; ++j)
			b.push_back(test[i][j]);
		A.push_back(b);
	}
	
	testGMM.testInverse(A);
}

//Constructors
GMM::GMM() { mixture_components = 1; }
GMM::GMM(int n) { mixture_components = n; }
//end constructors

double GMM::gmmProb(vector<double> x)
{
	double product = 1.0;
	for(size_t k = 0; k < mixture_components; ++k)
		product*=priors[k]*gmmProb(x,k);
	return product;
}

double GMM::gmmProb(vector<double> x, int component_number)
{
	double normalisation_constant = 1.0/( pow((2.0*PI),data_dimension/2.0) * pow(determinant(covariances[component_number]),0.5) );
	double exponent = -0.5*mahalanobisDistance(x,means[component_number],covariances[component_number]);
	
	return normalisation_constant*exp(exponent);
}

//Math functions
double GMM::mahalanobisDistance(vector<double> x,vector<double> mean,vector<vector<double> > covariance)
{
	vector<double> difference;
	for(size_t d = 0; d < data_dimension; ++d)
		difference[d] = x[d]-mean[d];
	
	vector<vector<double> > inverse_covariance = inverse(covariance);
	
	vector<double> distance ;
	for(size_t d = 0; d < data_dimension; ++d)
		distance[d]==innerProduct(difference,inverse_covariance[d]);
	
	return innerProduct(distance,difference);
}

double GMM::innerProduct(vector<double> a, vector<double> b)
{
	double sum = 0.0;
	for(size_t d = 0; d < a.size(); ++d)
		sum+=a[d]*b[d];
	return sum;
}

double GMM::determinant(vector<vector<double> > A)
{
	if(A.size() == 1)
		return A[0][0];
	else if(A.size() == 2)
		return A[0][0]*A[1][1] - A[1][0]*A[0][1];
	else
	{
		double det = 0.0;
		for(size_t d = 0; d < A.size(); ++d)
		{
			if(d%2 == 0)
				det+=A[0][d]*determinant(getMinor(A,0,d));
			else
				det-=A[0][d]*determinant(getMinor(A,0,d));
		}
		return det;
	}	
}

vector<vector<double> > GMM::getMinor(vector<vector<double> > mat, int m, int n)
{
	mat.erase(mat.begin()+m);
	for(size_t j = 0; j < mat.size(); ++j)
		mat[j].erase(mat[j].begin()+n);
	
	return mat;
}

vector<vector<double> > GMM::inverse(vector<vector<double> > A)
{
	double detA = determinant(A);
	if(detA == 0.0)//Matrix is singular
		return A;
	else
	{
		vector<vector<double> > AInverse;
		vector<double> row;
		for(size_t m = 0; m < A.size(); ++m)
		{
			row.clear();
			for(size_t n = 0; n < A.size(); ++n)
			{
				if((m+n)%2 == 0)
					row.push_back( (1.0/detA)*determinant(getMinor(A,m,n)));
				else
					row.push_back( (-1.0/detA)*determinant(getMinor(A,m,n)));
			}
			AInverse.push_back(row);
		}
		
		return transpose(AInverse);
	}
}

vector<vector<double> > GMM::transpose(vector<vector<double> > A)
{
	vector<vector<double> > Atranspose;
	vector<double> row;
	
	for(size_t m = 0; m < A.size(); ++m)
	{
		row.clear();
		for(size_t n = 0; n < A.size(); ++n)
			row.push_back(0.0);
		Atranspose.push_back(row);
	}
	for(size_t m = 0; m < A.size(); ++m)
		for(size_t n = 0; n < A.size(); ++n)
			Atranspose[m][n] = A[n][m];
		
	return Atranspose;
	
}

//End math functions

//Getters and setters
int GMM::getMixtureComponents() { return mixture_components; }
void GMM::setMixtureComponents(int N) { mixture_components =  N; }

int GMM::getPrior(int component_number) { return priors[component_number]; }
void GMM::setPrior(int component_number,double probability) { priors[component_number] = probability; }

vector<double> GMM::getMean(int component_number) { return means[component_number]; }
void GMM::setMean(int component_number,vector<double> mean) { means[component_number] = mean; }

vector<vector<double> > GMM::getCovariance(int component_number) { return covariances[component_number]; } 
void GMM::setCovariance(int component_number, vector<vector<double> >covariance) { covariances[component_number] = covariance; }
//End getters and setters

// //Print functions
// void GMM::printPrior(int component_number);
// void GMM::printMean(int component_number);
// void GMM::printCovariance(int component_number);
// //end print functions

// Testing and debugging
void GMM::testMahalanobisDistance(vector<double>,vector<double>,vector<vector<double> >)
{
}
void GMM::testInnerProduct(vector<double> a,vector<double> b)
{
	cout << innerProduct(a,b) << endl;
}
void GMM::testDeterminant(vector<vector<double> > A)
{
	cout << "Determinant of matrix: " << endl;
	for(size_t i = 0; i < A.size(); ++i)
	{
		for(size_t j = 0; j < A[i].size(); ++j)
			cout << A[i][j] << " ";
		cout << endl;
	}
	cout << "is: " << determinant(A) << endl;
}

void GMM::testInverse(vector<vector<double> > A)
{
	vector<vector<double> > Ainverse = inverse(A);
	cout << "Inverse of :" << endl;
	printMatrix(A);
	cout << "Is: " << endl;
	printMatrix(Ainverse);
}

void GMM::testMVNPDF(vector<double>)
{
}
void GMM::testGMM(vector<double>)
{
}
void GMM::printMatrix(vector<vector<double> > A)
{
	for(size_t i = 0; i < A.size(); ++i)
	{
		for(size_t j = 0; j < A[i].size(); ++j)
			cout << A[i][j] << " ";
		cout << endl;
	}
}
// vector<double> GMM::makeVector(double[] a)
// {
// 	
// }
// end 