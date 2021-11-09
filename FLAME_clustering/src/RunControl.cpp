/*! @file RunControl.cpp
 *  @brief main() 
 * 	@details Read in from config file the input data directory, file name, run number. Opens TApplication which allows to run some plots from Online Monitor. Opens TreeOnFireReader.
 *  @todo Move gStyle to other fil
 */

#include "TSystem.h"
#include "TInterpreter.h"
#include "TApplication.h"
#include "TGClient.h"
#include "TGFileDialog.h"

#include "data_structures.h"
#include "TreeOnFireReader.h"
#include "HistoPlotter.h"
#include "cout_msg.h"
#include "plane_str.h"


int main(int argc, char **argv) 
{	
	MSG(INFO, "\n\n >>>>>>> Running data RunControl for FLAME analysis <<<<<<<< \n\n");

    TApplication theApp("App", &argc, argv);
	 
	gROOT->ForceStyle();
	gStyle->SetOptStat(1);	
	gStyle->SetPalette(kCool);

	// --- Reading config data
	const char *config_file {"./config/config_file.cfg"};
	
	TEnv config;
	if(config.ReadFile("./config/config_file.cfg", kEnvUser) < 0)
	{ 	
		MSG(ERR, "Could not read configuration file in RunControl: ./config/config_file.cfg - exit");
		exit(0);
	}
	
	int	_event_to_plot =	config.GetValue("_event_to_plot",1000);
	
	
	// Check pad maps
	/*
	std::pair<int, int> coor;
	for(int i = 0; i<128 ; i++)
	{	
		coor = GetChanelCoordinates(i);
		MSG(INFO, "ch num: " << i << "\t x: " << coor.first << "\t y: " << coor.second );
	}
	*/

	// --------------------------------------------------------------------------------------------------------------------

    //Open TreeOnFireReader
	//TreeOnFireReader *reader = new TreeOnFireReader(_input_data_dir+"/"+_input_data_name+std::to_string(_run)+".root", _input_tree_name);
    TreeOnFireReader *reader = new TreeOnFireReader();
        reader 	-> init(config_file);
        reader		-> LoadTree();			
        std::cout 	<< *reader;
        reader		-> ProcessAndClusterize();
        reader		-> plot_full_event(_event_to_plot);

	HistoPlotter *plotter = new HistoPlotter(reader);
		plotter 	-> init(config_file);		
		std::cout 	<< *plotter;
		plotter -> open_output_tree();
		plotter -> copy_histos();
		//plotter -> save_png();
		plotter -> close_output_tree();
		//plotter -> fast_monitor();
		
		
	theApp.Run();	
	
	//reader->delete();
	return 0;
}

