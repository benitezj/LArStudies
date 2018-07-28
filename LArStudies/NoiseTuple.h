#ifndef LArStudies_NoiseTuple_H
#define LArStudies_NoiseTuple_H
#include <iostream>
#include <vector>
using namespace std;

class TString;
class TTree;

namespace LArSamples {
 
  class RunData;
  class EventData;
  class HistoryContainer;

  class NoiseTuple { 
  public:

    NoiseTuple(std::vector<std::string> input);
    NoiseTuple(){};
    ~NoiseTuple();

    void setTriggerPaths(std::vector<std::string> paths){ m_triggerPaths = paths;}
    void setDebugLevel(int level){m_debugLevel=level;}
    void setMaxCells(int max){m_maxCells=max;}
    void run(TString outpath);

  private:
    int m_debugLevel;
    std::vector< std::string > m_input;
    unsigned int m_maxCells;
    
    const HistoryContainer * CELL;
    const EventData * EVENT;
    const RunData * RUN;

    TTree * CELLTREE;
    TTree * EVENTTREE;
    TTree * RUNTREE;

    TTree * makeTree(TString name);
    int    det_;
    int    side_;
    int    FT_;
    int    slot_;
    int    channel_;
    int    layer_;

    int    run_;
    int    event_;
    int    lb_;

    int    nSamples_;
    int    sample0_;
    float   pedestal_;
    float   pedestalRMS_;
    float    energy_;
    float    time_;
    float    eta_;
    float    phi_;
    
    //Triggger 
    std::vector< std::string > m_triggerPaths;
    std::vector< std::pair<int,int> > m_triggerEvents;
    bool fillTrigger();
    bool checkTrigger(int run, int event);
  };
}

#endif
