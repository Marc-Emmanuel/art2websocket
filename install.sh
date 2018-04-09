#!/bin/bash
# Check if websocketcpp already in the right location
# echo "Copying websocketpp to /opt/local/include"
# if [ ! -d /opt/local/include/websocketpp ];
# # Not there, install it
# then sudo cp -r websocketpp /opt/local/include/
# else echo "Websocketpp includes already installed, skipping"
# fi

# Check if boost has been installed
if [ ! -f /opt/local/lib/libboost_system.dylib ] && [ ! -d /opt/local/include/boost ];
# If not
then
	# Check if boost tarball has already been downloaded
	if [ ! -f ./boost_1_57_0.tar.gz ];
	# If not, download the archive
	then curl  -o boost_1_57_0.tar.gz -L "http://downloads.sourceforge.net/project/boost/boost/1.57.0/boost_1_57_0.tar.gz?r=&ts=1421621542&use_mirror=heanet"
	else echo "Boost tarball already downloaded, skipping";
	fi
	
	tar zxvf boost_1_57_0.tar.gz
	# Install boost
	cd boost_1_57_0 && ./bootstrap.sh --prefix=/opt/local/ && sudo ./b2 install && export DYLD_LIBRARY_PATH=.:/opt/local/lib && cd ..
else echo "Boost already installed, skipping"
fi
echo "Proceeding with make ..."
make
if [ "$?" == 0 ];
then echo "Looks like everything went fine. To run, use the run.sh script with the websocket port as argument."
else echo "Something wrong happened"
fi
