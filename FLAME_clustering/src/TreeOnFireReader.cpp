// -----------------------------------------------------------------------
// ------------------------- Compilation ---------------------------------
// `root-config --cxx` `root-config --cflags`  -O2 -W TreeOnFireReader.cpp -o offline_analysis `root-config --ldflags` `root-config --glibs`
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------



#include "TreeOnFireReader.h"

std::ostream &operator<<(std::ostream &os, const TreeOnFireReader &treereader) 
{
	os << std::setw(30) << "TreeOnFireReader runs with: " 	<< std::endl;
	os << std::setw(30) << "_nb_of_planes: " 				<< treereader._nb_of_planes					<< std::endl;	
	os << std::setw(30) << "_nb_of_channels: "				<< treereader._nb_of_channels				<< std::endl;				
	os << std::setw(30) << "_skipEvents: " 					<< treereader._skipEvents					<< std::endl;					
	os << std::setw(30) << "_statisticFraction: " 			<< treereader._statisticFraction			<< std::endl;					
	os << std::setw(30) << "_singleEvent: " 				<< treereader._singleEvent					<< std::endl;					
	os << std::setw(30) << "_output_image_path " 			<< treereader._output_image_path			<< std::endl;
	os << std::setw(30) << "_input_data_dir: " 				<< treereader._input_data_dir				<< std::endl;	
	os << std::setw(30) << "_input_data_name: " 			<< treereader._input_data_name				<< std::endl;	
	os << std::setw(30) << "_input_tree_name: " 			<< treereader._input_tree_name				<< std::endl;
	os << std::setw(30) << "_run: " 						<< treereader._run							<< std::endl;
	os << std::setw(30) << "_is_chain: " 					<< treereader._is_chain						<< std::endl;
	os << std::setw(30) << "_run_start : " 					<< treereader._run_start 					<< std::endl;
	os << std::setw(30) << "_run_end : " 					<< treereader._run_end 						<< std::endl;
	os << std::setw(30) << "_step : " 						<< treereader._step 						<< std::endl;
	os << std::setw(30) << "_incl : " 						<< treereader._incl 						<< std::endl;
	os << std::setw(30) << "_excl : " 						<< treereader._excl 						<< std::endl;
	os << std::setw(30) << "_noise_upper_cut_global : " 	<< treereader._noise_upper_cut_global 		<< std::endl;
	os << std::setw(30) << "_noise_low_cut_plane_0_a1 : " 	<< treereader._noise_low_cut_plane_0_a1 	<< std::endl;
	os << std::setw(30) << "_noise_low_cut_plane_1_a1 : " 	<< treereader._noise_low_cut_plane_1_a1 	<< std::endl;
	os << std::setw(30) << "_noise_low_cut_plane_2_a1 : " 	<< treereader._noise_low_cut_plane_2_a1 	<< std::endl;
	
	
	
	return os;                          
}

// find_run_list --------------------------------------------------------------------------------------
void TreeOnFireReader::find_run_list()
{
	if(_is_chain==0) 
	{
		_run_list.push_back(_run);
		MSG(INFO, "Single run analysed, _run_list.size : [" << _run_list.size() << "].");
	}
	else
	{

		std::vector<int> exclude_list;
		std::vector<int> include_list;
		std::vector<int>::iterator it;
		int f;
	
	
		for(int r = _run_start; r <= _run_end ; r += _step)
		{	
			_run_list.push_back(r);
		}

		//Include files 
		std::stringstream si (_incl); 
		while ((si >> f)) include_list.push_back(f);
  
		for(int i=0; i < (int)(include_list.size()); i++) 
		{	
			_run_list.push_back(include_list[i]);
		}
  
		//Exclude files 
		std::stringstream se (_excl); 
		while ((se >> f)) exclude_list.push_back(f);
		
		for(int i=0; i < (int)(exclude_list.size()); i++) 
		{	
			it =  find ( _run_list.begin(), _run_list.end(), exclude_list[i] );
			if ( it != _run_list.end() )
			{
				_run_list.erase(it);
			}
		}
	
		MSG(INFO, "Several runs analysed, _run_list.size : [" << _run_list.size() << "].");

	}
	
	std::cout << "Analysed runs: " ;
	for(auto &i : _run_list) {std::cout << i << ", ";}
	std::cout << std::endl;

}



