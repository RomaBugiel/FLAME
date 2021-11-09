#include "HistoPlotter.h"

/// ------------------------- langaus --------------------------------------

double langaufun(double *x, double *par) {
  
    //Fit parameters:
    //par[0]=Width (scale) parameter of Landau density
    //par[1]=Most Probable (MP, location) parameter of Landau density
    //par[2]=Total area (integral -inf to inf, normalization constant)
    //par[3]=Width (sigma) of convoluted Gaussian function
    //
    //In the Landau distribution (represented by the CERNLIB approximation),
    //the maximum is located at x=-0.22278298 with the location parameter=0.
    //This shift is corrected within this function, so that the actual
    //maximum is identical to the MP parameter.
  
       // Numeric constants
       Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
       Double_t mpshift  = -0.22278298;       // Landau maximum location
  
       // Control constants
       Double_t np = 100.0;      // number of convolution steps
       Double_t sc =   5.0;      // convolution extends to +-sc Gaussian sigmas
  
       // Variables
       Double_t xx;
       Double_t mpc;
       Double_t fland;
       Double_t sum = 0.0;
       Double_t xlow,xupp;
       Double_t step;
       Double_t i;
  
  
       // MP shift correction
       mpc = par[1] - mpshift * par[0];
  
       // Range of convolution integral
       xlow = x[0] - sc * par[3];
       xupp = x[0] + sc * par[3];
  
       step = (xupp-xlow) / np;
  
       // Convolution integral of Landau and Gaussian by sum
       for(i=1.0; i<=np/2; i++) {
          xx = xlow + (i-.5) * step;
          fland = TMath::Landau(xx,mpc,par[0]) / par[0];
          sum += fland * TMath::Gaus(x[0],xx,par[3]);
  
          xx = xupp - (i-.5) * step;
          fland = TMath::Landau(xx,mpc,par[0]) / par[0];
          sum += fland * TMath::Gaus(x[0],xx,par[3]);
       }
  
       return (par[2] * step * sum * invsq2pi / par[3]);
 }

// --------------------------------------------------------------------------------------------

/*
 * Provides the list of all the parameters stored by the class
 */
 std::ostream &operator<<(std::ostream &os, const HistoPlotter &plotter) 
{
	
	os << std::setw(30) << std::endl;
	os << std::setw(30) << " HistoPlotter runs with: " 	<< std::endl;
	os << std::setw(30) << "_run : " 					<< plotter._run						<< std::endl;
	os << std::setw(30) << "_output_data_path : " 		<< plotter._output_data_path		<< std::endl;
	os << std::setw(30) << "_output_image_path " 		<< plotter._output_image_path		<< std::endl;
	os << std::setw(30) << "_output_file_name_core : "	<< plotter._output_file_name_core	<< std::endl;
	os << std::setw(30) << "_output_tree_name : "  		<< plotter._output_tree_name		<< std::endl;
	os << std::setw(30) << "_output_postfix : "  		<< plotter._output_postfix			<< std::endl;
	os << std::setw(30) << "_nb_of_planes : "  			<< plotter._nb_of_planes			<< std::endl;
	os << std::setw(30) << "_nb_of_channels : "  		<< plotter._nb_of_channels			<< std::endl;

							  									
	return os;                          
}

/*
 * Initializes class members taking the values from the file
 */

void HistoPlotter::init(const char * config_file)
{
	MSG(CNTR, "[HP] Init running..." );
	
	if(config.ReadFile(config_file, kEnvUser) < 0)
	{ 
		MSG(ERR, "[HP] Config file not loaded --> exit." );
		exit(0);
	}
	else
	{   
		_run						=	config.GetValue("_run", 999999);
		_output_data_path			=	config.GetValue("_output_data_path", "./outputData");
		_output_file_name_core		=	config.GetValue("_output_file_name_core", "out_TB_FIRE_");
		_output_image_path			=	config.GetValue("_output_image_path", "./outputData");
		_output_tree_name			=	config.GetValue("_output_tree_name", "flame_phys");
		_output_postfix				=	config.GetValue("_output_postfix", "");  
		_nb_of_planes				=	config.GetValue("NB_OF_PLANES", 3);  
		_nb_of_channels				=	config.GetValue("NB_OF_CHANNELS", 128);  
		_is_chain			=	config.GetValue("_is_chain",0);	
		_run_start 			=	config.GetValue("_run_start",0);	
		_run_end 			=	config.GetValue("_run_end",0);	
	}
	
	if(_is_chain==0)
	{
		_output_full_name =	_output_data_path 			+ "/" +	
						_output_file_name_core 			+ "_" + 
						"run_" + std::to_string(_run) 	+  
						_output_postfix 				;
	}
	else
	{
		_output_full_name =	_output_data_path 			+ "/" +	
						_output_file_name_core 			+ "_" + 
						"chain_" + std::to_string(_run_start) 	+ "_" +   std::to_string(_run_end)+
						_output_postfix 				;	
	
	}

}

