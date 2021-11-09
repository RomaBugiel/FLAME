#include "ClusterFormation.h"


std::ostream &operator<<(std::ostream &os, const ClusterFormation &clstrform) 
{
	os << std::setw(30) << "TreeOnFireReader runs with: " 	<< std::endl;
	os << std::setw(30) << "_output_data_path: "			<< clstrform._output_data_path					<< std::endl;	
	os << std::setw(30) << "_output_file_name_core: " 		<< clstrform._output_file_name_core				<< std::endl;	
	os << std::setw(30) << "_output_clstr_tree_name: "		<< clstrform._output_clstr_tree_name			<< std::endl;				
	os << std::setw(30) << "_output_postfix: " 				<< clstrform._output_postfix					<< std::endl;					
	os << std::setw(30) << "_run: " 						<< clstrform._run								<< std::endl;
	os << std::setw(30) << "_is_chain: " 					<< clstrform._is_chain							<< std::endl;
	os << std::setw(30) << "_run_start: " 	 				<< clstrform._run_start							<< std::endl;
	os << std::setw(30) << "_run_end: "	 					<< clstrform._run_end							<< std::endl;
	os << std::setw(30) << "_nb_of_planes : "  				<< clstrform._nb_of_planes						<< std::endl;
	os << std::setw(30) << "_nb_of_channels : "  			<< clstrform._nb_of_channels					<< std::endl;
	os << std::setw(30) << "_cluster_ene_down_cut: "  		<< clstrform._cluster_ene_down_cut				<< std::endl;
	os << std::setw(30) << "_cluster_radius_y: " 			<< clstrform._cluster_radius_y					<< std::endl;
	os << std::setw(30) << "_pad_above_threshold: " 		<< clstrform._cluster_radius_y					<< std::endl;
	os << std::setw(30) << "_event_to_plot: " 				<< clstrform._event_to_plot					<< std::endl;

	return os;                          
}


/*
 * Initializes class members taking the values from the file
 */

void ClusterFormation::init(const char * config_file)
{
	MSG(CNTR, "[CF] Init running..." );
	
	if(config.ReadFile(config_file, kEnvUser) < 0)
	{ 
		MSG(ERR, "[CF] Config file not loaded --> exit." );
		exit(0);
	}
	else
	{   
	
		_is_chain					=	config.GetValue("_is_chain",0);
		_run						=	config.GetValue("_run", 999999);
		_output_data_path			=	config.GetValue("_output_data_path", "./outputData");
		_output_file_name_core		=	config.GetValue("_output_file_name_core", "out_TB_FIRE_");
		_output_clstr_tree_name		=	config.GetValue("_output_clstr_tree_name", "flame_clstr");
		_output_postfix				=	config.GetValue("_output_postfix", "");  
		_run_start 					=	config.GetValue("_run_start",0);	
		_run_end 					=	config.GetValue("_run_end",0);	
		_run_end 					=	config.GetValue("_run_end",0);	
		_nb_of_planes				=	config.GetValue("NB_OF_PLANES", 3);  
		_nb_of_channels				=	config.GetValue("NB_OF_CHANNELS", 128);  
		_cluster_ene_down_cut		=	config.GetValue("_cluster_ene_down_cut", 0);  
		_cluster_radius_y			=	config.GetValue("_cluster_radius_y", 0);  
		_pad_above_threshold		=	config.GetValue("_pad_above_threshold",1024);  
		_event_to_plot 				=	config.GetValue("_event_to_plot",1000);

	}
	
	if(_is_chain==0)
	{
		_output_full_name =	_output_data_path 			+ "/cluster_" +	
						_output_file_name_core 			+ "_" + 
						"run_" + std::to_string(_run) 	+  
						_output_postfix 				;
	}
	else
	{
		_output_full_name =	_output_data_path 			+ "/cluster_" +	
						_output_file_name_core 			+ "_" + 
						"chain_" + std::to_string(_run_start) 	+ "_" +   std::to_string(_run_end)+
						_output_postfix 				;	
	
	}
	
	open_output_tree();
	init_histo();
}