// init --------------------------------------------------------------------------------------

void TreeOnFireReader::init(const char * config_file)
{

	TEnv config;
	if(config.ReadFile(config_file, kEnvUser) < 0)
	{ 	
		MSG(ERR, "Could not read configuration file in TreeOnFireReader ./config/config_file.cfg  -- exit");
		exit(0);
	}
			
	_nb_of_planes		=	config.GetValue("NB_OF_PLANES",3);
	_nb_of_channels		=	config.GetValue("_nb_of_channels",128);
	
	_input_data_dir		=	config.GetValue("_input_data_dir","./inputData");
	_input_data_name	=	config.GetValue("_input_data_name","TB_FIRE_");
	_run				= 	config.GetValue("_run",671);
	_input_tree_name	=	config.GetValue("_input_tree_name","TreeOnFire");
	
	_skipEvents			=	config.GetValue("_skipEvents",0);
	_statisticFraction	=	config.GetValue("_statisticFraction",1.0);
	_singleEvent		=	config.GetValue("_singleEvent",100);
	_output_image_path	=	config.GetValue("_output_image_path","./outputData/");
	_is_chain			=	config.GetValue("_is_chain",0);	
	_run_start 			=	config.GetValue("_run_start",0);	
	_run_end 			=	config.GetValue("_run_end",0);	
	_step				=	config.GetValue("_step",1);		
	_incl				=	config.GetValue("_incl","");
	_excl				=	config.GetValue("_excl","");
	
	_noise_upper_cut_global 	=	config.GetValue("_noise_upper_cut_global",900.0);
	_noise_low_cut_plane_0_a1 	=	config.GetValue("_noise_low_cut_plane_0_a1",6.4);
	_noise_low_cut_plane_1_a1 	=	config.GetValue("_noise_low_cut_plane_1_a1",6.3);
	_noise_low_cut_plane_2_a1 	=	config.GetValue("_noise_low_cut_plane_2_a1",7.6);
	
	std::string tmp_noisy_list;
	int f;
	tmp_noisy_list			=   config.GetValue("_noisy_channels_on_plane0", "");
	std::stringstream stream0 (tmp_noisy_list); 
	while ((stream0 >> f)) _noisy_channels_list[0].push_back(f);			

	tmp_noisy_list			=   config.GetValue("_noisy_channels_on_plane1", "");
	std::stringstream stream1 (tmp_noisy_list); 
	while ((stream1 >> f)) _noisy_channels_list[1].push_back(f);
		
	tmp_noisy_list			=   config.GetValue("_noisy_channels_on_plane2", "");
	std::stringstream stream2 (tmp_noisy_list); 
	while ((stream2 >> f)) _noisy_channels_list[2].push_back(f);	

	find_run_list();
	HistoInit();

}

// HistoInit ----------------------------------------------------------------------------------


