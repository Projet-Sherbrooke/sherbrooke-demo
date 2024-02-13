all: unix_server tcp_server #unix_server_poll

tcp_server: tcp_server.c
	gcc -g -O0 tcp_server.c -otcp_server

unix_server: unix_server.c
	gcc -g -O0 unix_server.c -ounix_server

#unix_server_poll: unix_server_poll.c
#	gcc -g -O0 unix_server_poll.c -ounix_server_poll

.PHONY: clean

clean:
	-rm unix_server tcp_server #unix_server_poll