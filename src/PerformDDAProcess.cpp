#include "PerformDDAProcess.h"

// this function is used to run everything with one or more cores
List PerformDDA_main(NumericMatrix pm, 
                    NumericVector scant1,
                    NumericVector scant2,
                    List scanms1,
                    List scanms2,
                    NumericMatrix prec_mzs,
                    double win_size,
                    double ppm1,
                    double ppm2,
                    double sn,
                    double filt,
                    double intensity_thresh,
                    int ionmode,
                    std::string db_path,
                    bool decoOn,
                    bool useEntropy,
                    bool showOutput,
                    int thread_num,
                    string file_nm) {
  if(showOutput){
    cout << "Running into PerformDDA_main <----==== " << endl;
  }
  ddaprocess runObj;
  runObj.setDDA_Arguments(pm, scant1, scant2, scanms1, scanms2, prec_mzs, 
                          win_size, ppm1, ppm2, sn, filt, showOutput, thread_num, file_nm, useEntropy,
                          0, intensity_thresh, ionmode, db_path);
  runObj.PerformDDAProcess_core(decoOn, showOutput);
  List resList = runObj.getResultsList();
  List res;
  
  // if no results, return an empty list
  
  if(resList.size() == 0){
    List rs, ri;
    IntegerVector intf;
    res = List::create(Named("Spectra") = rs, 
                       Named("Indicator") = ri, 
                       Named("FeatureIdx") = intf);
    return res;
  }
  
  IntegerVector intFtvec = runObj.getFeatureIdxVec();
  
  if(pm.ncol()>1){
    // if peak matrix is from users' input, no need to return it
    res = List::create(Named("Spectra") = resList["Spectra"], 
                       Named("Indicator") = resList["Indicator"],  
                       Named("FeatureIdx") = intFtvec);
  } else {
    // if peak matrix is generated by this algorithm, return it 
    pm = runObj.get_peak_matrix();
    res = List::create(Named("Spectra") = resList["Spectra"], 
                       Named("Indicator") = resList["Indicator"],  
                       Named("FeatureIdx") = intFtvec,
                       Named("Peak_matrix") = pm);
  }
  
  return res;
}