void TreeOnFireReader::HistoInit() {
	
    MSG(CNTR, "[TOFR] Control plots histograms initialization...");

    for(int i = 0 ; i < _nb_of_planes ; i++) 
    {
		
		hitMap[i] 				= new TH2D("hitMap["+TString::Itoa(i,10)+"]", "Hit Map of plane "+TString::Itoa(i,10)+";x;y;signal", 4,0,4,64,0,64);
		ampSpectra[i]			= new TH1D("ampSpectra["+TString::Itoa(i,10)+"]", "Amplitude for plane: "+TString::Itoa(i,10)+";amp;entries", 1024,0,1024);
		ToASpectra[i]			= new TH1D("ToASpectra["+TString::Itoa(i,10)+"]", "ToA of plane "+TString::Itoa(i,10)+";ToA;entries", 1024,0,1024);
		singleEventMap[i] 		= new TH2D("singleEventMap["+TString::Itoa(i,10)+"]", "For plane: "+TString::Itoa(i,10)+";x;y;signal", 4,0,4,64,0,64);
		tluNumber[i]			= new TH1D("tluNumber["+TString::Itoa(i,10)+"]", "TLU number: "+TString::Itoa(i,10)+";tlu;entries", 60000,0,60000);
		extendedtluNumber[i]	= new TH1D("extendedtluNumber["+TString::Itoa(i,10)+"]", "Extended TLU: "+TString::Itoa(i,10)+";extended tlu;entries", 60000,0,60000);
		eventNumber[i]			= new TH1D("eventNumber["+TString::Itoa(i,10)+"]", "eventNumber: "+TString::Itoa(i,10)+";extended tlu;entries", 60000,0,60000);
		
		h_iTimePlane_signal_plane[i] =  new TH1D("h_iTimePlane_signal_plane["+TString::Itoa(i,10)+"]", "Nummber of timeplanes with signal in event per plane: "+TString::Itoa(i,10)+";timeplanes with signal ;entries",  7,0,7);
		
		for(int t = 0; t < 7 ; t++)
		{
			iTimePlane_signal[i][t] 	= new TH1D("iTimePlane_signal["+TString::Itoa(i,10)+"]["+TString::Itoa(t,10)+"]", "Signal on timePlane:"+ TString::Itoa(t,10)+ " for plane : "+TString::Itoa(i,10)+";time plane;entries", 1024,0,1024);
		}
		
		h_non_zero_timeplanes[i] 	= new TH1D("h_non_zero_timeplanes["+TString::Itoa(i,10)+"]","In how many timeplanes there was a non zero signal for plane: "+TString::Itoa(i,10)+";  timeplanes number ;entries",8,0,8);
		h_min_energy_timeplane[i] 	= new TH1D("h_min_energy_timeplane["+TString::Itoa(i,10)+"]","The energy spectrum of the lower signal timeplane, if more than one timeplane had signal" +TString::Itoa(i,10)+"; energy; entries",1024,0,1024);
		h_diff_timeplane[i] 		= new TH1D("h_diff_timeplane["+TString::Itoa(i,10)+"]","Difference between the lowest timeplane number and next timeplanes when signal on more than one for plane; " +TString::Itoa(i,10)+"; timeplane difference; entries",8,0,8);
		h_diff_channel[i] 			= new TH2D("h_diff_channel["+TString::Itoa(i,10)+"]","Difference between channels responded in different timeplanes when signal on more than one for plane " +TString::Itoa(i,10)+"; diff in x; diff in y",4,0,4,200,0,200);
		h_energy_if_the_same_pad[i] = new TH1D("h_energy_if_the_same_pad["+TString::Itoa(i,10)+"]","Energy of event for which the same pad responded in different timeplanes for plane" +TString::Itoa(i,10)+"; timeplane difference; entries",1024,0,1024);

		
		for(int ch = 0; ch < _nb_of_channels ; ch++) 
		{ 
			_channels_signal[i].a_channel_signal[ch] = new TH1D("channelSpectra["+ TString::Itoa(i,10)+"]["+TString::Itoa(ch,10)+"]", "Amp per plane " + TString::Itoa(i,10) + " per channel " + TString::Itoa(ch,10) + "; amp; entries", 1024,0,1024);
			_channels_signal[i].a_channel_signal[ch]->SetLineWidth(2);
			_channels_signal[i].a_channel_signal[ch]->SetFillColor(kBlue);
			_channels_signal[i].a_channel_signal[ch]->SetFillStyle(3244);
		}
	
	}
		
	h_integrated_detector_hit_map = new TH3F("h_integrated_detector_hit_map", "Integrated hit map", 3, 0, 3 , 4, 0, 4 , 64, 0, 64  );
}

	
bool TreeOnFireReader::IsNoisy(int channel, int iPlane){  
	
	 for (unsigned i=0; i < _noisy_channels_list[iPlane].size(); i++)
	 {
		 if (_noisy_channels_list[iPlane].at(i) == channel)
		 {		
			//MSG(CNTR, "[TOFR] Noisy shannel skipped: ch = " << _noisy_channels_list[iPlane].at(i) <<" at plane " <<  iPlane);
			return 1;
		 }
	 }
	return 0;
		
}

