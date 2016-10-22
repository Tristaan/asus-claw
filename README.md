# asus-claw
This is a driver I am making to learn something about drivers for USB devices.
Please note that this is still in development.

## claw-cli
This is a program that turns settings into bytes understandeable by the driver.

    usage: claw-cli [options]
      -p<x>   Profile DPI Settings. Note: only use values incremented by 50
              from 50-5000.Program will round down any other values.
      -ps     Same as above, Setting for Sniper Mode.
      -ls     Scrollwheel LED Setting. 0=disabled 1=enabled
      -ll     Logo LED Setting. 0=disabled 1=enabled
      -o      Polling rate settings. 0=125Hz 1=500Hz 2=1000Hz
      -h      Display this help message

## Installation
    git clone https://github.com/Tristaan/asus-claw.git
    cd asus-claw
    make

To make DPI buttons work ./claw must be running.

####To-Do
There is some work left to do:
* Make the event handling better (??)
* Writing new configs to the mouse (half of it is finished)
* Communication between the driver and the frontend application

