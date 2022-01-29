# FreezeRender
A software renderer written by c++2x.


## Platform
Windows only. with visual studio 2019.


## Feature
* Homogeneous clipping.  
* Back-face culling.  
* Perspective correct interpolation.  
* Implemention( SIMD ) of "[Accelerated __Block-based__ Half-Space Triangle Rasterization](http://acta.uni-obuda.hu//Mileff_Nehez_Dudra_63.pdf)".  **(new)**  
* Blinn–Phong reflection model.  
* 4 texture address mode (warp, mirror, clamp, border).  
* 2 texture filter mode (nearest, biilinear).  
* Multi-thread rasterizer. **(new)**  
* [Visibility buffer](https://jcgt.org/published/0002/02/04/). **(new)**  
* Deferred shading.  **(new)**  

## Todo list
* PBR material system.  
* Cubemap.  
* ECS.  
* Software ray tracing.  