// LoadTree --------------------------------------------------------------------------------

int TreeOnFireReader::LoadTree(){
        
		MSG(CNTR, "[TOFR] Loading input tree/trees...");

        TString name;
		tree = new TChain(_input_tree_name);		
		
		for(int r = 0; r < (int) _run_list.size(); r++)
		{
			name = _input_data_dir+ "/" + _input_data_name +TString::Itoa(_run_list[r],10)+ ".root";
			TFile *f = new TFile(name);
			
			if( !f )  
			{ 
				MSG(ERR, "[TOFR] Tree: " << name << " NOT LOADED.");
				exit(0); return 0;
			}
        
			else if( f->IsZombie()) 
			{
        
				MSG(ERR, "[TOFR]  Tree: " << name << " IS ZOMBIE.");
				exit(0); return 0;
			}
			else 
			{ 
				MSG(CNTR, "[TOFR]  Tree: " << name << " will be loaded." );
			}
			
						
			tree->Add(name);
			delete f;
			
			//MSG(CNRL, "[TOFR]  File: " << _input_data_dir+ "/" + _input_data_name +"_" +TString::Itoa(r,10)+ ".root loaded.");
		}

        tree->SetBranchAddress("NB_OF_PLANES"  , &NB_OF_PLANES);
        tree->SetBranchAddress("missing_planes", &missing_planes);
        tree->SetBranchAddress("has_SRS", &has_SRS);
        tree->SetBranchAddress("TLU_number", &TLU_number);
        tree->SetBranchAddress("extended_TLU_number", &extended_TLU_number);
        tree->SetBranchAddress("trigg_source", &trigg_source );
        tree->SetBranchAddress("SRS_0", &SRS_0 );
        tree->SetBranchAddress("SRS_1", &SRS_1 );
        tree->SetBranchAddress("SRS_2", &SRS_2 );	
             
        tree->GetEntry(0);  
            
        for(int iPlane=0; iPlane < NB_OF_PLANES; iPlane++ )
        {
            tree->SetBranchAddress("Plane_"+TString::Itoa(iPlane,10), &(plane[iPlane]));
        }
	
        this -> TreeInfo();      
    return 1;
}

// TreeInfo ---------------------------------------------------------

void TreeOnFireReader::TreeInfo(){ 
     
    MSG(INFO, "[TOFR] Entires in tree: " << tree->GetEntries());
    //MSG(INFO, "[TOFR] Skipped events in the analysis: " << _skipEvents << ". " << _statisticFraction*100. << "% of the statistic will be analysed");
    MSG(INFO, "[TOFR] Number of planes: " 	<< NB_OF_PLANES );
    MSG(INFO, "[TOFR] Number of missing  planes: " << missing_planes );

}

// ------------------------------------------------------------------


