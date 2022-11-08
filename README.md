## ClockSynchronization

### Cristian

#### Usage

Build
```
make
```

Open terminal1
```
./server
```

Open terminal2
```
./client
```

### Berkeley

#### Usage

Build
```
make all
```

Open terminal1
```
./server 5333 server1
```
Open terminal2
```
./server 6444 server2
```
Open terminal3
```
./server 7555 server3
```
Open terminal4
```
./daemon input.csv
```

If you don't want to open multiple terminals, you can write your own script.

### NTP

Build 
```
mkdir build && cd build
cmake ..
make
```

Usage
```
./ntp_example 0.pool.ntp.org 123
```