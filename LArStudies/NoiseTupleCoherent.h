#ifndef LArStudies_NoiseTupleCoherent_H
#define LArStudies_NoiseTupleCoherent_H
#include <iostream>
#include <vector>
#include <TString.h>

#define NDET 5
#define NSIDE 2
#define NFT 32
#define NFEB 15 
#define NCHAN 128

using namespace std;

class TTree;
class TChain;

namespace LArSamples {
 
  class RunData;
  class EventData;
  class HistoryContainer;

  class NoiseTupleCoherent { 
  public:

    NoiseTupleCoherent(TString input);
    NoiseTupleCoherent(){};
    ~NoiseTupleCoherent();

    void setDebugLevel(int level){m_debugLevel=level;}
    void setMaxCells(int max){m_maxCells=max;}
    void run(TString outpath);

  private:
    int m_debugLevel;
    TString m_input;
    unsigned int m_maxCells;

    TChain * InputTree[NDET][NSIDE][NFT];
    void setBranches(TTree*T);
    
    int    slot_;
    int    channel_;
    //int    layer_;
    int    run_;
    int    event_;
    int    lb_;
    int    nSamples_;
    int    sample0_;
    float   pedestal_;

  };
}

#endif
