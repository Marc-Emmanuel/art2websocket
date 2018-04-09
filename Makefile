all: ART2WebSocket

ART2WebSocket:
	c++ -g -Wall -Ofast -L/opt/local/lib -I . -ILib/ -I/opt/local/include -Iwebsocketpp/ Lib/*.cpp -lboost_system -std=c++11 Message.cpp MarkerMessage.cpp FingerMessage.cpp BodyMessage.cpp HandMessage.cpp ART2WebSocket.cpp -o ART2WebSocket

clean:
	rm ART2WebSocket
	rm -Rf ART2WebSocket.dSYM
	-@$(RM) *o depend-${SERVER} depend-${CLIENT1} depend-${CLIENT2} ${SERVER} ${CLIENT1} ${CLIENT2}  ${CLIENT3} core
  


###########################################

# Regles implicites

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c (CFLAGS) $(INCPATH) -o $@ $<




