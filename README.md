PICARD Gamma Sky
================

![gamma_sky](https://user-images.githubusercontent.com/69904414/199562369-eae09fdb-55f2-46f1-8c16-4835bc5197e3.png)

Author: Stefan Lepperdinger

Evaluates the gamma sky at a specified location via line of sight integration 
of the galactic gamma ray emissivities simulated by PICARD.

### Building

The software requires the GNU C++ compiler g++, cmake for building, the HDF5
library for reading an writing H5 files, the TBB library for parallelization,
and the HEALPix library for the Mollweide projection of the gamma sky. CMake
additionally fetches googletest for testing.

For Debian or on Debian based distributions (e.g., Linux Mint or the Ubuntu 
flavors), the following command should usually work and suffice to install the 
required dependencies:
```
sudo apt install -y g++ cmake libhdf5-dev libtbb-dev libhealpix-cxx-dev
```

The software can be downloaded via cloning the git repository
```
git clone https://github.com/lepperdinger/picard_gamma_sky
```
and can be built with `cmake` and `make`
```
cd picard_gamma_sky
mkdir build
cd build
cmake ..
make -j
```

### Usage

The usage of `gamma_sky` is
```
usage: gamma_rays <parameter file> <input H5 file> <output H5 file>
``` 
The parameter file contains, for example, the location of the observer and the
direction into which the observer looks (see `example_parameters.config`).
`<input H5 file>` should contain the emissivities calculated by PICARD. The
resulting gamma sky gets saved into `<output H5 file>`.

### Tests

The tests can be executed with the `ctest` command:
```
cd build
ctest
```

### Visualization

The gamma skies can be visualized via a Python script located in
`picard_gas_interpolation`. First, I recommend creating a virtual environment:
```
virtualenv venv
. venv/bin/activate
```
The visualization tool can then be installed within the virtual environment 
with pip
```
pip install ./gamma_sky_visualization
```
Once installed, it can be used with the `gamma_sky_visualization` command:
```
(venv) $ gamma_sky_visualization --help
usage: gamma_sky_visualization [-h] [--save <file>] [--log] gamma_sky_file energy_index

Visualizes the gamma skies produced by gamma_sky.

positional arguments:
  gamma_sky_file  H5 file created by gamma_sky
  energy_index    index of the energy

options:
  -h, --help      show this help message and exit
  --save <file>   save the plot instead of showing it
  --log           create a logarithmic plot
```

