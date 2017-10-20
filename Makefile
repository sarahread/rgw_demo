#Outreachy: ceph - radosgw-admin application
#Boost::program_options demo

PROG = radosgw-admin
CC = g++

CPPFLAGS =
LDFLAGS = -lboost_program_options
SRC = rgw_demo.cc

$(PROG) :
	$(CC) $(CPPFLAGS) -o $(PROG) $(SRC) $(LDFLAGS)

clean: 
	rm $(PROG)