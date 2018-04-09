#!/bin/sh

#  buildandrun.sh
#  
#
#  Created by Marc-Emmanuel PERRIN on 27/03/2015.
#
#!/bin/bash
#Replace FILE and FILE2 by program arguments if needed
FILE=ART2WebSocket
FILE2=ART2WebSocket.dSYM
PORT=9012
if [ -f $FILE ];
then
    echo "File $FILE exists - Removing"
    rm $FILE
    echo "File $FILE2 exists - Removing assembly"
    rm -rf $FILE2
fi
echo "Building $FILE & $FILE2 from Makefile"
make
echo "Launching ART2WebSocket on port: $PORT "
./run.sh $PORT