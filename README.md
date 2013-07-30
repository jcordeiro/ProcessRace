ProcessRace
===========

A small C program I had to write for my OS class that pits 3 processes against each other in a race

There are three processes, red, green, and blue, that are all writing to a named pipe. A fourth process reads from the pipe and tallies up the totals, printing out the progress of the race every second. After 5 seconds the pipe is closed and totals are printed. 

```shell

$ ./ProcessRace 
Pid 71391 (red) started
Pid 71392 (green) started
Pid 71393 (blue) started

Red is ahead with 1, Blue is behind wth 0, Blue is last with 0!
Red is ahead with 74787, Blue is behind wth 67869, Green is last with 58753!
Red is ahead with 142346, Green is behind wth 136039, Blue is last with 123768!
Red is ahead with 227397, Green is behind wth 216379, Blue is last with 204687!
Green is ahead with 306955, Red is behind wth 304765, Blue is last with 282990!

red: I was doing step number 394935, I hope I won!
green: I was doing step number 391462, I hope I won!
blue: I was doing step number 355408, I hope I won!
```
