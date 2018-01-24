/*Header-MicMac-eLiSe-25/06/2007

    MicMac : Multi Image Correspondances par Methodes Automatiques de Correlation
    eLiSe  : ELements of an Image Software Environnement

    www.micmac.ign.fr

   
    Copyright : Institut Geographique National
    Author : Marc Pierrot Deseilligny
    Contributors : Gregoire Maillet, Didier Boldo.

[1] M. Pierrot-Deseilligny, N. Paparoditis.
    "A multiresolution and optimization-based image matching approach:
    An application to surface reconstruction from SPOT5-HRS stereo imagery."
    In IAPRS vol XXXVI-1/W41 in ISPRS Workshop On Topographic Mapping From Space
    (With Special Emphasis on Small Satellites), Ankara, Turquie, 02-2006.

[2] M. Pierrot-Deseilligny, "MicMac, un lociel de mise en correspondance
    d'images, adapte au contexte geograhique" to appears in 
    Bulletin d'information de l'Institut Geographique National, 2007.

Francais :

   MicMac est un logiciel de mise en correspondance d'image adapte 
   au contexte de recherche en information geographique. Il s'appuie sur
   la bibliotheque de manipulation d'image eLiSe. Il est distibue sous la
   licences Cecill-B.  Voir en bas de fichier et  http://www.cecill.info.


English :

    MicMac is an open source software specialized in image matching
    for research in geographic information. MicMac is built on the
    eLiSe image library. MicMac is governed by the  "Cecill-B licence".
    See below and http://www.cecill.info.

Header-MicMac-eLiSe-25/06/2007*/


#ifndef _ExternNewRechPH_H_
#define _ExternNewRechPH_H_

#include "../../../include/StdAfx.h"

std::string NameFileNewPCarac(const std::string & aNameGlob,bool Bin,const std::string & anExt);
void ShowPt(const cOnePCarac & aPC,const ElSimilitude & aSim,Video_Win * aW,bool HighLight);
cSetPCarac * LoadStdSetCarac(const std::string & aNameIm,const std::string & Ext="Std");

void TestMatchInvRad(const std::vector<cOnePCarac> & aVH,const cOnePCarac * aHom1,const cOnePCarac * aHom2);


class cAppli_NewRechPH;


typedef float   tElNewRechPH ;
typedef double  tElBufNRPH ;
typedef Im2D<tElNewRechPH,tElBufNRPH>  tImNRPH;
typedef TIm2D<tElNewRechPH,tElBufNRPH> tTImNRPH;
typedef cInterpolateurIm2D<tElNewRechPH>  tInterpolNRPH;



double Gauss(double aSig,double aVal);
double Sigma2FromFactExp(double a);
double FactExpFromSigma2(double aS2);
void TestSigma2(double a);
template <class T1> void  LocFilterGauss(T1 & anIm, double aSigmaN,int aNbIter);
void FilterGaussProgr(tImNRPH anIm,double  aSTarget,double  aSInit,int aNbIter);
void TestDist(Pt2di aSz,Fonc_Num aP,double aScale);

int SignOfType(eTypePtRemark aKind);







// ====================== Fonctions-classes qui pourraient etre exportees car d'interet ===================
// ====================== general

/*
    Calcul le graphe, sous forme de flag, des pixel superieurs;
   Pourra etre utilise pour tabuler rapidement les max,min cols ...
*/
template<class T1,class T2> Im2D_U_INT1 MakeFlagMontant(Im2D<T1,T2> anIm)
{
    Pt2di aSz = anIm.sz();
    Im2D_U_INT1 aRes(aSz.x,aSz.y);
    TIm2D<U_INT1,INT> aTRes(aRes);
    TIm2D<T1,T2> aTIm(anIm);

    Pt2di aP;
    for (aP.x=1 ; aP.x<aSz.x-1 ; aP.x++)
    {
        for (aP.y=1 ; aP.y<aSz.y-1 ; aP.y++)
        {
            T1 aV1 = aTIm.get(aP);
            int aFlag=0;
            for (int aKV=0 ; aKV<8 ; aKV++)
            {
                 Pt2di aV = TAB_8_NEIGH[aKV];
                 T1 aV2 = aTIm.get(aP+aV);
                 // Comparaison des valeur et du voisinage en cas
                 // d'egalite pour avoir une relation d'ordre stricte
                 if (CmpValAndDec(aV1,aV2,aV)==-1)
                 {
                    aFlag |= (1<<aKV);
                 }
            }
            aTRes.oset(aP,aFlag);
        }
    }

    return aRes;
}

/*
    eTPR_Corner  = 2,
    eTPR_MaxLapl = 3,
    eTPR_MinLapl = 4,
    eTPR_NoLabel = 5
*/

Pt3di Ply_CoulOfType(eTypePtRemark,int aN0,int aLong);
Pt3dr X11_CoulOfType(eTypePtRemark);

void ShowPt(const cOnePCarac & aPC,const ElSimilitude &,Video_Win * aW);



int  * TabTypePOfFlag();

// Stucture de points remarquables
class cPtRemark;
typedef std::list<cPtRemark*> tContHRPR;

class cPtRemark
{
    public :
       cPtRemark(const Pt2dr & aPt,eTypePtRemark aType,int aNiv) ;

       const Pt2dr & Pt() const          {return mPtR;}
       const eTypePtRemark & Type() const {return mType;}
       
       void MakeLink(cPtRemark * aHR /*Higher Resol */);
       tContHRPR &  HRs()  {return mHRs;}
       cPtRemark * LR()    {return mLR;}
       int   Niv() const   {return mNiv;}

       void  RecGetAllPt(std::vector<cPtRemark *> &);
    private :

       cPtRemark(const cPtRemark &); // N.I.
       Pt2dr           mPtR;
       eTypePtRemark   mType;
       tContHRPR         mHRs; // Higher Resol
       cPtRemark     *   mLR; // Lower Resol
       int               mNiv;
};


class cStatBrin
{
   public :
      int mNbMult;
      int mNbNivMin;
};

// Stucture de brins , suite de points sans embranchement
class cBrinPtRemark
{
    public :
        cBrinPtRemark(cPtRemark * aP0,cAppli_NewRechPH &);
        // void StatBr(int & );
        std::vector<cPtRemark *> GetAllPt();
        bool    Ok() const {return mOk;}
        double  Scale() const {return  mScale;}
        double  ScaleStab() const {return  mScaleStab;}
        int     NivScal() const {return mNivScal;}
        double  LaplMax() const {return mLaplMax;}
    private :
        cPtRemark * mLR;
        int         mNiv0;
        bool        mOk;
        int         mNivScal;
        double      mScale;
        double      mScaleStab;
        double      mLaplMax;
};

typedef cPtRemark * tPtrPtRemark;

// std::vector<Pt2di> SortedVoisinDisk(double aDistMin,double aDistMax,bool Sort);


#endif //  _ExternNewRechPH_H_


/*Footer-MicMac-eLiSe-25/06/2007

Ce logiciel est un programme informatique servant à la mise en
correspondances d'images pour la reconstruction du relief.

Ce logiciel est régi par la licence CeCILL-B soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL-B telle que diffusée par le CEA, le CNRS et l'INRIA 
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant 
donné sa spécificité de logiciel libre, qui peut le rendre complexe à 
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement, 
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité. 

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez 
pris connaissance de la licence CeCILL-B, et que vous en avez accepté les
termes.
aooter-MicMac-eLiSe-25/06/2007*/
