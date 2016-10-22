# asus-claw
This is a driver I am making to learn something about drivers for USB devices.
Please note that this is still in development.

## claw-cli
This is a program that turns settings into bytes understandeable by the driver.

usage: claw-cli [options]\n
  -p<x>   Profile DPI Settings. Note: only use values incremented by 50\n
          from 50-5000.Program will round down any other values.\n
  -ps     Same as above, Setting for Sniper Mode.\n
  -ls     Scrollwheel LED Setting. 0=disabled 1=enabled\n
  -ll     Logo LED Setting. 0=disabled 1=enabled\n
  -o      Polling rate settings. 0=125Hz 1=500Hz 2=1000Hz\n
  -h      Display this help message\n

####To-Do
There is some work left to do:
* Make the event handling better (??)
* Writing new configs to the mouse (half of it is finished)
* Communication between the driver and the frontend application