void TreeOnFireReader::ProcessAndClusterize()
{
	
	 Plane *planePtr;
     TimeFrame  singleTimeFrame;
	 int fullPlane = 0;
	 std::vector<int> fullEvents; 
	 ClusterFormation *cluster_maker = new ClusterFormation();	
		cluster_maker->init("./config/config_file.cfg");
		std::cout << *cluster_maker;
		
	TBenchmark *benchmark = new TBenchmark();
	benchmark->Start("SearchTime");
	
	int flag = 0;
	int how_many = 0;
	int the_same_pad = 0;
	
	int energy_in_event[7] {0,0,0,0,0,0,0};
	std::array< std::vector<std::pair<int,int>>, 7 > channels_timeplane;
	int events_with_timeplane_one_by_one = 0;
	
	
     //Loop over events
     MSG(CNTR, "[TOFR] Offline monitor running... wait for control plots.");

     for (signed int iEvent = _skipEvents; iEvent < _statisticFraction*tree->GetEntries() ; iEvent++)
     {	
		 
		if(iEvent%10000==0) 
		std::cout << "iEvent: " << iEvent << std::endl;
		
		tree->GetEntry(iEvent);  
		tluNumber[0]->Fill(TLU_number);
		extendedtluNumber[0]->Fill(extended_TLU_number);
			
		TH2D* eventMap[3];
		for(int i = 0 ; i < _nb_of_planes ; i++) 
		{
			eventMap[i] = new TH2D("eventMap["+TString::Itoa(i,10)+"]", "eventMap "+TString::Itoa(i,10)+";x;y;signal", 4,0,4,64,0,64);
		}	
		
		// Loop over planes in event 
		for(signed int iPlane=0; iPlane < NB_OF_PLANES ; iPlane++)
		{
			//if(iPlane != 0) continue;
			
			planePtr = plane[iPlane];     

			eventNumber[iPlane]->Fill(planePtr->board_ev_number);
			(_channels_signal[iPlane]).plane_nb = iPlane;
			
			
			//Remove events with signal on the same pad on for timeplane = 2 and 4
			if( ( (planePtr->time_frame)[2].nb_of_samples != 0 ) && ( (planePtr->time_frame)[4].nb_of_samples != 0 ) ) 
			{	
				std::vector<short> v1;
				std::vector<short> v2;
				
				for(int i = 0; i < (int)(((planePtr->time_frame)[2]).ch_number).size(); i++) 
				{
					if(((planePtr->time_frame)[2].amp)[i] 				  > _noise_upper_cut_global)    continue; //Cut high energetic noise, put this cut in the config
					if(iPlane == 0 && (((planePtr->time_frame)[2].amp)[i] < _noise_low_cut_plane_0_a1)) continue; //Cut high energetic noise, put this cut in the config
					if(iPlane == 1 && (((planePtr->time_frame)[2].amp)[i] < _noise_low_cut_plane_1_a1)) continue; //Cut high energetic noise, put this cut in the config
					if(iPlane == 2 && (((planePtr->time_frame)[2].amp)[i] < _noise_low_cut_plane_2_a1)) continue; //Cut high energetic noise, put this cut in the config
					v1.push_back( (((planePtr->time_frame)[2]).ch_number)[i] );
				
				}
				
				for(int i = 0; i < (int)(((planePtr->time_frame)[4]).ch_number).size(); i++) 
				{
					if(((planePtr->time_frame)[4].amp)[i] 				  > _noise_upper_cut_global)    continue; //Cut high energetic noise, put this cut in the config
					if(iPlane == 0 && (((planePtr->time_frame)[4].amp)[i] < _noise_low_cut_plane_0_a1)) continue; //Cut high energetic noise, put this cut in the config
					if(iPlane == 1 && (((planePtr->time_frame)[4].amp)[i] < _noise_low_cut_plane_1_a1)) continue; //Cut high energetic noise, put this cut in the config
					if(iPlane == 2 && (((planePtr->time_frame)[4].amp)[i] < _noise_low_cut_plane_2_a1)) continue; //Cut high energetic noise, put this cut in the config
				
					v2.push_back( (((planePtr->time_frame)[4]).ch_number)[i] );
				}
				
				if(v1.size() != 0 && v2.size()!=0)
				{
					how_many++;
					//if( find_events_with_same_pad_responded((((planePtr->time_frame)[2]).ch_number), (((planePtr->time_frame)[4]).ch_number))) 
					if( find_events_with_same_pad_responded(v1,v2) )
					{	 
						MSG(CNTR, "[TOFR] Event: " << iEvent << " has signal on the same pad in timeplane 2 and 4");
						the_same_pad++;
						break; 
					}
				}
			}
			
			// Loop over stored timeframes
			for(unsigned int iTime = 0; iTime < (planePtr->time_frame).size(); iTime++) 
			{  
				// ------!! Cut for timeplane
				if( (iTime < 2) || (iTime > 4) ) continue; 
				singleTimeFrame = (planePtr->time_frame)[iTime];

				// Loop over inividual hits in certain timeframe					
                for(int iSample = 0; iSample < singleTimeFrame.nb_of_samples ; iSample ++) 
                {	
					//Skip noisy channels
					if( this -> IsNoisy( (singleTimeFrame.ch_number)[iSample], iPlane  ) ) continue;
					
					// ------!! Cut for noise
					if((singleTimeFrame.amp)[iSample] 				  > _noise_upper_cut_global) 	continue; //Cut high energetic noise, put this cut in the config
					if(iPlane == 0 && ((singleTimeFrame.amp)[iSample] < _noise_low_cut_plane_0_a1)) continue; //Cut high energetic noise, put this cut in the config
					if(iPlane == 1 && ((singleTimeFrame.amp)[iSample] < _noise_low_cut_plane_1_a1)) continue; //Cut high energetic noise, put this cut in the config
					if(iPlane == 2 && ((singleTimeFrame.amp)[iSample] < _noise_low_cut_plane_2_a1)) continue; //Cut high energetic noise, put this cut in the config


					// Fill some histograms, and convert channel number to X/Y position
					std::pair<int, int> pad_coordinates = GetChanelCoordinates((singleTimeFrame.ch_number)[iSample]);
					
				
					hitMap[iPlane] 			-> Fill( pad_coordinates.first , pad_coordinates.second );						
					eventMap[iPlane] 		-> Fill( pad_coordinates.first , pad_coordinates.second , (singleTimeFrame.amp)[iSample]  );
					h_integrated_detector_hit_map -> Fill(iPlane , pad_coordinates.first , pad_coordinates.second , (singleTimeFrame.amp)[iSample] );
					
										
					ampSpectra[iPlane] 	-> Fill( (singleTimeFrame.amp)[iSample] );			
					ToASpectra[iPlane] 	-> Fill( (singleTimeFrame.ToA)[iSample] );				
					
					((_channels_signal[iPlane]).a_channel_signal[((singleTimeFrame.ch_number)[iSample])])->Fill( (singleTimeFrame.amp)[iSample] );	

					if(iEvent == _singleEvent) { singleEventMap[iPlane] -> SetBinContent(pad_coordinates.first , pad_coordinates.second , (singleTimeFrame.amp)[iSample] ); }					
					
								
				} //iSample end		
																
			} //iTimeend

		} //iPlane end
		
		//MAKE CLUSTER
		cluster_maker->process_event(eventMap, iEvent, 9, TLU_number, extended_TLU_number);
		delete eventMap[0]; delete eventMap[1]; delete eventMap[2]; 

		if( fullPlane != 0 ) fullEvents.push_back(iEvent);
		fullPlane = 1;
	
	} //iEvent end
	
	MSG(INFO, "[TOFR] Number of events with singal in timeplanes in 2 and 4: " << how_many << " per events: " << tree->GetEntries());
	MSG(INFO, "[TOFR] Number of events with signal in timeplanes in 2 and 4 and the same pad: " << the_same_pad);


	TCanvas *c_nbOfSamples = new TCanvas("c_nbOfSamples", "...", width , height); 	
	
	TCanvas *c_hitMap = new TCanvas("c_hitMap", "Hit map for all planes", width , height); 	
		c_hitMap->Divide(3,1);
	TCanvas *c_singleEvent = new TCanvas("c_singleEvent", "Single event for all planes", width , height); 
		c_singleEvent->Divide(3,1);
	TCanvas *c_ampSpec = new TCanvas("c_ampSpec", "Amplitude spectrum for all planes", width , height); 
		c_ampSpec->Divide(3,1);
	TCanvas *c_ToASpec = new TCanvas("c_ToASpec", "ToA spectrum for all planes", width , height); 
		c_ToASpec->Divide(3,1);
	TCanvas *c_integrated_hitmap = new TCanvas("c_integrated_hitmap", "Hit map c1 for all planes", 800 , 800); 
	TCanvas *c_eventNumber = new TCanvas("c_eventNumber", "Event number for all planes", width , height); 	
		c_eventNumber->Divide(3,1);


		for(int i = 0 ; i < _nb_of_planes ; i++) 
		{		
			c_hitMap->cd(i+1);  		hitMap[i]->Draw("colz"); 
			c_singleEvent->cd(i+1);  	singleEventMap[i]->Draw("colz"); 
			c_ampSpec->cd(i+1);  		ampSpectra[i]->Draw();
			c_ToASpec->cd(i+1);  		ToASpectra[i]->Draw(); 
			c_eventNumber->cd(i+1);  	eventNumber[i]->Draw(); 
		}
					
		c_integrated_hitmap->cd(); 
		(h_integrated_detector_hit_map)->Draw("BOX2");	
		
		c_integrated_hitmap	->Print(_output_image_path+"/h3_integrated_detector_hit_map.pdf");
		c_hitMap			->Print(_output_image_path+"/h2_hitMap.pdf");
		c_ampSpec			->Print(_output_image_path+"/h_ampSpec.pdf");
		c_ToASpec			->Print(_output_image_path+"/h_ToASpec.pdf");

	TCanvas *c_TLU = new TCanvas("c_TLU", "TLU", 800 , 800); 
		tluNumber[0]->Draw();
	TCanvas *c_TLUextended = new TCanvas("c_TLUextended", "extendedTLU", 800 , 800); 
		extendedtluNumber[0]->Draw();
/**/
	
	
	cluster_maker->terminate();
	std::cout << "Processing events + clusterization() time: "; benchmark->Show("SearchTime");	

	
}


