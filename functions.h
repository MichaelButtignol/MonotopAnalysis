#include "TGraphAsymmErrors.h"


double getY( TGraphAsymmErrors* graph, double Xvalue)
{
  double Yvalue = 1; 
  double* X = graph->GetX();
  double* Y = graph->GetY();

  for ( int i = 0; X[i] <= 500 ; i++)
  {
     if( i != 0 && X[i] > Xvalue)
     {
        // Y = a*X + b with b equal to Y[i] - a*X[i]
        Yvalue = ((Y[i] - Y[i-1])/(X[i] - X[i-1]))* Xvalue + Y[i] - ((Y[i] - Y[i-1])/(X[i] - X[i-1]))*X[i];
        break;
     }
  }

  return Yvalue; 
}

double getSFtrigger( TGraphAsymmErrors* ratioPlot,  double pT, double eta)
{
  double SF = 1;
 
  if (abs(eta) <= 2.1 && pT <= 500)  SF = getY( ratioPlot, pT ); 
  else                               SF = 1;
  return SF;
}