/*
 * Opens output tree that will store the histos
 */

int HistoPlotter::open_output_tree()
{	
	_output_data_file	=	TFile::Open(_output_full_name+ ".root" , "recreate");
		
	if (!_output_data_file) 
	{ 	
		MSG(ERR, "[HP] Failed to open output tree --> exit." );
		exit(0);
	}
	else
	{	
		_output_data_file	->	cd();
		_out_tree 			= new TTree((TString)_output_tree_name, (TString)_output_tree_name);
		MSG(INFO, "[HP] Output file open: " + _output_full_name + ".root with TTree: " + _output_tree_name);
		
		return 1;
	}
	
}

/*
 * Initializes histograms that are saved in output tree
 */



void HistoPlotter::init_histo()
{
	
}   

/*
 * copy_histos mainly from TreeOnFireReader -- control plots to output tree
 */

void HistoPlotter::copy_histos()
{

	TH2D *h_hitMap;			
	TH2D *h_singleEventMap;	
	TH1D *h_ampSpectra;		
	TH1D *h_ToASpectra;		
	TH1D *h_tluNumber;			
	TH1D *h_extendedtluNumber;	
	TH1D *h_eventNumber;		
	TH3F *htemp_integrated_detector_hit_map;
	TH1D *h_ampChannel;			
	TH1D *h_iTimePlane_signal;
	
	for(int i = 0; i < _nb_of_planes; i++) 
	{	
		h_hitMap			= (TH2D*) ((_treeReader->hitMap[i]) 			->Clone());		
		h_singleEventMap	= (TH2D*) ((_treeReader->singleEventMap[i])		->Clone());
		h_ampSpectra	    = (TH1D*) ((_treeReader->ampSpectra[i])			->Clone());
		h_ToASpectra		= (TH1D*) ((_treeReader->ToASpectra[i])			->Clone());
		h_tluNumber			= (TH1D*) ((_treeReader->tluNumber[i])			->Clone());
		h_extendedtluNumber	= (TH1D*) ((_treeReader->extendedtluNumber[i])	->Clone());
		h_eventNumber       = (TH1D*) ((_treeReader->eventNumber[i])		->Clone());
		//h_iTimePlane_signal = (TH1D*) ((_treeReader->iTimePlane_signal[i])	->Clone());
		
		/*for(int ch = 0; ch < _nb_of_channels; ch++) 
		{
			h_ampChannel 	=	(TH1D*) (((_treeReader->_channels_signal[i]).a_channel_signal[ch]) ->Clone());
			//h_ampChannel 	= 	nullptr;
		}*/
	}

	htemp_integrated_detector_hit_map  = (TH3F*) (_treeReader->h_integrated_detector_hit_map->Clone());

}

/*
 * Saves histos in the output tree and closes it
 */		
int HistoPlotter::close_output_tree()
{	
	_output_data_file	->	cd();

	_output_data_file  ->	Write();
	_output_data_file  ->	Close();
	
	return 1;
}

/*
 * Saves histos as png -- should be redesigned
 */		
