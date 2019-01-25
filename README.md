# adventure2019

"Immersive Worlds" project for CMPT373 - Group Eierschecke

## Getting Started

These instructions will help you set up your environments to properly run the project on your local machine.

### Prerequisites

Here are some required things you need to get started:

```
1. Boost v.1.68 or greater
2. Cmake v3.12 or greater
3. Ncurses library
4. Build-essential tools (C++17)
```

### Tools

Here are some recommended tools that you can use to make your life easier


1. [SouceTree](https://www.sourcetreeapp.com) (Source control)
2. [CLion](https://www.jetbrains.com/student/) (C++ IDE & Free for University students)


### Installing
For `Mac` users, follow [this](https://coursys.sfu.ca/2019sp-cmpt-373-d1/discussion/topic/guideline-for-mac-users/) guideline provided by the instructor.

For `Windows 10` users, follow [this](https://coursys.sfu.ca/2019sp-cmpt-373-d1/discussion/topic/for-windows-users/) guideline provided by the instructor.

For `Linux` environments, installing the necessary tools and libraries is easy. However, before running any commands in `bash`, make sure idOfRoomDoorLeadsTo run the command:

```
sudo apt-get update
```

build-essentials (g++, gcc, make), run this command in bash:

```
sudo apt install build-essential
```

Ncurses library, run this command in bash:

```
sudo apt install libncurses5-dev
```

Boost library, run this command in bash:

```
sudo apt-get install libboost-all-dev
```

After that, just make sure that you have all the correct versions and you are good to go!

### Building

For this project, we will be utilizing the `Out-of-source' building method.

To build your project using bash:

```
1. `cd` out of the project's root directory.
2. `mkdir` a 'adventureBuild' directory
3. `cd` into the 'adventureBuild' directory
4. call `cmake ../adventure2019`
5. call `make`
```

After that, you're all set. Your executable files should be in the `adventureBuild/bin` directory.

`NEVER` build from the adventure2019 directory.

### Running
First run the game server on an unused port of the server machine. The server also takes an HTML file that it will server to standard http requests for `index.html`.

```
bin/gameserver 4000 ../adventure2019/webgame.html
```

In separate terminals, run multiple instances of the game client using:

```
bin/gameclient localhost 4000
```

This will connect to the given port (4000 in this case) of the local machine. Connecting to a remote machine can be done by explicitly using the remote machine's IP address instead of `localhost`. Inside the game client, you can enter commands or game with other clients by typing text and hitting the ENTER key. You can disconnect from the server by typing `quit`. You can shut down the server and disconnect all clients by typing `shutdown`. Typing anything else will send a game message to other clients.

A browser based interface can be accessed by opening the URL `http://localhost:4000/index.html`. The server will respond with the specified web page above. By clicking `Connect`, the page gains access to a game on the server via web sockets in browsers that support web sockets.

## Provisioning with Vagrant

If you are having troubles setting up cmake and boost, you may run a virtual machine that can build the project by Vagrant provisioning.

To get started, first install Vagrant at https://www.vagrantup.com/downloads.html

Then download and install VirtualBox at https://www.virtualbox.org/wiki/Downloads or the Ubuntu Software app if available.

You should then be able to use the ```vagrant up``` command within the project directory to begin initializing a virtual machine (VM).

This will first install the xenial64 box. This process will take some time to complete.

Afterwards, the machine will then provision the VM with the commands found in ```/cookbooks/baseconfig/recipes/default.rb```.

Once the provisioning has completed, use ```vagrant ssh``` to access the VM's command line interface.

The project built with cmake and make is found at ```/home/vagrant/build``` or ```~/build```.

You can run the gameserver with ```./bin/gameserver 3000 ../adventure2019/webgame.html``` and access the server (localhost:3000) with your browser.
**Note: Vagrant is configured to forward ports 3000 and 4000 to your computer, so be sure to only use these ports or reconfigure the VagrantFile and run ```vagrant reload```.**

Use the command ```exit``` to disconnect from the VM.

Other commands for Vagrant include:
  - ```vagrant provision``` - Runs the entire provisioning process (Chef Solo cookbook recipe) again
  - ```vagrant halt``` - Shuts down the VM
  - ```vagrant up``` - Starts the VM and provisions if the VM does not exist
  - ```vagrant destroy``` - Deletes the VM
