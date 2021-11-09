/*! \class 		TreeOnFireReader
 *  \brief 		Header file for TreeOnFireReader
 *  \author    	Szymon Bugie, Roma Bugiel
 *  \date      	March 2020
 *  \pre       	ROOT installation, shared libraries of plane_str
 *  \warning   	Exeception handling not added
 *	\details	Class for reading the input data ROOT Tree - example as provided after test-beam campaing, no major changes. main
 *  \todo 		
 */


#ifndef TREEONFIREREADERL_h
#define TREEONFIREREADERL_h

#include <bitset>
#include "plane_str.h"
#include "globals.h"
#include "cout_msg.h"
#include "data_structures.h"
#include "TBenchmark.h"


#include <TString.h>
#include <TFile.h>
#include <TH2.h>
#include <TH1.h>
#include <TChain.h>
#include "TCanvas.h"
#include "TStyle.h"
#include "TH3.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TString.h"
#include "TF1.h"
#include "ClusterFormation.h"

/*! \brief Number of planes definition, for initializing the &Plane array
 * * \todo this should be parameter taken from config file, but since the array init have to take constant value, it should be solved in other way than assigning this variable to NB_OF_PLANES*/
const int PLANES_NUMBER =3;


class TreeOnFireReader 
{
    
public:
  
    
    /*!  \brief Default contructor -- creates shared libraries */
    TreeOnFireReader()
    {
       
       // --- Create dictionary with the output file objects collection
		#if !defined(__CINT__)
		if (!(gInterpreter->IsLoaded("./src/plane_str")))
		gInterpreter->ProcessLine("#include \"./src/plane_str.h\"");
		gInterpreter->GenerateDictionary("Plane", "./src/plane_str.h");	
		gInterpreter->GenerateDictionary("TimeFrame", "./src/plane_str.h");	
		#endif /* !defined(__CINT__) */
		
		#ifdef __MAKECINT__
		#pragma link C++ class Plane+;  
		#pragma link C++ class TimeFrame+;  
		#endif
             
    }
    
   	 /*!  Overloaded stream insertion operator. 
	  *   Lists all class members and theris current values. */
	friend std::ostream &operator<<(std::ostream &os, const TreeOnFireReader &treereader);
  
	 /*! \brief Initializes the class memebers. */
    void init(const char * config_file);
    
    /*! \brief Finds out which runs has to be taken to analysis (TChain) 
     * \details Creates a run list starting from #_run_start to #_run_end with the step of #_step. In addition, adds the runs from _incl list. In the end excludes the runs from #_excl list.*/
    void find_run_list();
    
	TH1D *histo_test;
	TH1D *get_histo_test();
	
    const int width		= 1600;
	const int height 	= 600;
    int 	_nb_of_planes{PLANES_NUMBER}; 	/*!< \brief Number of used planes*/
    int		_nb_of_channels{128};			/*!< \brief Number of channels per single plane*/		
    int 	_skipEvents{0};					/*!< \brief Number of events that are skipped, starting from 0 event */
	double 	_statisticFraction{1};			/*!< \brief Fraction [0-1] of statistic that is analysed */
	int		_singleEvent{5435};				/*!< \brief Event number for which some control plots are shown */
    std::vector<int> _run_list;				/*!< \brief The list of runs to be analysed -- especially used for TChain of many input trees */
    		
	TH2D *hitMap[PLANES_NUMBER];				/*!< \brief   */
    TH2D *singleEventMap[PLANES_NUMBER];		/*!< \brief   */
    TH1D *ampSpectra[PLANES_NUMBER];			/*!< \brief   */
	TH1D *ToASpectra[PLANES_NUMBER];			/*!< \brief   */
	TH1D *tluNumber[PLANES_NUMBER];				/*!< \brief   */
	TH1D *extendedtluNumber[PLANES_NUMBER];		/*!< \brief   */
	TH1D *eventNumber[PLANES_NUMBER];			/*!< \brief   */
	TH1D *iTimePlane_signal[PLANES_NUMBER][7];		/*!< \brief   */
	
	TH1D* h_non_zero_timeplanes[PLANES_NUMBER];	
	TH1D* h_min_energy_timeplane[PLANES_NUMBER]; 
	TH1D* h_diff_timeplane[PLANES_NUMBER];
	TH2D* h_diff_channel[PLANES_NUMBER];
	TH1D* h_energy_if_the_same_pad[PLANES_NUMBER];
	TH1D* h_iTimePlane_signal_plane[PLANES_NUMBER];
	//TH2D *event_hitMap;
	
	TH3F *h_integrated_detector_hit_map;

     // ----  Branches definition ----
     
	short NB_OF_PLANES;
	short missing_planes;	
	bool has_SRS;
	int TLU_number;
	int extended_TLU_number;
	short trigg_source;
	int SRS_0;
	int SRS_1;
	int SRS_2;
	
