
/*! \class 		HistoPlotter
 *  \brief 		
 *  \author    	Roma Bugiel
 *  \date      	November 2020
 *  \pre       	ROOT installation
 *  \warning   	Exeception handling not added
 *	\details	
 *  \todo 		
 */


#ifndef _HISTOPLOTTER_H_      
#define _HISTOPLOTTER_H_   


#include "globals.h"
#include "cout_msg.h"
#include "TreeOnFireReader.h"
#include "TBranch.h"
#include "TF1.h"
#include "TMath.h"
 
class HistoPlotter { 
    

	public:

		HistoPlotter()	{}; //!< Default constructor
	
		/*! \brief Overloaded constructor of HistoPlotter
		 * 	\details Takes TreeOnFireReader, mainly to copy control plots and save in the output tree
		 *  \param[in] *treeReader pointer to TreeOnFireReader object */
		HistoPlotter(TreeOnFireReader* treeReader) :
			_treeReader(treeReader)
		{};
	
		/*! \brief Overloaded stream operator to print all variable values for HistoPlotter
		*/
		friend std::ostream &operator<<(std::ostream &os, const HistoPlotter &plotter);

		//Methods
		/*! \brief Mainly initialized class members
		 * 	\details Takes values from config_file.cfg to intialize class variables. The name of variables in config file and
		 * class variables are the same for simpilicity. */
		void 	init(const char * config_file);

		/*! \brief Opens ROOT tree with output results.
		 * 	\details The path to the output tree is given in config_file.cfg as #_output_data_path
		 *  The name of the output tree is constructed as follows:
		 * 
		 * _output_file_name_core + _ + run + _output_postfix + ".root"
		 * 	
		 * Output tree name is #_output_tree_name.
		 *  \return 1 if succesfull, 0 if failed to open ROOT tree
		 * */
		int	 	open_output_tree();
		
		/*! \brief Initializes the pointers to output histograms
		 * 	\details The list of output histos: 
		 * */
		void 	init_histo();
				
		/*! \brief Clones histograms from other sources to save it in the output tree 
		 *  \details Up to now saves the histos from OfflineMonitor of TreeOnFireReader
		 * */
		void 	copy_histos();
			
		/*! \brief Write and closes output .root tree.
		 * */
		int 	close_output_tree();
		
		/*! \brief Saves the histograms as images */
		void 	save_png();

		/*! \brief fast_monitor */
		void 	fast_monitor();

		// ----- langaus
		TF1* 	langaufit(TH1D *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF);
		
		TH2D* fit_width_0 		;
		TH2D* fit_MPV_0			;
		TH2D* fit_area_0 		;
		TH2D* fit_gsigma_0 		;
								;
		TH1D* fit_spec_width_0 	;
		TH1D* fit_spec_MPV_0	;
		TH1D* fit_spec_area_0 	;
		TH1D* fit_spec_gsigma_0 ;
		
		
		
		
		
		
		TString 			_output_full_name;
		TreeOnFireReader 	*_treeReader;
		//Events_list 		_event_list;
		
		~HistoPlotter()	{};		//!< Destructor 

	private:
		
		std::string _output_data_path		{""};
		std::string _output_image_path		{""};
		std::string _output_file_name_core  {""};
		std::string _output_tree_name 		{""};
		std::string _output_postfix			{""};	

		int	_run;

	#ifndef DOXYGEN_SHOULD_SKIP_THIS
		
		TEnv 	config;
		TFile 	*_output_data_file;
		TTree 	*_out_tree;
	
		int 	_nb_of_planes{3}; 				/*!< \brief Number of used planes*/
		int		_nb_of_channels{128};			/*!< \brief Number of channels per single plane*/		
		
		int _is_chain{0}; /*!< \brief Flag for marking if analysis is made from single run or from TChain \details If =0, the analysis uses run number given in #_run simply. If = 1, it generates the list of runs using #_run_start, #_run_end, #_step, #_incl and #_excl*/	
		int _run_start;			//!< First run taken to TChain (taken to analysis)
		int _run_end;			//!< Last run taken to TChain (taken to analysis)
	
	#endif /* DOXYGEN_SHOULD_SKIP_THIS */
};

#endif
