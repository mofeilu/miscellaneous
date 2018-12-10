%module(directors="1") pOpt

%{
  #include "Message.h"
  #include "DOE.h"
  #include "PopObjBase.h"
  #include "OptimTarget.h"
  #include "VecFunctor.h"
  #include "TargetPython.h"
  #include "TargetInterface.h"
  #include "RSMBase.h"
  #include "CutHDMR.h"
  #include "Solver.h"
  #include "Optimizer.h"
  #include "TargetFuncParser.h"
  using namespace OPTIM;
%}

%ignore targetFunc1;

%feature("director") TargetPython; // to be inherited
%feature("director") OptimTarget;  // OptimTarget is the base of TargetPython, we need that one.
%feature("director") VecFunctor;   // to be inherited
%include "std_vector.i"
%include "std_string.i"
%include "std_map.i"


/*  in swig, vector is converted to list,
vector of vector is converted to tuple of tuple*/
namespace std {
 %template(vector_i) vector<int>;
 %template(vector_d) vector<double>;
 %template(vector_s) vector<string>;
 %template(vecvec_d) vector<vector<double> > ;
 %template(map_str_d) map<string,double>;
}


%include "Message.h"
%include "DOE.h"
%include "PopObjBase.h"
%include "OptimTarget.h"
%include "VecFunctor.h"
%include "TargetPython.h"
%include "TargetInterface.h"
%include "RSMBase.h"
%include "CutHDMR.h"
%include "Solver.h"
%include "Optimizer.h"
%include "TargetFuncParser.h"
