# config_file.cfg
## Configuration parameter list

This file contains all the needed variables for running the software, such as files paths, scan parameters value, etc.<br/>
Changes in __does not need__ the program to be __recompiled__. <br/>
For the documentation the markdown format file is created, but in the DataConverter class the config_file.cfg is loaded. .md and .cfg are not created automatically. If some adds any parameter to .cfg a proper entry in .md should be done. <br/>



| Name                | Value (example)        | Info                                   | Description                                                                                       |
|---------------------|------------------------|----------------------------------------|---------------------------------------------------------------------------------------------------|
| NB_OF_PLANES  	  | 3			           | __int__               					| Number of FLAME planes used during test-beam											            |
| NB_OF_CHANNELS  	  | 128			           | __int__               					| Number of channels on single FLAME plane										           			|
| _input_data_dir  	  | ./inputDat             | __std::string__               			| Path for directory with input data													            |
| _input_data_name 	  | TB_FIRE_               | __std::string__                 		| Input data file name                                 									            |
| _input_tree_name	  | 0                      | __std::string__  		                | Input tree name																                 	|
| _run	 	          | 671                    | __int__, arbitrary, > 0                | Number of run to be analysed									                                    |
| _skipEvents	 	  |	0					   | __int__, > 0 & < nb_of_events			| Number of events that are skipped, starting from 0 event					                        |
| _statisticFraction  | 1					   | __double__, [0:1]						| Fraction of statistic that is analysed									                        |
| _singleEvent	 	  | 100					   | __int__, > 0 & < nb_of_events			| Event number for which some control plots are shown						                        |
| _output_data_path	  | ./outputData		   | __std::string__               			| Path of directory with output data													            |
| _output_image_path  | ./outputData/img	   | __std::string__               			| Path of directory with output data plots												            |
| _output_file_name_core | out_TB_FIRE		   | __std::string__               			| Name of the output ROOT tree file													          	    |
| _output_tree_name	  | flame_phys		   	   | __std::string__               			| Name of the output ROOT tree												                		|
| _output_postfix	  | 					   | __std::string__               			| String added to the output ROOT tree file name (at the end) in case of user's need to change a name for the same run  	        |
| _is_chain			  |	1 						| __int__ [0,1]	              			| Flag for selecting multiple-run [1] analysis or single run [0]. If =0, _run number is used. If [1], _run_start, _run_end, _step, _incl, _excl used.        |
| _run_start		  |	833 					| __std::string__               		| First run taken to TChain (taken to analysis) 	        |
| _run_end			  |	868 					| __std::string__               		| Last run taken to TChain (taken to analysis)	        |
| _step			 	  |	1 						| __std::string__               		| Step of going from #_run_start to #_run_end 	        |
| _incl			  	  |				 			| __std::string__               		| Additonal runs taken to analysis might be declared here with whitespace separation	       		 |
| _excl			  	  |	838 839 842 845 		| __std::string__               		| Runs excluded from the analysis might be declared here with whitespace separation  	       	 	 |
| _cluster_ene_down_cut	  |	0 					| __int__, [0;1023]               		| Determines the down cut energy of the cluster, taken as a noise not signal if energy is below	     |
| _cluster_radius_y   |	5 						| __int__, arbitrary, > 0               | This is the range in Y [pads] of area for cluster formation       								 |
