# README #

### Summary ###

Listens for ART messages and dispatch them through websocket.

### Installation ###

Run the **install.sh** shell script for dependencies installation.

```
#!bash

sh install.sh
``` 

### Running the program ###

Setup the **ART2Websocket** through the run.sh shell script. The shell script runs ART2Websocket on host **pixls** with ports **5000** and **5001** by default. **ART2Websocket** accepts 4 parameters: the ART tracking system hostname, the incoming and outgoing port for ART communication and the web socket port in this order.

```
#!bash

sh run.sh <websocketPort>
```