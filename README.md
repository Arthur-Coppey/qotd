# qotd
A [RFC 865](https://datatracker.ietf.org/doc/html/rfc865) compliant Quote of the Day ~~daemon~~ program

## Done
* get random quote within file
* tcp server
* tcp client
* udp server
* udp client
* IPv6 compat

## Todo
* t h r e a d s to accept several clients and/or run both udp and tcp servers from same executable and instance
* macros (#define XXX yyy) for currently hard-coded values
* switches (-x) and options (--xxxx) ?
* logging ?
* conf files ?

## Installation

```sh
git clone https://github.com/Arthur-Coppey/qotd
cd qotd
cmake -B build
cd build
make
```

## Use
A "quotes.txt" file in the same folder as the server containing one quote per line. Quotes are separated by the line feed `\n` character.

Server as root (because port 17)
```sh
# ./server
$ ./client
```
