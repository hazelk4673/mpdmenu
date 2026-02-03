$ telnet 0 6600
Trying 0.0.0.0...
Connected to 0.
Escape character is '^]'.
OK MPD 0.15.0
currentsong
file: A/Alexandria Maillot/Just Another Girl/Just Another Girl.mp3
Time: 224
Artist: Alexandria Maillot
AlbumArtist: Alexandria Maillot
Title: Just Another Girl
Album: Just Another Girl
Track: 1/4
Genre: Pop
Composer: Alexandria Maillot and Joby Baker
Pos: 1
Id: 159
OK
stats
artists: 56
albums: 56
songs: 392
uptime: 124
playtime: 54
db_playtime: 84859
db_update: 1291807454
OK
pasus
ACK [5@0] {} unknown command "pasus"
pause
OK
play
OK
close
Connection closed by foreign host.

above is an example of using straight up telnet as an mpd client



You don't really need mpc if you are a script nut with nc (netcat) or expect. If you don't need to process the response, you can always give commands to MPD, e.g.

echo "next" | telnet 0 6600

This will make MPD to play next song.


python mpd client
shebang/usr/bin/env python 
try: import readline except ImportError: pass import socket import sys HOST = 'localhost' PORT = 6600 RECV_SIZE = 2**20 # One megabyte try: s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) print 'Connecting to %s:%d...' % (HOST, PORT) s.connect((HOST, PORT)) except socket.error: print 'Unable to connect.' sys.exit(1) print s.recv(RECV_SIZE) print '*** <Control+C> to exit ***\n' while True: try: cmd = raw_input(">>> ") if cmd: s.send(cmd + '\n') if 'close' in cmd: break print s.recv(RECV_SIZE) except KeyboardInterrupt: break print s.close()



