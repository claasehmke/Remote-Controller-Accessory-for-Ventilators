To set up this small project something like the following steps should work

- `git clone --recursive https://github.com/claasehmke/Remote-Controller-Accessory-for-Ventilators.git` (important to use --recursive for submodules)
- `cd Remote-Controller-Accessory-for-Ventilators`
- `git checkout raspi_stream`
- `cd Raspberry`
- `cd WiringPi`
- `bash ./build`
- `cd ..` (you should be back in Remote-Controller-Accessory-for-Ventilators/Raspberry)
- `mkdir build; cd build`
- `cmake ..`
- `make`

(In case ssh connection is used, use the following command to get the submodule: git submodule update --init --recursive)
