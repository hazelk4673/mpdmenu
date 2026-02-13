#mpdmenu

super simple scripts for controlling music player daemon
through dmenu

#installing

run `make build` to compile all of the executables needed, 
they'll be pushed to a build directory for you to do whatever
you like with

run make `build install` if you don't particularly care how 
things are installed, it will add all of the executables to /usr/bin

#hacking

mpdmenu is just a collection of scripts, and a small tcp packet
sender (with a little bit of mpd protocol baked into it), so
it is pretty easy to customize exactly to your liking
