/*! @file globals.h
 *  @brief globals.h -- includes basic libraries, defines verbosity level of output stream.
 *	
 * 	The hierarchy graphs of this header do not list standard libraries that are included, only the ROOT file.
 * 	After changing VERBOSITY LEVEL software needs to be recompiled. 
 * 
 */

#ifndef _GLOBALS_H_        // include guard 
#define _GLOBALS_H_ 


#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <ostream>
#include <TROOT.h>

#include <TEnv.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

 /**
 * \var VERBOSITY_LEVEL
 * \brief Verbosity level of output messages
 * 
 * <TABLE>
 * <TR><TD>Value</TD><TD>Verbosity level</TD>
 * <TR><TD>ERR</TD><TD>0</TD>
 * <TR><TD>WARN</TD><TD>1</TD>
 * <TR><TD>INFO</TD><TD>2</TD>
 * <TR><TD>CNTR</TD><TD>3</TD>
 * <TR><TD>DEB</TD><TD>4</TD>
 * </TABLE>
 */
 
const int VERBOSITY_LEVEL = 3;

#endif
