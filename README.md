# raytracer

A simple raytracer written in C++.
Running the program generates a scene with randomly sized and randomly positioned spheres to demonstrate the raytracer.

## Usage

To compile:
```
$ g++ *.cpp -o raytracer
```

To run:
```
$ ./raytracer
```
A file named `image.ppm` will be created in the current directory.
You may need to convert this file to PNG format in order to view it with your image viewer.

## Advanced

### Seeds
Each time you run `./raytracer`, it outputs a seed corresponding to the scene.
You can save the seed and pass it as an argument to `./raytracer` to generate the same scene again.
For example, if the seed is `1672386510` then you can run the following command.
```
$ ./raytracer 1672386510
```

### Resolution
By default, the `image.ppm` file is in FHD resolution (1920x1080).
To set the resolution of the output file, open `raytracer.cpp` and uncomment the desired line near the top.
A few present resolutions are included in the comment of `raytracer.cpp`.
To set your own resolution, simply initialize the `width` and `height` variables as desired.
