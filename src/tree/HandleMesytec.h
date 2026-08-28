// HandleMesytec.h

#ifndef HandleMesytec_H
#define HandleMesytec_H
#include "TMidasEvent.h"
#include "TEvent.h"
#include "IDet.h"
#include <string>

int clearDetectors();
void Calibrate_Mesytec(int run);
void HandleMesytec(TMidasEvent &event, void *ptr, int wsize, int bank, IDet *pdet, TString CalibFile, bool gUseRaw);
void HandleBOR_Mesytec(int run, int file, int time, IDet *&pdet, std::string ConfigFile);
void HandleEOR_Mesytec(int run, int time);

#endif
// end
