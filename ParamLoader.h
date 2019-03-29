#ifndef PARAMLOADER_H
#define PARAMLOADER_H

#include "constants.h"
#include "misc/iniFileLoaderBase.h"
#include "misc/utils.h"



#define Prm (*ParamLoader::Instance())


class ParamLoader : public iniFileLoaderBase
{
private:
  
    ParamLoader():iniFileLoaderBase("parametri.ini")
  {
	NumSemaf                = GetNextParameterInt();
    NumVeicoli              = GetNextParameterInt();
    //NumObstacles            = GetNextParameterInt();
    //MinObstacleRadius       = GetNextParameterFloat();
   // MaxObstacleRadius       = GetNextParameterFloat();
   //NumCannon               = GetNextParameterInt();
    NumCellsX               = GetNextParameterInt();
    NumCellsY               = GetNextParameterInt();

    NumCicliPerAddolcire  =   GetNextParameterInt();

    MoltiplicatoreForzaMovimento    = GetNextParameterFloat();
    MaxSteeringForce        = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    VelocitaMassima         = GetNextParameterFloat();
    MassaVeicolo            = GetNextParameterFloat();
    ScalaVeicolo            = GetNextParameterFloat();

    PesoSeparazione         = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoAllineamento        = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoCoesione            = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoEvitareOstacoli     = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoEvitareMuri         = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoVagabondare         = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoRicerca             = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoEvasione            = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoArrivo              = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoInseguimento        = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    OffsetPesoInseguimento  = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoInterposizione      = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoNascondere          = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoEvadere             = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;
    PesoSeguirePath         = GetNextParameterFloat() * MoltiplicatoreForzaMovimento ;

    DistanzaVisuale         = GetNextParameterFloat();
    MinLunghezzaBoxDetenzione  = GetNextParameterFloat();
    LunghezzaAntennaRiconoscimentoMuri = GetNextParameterFloat();

    prEvitareMuri           = GetNextParameterFloat();
    prEvitareOstacoli       = GetNextParameterFloat();  
    prSeparazione           = GetNextParameterFloat();
    prAllineamento          = GetNextParameterFloat();
    prCoesione              = GetNextParameterFloat();
    prVagabondaggio         = GetNextParameterFloat();
    prRicerca               = GetNextParameterFloat();
    prNascondere            = GetNextParameterFloat();
    prEvasione              = GetNextParameterFloat();
    prAllontanarsi          = GetNextParameterFloat();
    prArrivo                = GetNextParameterFloat();

    MaxTurnRatePerSecond    = Pi;
  }

public:

  static ParamLoader* Instance();


  int   NumSemaf;
  int	NumVeicoli;
  //int   NumCannon;
  //int	NumObstacles;
 // double MinObstacleRadius;
 // double MaxObstacleRadius;

  //number of horizontal cells used for spatial partitioning
  int   NumCellsX;
  //number of vertical cells used for spatial partitioning
  int   NumCellsY;

  //how many samples the smoother will use to average a value
  int   NumCicliPerAddolcire;

  //used to tweak the combined steering force (simply altering the MaxSteeringForce
  //will NOT work!This tweaker affects all the steering force multipliers
  //too).
  double MoltiplicatoreForzaMovimento ;

  double MaxSteeringForce;
  double VelocitaMassima ;
  double MassaVeicolo;

  double ScalaVeicolo;
  double MaxTurnRatePerSecond;

  double PesoSeparazione;
  double PesoAllineamento;
  double PesoCoesione;
  double PesoEvitareOstacoli;
  double PesoEvitareMuri;
  double PesoVagabondare;
  double PesoRicerca;
  double PesoEvasione;
  double PesoArrivo;
  double PesoInseguimento;
  double OffsetPesoInseguimento;
  double PesoInterposizione;
  double PesoNascondere;
  double PesoEvadere;
  double PesoSeguirePath;

  //how close a neighbour must be before an agent perceives it (considers it
  //to be within its neighborhood)
  double DistanzaVisuale;

  //used in obstacle avoidance
  double MinLunghezzaBoxDetenzione;

  //used in wall avoidance
  double LunghezzaAntennaRiconoscimentoMuri;

  //these are the probabilities that a steering behavior will be used
  //when the prioritized dither calculate method is used
  double prEvitareMuri ;
  double prEvitareOstacoli;
  double prSeparazione;
  double prAllineamento;
  double prCoesione;
  double prVagabondaggio;
  double prRicerca ;
  double prNascondere;
  double prEvasione;
  double prAllontanarsi;
  double prArrivo;
  
};





#endif
  