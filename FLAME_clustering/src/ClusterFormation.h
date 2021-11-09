
/*! \class 		ClusterFormation
 *  \brief 		Header file for ClusterFormation
 *  \author    	Roma Bugiel
 *  \date      	November 2020
 *  \pre       	ROOT installation, shared libraries of plane_str
 *  \warning   	Exeception handling not added
 *	\details	Performs clusterization procedure, produces TTree with clusters
 *  \todo 		
 */


#ifndef CLUSTERFORMATION_h
#define CLUSTERFORMATION_h

#include <bitset>
#include "plane_str.h"
#include "globals.h"
#include "cout_msg.h"
#include "data_structures.h"


#include <TString.h>
#include <TH2.h>
#include <TH1.h>
#include <TChain.h>
#include "TH3.h"
#include "TString.h"
#include "TFile.h"
#include "TCanvas.h"

class ClusterFormation 
{
    public:
	
	/*!  \brief Default contructor */
	ClusterFormation() 
	{
		gROOT->ProcessLine("#include <vector>");
		
		 // --- Create dictionary with the output file objects collection
		#if !defined(__CINT__)
		if (!(gInterpreter->IsLoaded("./src/data_structures")))
		gInterpreter->ProcessLine("#include \"./src/data_structures.h\"");
		gInterpreter->GenerateDictionary("Pad", "./src/data_structures.h");	
		gInterpreter->GenerateDictionary("std::vector<Pad>", "./src/data_structures.h");	
		gInterpreter->GenerateDictionary("Cluster", "./src/data_structures.h");	
		gInterpreter->GenerateDictionary("std::vector<Cluster>", "./src/data_structures.h");	

		#endif /* !defined(__CINT__) */
		
		#ifdef __MAKECINT__
		
		#pragma link C++ class Pad+;  
		#pragma link C++ class std::vector<Pad>+; 		
		#pragma link C++ class Cluster+;  
		#pragma link C++ class std::vector<Cluster>+;  

		#endif
		
	};
   
   
   	/*!  Overloaded stream insertion operator. 
	 *   Lists all class members and theris current values. */
	friend std::ostream &operator<<(std::ostream &os, const ClusterFormation &clstrform);

	//Methods
   /*!  \brief Method that finds a cluster in event
    *   \param[in] hit map with the event
    * 	\return number of found clusters
    * 	 */
	std::vector< Cluster >	find_clusters(TH2D*);
    
    
    /*!  \brief Method that executes find_cluster, ...
    * 	 \param[in] h2_hit_map -- hit map with frame
    * 	 \param[in] event_nb -- event number
    * 	 \param[in] plane_nb -- plane number
    * 	 \param[in] time_plane -- time plane
    * 	 \param[in] TLU number -- time stamp 
    * 	 \param[in] TLU extended -- extended TLU number
    */
	void 	process_event(TH2D *h2_hit_map, int event_nb = 0, int plane_nb = 0, int time_plane = 0, int TLU_number = 0, int extended_TLU_number =0);
   	
   	/*!  \brief Method that executes find_cluster, ...
    * 	 \param[in] h2_hit_map -- array of hit maps with frame. [0] is for FLAME plane = 0, and so on
    * 	 \param[in] event_nb -- event number
    * 	 \param[in] time_plane -- time plane
    * 	 \param[in] TLU number -- time stamp 
    * 	 \param[in] TLU extended -- extended TLU number
    */
   	void 	process_event(TH2D *h2_hit_map[3], int event_nb, int time_plane, int TLU_number, int extended_TLU_number);

   	

	/*! \brief Mainly initialized class members
	 * 	\details Takes values from config_file.cfg to intialize class variables. The name of variables in config file and
	 * class variables are the same for simpilicity. */
	void 	init(const char * config_file);

	/*! \brief Opens ROOT tree with output results.
	 * 	\details The path to the output tree is given in config_file.cfg as #_output_data_path
	 *  The name of the output tree is constructed as follows:
	 * 
	 * "cluster" + _output_file_name_core + _ + run + _output_postfix + ".root"
	 * 	
	 * Output tree name is #_output_tree_name.
	 *  \return 1 if succesfull, 0 if failed to open ROOT tree
	 * */
	int	 	open_output_tree();
	
	/*! \brief Initializes the pointers to output histograms
	 * 	\details The list of output histos: 
	 * */
	void 	init_histo();
		
	/*! \brief Write and closes output .root tree.
	 * */
	int 	close_output_tree();
	
	/*! \brief Clear branches saved to tree
	 * */	
	void 	clear_event();
		
	/*! \brief Call tree closing and whatever needed for termination of clustering procedure
	 * */
	int 	terminate();
	
	
	//Helper functions
	/*! \brief sums all entries of 2-dim histo
	 * */
	int histo_cumulative(TH2D*);

	/*! \brief print_cluster
	 * */
	void print_cluster(std::vector<Cluster> vcl);

	/*! \brief returns channel for particular bin in hit map
	 *  \return channel number corresponding to coordinates [bin_x, bin_y] given as bin number
	 * */
	int get_seed_channel(int bin_x, int bin_y);    
	
	
	std::vector< std::vector<int> > _v_cluster_list;
    std::vector< int > 				_v_cluster_energies;
	TString 						_output_full_name;

	//Branch definition
	int _event_nb;
	int _TLU_number;
	int _extended_TLU_number;
	int _plane_nb;
	int _time_plane;
	
	/*std::vector<float> 		_cl_x_seed;
   	std::vector<float> 		_cl_y_seed;
   	std::vector<float> 		_cl_seed_energy;
	std::vector<float> 		_cl_pad_above_th;
	std::vector<float> 		_cl_x_pos;
   	std::vector<float> 		_cl_y_pos;
	std::vector<float> 		_cl_energy;
  	std::vector<int> 		_cl_size;
   	std::vector<int> 		_cl_x_size;
   	std::vector<int> 		_cl_y_size;
	*/

	std::vector< Cluster > 	_clusters_on_plane[3];

	
	TH2D*	_h2_event;
    TH2D*	_h2_cluster_map[3];
    TH1D*	_h_cluster_size[3];
	TH1D*	_h_cluster_energy[3];	
	TH1D*	_h_cluster_seed_energy[3];
	TH1D*	_h_pads_above_threshold[3];
	TH2D*	_h2_energy_ypos[3];		

	TH2D* 	_channel_map;
	int 	_empty_events{0};

	//! Default constructor
    ~ClusterFormation(){ };


	private:
		
		int _cluster_ene_down_cut{0};	//!< Down cut for energy of the cluster (below this energy taken as a noise}
		int _pad_above_threshold{1024}; //!< Countes pads with energy above this threshold
		
		int _is_chain;
		int _run;
		int _run_start;			//!< First run taken to TChain (taken to analysis)
		int _run_end;			//!<a_channel_signal Last run taken to TChain (taken to analysis)
		int _event_to_plot;
		
		std::string _output_data_path		{""};
		std::string _output_file_name_core  {""};
		std::string _output_clstr_tree_name	{""};
		std::string _output_postfix			{""};	

 		TEnv 	config;
		TFile 	*_output_data_file;
		TTree 	*_out_tree;
		
		int 	_nb_of_planes{3}; 				/*!< \brief Number of used planes*/
		int		_nb_of_channels{128};			/*!< \brief Number of channels per single plane*/	
		int		_cluster_radius_y{0};			/*!< \brief This is the range in Y [pads] of area for cluster formation*/		
};



#endif
