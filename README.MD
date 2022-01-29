# FreezeRender
A software renderer written by c++2x.


## Platform
Windows only. With Visual studio 2019.


## Feature
* Homogeneous clipping.  
* Back-face culling.  
* Perspective correct interpolation.  
* Implemention(SIMD version) of "[Accelerated Half-Space Triangle Rasterization](http://acta.uni-obuda.hu//Mileff_Nehez_Dudra_63.pdf)".  
* Blinn–Phong reflection model.
* 4 texture address mode (Warp, Mirror, Clamp, Border).  
* 2 texture filter mode (Nearest, Biilinear).  


## TODO list
* Deferred shading.  
* PBR material system.  
* Multi-thread rasterizer.  
* Cubemap.  
* ECS.  
* Software ray tracing.  


## TOFIX
* Pixel loss in specific view point.  
> Loss of floating precision? but "[Kahan Summation Algorithm](https://en.wikipedia.org/wiki/Kahan_summation_algorithm)" cannot fixed it, I'm not sure how to fixed it.
