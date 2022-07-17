#!/usr/bin/env python3
"""
Visualizes the gamma skies produced by gamma_sky.

Author: Stefan Lepperdinger
"""
from h5py import File
from healpy.newvisufunc import projview
import numpy as np
import matplotlib.pyplot as plt
import argparse
import sys


def read_data(gamma_sky_file_path, energy_index):
    with File(gamma_sky_file_path) as file:
        skies = np.array(file['gamma ray skies'])
        energies = np.array(file['energies'])
        energy_unit = file['energies'].attrs['unit'].decode()
        sky_flux_unit = file['gamma ray skies'].attrs['unit'].decode()

    assert_message = 'error: input file: wrong energy unit'
    assert energy_unit == 'MeV', assert_message

    assert_message = 'error: input file: wrong sky flux unit'
    assert sky_flux_unit == 'MeV / (cm^2 sr s)', assert_message

    print('Available Energies:')
    for energy_index_, energy_ in enumerate(energies):
        mev_to_gev = 1e-3
        print(f'  {energy_index_}: {energy_ * mev_to_gev:.2f} GeV')

    try:
        energy = energies[energy_index]
        sky = skies[energy_index]
    except IndexError:
        print(f'error: invalid energy index "{energy_index}".')
        sys.exit(1)

    return energy, sky


def plot_sky(sky, energy, logarithmic):
    """
    Plots the gamma sky.

    :param sky: gamma sky fluxes in MeV / (cm² sr s)
    :type sky: np.array

    :param energy: energy of the gamma sky in MeV
    :type energy: float

    :param logarithmic: plot the logarithm of the fluxes if true
    :type logarithmic: bool
    """
    # convert gamma fluxes from MeV / (cm² sr s) to GeV / (m² sr s)
    sky *= 10.
    # convert energy from MeV to GeV
    energy *= 1e-3
    color_map_label = r'Gamma ray flux / (GeV m$^{-2}$ sr$^{-1}$ s$^{-1}$)'
    if logarithmic:
        sky = np.log(sky)
        color_map_label = f'log({color_map_label})'
    projview(
        sky,
        graticule=True,
        graticule_labels=True,
        xtick_label_color='white',
        xlabel='Longitude',
        ylabel='Latitude',
        projection_type="mollweide",
        cmap='magma',
        unit=color_map_label,
        title=f'Energy = {energy:.2f} GeV',
    )


def parse_arguments():
    """
    Parses and returns the arguments.

    :return: arguments
    :rtype: argparse.Namespace
    """
    parser = argparse.ArgumentParser(
        description='Visualizes the gamma skies produced by gamma_sky.'
    )

    parser.add_argument(metavar='gamma_sky_file',
                        dest='gamma_sky_file_path',
                        help='H5 file created by gamma_sky')
    parser.add_argument('energy_index',
                        type=int,
                        help='index of the energy')
    parser.add_argument('--save',
                        metavar='<file>',
                        dest='destination_file',
                        help='save the plot instead of showing it')
    parser.add_argument('--log',
                        dest='logarithmic_plot',
                        action='store_true',
                        help='create a logarithmic plot')

    arguments = parser.parse_args()
    return arguments


def main():
    arguments = parse_arguments()
    energy, sky = read_data(arguments.gamma_sky_file_path,
                            arguments.energy_index)
    plot_sky(sky, energy, arguments.logarithmic_plot)
    if arguments.destination_file is not None:
        plt.savefig(arguments.destination_file)
    else:
        plt.show()


if __name__ == '__main__':
    main()
