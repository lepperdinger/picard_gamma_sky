import setuptools

setuptools.setup(
    name='picard_gas_interpolation',
    version='1.0.0',
    description='',
    author='Stefan Lepperdinger',
    author_email='lepperdinger.stefan@gmail.com',
    url='https://github.com/lepperdinger/picard_gas_interpolation',
    packages=setuptools.find_packages(),
    entry_points={
        'console_scripts': [
            'gamma_sky_visualization = src.gamma_sky_visualization:main'
        ]
    },
    install_requires=[
        'astropy==5.1',
        'cycler==0.11.0',
        'fonttools==4.34.4',
        'h5py==3.7.0',
        'healpy==1.15.2',
        'kiwisolver==1.4.4',
        'matplotlib==3.5.2',
        'numpy==1.23.1',
        'packaging==21.3',
        'Pillow==9.2.0',
        'pyerfa==2.0.0.1',
        'pyparsing==3.0.9',
        'PyQt5==5.15.7',
        'PyQt5-Qt5==5.15.2',
        'PyQt5-sip==12.11.0',
        'python-dateutil==2.8.2',
        'PyYAML==6.0',
        'scipy==1.8.1',
        'six==1.16.0',
    ],
    python_requires='>=3.8'
)
