# Connect-K-AI

## Project Description

ConnectK is an enhanced version of Connect4 the board game. ConnectK's width (w), height (h), and K are all configurable. ConnectK is also more interesting as it supports 2 different gravity modes: ON | OFF. With gravity off, the pieces stay wherever they are placed. With gravity on, they fall till they meet another piece or hit the bottom.

##Getting Started

1. Running ConnectK.jar:

You can run it from the directory the file is in using the commandline/terminal with the command:

java -jar ConnectK.jar
Alternatively, you can simply doubleclick the jar to run it.

2. Playing Connect K with ConnectK.jar:

After starting up ConnectK, the game will default to a two player (human vs. human) game of connect 5.

You can change the settings of the game by selecting File->New from the drop down menu on the upper left. The width, height, K, and gravity of the board can be modified by changing the values in the 4 boxes labeled width, height, K, and gravity.
Additionally, you can choose to import AI opponents. 3 AI opponents are provided for you in the SampleAI directory. Click Add AI and navigate to 'ConnectKSource_java/SampleAI/AverageAI/AverageAI.class'. Click open, and new game to start a game against the average AI.

You can add multiple AIs and watch 2 of them play against eachother.
3. Writing an AI:

##C++ AIs:

ConnectK.cpp has main function to get a current state from the java shell. Please implement your AI agent in AIShell::makeMove(). AIShell::gameState has the current game state as array of integers. 0, 1 and -1 represent empty, AI piece and human piece respectively. Note that these integer values are different from the Java version.
Then compile your code and make an executable file (for example on Windows, myAI.exe). On Linux, open a terminal and type

g++ ConnectK.cpp Move.cpp AIShell.cpp -o myAI
Then you can test your AI agent by running ConnectK.jar with the following command. On Windows:

java -jar ConnectK.jar cpp:myAI.exe
##On Linux:

java -jar ConnectK.jar cpp:myAI
