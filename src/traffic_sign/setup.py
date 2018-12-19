from distutils.core import setup

setup(
    version='0.0.0',
    scripts=['launch/traffic_sign.launch'],
    packages=['traffic_sign'],
    package_dir={'': 'src'}
)