/*
 * initis histograms
 */

void ClusterFormation::init_histo()
{
	
	for(int i = 0; i < _nb_of_planes; i++) 
	{	
		_h2_cluster_map[i]  = new TH2D("_h2_cluster_map["+TString::Itoa(i,10)+"]", "Cluster seed map of plane "+TString::Itoa(i,10)+";x;y;signal", 4,0,4,64,0,64);
		_h_cluster_size[i]	= new TH1D("_h_cluster_size["+TString::Itoa(i,10)+"]", "Cluster size per plane "+TString::Itoa(i,10)+";cluster size; entries", 20,0,20);
		_h_cluster_energy[i]		= new TH1D("_h_cluster_energy["+TString::Itoa(i,10)+"]", "Cluster energy per plane "+TString::Itoa(i,10)+";cluster energy; entries", 900,0,900);	
		_h_cluster_seed_energy[i]	= new TH1D("_h_cluster_seed_energy["+TString::Itoa(i,10)+"]", "Cluster seed energy per plane "+TString::Itoa(i,10)+";cluster seed energy; entries", 900,0,900);	
		_h_pads_above_threshold[i] 	= new TH1D("_h_pads_above_threshold["+TString::Itoa(i,10)+"]", "Pads with signal above " + TString::Itoa(_pad_above_threshold,10) + " per plane: " +TString::Itoa(i,10)+";nb of pads above th; entries", 20,0,20);
		_h2_energy_ypos[i]			= new TH2D("_h2_energy_ypos["+TString::Itoa(i,10)+"]", "Correlation between energy and position in y "+TString::Itoa(i,10)+";energy; y", 400,0,900,64,0,64);
	}
	

	
}

/*
 * Opens output tree that will store the histos
 */

int ClusterFormation::open_output_tree()
{	
	_output_data_file	=	TFile::Open(_output_full_name+ ".root" , "recreate");
		
	if (!_output_data_file) 
	{ 	
		MSG(ERR, "[CF] Failed to open output tree --> exit." );
		exit(0);
	}
	else
	{	
		_output_data_file	->	cd();
		_out_tree 			= new TTree((TString)_output_clstr_tree_name, (TString)_output_clstr_tree_name);
		MSG(INFO, "[CF] Output file open: " + _output_full_name + ".root with TTree: " + _output_clstr_tree_name);
		
		
	//Add branches  
	
	_out_tree->Branch("_event_nb"  , 			&_event_nb, 				"_event_nb/I");
	_out_tree->Branch("_TLU_number", 			&_TLU_number, 				"_TLU_number/I");
	_out_tree->Branch("_extended_TLU_number", 	&_extended_TLU_number, 		"_extended_TLU_number/I");
	
	



	for(int iPlane = 0; iPlane <  _nb_of_planes; iPlane++ )
	{
		_out_tree->Branch("_clusters_on_plane_"+TString::Itoa(iPlane,10)+"", &((_clusters_on_plane[iPlane])));
		
	}

	return 1;
	}
	
	
}


/*
 * Saves histos in the output tree and closes it
 */		
int ClusterFormation::close_output_tree()
{	
	_output_data_file	->	cd();

	_output_data_file  ->	Write();
	_output_data_file  ->	Close();
	
	return 1;
}



/*
 * process clusterization and saving the event to tree
 */		
void ClusterFormation::process_event(TH2D *h2_hit_map, int event_nb, int plane_nb, int time_plane, int TLU_number, int extended_TLU_number)
{	
	//Save histo to tree
	std::vector< Cluster > v_clusters = find_clusters(h2_hit_map);
	
	if(v_clusters.size()>0) // Do not save empty frames
	{
		_event_nb 				= event_nb;
		_TLU_number 			= TLU_number;
		_extended_TLU_number	= extended_TLU_number;	
				
		//_clusters_on_plane[plane_nb] =  v_clusters;
	}
	
	
	_out_tree	->	Fill();
	clear_event();	
}

/*
 * process_event taking info for all planes
 */	
 
