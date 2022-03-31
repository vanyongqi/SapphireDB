AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=sapphiredb sdb
sapphiredb_SOURCES=\
	pmd/pmdMain.cpp pmd/pmdTcpListener.cpp \
	bson/src/bsonobj.cpp bson/src/util/json.cpp bson/src/oid.cpp \
	bson/src/lib/base64.cpp bson/src/lib/md5.cpp bson/src/lib/nonce.cpp \
	oss/ossSocket.cpp 
sdb_SOURCES=\
	client/edb.cpp client/command.cpp client/commandFactory.cpp \
	bson/src/bsonobj.cpp bson/src/util/json.cpp bson/src/oid.cpp \
	bson/src/lib/base64.cpp bson/src/lib/md5.cpp bson/src/lib/nonce.cpp \
	oss/ossSocket.cpp 

sapphiredb_CXXFLAGS=-I../boost -Ibson/src -Iinclude -D_FILE_OFFSET_BITS=64 -ggdb -Wall -O0
sapphiredb_LDADD=-lpthread -lm -lboost_system -lboost_thread -lboost_program_options -lrt
sapphiredb_LDFLAGS=-fPIC -rdynamic -L../boost/stage/lib -pthread

sdb_CXXFLAGS=-I../boost -Ibson/src -Iinclude -D_FILE_OFFSET_BITS=64 -ggdb -Wall -O0
sdb_LDADD=-lpthread -lm -lboost_system -lboost_thread -lrt
sdb_LDFLAGS=-fPIC -rdynamic -L../boost/stage/lib 