void HistoPlotter::save_png()
{	
	// TF1* HistoPlotter::langaufit(TH1D *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF)
    // Once again, here are the Landau * Gaussian parameters:
    //   par[0]=Width (scale) parameter of Landau density
    //   par[1]=Most Probable (MP, location) parameter of Landau density
    //   par[2]=Total area (integral -inf to inf, normalization constant)
    //   par[3]=Width (sigma) of convoluted Gaussian function
    //
    // Variables for langaufit call:
    //   his             histogram to fit
    //   fitrange[2]     lo and hi boundaries of fit range
    //   startvalues[4]  reasonable start values for the fit
    //   parlimitslo[4]  lower parameter limits
    //   parlimitshi[4]  upper parameter limits
    //   fitparams[4]    returns the final fit parameters
    //   fiterrors[4]    returns the final fit errors
    //   ChiSqr          returns the chi square
    //   NDF             returns ndf
  
	TCanvas *c;
	TF1 *fitsnr;
	Double_t fr[2];
    Double_t sv[4], pllo[4], plhi[4], fp[4], fpe[4];

	fit_width_0 	= new TH2D("fit_width_0", 	"Width (scale) parameter of Landau;x;y",  4,0,4,64,0,64);
	fit_MPV_0		= new TH2D("fit_MPV_0", 	"MPV ;x;y",  4,0,4,64,0,64);
	fit_area_0 		= new TH2D("fit_area_0", 	"Total area (integral -inf to inf, normalization constant);x;y",  4,0,4,64,0,64);
	fit_gsigma_0 	= new TH2D("fit_gsigma_0", 	"Width (sigma) of convoluted Gaussian function;x;y",  4,0,4,64,0,64);

	fit_spec_width_0 	= new TH1D("fit_spec_width_0 ", "Spectrum: Width parameter of Landau ;width;#",100,0,5);
    fit_spec_MPV_0		= new TH1D("fit_spec_MPV_0	 ", "Spectrum: MPV ;mpv;#",200,0,20);
    fit_spec_area_0 	= new TH1D("fit_spec_area_0  ", "Spectrum: Total area (normalization constant) ;norm;#",1000,0,12000);
    fit_spec_gsigma_0 	= new TH1D("fit_spec_gsigma_0", "Spectrum: Width (sigma) of convoluted Gaussian function;gsigma;#",100,0,4);
 
    //pllo[0]=0.5; pllo[1]=5.0; pllo[2]=1.0; pllo[3]=0.4;
    //plhi[0]=5.0; plhi[1]=50.0; plhi[2]=1000000.0; plhi[3]=5.0;
    //sv[0]=1.8; sv[1]=20.0; sv[2]=50000.0; sv[3]=3.0;
  
    pllo[0]=0.0;  pllo[1]=5.0; pllo[2]=1.0; pllo[3]=0.01;
    plhi[0]=10.0; plhi[1]=50.0; plhi[2]=100000.0; plhi[3]=3.5;
    sv[0]=1.8; sv[1]=10.0; sv[2]=10000.0; sv[3]=1.5;
  
    Double_t chisqr;
    Int_t    ndf;     
	gStyle->SetOptFit(1);

	double fitparams[4];
	int x, y;
	std::pair<int, int> coor;
	
	for(int i = 0; i < _nb_of_planes ; i++) 
	{				
		for(int j = 0; j < _nb_of_channels ; j++) 
		//for(int j = 0; j < 5 ; j++) 
		{			
			if(j == 13 || j == 22 || j == 78 || j == 71 || j == 100 ) continue; //mask channels
				
			c = new TCanvas("c","channelSpectra["+TString::Itoa(i,10)+"]["+TString::Itoa(j,10)+"]",500,500); 
			c->cd(); c->SetGrid();
			//Axis in x
			if(i == 0 || i == 1) ((_treeReader->_channels_signal[i]).a_channel_signal[j])->GetXaxis()->SetRangeUser(0,50);
			else ((_treeReader->_channels_signal[i]).a_channel_signal[j])->GetXaxis()->SetRangeUser(0,150);
			
	        //fr[0]=0.3*(((_treeReader->_channels_signal[i]).a_channel_signal[j]))->GetMean();
			//fr[1]=3.0*(((_treeReader->_channels_signal[i]).a_channel_signal[j]))->GetMean();
  	        fr[0]=8;
			fr[1]=50;
			
  			fitsnr = langaufit(  ((_treeReader->_channels_signal[i]).a_channel_signal[j])  ,fr,sv,pllo,plhi,fp,fpe,&chisqr,&ndf);
			
			if(i==0)
			{
				fitsnr->GetParameters(fitparams); 
				
				coor = GetChanelCoordinates(i);
				x = coor.first;
				y = coor.second;
				

				std::cout << "channel: " << j << "\t[" << x << ", " << y << "] width: "  << fitparams[0] << "  MPV:  " << fitparams[1] <<  "  area:  " << fitparams[2] <<  "  gsigma:  " << fitparams[3] << std::endl;
				
				fit_width_0 	-> Fill(x,y,fitparams[0]);
				fit_MPV_0		-> Fill(x,y,fitparams[1]);
				fit_area_0 	    -> Fill(x,y,fitparams[2]);
				fit_gsigma_0    -> Fill(x,y,fitparams[3]);
				
				fit_spec_width_0 	-> Fill( fitparams[0] );
				fit_spec_MPV_0	    -> Fill( fitparams[1] );
				fit_spec_area_0 	-> Fill( fitparams[2] );
				fit_spec_gsigma_0   -> Fill( fitparams[3] );
				
				//std::cout << "fitparams[0]: " << fitparams[0] << " fitparams[1]: " << fitparams[1] << std::endl;
			}
			
			((_treeReader->_channels_signal[i]).a_channel_signal[j])->Draw();
			c->Print(_output_image_path+"/channelSpectra["+TString::Itoa(i,10)+"]["+TString::Itoa(j,10)+"].pdf");

		}
	}
	
	
	TCanvas *c_width = new TCanvas("c_width","c_width for plane 0",1200,800); 
		c_width->Divide(2,1); 
		c_width->SetGrid();
		c_width->cd(1); fit_width_0->Draw("colz"); 
	//	c_width->cd(2); fit_spec_width_0->Draw();
		c_width->Print((TString)(_output_image_path+"/fit_width_plane0.pdf"));
	TCanvas *c_mpv = new TCanvas("c_mpv","c_mpv for plane 0",1200,800);
		c_mpv->Divide(2,1); 
		c_mpv->SetGrid();
		c_mpv->cd(1); fit_MPV_0->Draw("colz"); 
	//	c_mpv->cd(2); fit_spec_MPV_0->Draw();
		c_mpv->Print((TString)(_output_image_path+"/fit_mpv_plane0.pdf"));
	TCanvas *c_area = new TCanvas("c_area","c_area for plane 0",1200,800);
		c_area->Divide(2,1); 
		c_area->SetGrid();
		c_area->cd(1); fit_area_0->Draw("colz"); 
		//c_area->cd(2); fit_spec_area_0->Draw();
		c_area->Print((TString)(_output_image_path+"/fit_area_plane0.pdf"));
	TCanvas *c_gsigma = new TCanvas("c_gsigma","c_gsigma for plane 0",1200,800);
		c_gsigma->Divide(2,1); 
		c_gsigma->SetGrid();
		c_gsigma->cd(1); fit_gsigma_0->Draw("colz"); 
	//	c_gsigma->cd(2); fit_spec_gsigma_0->Draw();
		c_gsigma->Print((TString)(_output_image_path+"/fit_gsigma_plane0.pdf"));    
    
}

