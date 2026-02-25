
# MPDMENU #

super simple collection of scripts to control
mpd either through the command line or really
any menu you want, an example for tmux is provided
though it wouldn't be super hard to port to really
anything

currently uses fzy to generate the menus, though
you can use anything for a menu as long as it takes
options deliminated by newlines through stdin and
returns the choice that was picked, so dmenu probably 
works, all of the other fuzzy finders probably work, etc.

ncless is a tiny little tcp packet sender, completely
optional and if you have a better tcp interface already
installed use that, all you need to do to compile it is
feed it to a c compiler and make sure the executable is
named ncless, and it shouldn't need to link to any libraries
that it won't automatically link to

### THIS DOES NOT WORK ON WINDOWS, WINDOWS DOES NOT HAVE A POSIX SHELL ###
and also I'd probably have to do sockets weird for ncless to work

### INSTALLING ###
just throw all of the scripts into your path somewhere,
doesn't matter where
buy default mpdmenu assumes that your music is in the
$HOME/Music directory, feel free to change that in the 
musicmenu script if it's elsewhere

## TODO ##
make sure the protocol that I do have implemented works
as expected and implement more features for playlist building



