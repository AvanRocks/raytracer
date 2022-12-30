# raytracer

A simple raytracer written in C++.

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
