The compile line is:

        gcc -o output file.c -lsocket -lnsl

Streamread is the server and streamwrite is the client.

Run the programs in separate windows.

If you start streamread, it will print out a socket number.
Then it will wait.

Then you can run streamwrite.  The command line to run this will be:
        streamwrite  borg  socket_number_from_streamread

The rebind_socket message is to free up a socket number immediately rather
than letting it time out.

