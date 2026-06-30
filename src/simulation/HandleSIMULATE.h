// HandleSIMULATE.h
#ifndef HandleSIMULATE_H
#define HandleSIMULATE_H
#include "TChain.h"
#include "TEvent.h"
#include "IDet.h"
#include "ITdc.h"
#include "IScaler.h"
#include "PTrack.h"
#include <string>

void clearEvt();
void HandleSIMULATE(int nSim, std::string dwbaname);
void HandleBOR_SIMULATE(std::string BinPath, std::string CalibFile, std::string OutputFile, Int_t Run);
void HandleEOR_SIMULATE();
#endif
// end
