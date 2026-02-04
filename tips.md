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

communicates over tcp, client connects and mpd sends `OK MPD $version` where version is an identifier for the protocol version NOT MPD VERSION,
protocol version (honestly more useful) 

commands are sent `$command [$arg...]` argument strings are separated from the command and other arguments by spaces or tabs

commands return `OK` on completion or `ACK $some_error` on failure, some commands return data before the `OK`, lines are usually `$name: $value`

some commands will be returned binary data, in the format:
`$name: $identifier
 binary: $size
 <$size bytes>
 OK`

`ACK` lines are in the following format: `ACK [$error@$command_list_offset] {$current_command} $error_message`

you can send lists of commands by beginning the message with the line `command_list_begin`, on success for all, `OK` is returned, if a command fails all of them stop and an `ACK` is returned
you can also start a command list with `command_list_ok_begin` to get back a `list_OK` FOR EACH command successful execution in the list

example:

command_list_begin
volume 86
play 10240
status
command_list_end

server responds with:

ACK [2@1] {play} Bad song index

things can be searched with `find $expression` where expression is a string enclosed in parentheses which can be

expression syntax is as follows

($tag == '$value') which matches a tag value, the special `any` tag matches against all tag types, AlbumArtist checks for a value in AlbumArtist or falls back to Artist if AlbumArtist doesn't exist
($tag contains '$value') checks if the given value is a substring of the tag value
($tag starts_with '$value') check if the tag value starts with the given value
($tag =~ '$value') and ($tag !~ '$value') use perl regex to compare instead of just a string, only available if MPD is compiled with libpcre
(file == '$value') match full song URI relative to music directory
(base '$value') restrict search to songs in the given directory relative to music directory
(modified-since '$value') compares file time stamp with given value, ISO or UNIX timestamp
(added-since '$value') compares time stamp when file was added with given value, ISO or UNIX
(AudioFormat == '$samplerate:$bits:$channels') compares audio format with given value, see manual for extra details
(AudioFormat =~ '$samplerate:$bits:$channels') matches audio format with given mask, one or more attributes can be `*`
(prio >= 42) compares priority of queued songs (idk whether or not the 42 is just what you put there or if it's a value you can change)
(!($expression)) negate an expression, expression must be enclosed in parens
(($expression1) AND ($expression2)) combine two or more expressions with AND, each expression must be enclosed in parens

most commands can be given `_cs` to be given case sensitivity and `_ci` for case insensitivity

escape special chars with \ obv

following tags are supported by mpd

artist
artistsort (for sorting, omits 'The')
album
albumsort (same as artistsort)
albumartist (on multi artist albums the artist name that is used for the whole album)
albumartistsort (yk the drill)
title
titlesort
track (decimal track number)
name (NOT the song title nobody knows what this does really)
genre
mood
date (4 digit year)
originaldate (song's original release date)?
composer
composersort
performer
conductor
work ("a work is a distinct intellectual or artistic creation, which can be expressed in the form of one or more audio recordings")???
ensemble
movement
movementnumber
showmovement (if this is set to 1, players display work, movement, and movementnumber instead of track title)
location
grouping (used if the sound belongs to a larger category of sounds/music)
comment
disc (decimal disc number)
label
musicbrainz_artistid (artist id in the MusicBrainz database)
musicbrainz_albumid
musicbrainz_albumartistid
musicbrainz_trackid
musicbrainz_releasegroupid
musicbrainz_releasetrackid
musicbrainz_workid

some tags can have multiple values

other metadata

duration: song in seconds, may contain fractional part
Time: duration but integer value, do not use!
Range: if this item is only a fraction of a song it will have the start and end points, eg. 60-120
Format: audio format
Last-Modified: time stamp of last modification
added: time stamp of when the song was added

higher priority on a song means it will play before other songs

COMMAND REFERENCE -----------------------------------------------------------------------------------
clearerror
    clears current error message in status, can also be done by any command that starts playback

currentsong
    displays song info of current song, info about the current song is represented in key value pairs one on each line

idle [$subsystems...]
    waits until there is a change in the subsystem given where subsystem is one of the following
        database: song database has been modified after update
        update: database update has started or finished, if database was modified during update, database is also returned
        stored_playlist: a playlist has been modified, renamed, created or deleted
        playlist: the queue has been modified
        player: the player has been started, stopped, or seekd or tags of the currently playing song have changed
        mixer: volume has been changed
        output: audio output has been added removed or modified
        partition: a partition was added removed or changed
        sticker: the sticker database has been modified
        subscription: a client has subscribed or unsubscribed to a channel
        message: a message was received on a channel this client is subscribed,l only emitted when the client's message queue is empty
        neighbor: a neighbor was found or lost
        mount: the mount list has changed

    idle command can be canceled by giving the noidle command, mpd will then leave idle mod and print results immediately, may be empty

status
    report surrent status of player and volume level
        partition: name of current partition
        volume: 0 - 100
        repeat: 0 || 1
        random: 0 || 1
        single: 0 || 1 || oneshot
        consume: 0 || 1 || oneshot
        playlist: 31 bit unsigned int, playlist version number
        playlistlength: integer length of playlist
        state: play || stop || pause
        song: playlist song number of current song stopped on or playing
        songid: playlist songid of current song stopped on or playing
        nextsong: playlist song number of next song to be played
        nextsongid: playlist songid of next song to be played
        elapsed: total time elapsed within current song in seconds
        duration: duration of current song in seconds
        bitrate: instantaneous bitrate in kbps
        xfade: crossfade in seconds
        mixrampdb mixramp threshold in dB
        mixrampdelay: mixrampdelay in seconds
        audio: format emitted by decoder plugin during playback in format samplerate:bits:channels
        updating_db: job id
        error: returns error here
        lastloadedplaylist: last loaded stored playlist

stats
    displays stats
        artists: number of artists
        albums: number of albums
        songs: number of songs
        uptime: daemon uptime in seconds
        db_playtime: sum of all song times in database in seconds
        db_update: last db update in UNIX time
        playtime: time lenghth of music played

consume {$state}
    sets consume state to $state; $state should be 0 || 1 || oneshot, when consume is activated each song played is removed from playlist

crossface {$seconds}
    sets crossface to $seconds

mixrampdb {$decibels}
    sets threshold at which songs will be overlapped

mixrampdelay {$seconds}
    additional time subtraced from the overlap calculated by mixrampdb, value of nan disables MixRamp overlapping and falls back to crossfading

random {$state}
    sets random state to $state; $state should be 0 || 1

repeat {$state}
    sets repeat state to $state; $state should be 0 || 1

setvol {$vol}
    sets volume to $vol, value should be 0 - 100

getvol
    read volume, result is a volume: line like in status, if there is no mixer mpd will emit an empty response

single {$state}
    sets single state to $state; $state should be 0 || 1 || oneshot, when single is activated playback 
    is stopped after current song or some is repeated if repeat is enabled

replay_gain_mode {$mode}
    sets replay gain mode, $mode should be off || track || album || auto, changing mode during playback
    may take several seconds because new settings don't affect buffered data, triggers options idle event

replay_gain_status
    prints replay gain options, only replay_gain_mode is returned

volume {$change}
    changes volume by amount $change

next
    plays next song in playlist

pause {$state}
    pause or resume playback, 1 pauses and 0 resumes, without argument pause state is toggled

play [$songpos]
    begins playing the playlist at song numnber $songpos

playid [$songid]
    begins playing playlist at $songid

previous
    plays previous song in playlist

seek {$songpos}{$time}
    seeks to position $time in seconds of entry $songpos in playlist, fractions allowed

seekcur {$time}
    seeks to position $time in seconds within current song, if prefixe by + or -, time is relative to current position, fractions allowed

stop
    stops playing

add {$uri}[$position]
    add file $uri to playlist, directories add recursively, $uri can be single file

addid {$uri}[$position]
    adds song to playlist (non recursive) and returns song id, $uri is always single file or url
    if seconf parameter is given, inserts song at specified position, if parameter starts with + || -, relative to current song
    +0 inserts right after current song, -0 inserts right before current song

clear 
    clears the queue

delete [{$pos}|{$start:$end}]
    deletes song $pos from playlist or deletes songs from $start to $end in the playlist

deleteid {$songid}
    deletes song $songid from plalist

move [{$from}|{$start:$end}]{$to}
    moves song at $from or range of songs at $start:$end to $to in the playlist if $to starts 
    with + || -, it is relative to current song, +0 moves to right after current song, -0 moves to right before current song

moveid {$from}{$to}
    moves song $from to $to in playlist, if $to starts with + || -, it is relative to current song,
    +0 moves to right after current song, -0 moves to right before current song

playlistfind {$filter}[sort {$type}][window{$start:$end}]
    search the queue for songs matching $filter, sort sorts the result by specified tag, sort is descending if prefixed by -, only first tag
    will be used if multiple of the same type exist, use the -sort tags, they will fall back to the regular ones if a -sort doesn't exist
    window can be used to query only a portion of the real reaponse, the parameter is two zero based queue positions, a start index (inclusive)
    and an end index (exclusive) end index can be omitted meaning range is open-ended

playlistid {$songid}
    displays a list of songs in playlist, $songid is optional and specified a single song to display info for

playlistinfo [[$songpos]|[$start:$end]]
    displays a list of all songs in the playlist or only information for $songpos or the range specified by the range argument

playlistsearch {$filter}[sort {$type}][window {$start:$end}]
    search queue for songs matching $filter, parameters have same meaning as for find except that search is NOT case sensitive

plchanges {$version}[$start:$end]
    displays changed songs currently in playlist since $version start and end positions may be give to limit output to changes in given range
    to detect songs that were deleted at the end of the playlist use playlistlength returnd by status command

plchangesposid {$version}[$start:$end]
    displays changed songs currently in playlist since $version, function only returns plsition and id of changed song, not complete metadata
    to detect songs that were deleted at the end of the playlist, use plalistlength returned by status command

prio {$priority} {$start:$end...}
    set the priority of specified songs, higher prio means it will be played first when random is enabled, prio is and integer between 0 and 255, default is 0

prioid {$priority}{$id...}
    set prio of specified songs, higher prio means it will be played first when random is enabled, prio is integer between 0 and 255, default is 0

rangeid {$id}{$start:$end}
    specifies portion of song that will be played, $start and $end are offsets in seconds, fractions allowed, both are optional ommitting both
    plays everything, song currently playing can not be manipulated by this command

shuffle [$start:$end]
    shuffles the queue $start:$end is optional

swap {$song1}{$song2}
    swap positions of $song1 and $song2

swapid {$song1}{$song2}
    swaps positions of $song1 and $song2 (referred to by ids)

addtagid {$songid}{$tag}{$value}
    add tag to specified song, editing tags is only possible for remote songs, change is volatile, may be overwritten by tags received from server
    data is removed when song gets removed from queue

cleartagid {$songid}[$tag]
    removes tags from specified song, if $tag isn't specified, removes all tags, editing song tags is only possible for remote songs

just get the rest from the manual i don't wanna type any more of this
    
