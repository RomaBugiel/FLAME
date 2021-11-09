#ifndef PLANE_STR_h
#define PLANE_STR_h

#include <vector>


inline std::pair<int, int> GetChanelCoordinates(int ch_number)
{
	std::pair<int, int> coorddinates;

	if( (ch_number/64) == 0 )
	{
		coorddinates.first = ch_number/64;
		coorddinates.second = 63 -	(ch_number%64) ;						
	}
	else
	{ 
		coorddinates.first = ch_number/64;
		coorddinates.second = 63 - ( 127 - (ch_number)%64 - 64);	
	}
	return coorddinates;
}


// Definition of structures defining the outpou tree shape
struct TimeFrame
{
  long time_stamp;
  short nb_of_samples;
  std::vector<short> ch_number;
  std::vector<short> gain;
  std::vector<short> amp;
  std::vector<short> ToA;
   TimeFrame()  // initialization list
	: time_stamp(-1),
	  nb_of_samples(0){}
};

struct Plane
{
  bool isValid;
  bool hasDebug; 
  short raw_TLU_nb;
  short plane_number;
  short FE_board_SN;
  int board_ev_number;
  long global_time_stamp;
  std::vector<TimeFrame> time_frame;
  std::vector<int> debug_data;
  Plane()  // initialization list
	: isValid(0),
	  hasDebug(0),
	  raw_TLU_nb(-1),
	  plane_number(-1),
	  FE_board_SN(-1),
	  board_ev_number(-1),
	  global_time_stamp(-1){ } 

};


#endif