	Plane *plane[PLANES_NUMBER] {nullptr};  
    Channels_Signal _channels_signal[PLANES_NUMBER]; 
    
    //class functions
    int  LoadTree();
    void TreeInfo();
    
    void HistoInit();
    void ProcessAndClusterize();
    void SetNoisyChanells(int iPlane, std::vector<int> noisy_channels ) {_noisy_channels_list[iPlane] = noisy_channels;} /*!< \brief Set list of noisy channels to be excluded from the clustering */
	bool IsNoisy(int channel, int iPlane); /*!< \brief Checks if given chanel on given board (plane) is noisy  \return isNoisy */

	TH2D** getHisto_hitMap();						/*!< \brief for getting a control plot for output tree  \return hitMap */
	TH2D** getHisto_singleEventMap();				/*!< \brief for getting a control plot for output tree  \return singleEventMap */
	TH1D** getHisto_ampSpectra();					/*!< \brief for getting a control plot for output tree  \return ampSpectra */
	TH1D** getHisto_ToASpectra();					/*!< \brief for getting a control plot for output tree  \return ToASpectra */
	TH1D** getHisto_tluNumber();					/*!< \brief for getting a control plot for output tree  \return tluNumber */
	TH1D** getHisto_extendedtluNumber();			/*!< \brief for getting a control plot for output tree  \return extendedtluNumber */
	TH1D** getHisto_eventNumber();					/*!< \brief for getting a control plot for output tree  \return eventNumber */
	TH1D** getHisto_iTimePlane_signal();			/*!< \brief for getting a control plot for output tree  \return iTimePlane_signal */
	TH3F*  getHisto_integrated_detector_hit_map();	/*!< \brief for getting a control plot for output tree  \return h_integrated_detector_hit_map */	
	
	
	int _run;				//!< Run number to be analysed if #_is_chain =0
	int _run_start;			//!< First run taken to TChain (taken to analysis)
	int _run_end;			//!< Last run taken to TChain (taken to analysis)
	int _step;				//!< Step of going from #_run_start to #_run_end
	std::string _incl;		//!< \brief Additonal runs taken to analysis might be declared here with whitespace separation \details If any run cannot be given by #_run_start, #_run_end and #_step can by listed here with whitespace between numbers. See #find_run_list() method. Inclusion list has low priority than exclusion list.  Leave empty if not used.
	std::string _excl;		//!< \brief Runs excluded from the analysis might be declared here with whitespace separation  \details If any run declared by #_run_start, #_run_end and #_step has to be excluded from the analysis, it can be listed here. See #find_run_list() method. Exclusion list has highier priority than inclustion list. Leave empty if not used.		

	//Helper function
	/*!< \brief  For finding the same pad reponded in two frames. 
	 * 	 \param[in] chn1 vector of shorts, corresponding to the vector of channels that had a signal for particular frame 
	 *   \param[in] chn2 vector of shorts, corresponding to the vector of channels that had a signal for second frame to be compared
	 * 	 \details Function for finding the same elemetns in two vectors. It is dedicated for comparing if the same pad responded in the timeplane = 2 and timeplane = 4. But might be used for finding the same entries for two arbitrary frames.
		 \return numer of the same elements in chn1 and ch2 */
	int 	find_events_with_same_pad_responded(std::vector<short> chn1, std::vector<short> chn2);//!< \brief For finding the same pad reponded in two events. \par  \details If any run declared by #_run_start, #_run_end and #_step has to be excluded from the analysis, it can be listed here. See #find_run_list() method. Exclusion list has highier priority than inclustion list. Leave empty if not used.		
	void	plot_full_event(int event);
	
	//! Default constructor
    ~TreeOnFireReader(){ };


private:
	TEnv config; 
    TChain *tree; /*!< \todo [HIGH PRIOR] Input data are read out as a TChain. Needs to add possibility to provide the range of runs in config. */

	TString _output_image_path		{""}; /*!< \brief path to output plots directory  */
	TString _input_data_dir			{""}; /*!< \brief path to input data directory */
	TString _input_data_name		{""}; /*!< \brief core of the input file name */
	TString _input_tree_name		{""}; /*!< \brief input tree name */
	
	int _is_chain{0}; /*!< \brief Flag for marking if analysis is made from single run or from TChain \details If =0, the analysis uses run number given in #_run simply. If = 1, it generates the list of runs using #_run_start, #_run_end, #_step, #_incl and #_excl*/	
	
	double _noise_upper_cut_global{900};
	
	double _noise_low_cut_plane_0_a1{ 6.4 };
	double _noise_low_cut_plane_1_a1{ 6.4 };
	double _noise_low_cut_plane_2_a1{ 7.6 };
	std::vector<int> _noisy_channels_list[3] = {};
};



#endif
