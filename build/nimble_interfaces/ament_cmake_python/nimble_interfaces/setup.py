from setuptools import find_packages
from setuptools import setup

setup(
    name='nimble_interfaces',
    version='0.0.0',
    packages=find_packages(
        include=('nimble_interfaces', 'nimble_interfaces.*')),
)
