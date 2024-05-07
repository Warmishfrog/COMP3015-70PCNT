# COMP3015 70PCNT
 comp3015 part two (70pcnt)

WORK IN PROGRESS

This project was created in Windows 10 (Version 10.0.19045 Build 19045.4291) using the software Visual Studio 2022 (Version 17.9.3). 
---------------------------------------------------------------
The code works by making use of a variety of shaders that use .vert, .frag, and in some cases, .gs files to function. The three shaders used are prog, spriteProg and PBRprog. They make up different techniques for implementing general shading, sprites and physical-based rendering (PBR). The shaders are then used in the main scenebasic_uniform.cpp script to calculate the vertices, rendering and light interaction of objects established in the scene. The shaders can be used simultaneously by compiling the shaders separately and then evoking the shader to be used when it’s needed. the header file is used to initialise all the models and assets used in the .cpp file so the .cpp file compiles initialises and animates the assets
In terms of construction, I have implemented point sprites around the playable area that act as little fire particles to fit the theme of the world. Additionally, I have made use of a PBR shader to affect and light the skeleton model to illuminate it from the red-hot lava. Furthermore, I’ve included camera controls that allow the user to use a mouse and keyboard to move and look around. With some gamification to simulate the player walking around in an enclosed space. I feel this makes the program special as it enforces the pre-existing idea of a hellish landscape that is hot and unwelcoming. 
---------------------------------------------------------------
The process originated from my first submission (COMP3015 30 PERCENT) and I’ve expanded upon it by adding the following
•	Point Sprites
•	Physical Based Rendering (PBR)
•	Camera Controls (Mouse and Keyboard)
o	W, A, S, D for directional movement
o	Mouse for pitch and yaw movement
o	Arrow keys for pitch and yaw movement
•	Gamification (World Border & Player View)
YouTube link: BLANK 
Model Skeleton used: https://www.turbosquid.com/3d-models/3d-model-lowpolyskeletonrigged-1970201
Github: https://github.com/Warmishfrog/COMP3015-70PCNT


