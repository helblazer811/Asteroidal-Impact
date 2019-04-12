## Asteroidal Impact ##

This is a game that I made that runs on a Gameboy Advanced.
It is largely inspired by Galaga and games similair to it. 
You can run it through the directions below. It was written
entirely in C. 

![alt text](https://github.com/helblazer811/Asteroidal-Impact/blob/master/screenshots/StartScreenShot.png)
![alt text](https://github.com/helblazer811/Asteroidal-Impact/blob/master/screenshots/GameScreenShot.png)
![alt text](https://github.com/helblazer811/Asteroidal-Impact/blob/master/screenshots/GameOverScreenShot.png)

## Install Directions ##

- Install dependencies

`sudo apt update`</br>
`sudo apt install gcc-arm-none-eabi cs2110-vbam-sdl mednafen cs2110-gba-linker-script nin10kit`</br>
`sudo add-apt-repository ppa:tricksterguy87/ppa-gt-cs2110`</br>
`sudo apt update`</br>

- Build and run

`make emu`</br>

## Control Scheme ##

Press A(Z on a non Gameboy keyboard) to start the game

- Controls
 -- Press A(Z) to fire </br>
 -- Use arrow keys to move </br>
 -- Backspace to go back to the start screen </br>
- Info
 -- The score shows how many kills you have </br>
 -- The number of lives you have is shown by the number of hearts in the bottom right of the screen </br>