void ClusterFormation::process_event(TH2D *h2_hit_map[3], int event_nb, int time_plane, int TLU_number, int extended_TLU_number)
{	
	int control_sum = 0;

	for(int i = 0; i < _nb_of_planes; i++) 
	{		
		_event_nb 				= event_nb;
		_TLU_number 			= TLU_number;
		_extended_TLU_number	= extended_TLU_number;	
	
		_clusters_on_plane[i] = find_clusters(h2_hit_map[i]);
		
		//Control sum for not adding an empty events			
		//control_sum = control_sum + _clusters_on_plane[i].size();
				
		//Control plots -- probably should not be there in the future
		for(unsigned int c = 0;  c < _clusters_on_plane[i].size(); c++) 
		{
			if(_clusters_on_plane[i].size()==0) break;
			//MSG(INFO, "_clusters_on_plane[i].size() " << _clusters_on_plane[i].size() << " _clusters_on_plane[i][c].cluster_size : " << _clusters_on_plane[i][c].cluster_size );

			_h2_cluster_map[i] 			-> Fill ( _clusters_on_plane[i][c].x_pos, _clusters_on_plane[i][c].y_pos );
			_h_cluster_size[i] 			-> Fill ( _clusters_on_plane[i][c].cluster_size );
			_h_cluster_energy[i]		-> Fill ( _clusters_on_plane[i][c].energy );
			_h_cluster_seed_energy[i]	-> Fill ( _clusters_on_plane[i][c].seed_energy );
			_h_pads_above_threshold[i] 	-> Fill ( _clusters_on_plane[i][c].pad_ab_th );
			_h2_energy_ypos[i]			-> Fill ( _clusters_on_plane[i][c].energy, _clusters_on_plane[i][c].y_pos);
		}
		// ----		
				
		if(event_nb == _event_to_plot) 
		{
			MSG(INFO, "[CF] Event: " << event_nb << " plane: " << i);
			print_cluster (_clusters_on_plane[i]);
		}		
	}
	
	/*
	if(control_sum==0)
	{
		_empty_events++;
	}
	else
	{	
		_out_tree	->	Fill();
	}
	*/
	
	_out_tree	->	Fill();	
	clear_event();

	

}




/*
 * terminate clustering process
 */		

int ClusterFormation::terminate()
{	
	MSG(INFO, "[CF] Number of empty events: " << _empty_events) ;

	close_output_tree();
	return 1;
}


/*
 * clear_event
 */	
void ClusterFormation::clear_event()
{
	_event_nb 				= 0;
	_TLU_number 			= 0;
	_extended_TLU_number	= 0;
	_clusters_on_plane[0].clear();
	_clusters_on_plane[1].clear();
	_clusters_on_plane[2].clear();

}

/*
 * find clusters
 */		
