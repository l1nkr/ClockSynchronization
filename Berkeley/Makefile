# Makefile for the socket programming example
#
master_object  = TimeDaemon.o Socket.o TimeDaemonMain.o 
slave_object = Slave.o Socket.o SlaveMain.o 

all : daemon server

daemon: $(master_object)
	g++ -pthread -o daemon $(master_object)

server: $(slave_object)
	g++ -pthread -o server $(slave_object)

Socket: Socket.cpp
TimeDaemon: TimeDaemon.cpp
Slave: Slave.cpp
daemon_main: TimeDaemonMain.cpp
server_main: SlaveMain.cpp

clean:
	rm -rf *.o daemon server
