# COMP3015 70PCNT
 comp3015 part two (70pcnt)

WORK IN PROGRESS
"
This project was created in Windows 10 (Version 10.0.19045 Build 19045) using the software Visual Studio 2022 (Version 17.9.3). The code is broken into different files. The .frag, .vert and .gs files act as the shaders. Frag is broken down into declaring the values of mainly diffuse, specular and ambient values of the different light sources and materials, which are then used in the phong lighting shaders. The spot and Ambient phong shaders are then used to calculate the colour of the scene in the main() function. The scenebasic_uniform.cpp and scenebasic_unform.h are where the initialisation and rendering is done. the header file is used to initialise all the models and assets used in the .cpp file the .cpp file compiles initialises and animates the assets so that in render they can make use of the phong shader by inserting set diffuse, specular and ambient values to create a convincing scene. Main.cpp runs the scene so that it can be visualised.

In terms of construction. I've used toruses of different sizes to create the lava pool then a multilight system to create the emerging lava and finally a rotating spotlight to simulate moving lava. As well as the sin() wave on the Y translation of the skeleton model to simulate bobbing up and down

I've used custom skeleton mesh, called Skelly.obj. I've used BlinnPhong ambient shader lighting multiple times for multilight as well as a blinnphong spot shader light that is animated to rotate. I've also used a texture as well as texture mixing and alpha to decorate the scene using cement.jpg and fire.png. I've used fog and toon shading to improve the visual look of the scene. And finally i've incorporated a skybox using the templates HDRpisa although it's obscured by the fog

Youtube link: BLANK 
Model Skeleton used: https://www.turbosquid.com/3d-models/3d-model-lowpolyskeletonrigged-1970201
"
