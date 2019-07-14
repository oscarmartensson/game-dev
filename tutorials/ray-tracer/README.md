This is my following of the "Ray tracing in one weekend" tutorial made by Peter Shirley. I have made some minor adjustments and tweaks,
but the tutorial can be found here: http://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf,
and the source code of the tutorial can be found here: https://github.com/petershirley/raytracinginoneweekend

This project can be downloaded and compiled in Visual Studio 2017. To run the code, open the ray-tracer.sln Visual Studio solution file and simply hit F5 for Debug or Release mode to build.
Both x64 and x86 has been tested and work. The rendered scene can be seen in a file called "scene.ppm" and will be located in the ray-tracer/ folder. The .ppm file can be viewed with for example File Viewer Plus, which is a free program.

To render a random scene with lots of spheres of different sizes and materials, keep the macro COVER_SCENE defined in main.cpp (recommended), otherwise a smaller scene with fewer objects will be created.
To render the scene in a higher resolution (1280x720), keep the HD_RESOLUTION macro defined in main.cpp. Commenting it out will render the scene at 200x100 pixels. NOTE! The scene can take between 20-30 mins to render (depending on processor power) if this macro is defined, but the result is much better.

Note! This has only been tested for Windows 10, but should work on other operating systems as well provided that you have Visual Studio 2017.
