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

For `Linux` environments, installing the necessary tools and libraries is easy. However, before running any commands in `bash`, make sure to run the command:

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

### Running
First run the chat server on an unused port of the server machine. The server also takes an HTML file that it will server to standard http requests for `index.html`.

```
bin/chatserver 4000 ../web-socket-networking/webchat.html
```

In separate terminals, run multiple instances of the chat client using:

```
bin/chatclient localhost 4000
```

This will connect to the given port (4000 in this case) of the local machine. Connecting to a remote machine can be done by explicitly using the remote machine's IP address instead of `localhost`. Inside the chat client, you can enter commands or chat with other clients by typing text and hitting the ENTER key. You can disconnect from the server by typing `quit`. You can shut down the server and disconnect all clients by typing `shutdown`. Typing anything else will send a chat message to other clients.

A browser based interface can be accessed by opening the URL `http://localhost:4000/index.html`. The server will respond with the specified web page above. By clicking `Connect`, the page gains access to chat on the server via web sockets in browsers that support web sockets.