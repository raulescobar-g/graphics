# Rasterizer from scratch

### A rasterizer in C++ from scratch with help from tiny object loader and stb_image_write to read .obj files and write to .png file type

How to run:

```
mkdir build
cd build
cmake ..
make -j4
./A1 <path to .obj file> <filename for output> <height> <width> <task#>
```

- Task number description:

  - 1 : only draws the bounding boxes of triangles in random colors without any Z-buffering
  - 2 : draws triangles in random colors without Z-buffering
  - 3 : draws triangles with baryocentric interpolation with random colors and without Z-buffering
  - 4 : draws traingles interpolated with z-depth, (Z-buffering implemented)
  - 5 :
  - 6 : draws object file with
  - 7 : draws object file with lighting from [1 , 1] direction

- Classes:

  - Image (given in starter code)

  - Triangle

    - Vertex
      - Position
      - Color
      - Normal vector
    - BoundingBox

  - Zbuffer
