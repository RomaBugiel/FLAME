# How to run 

## General desciption 
__FLAME__ data analysis

Testbeam 2020:
The software is dedicated for basic analysis of FLAME data. Up to know it is rough software, draft version, with not optimized code.

This is the part of the software that reads in raw FLAME root tree (TreeOnFireReader) and makes clusterization (ClusterFormation). Also some basic plots are produced (HistoPlotter). 
RunControl is for controling program flow.

## GitHub
Repository can be cloned from:

__https://github.com/RomaBugiel/FLAME.git__

## Software
* C++11, ROOT 

### Needed packages 
The software was tested on:

* ROOT 6.22/00 
* Unix: Ubuntu 20
* Doxygen: 1.8.17
* g++: 9.3.0 

## Compilation and run

### Software
> __make__ <br/>
> ./offline_analysis

#### Configuration file

For configuration parameter list on HTLM documentation please go: Releated Pages --> config_file.cfg

#### Output
> In general the path for saving the root trees is provided via config_file.cfg.
> If not changed, one can find the outputs in: ./outputData 

### Documentation

> * in main directory run: __make docs__ 
> 	* for Latex go to /doc/latex: __make__ and open __refman.pdf__
> 	* for HTLM: go to /doc/html and open __index.html__

## Scirpits

## Essentail steps for running the software
* configuration file paths are hardcoded. One has to take care to change a file path in all classes (ClusterFormation, RunControl, TreeOnFireReader)
* check if outputData / inputData / docs directories are created and paths are correct
* after cloning the repository --> __make__ and __make docs__ (docs might not work now!)
* set analysis parameters
* run soft ./run_flame_clusterization

Definitions in doc. 