// ------------------------------------------------------------------

 TH2D** TreeOnFireReader::getHisto_hitMap()  			{return hitMap ;}		

// ------------------------------------------------------------------
					
 TH2D** TreeOnFireReader::getHisto_singleEventMap() 	{return singleEventMap;}					

// ------------------------------------------------------------------

 TH1D** TreeOnFireReader::getHisto_ampSpectra() 		{return ampSpectra;}						

// ------------------------------------------------------------------

 TH1D** TreeOnFireReader::getHisto_ToASpectra() 		{return ToASpectra;}						

// ------------------------------------------------------------------

 TH1D** TreeOnFireReader::getHisto_tluNumber() 			{return tluNumber;}						

// ------------------------------------------------------------------

 TH1D** TreeOnFireReader::getHisto_extendedtluNumber() 	{return extendedtluNumber;}				

// ------------------------------------------------------------------

 TH1D** TreeOnFireReader::getHisto_eventNumber() 		{return eventNumber;}					

// ------------------------------------------------------------------

// TH1D** TreeOnFireReader::getHisto_iTimePlane_signal() 	{return iTimePlane_signal;}					

// ------------------------------------------------------------------

 TH3F*  TreeOnFireReader::getHisto_integrated_detector_hit_map() {return h_integrated_detector_hit_map;}	