std::vector< Cluster > ClusterFormation::find_clusters(TH2D *h2_hit_map)
{	
	std::vector< Cluster > v_clusters;
	Cluster cl;

	int max_bin_X;
	int max_bin_Y;
	int max_bin_Z;
	int max_bin;
		
	int _cluster_radius_y = 5;
	
	//Loop until there is non-zero bin in the histogram
	//if ( (int)(histo_cumulative(h2_hit_map)) ) 
	//{
		//Find seed (max bin)
		max_bin = h2_hit_map->GetMaximumBin();
		h2_hit_map->GetBinXYZ(max_bin, max_bin_X, max_bin_Y, max_bin_Z);

		//Add seed to cluster
		cl.x_seed = h2_hit_map->GetXaxis()->GetBinCenter( h2_hit_map->GetXaxis()->FindBin(max_bin_X) );
		cl.y_seed = h2_hit_map->GetYaxis()->GetBinCenter( h2_hit_map->GetYaxis()->FindBin(max_bin_Y) );
		
		//Filling data for integrated frame cluster
		cl.cluster_size = h2_hit_map->GetEntries();
		cl.x_pos_COG	= h2_hit_map->GetMean(1);
		cl.y_pos_COG	= h2_hit_map->GetMean(2);
		cl.energy		= histo_cumulative(h2_hit_map);
		cl.seed_energy	= h2_hit_map->GetBinContent(max_bin_X, max_bin_Y);		
		
		
		//Use Weighted position reconstruction
		TH2D *h2_hit_map_weighted = (TH2D*) h2_hit_map->Clone();
		h2_hit_map_weighted->SetName("h2_hit_map_weighted");
		for(int xBin = 1; xBin< h2_hit_map_weighted->GetXaxis()->GetNbins()+1;xBin++) {
		for(int yBin = 1; yBin< h2_hit_map_weighted->GetYaxis()->GetNbins()+1;yBin++) {
			if (h2_hit_map_weighted -> GetBinContent(xBin, yBin) )	
			{
				h2_hit_map_weighted ->SetBinContent(xBin, yBin, std::max(0., 3.5 + std::log(h2_hit_map_weighted -> GetBinContent(xBin, yBin) / cl.energy ) ));
				Pad p = {xBin-1, yBin-1, (double) h2_hit_map -> GetBinContent(xBin, yBin)};
				cl.pad.push_back( p );
			}
		}
		}	
		cl.x_pos 		= h2_hit_map_weighted->GetMean(1);
		cl.y_pos 		= h2_hit_map_weighted->GetMean(2);
		
		h2_hit_map_weighted ->Delete();
		
		
		//Print maps
		/*
		std::cout << cl.energy << std::endl ;
	
		for(int yBin = 1; yBin< h2_hit_map_weighted->GetYaxis()->GetNbins()+1;yBin++) {
			for(int xBin = 1; xBin< h2_hit_map_weighted->GetXaxis()->GetNbins()+1;xBin++) {
			
				std::cout << "\t" << h2_hit_map->GetBinContent(xBin, yBin);
			}
				std::cout << "\t :: \t" ;
	
			for(int xBin = 1; xBin< h2_hit_map_weighted->GetXaxis()->GetNbins()+1;xBin++) {
			
				std::cout << "\t" << h2_hit_map_weighted->GetBinContent(xBin, yBin);
			}
		std::cout << std::endl;

		}
		*/


		for(int x = 1; x <= 2 ; x++)
		{
			for(int y = 1 ; y <= 64 ; y++) 
			{	
				if( h2_hit_map->GetBinContent(x,y) > _pad_above_threshold ) cl.pad_ab_th++;
			}
		}
				
		v_clusters.push_back(cl);
		cl.energy = 0;
		cl.x_seed = 0;
		cl.y_seed = 0;
		cl.seed_energy = 0;
		cl.pad_ab_th = 0;
		cl.x_pos = 0;
		cl.y_pos = 0;
		cl.cluster_size =0 ;
	//}

	if(v_clusters.size() > 1) std::cout << "Number of clusters in event: "  << v_clusters.size() 	<< "\t" 	<< std::endl;


	return v_clusters;
}





/*
 * sum all bins form h
 */	
int ClusterFormation::histo_cumulative(TH2D* h)
{
	return h->Integral();
}


/*
 * sum all bins form h
 */	
void ClusterFormation::print_cluster(std::vector<Cluster> vcl)
{
	
	MSG(INFO, "\t [CF] number of clusters: " << vcl.size()) ;
	for(int i = 0; i < (int) vcl.size(); i++)
	{
		MSG(INFO, "\t [CF] energy: " 		<< vcl[i].energy );
		MSG(INFO, "\t [CF] seed_energy: " 	<< vcl[i].seed_energy );
		MSG(INFO, "\t [CF] pad_ab_th: " 	<< vcl[i].pad_ab_th );
		MSG(INFO, "\t [CF] x_seed: " 		<< vcl[i].x_seed << " \t y_seed: " << vcl[i].y_seed);
		MSG(INFO, "\t [CF] x_pos: " 		<< vcl[i].x_pos << " \t y_pos: " << vcl[i].y_pos);
	}		
}

/*
 * Returns bin number
 */
int ClusterFormation::get_seed_channel(int bin_x, int bin_y) 
{
	return 1;
}
