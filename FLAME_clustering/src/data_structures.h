/*! @file data_structures.h
 *  @brief data_structures.h -- defines some helper data structers
 *	@details Might be that this is not usefull, should be redesgined -- to be verified with further analysis 
 * 	C-style structure
 */

#ifndef DATASTRUCT_h
#define DATASTRUCT_h

#include  <cstdint>
#include  <vector>
#include  <array>
#include  "TH1D.h"


/*! \brief Number of channels on single plane
 * \todo should be defined somewhere else?
*/
#define NB_OF_CHANNELS     128



// -----------------  CHANNELS_SIGNAL --------------------------

/*! \brief Structure keeping array of histograms with signal amplitude per single channel 
 *  \details Also the plane_nb is kept in the structure
 */
 
 




struct Channels_Signal 
{
	int plane_nb;
	std::array<TH1D*, NB_OF_CHANNELS> a_channel_signal;
};

struct Pad
{
	int x;
	int y;
	double ene;
//	Pad()  // initialization list
//	: x(0),
//	  y(0),
//	ene(0){}
};


struct Cluster 
{
	float energy;
	float seed_energy;
	float pad_ab_th;
	float x_seed;
	float y_seed;
	float x_pos_COG;
	float y_pos_COG;
	float x_pos;
	float y_pos;
	int cluster_size;
	std::vector<Pad> pad;
	Cluster()  // initialization list
	: energy(0),
	  seed_energy(0),
	  pad_ab_th(0){}

};

#endif

