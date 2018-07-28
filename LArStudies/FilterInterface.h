#ifndef LArStudies_FilterInterface_H
#define LArStudies_FilterInterface_H 1

#include "GaudiKernel/IAlgTool.h"

namespace LArSamples {
  
  static const InterfaceID IID_FilterInterface("FilterInterface", 1, 0);
   
  class FilterInterface :  virtual public IAlgTool {
  public:
    virtual ~FilterInterface(){}
    static const InterfaceID& interfaceID() { return IID_FilterInterface; }
    virtual bool eventPassesFilter() const =0;
  };


} // end of namespace

#endif 
