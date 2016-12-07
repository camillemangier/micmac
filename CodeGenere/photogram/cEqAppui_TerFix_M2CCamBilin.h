// File Automatically generated by eLiSe
#include "StdAfx.h"


class cEqAppui_TerFix_M2CCamBilin: public cElCompiledFonc
{
   public :

      cEqAppui_TerFix_M2CCamBilin();
      void ComputeVal();
      void ComputeValDeriv();
      void ComputeValDerivHessian();
      double * AdrVarLocFromString(const std::string &);
      void SetPts0_x(double);
      void SetPts0_y(double);
      void SetPts1_x(double);
      void SetPts2_y(double);
      void SetScNorm(double);
      void SetXIm(double);
      void SetXTer(double);
      void SetYIm(double);
      void SetYTer(double);
      void SetZTer(double);


      static cAutoAddEntry  mTheAuto;
      static cElCompiledFonc *  Alloc();
   private :

      double mLocPts0_x;
      double mLocPts0_y;
      double mLocPts1_x;
      double mLocPts2_y;
      double mLocScNorm;
      double mLocXIm;
      double mLocXTer;
      double mLocYIm;
      double mLocYTer;
      double mLocZTer;
};