# [Re] Reproductive pair correlations and the clustering of organisms
## Replication of Young et al. (2001)

[![DOI](https://zenodo.org/badge/339762861.svg)](https://zenodo.org/badge/latestdoi/339762861)

**Authors**: Coralie Picoche, William R. Young and Frédéric Barraquand

This repository contains the scripts and mathematical analyses replicating the results of the following paper:

> Young, W. R., Roberts, A. J., & Stuhne, G. (2001). _Reproductive pair correlations and the clustering of organisms_. Nature, 412(6844), 328-331.

We simulate the Brownian Bug Model in a two-dimensional, dynamic fluid. These particles are subject to both demographic processes (birth and death), and hydrodynamic processes that move them around (diffusion and advection). We provide in this folder two animated images as examples of the dynamics of the particles, with either [low](./gif_lowDelta.gif ) (![\Delta](https://latex.codecogs.com/svg.latex?\Delta)=10<sup>-7</sup>) or [high](./gif_highDelta.gif) (![\Delta](https://latex.codecogs.com/svg.latex?\Delta)=10<sup>-3</sup>) diffusivity. 

### Organisation

Folders are organized as follow:

* `article` contains all files used to produce the manuscript of the article.
* `code/simulation` contains the code for the simulations of the Brownian bug model. Output files are also stored in this folder.
* `code/figure` contains the codes to produce figures shown in the article, as well as said figures.
* `code/tests` provides the codes to run smaller (and therefore faster) simulations to enable the user to familiarize oneself with the structure of the code.

### Pre-requisites

Simulations of the Brownian bug model are run in C++ (v.5.4.0, using the GSL Library v2.6) and output files are then treated separately with R (v.3.6.3) to produce figures.

All codes were tested on Ubuntu 16.04.

### Contributing

Everyone is welcome to fork and re-use our code for further development in their own repository and project, following the conditions of the license (GPLv3). For any modification of this repository and/or push request, please open an issue first.

For citation, use: Picoche, C., Young, W.R. & Barraquand, F. \[Re\] Reproductive pair correlations and the clustering of organisms. _ReScience C_, 8(1).

If you wish to collaborate, please contact us at [coralie.picoche@u-bordeaux.fr](mailto:coralie.picoche@u-bordeaux.fr) and [frederic.barraquand@u-bordeaux.fr](mailto:frederic.barraquand@u-bordeaux.fr). 