/*
 * Saves histos as png -- should be redesigned
 */	
void HistoPlotter::fast_monitor()
{
	
	int _event_nb;
	int _TLU_number;
	int _extended_TLU_number;
	int _plane_nb;
	int _time_plane;
	
	std::vector<float> 		*_cl_x_seed  	= 0 ;
   	std::vector<float> 		*_cl_y_seed 	= 0 ;
	std::vector<float> 		*_cl_x_pos		= 0 ;
   	std::vector<float> 		*_cl_y_pos		= 0 ;
	std::vector<float> 		*_cl_energy		= 0 ;
  	std::vector<int> 		*_cl_size		= 0 ;
   	std::vector<int> 		*_cl_x_size		= 0 ;
   	std::vector<int> 		*_cl_y_size		= 0 ;
	
	 TBranch *_b_event_nb;
	 TBranch *_b_TLU_number;
	 TBranch *_b_extended_TLU_number;
	 TBranch *_b_plane_nb;
	 TBranch *_b_time_plane;
			  
	 TBranch *_b_cl_x_seed = 0 ;
   	 TBranch *_b_cl_y_seed = 0;
	 TBranch *_b_cl_x_pos = 0;
   	 TBranch *_b_cl_y_pos = 0;
	 TBranch *_b_cl_energy = 0;
  	 TBranch *_b_cl_size = 0;
   	 TBranch *_b_cl_x_size = 0;
   	 TBranch *_b_cl_y_size = 0;
	
	
	TFile *in_cluster_file 		= 	new TFile("./outputData/cluster_out_TB_FIRE_run_864_plane0.root");
	TTree *in_cluster_tree 		= 	(TTree*) in_cluster_file -> Get ( (TString) "flame_clstr" );

	std::cout << "Get Entries in fast monitor: " << in_cluster_tree->GetEntries() << std::endl;

		in_cluster_tree->SetBranchAddress("_event_nb"  ,			&_event_nb, 			&_b_event_nb);
        in_cluster_tree->SetBranchAddress("_TLU_number", 			&_TLU_number, 			&_b_TLU_number);
        in_cluster_tree->SetBranchAddress("_extended_TLU_number", 	&_extended_TLU_number, 	&_b_extended_TLU_number);
        in_cluster_tree->SetBranchAddress("_plane_nb", 	 			&_plane_nb, 			&_b_plane_nb);
        in_cluster_tree->SetBranchAddress("_time_plane",			&_time_plane, 			&_b_time_plane);
        in_cluster_tree->SetBranchAddress("_cl_x_pos", 				&_cl_x_pos, 			&_b_cl_x_pos );
		in_cluster_tree->SetBranchAddress("_cl_y_pos", 				&_cl_y_pos,				&_b_cl_y_pos);
		in_cluster_tree->SetBranchAddress("_cl_energy", 			&_cl_energy,			&_b_cl_energy);
		in_cluster_tree->SetBranchAddress("_cl_size", 				&_cl_size,				&_b_cl_size);
		in_cluster_tree->SetBranchAddress("_cl_x_seed", 			&_cl_x_seed,			&_b_cl_x_seed);
		in_cluster_tree->SetBranchAddress("_cl_y_seed", 			&_cl_y_seed,			&_b_cl_y_seed);

        
       int difference_in_event = 0; 
       int current_event = 0;
       int next_event = 0;
       int timeplane_with_signal = 0;
       int jump = 0;
       std::vector<int> energies_in_event;
    
    //Calibration - try
    Channels_Signal _channels_signal_clstr[PLANES_NUMBER]; 

    for(int i = 0 ; i < in_cluster_tree->GetEntries(); i++)
    {
		in_cluster_tree->GetEntry(i);
		
		if(_cl_size->size() != 1 )	continue; // Take only cases where one cluster found
		if( (*_cl_size)[0]  != 1 ) 	continue; // Take only one-pad clusters
		
		//_channels_signal_clstr[_plane_nb] 
		
		
		
	}
    
    
}



 
 
 //--------------------
 
 TF1* HistoPlotter::langaufit(TH1D *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF)
 {
    // Once again, here are the Landau * Gaussian parameters:
    //   par[0]=Width (scale) parameter of Landau density
    //   par[1]=Most Probable (MP, location) parameter of Landau density
    //   par[2]=Total area (integral -inf to inf, normalization constant)
    //   par[3]=Width (sigma) of convoluted Gaussian function
    //
    // Variables for langaufit call:
    //   his             histogram to fit
    //   fitrange[2]     lo and hi boundaries of fit range
    //   startvalues[4]  reasonable start values for the fit
    //   parlimitslo[4]  lower parameter limits
    //   parlimitshi[4]  upper parameter limits
    //   fitparams[4]    returns the final fit parameters
    //   fiterrors[4]    returns the final fit errors
    //   ChiSqr          returns the chi square
    //   NDF             returns ndf
  
    Int_t i;
    Char_t FunName[100];
  
    sprintf(FunName,"Fitfcn_%s",his->GetName());
  
    TF1 *ffitold = (TF1*)gROOT->GetListOfFunctions()->FindObject(FunName);
    if (ffitold) delete ffitold;
  
    TF1 *ffit = new TF1(FunName,langaufun,fitrange[0],fitrange[1],4);
    ffit->SetParameters(startvalues);
    ffit->SetParNames("Width","MP","Area","GSigma");
  
    for (i=0; i<4; i++) {
       ffit->SetParLimits(i, parlimitslo[i], parlimitshi[i]);
    }
  
    his->Fit(FunName,"RBQ");   // fit within specified range, use ParLimits, do not plot
  
    ffit->GetParameters(fitparams);    // obtain fit parameters
    for (i=0; i<4; i++) {
       fiterrors[i] = ffit->GetParError(i);     // obtain fit parameter errors
    }
    ChiSqr[0] = ffit->GetChisquare();  // obtain chi^2
    NDF[0] = ffit->GetNDF();           // obtain ndf
  
    return (ffit);              // return fit function
  
 }
  
