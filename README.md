# Tracer
This is a simple path tracer project done mainly for education purposes and practice in data-oriented design.

Currently in a very early stage.

## Status
[![Build Status](https://travis-ci.com/shchavinskyi/Tracer.svg?branch=master)](https://travis-ci.com/shchavinskyi/Tracer) 
[![Build status](https://ci.appveyor.com/api/projects/status/v5s6l9vr64wghu8k?svg=true)](https://ci.appveyor.com/project/shchavinskyi/tracer)

## Output examples
| Settings              |                     Cornell Box                     |               Random Spheres                 |
| :-------------------: | :-------------------------------------------------: | :------------------------------------------: |
| spp 200, bounces 5    |  ![image](./output/cornell_box_400x400_200_5.png)   |    ![image](output/random_400x400_200_5.png) |
| spp 500, bounces 10   |  ![image](./output/cornell_box_400x400_500_10.png)  |   ![image](output/random_400x400_500_10.png) |
| spp 2000, bounces 10  | ![image](./output/cornell_box_400x400_2000_10.png)  |  ![image](output/random_400x400_2000_10.png) |
| spp 10000, bounces 10 | ![image](./output/cornell_box_400x400_10000_10.png) | ![image](output/random_400x400_10000_10.png) |

## Todo
- [ ] Mesh loading and rendering
- [ ] Bounding volume hierarchy
- [ ] HDRi environments
- [ ] Improve sampling 
- [ ] Noise filtering

## 3rd party libraries
* stb\_image.h by Sean Barrett: https://github.com/nothings/stb/blob/master/stb_image.h
* Doctest by Viktor Kirilov https://github.com/onqtam/doctest
* GLM https://github.com/g-truc/glm