// ------------------------------------------------------------------

int TreeOnFireReader::find_events_with_same_pad_responded(std::vector<short> chn1, std::vector<short> chn2)
{
	
	for(int i = 0; i < (int)chn1.size(); i++)
	{
		if(chn1[i]==0) continue;
		
		for(int j = 0; j < (int)chn2.size(); j++)
		{
			if(chn2[i]==0) continue;
			if(chn1[i]==chn2[j]) return 1;
		}
	}
	
	return 0;
}

// ------------------------------------------------------------------

void TreeOnFireReader::plot_full_event(int event)
{		
		//TFile *_outevent =	TFile::Open("event_"+TString::Itoa(event,10)+".root" , "recreate");

	
		Plane *planePtr;
		TimeFrame  singleTimeFrame;
		
		TCanvas *c_event = new TCanvas("c_event", "event "+ TString::Itoa(event,10), 1500, 1000); 
				 c_event -> SetGrid();
				 c_event ->Divide(7,3);

		TH2D	*ev[3][7];
		
		TCanvas *c_event_ac = new TCanvas("c_event_c", "event after cut "+ TString::Itoa(event,10), 1500, 1000); 
				 c_event_ac -> SetGrid();
				 c_event_ac ->Divide(7,3);

		TH2D	*ev_ac[3][7];
		
		for(int p = 0; p < NB_OF_PLANES; p++)
		{
			for(int t = 0; t < 7; t++)
			{
				ev[p][t] 	= new TH2D("ev["+TString::Itoa(p,10)+"]["+TString::Itoa(t,10)+"]", "Event " + TString::Itoa(event,10),  4,0,4,64,0,64);
				ev_ac[p][t] = new TH2D("ev_ac["+TString::Itoa(p,10)+"]["+TString::Itoa(t,10)+"]", "Event after cut " + TString::Itoa(event,10),  4,0,4,64,0,64);

			}
		}
		

		tree->GetEntry(event);  
				
		// Loop over planes in event 
		for(signed int iPlane=0; iPlane < NB_OF_PLANES ; iPlane++)
		{
			planePtr = plane[iPlane];     
				
			// Loop over stored timeframes
			for(unsigned int iTime = 0; iTime < (planePtr->time_frame).size(); iTime++) 
			{
				singleTimeFrame = (planePtr->time_frame)[iTime];

				// Loop over inividual hits in certain timeframe					
                for(int iSample = 0; iSample < singleTimeFrame.nb_of_samples ; iSample ++) 
                {	
					
					// Fill some histograms, and convert channel number to X/Y position
					std::pair<int, int> pad_coordinates = GetChanelCoordinates((singleTimeFrame.ch_number)[iSample]);
		
					ev[iPlane][iTime] -> Fill( pad_coordinates.first , pad_coordinates.second , (singleTimeFrame.amp)[iSample]  );
					if( (singleTimeFrame.amp)[iSample] < _noise_upper_cut_global )
					{
						if(iPlane == 0 && ((singleTimeFrame.amp)[iSample] > _noise_low_cut_plane_0_a1)) ev_ac[0][iTime] -> Fill( pad_coordinates.first , pad_coordinates.second , (singleTimeFrame.amp)[iSample]  );
						if(iPlane == 1 && ((singleTimeFrame.amp)[iSample] > _noise_low_cut_plane_1_a1)) ev_ac[1][iTime] -> Fill( pad_coordinates.first , pad_coordinates.second , (singleTimeFrame.amp)[iSample]  );
						if(iPlane == 2 && ((singleTimeFrame.amp)[iSample] > _noise_low_cut_plane_2_a1)) ev_ac[2][iTime] -> Fill( pad_coordinates.first , pad_coordinates.second , (singleTimeFrame.amp)[iSample]  );
					}
																	
				} //iSample end
																						
			} //iTimeend

		} //iPlane end
	

		int i=1;
		for(int p = 0; p < NB_OF_PLANES; p++)
		{
			for(int t = 0; t < 7; t++)
			{
				c_event -> cd(i);
				ev[p][t]->Draw("colz");
				c_event_ac -> cd(i);
				ev_ac[p][t]->Draw("colz");
				i++;
			}
		}			
		//_outevent-cd();
		//_outevent->Write();
		//_outevent->Close();
	
}



