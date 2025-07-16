# Introduction

My name is Johnny Wannamaker, and this repository is a showcase of my progress on my journey to become a Graphics Programmer. I'm reading [learnopengl.com](learnopengl.com), which is a free, online book with readily digestible lessons, examples, and excercises that makes it a popular introduction to the realm of graphics programming. Below, you'll find section headers link to their corresponding sections in the book, headers to indicate whether I'm commenting/interacting with the main content of a section, or working through excercises from the end of the section.

---

# Getting Started

## [Hello Triangle](https://learnopengl.com/Getting-started/Hello-Triangle)

### Notes

<img src="/assets/images/hello-triangle-notes-1.png" alt="My first triangle"/> 

*Note:* I wanted to use my own colors, so I used [SunnySwamp](https://lospec.com/palette-list/sunnyswamp) by [Anubi](https://lospec.com/anubiarts).

<img src="/assets/images/hello-triangle-notes-2.png" alt="My first rectangle"/> 

My first rectangle, drawn using `glDrawElements`.

<img src="/assets/images/hello-triangle-notes-3.png" alt="My first rectangle, in wireframe mode"/> 

Wireframe of my first rectangle, drawn using `glPolygonMode`.

---

### Excercises

<img src="/assets/images/hello-triangle-ex-1.png" alt="Excercise 1 from Hello Triangle"/> 

**1. Try to draw 2 triangles next to each other using `glDrawArrays` by adding more vertices to your data.**

<img src="/assets/images/hello-triangle-ex-2.png" alt="Excercise 2 from Hello Triangle"/> 

**2. Now create the same 2 triangles using two different `VAO`s and `VBO`s for their data.**

<img src="/assets/images/hello-triangle-ex-3.png" alt="Excercise 3 from Hello Triangle"/> 

**3. Create two shader programs where the second program uses a different fragment shader that outputs the color yellow; draw both triangles again where one outputs the color yellow.**

---

## [Shaders](https://learnopengl.com/Getting-started/Shaders)

### Notes

Graph of the red and blue components of the left triangle:
<img src="/assets/images/shaders-notes-1-1-graph.png" alt="Color graph of the left triangle"/> 

Graph of the red and blue components of the right triangle: 
<img src="/assets/images/shaders-notes-1-2-graph.png" alt="Color graph of the right triangle"/> 

Video of two triangles shifting colors: 
<a href="/assets/videos/shaders-notes-2-video.mp4" title="Watch video">
  <img src="/assets/images/shaders-notes-2.png" alt="Video: Two triangles shifting colors between red, purple, and blue"/>
</a> 

Using a uniform to set the color the fragment shader outputs, and updating it over time. 

*Note:* I thought it would be more visually interesting to use two triangles, and change both the red and blue components of their color. 

In code, I accomplished this effect by swapping the red and blue component passed to the second fragment shader via uniform. 

```c++
float timeValue, redValue, blueValue;
int vertexColorLocation = glGetUniformLocation(shaderProgram, "customColor");
while (/* window is open */)
{
    [ ... ]
    timeValue = glfwGetTime();
    // scale & translate cos(x) --> range [0.0f - 1.0f]
    redValue = (0.5f * cos(timeValue)) + 0.5f;

    // add PI to make blueValue out of phase with redValue
    blueValue = (0.5f * cos(timeValue + M_PI)) + 0.5f;
    glUseProgram(shaderProgram);

    glUniform4f(vertexColorLocation, redValue, 0.0f, blueValue, 1.0f);
    // bind the first triangle's VAO & call draw function
    glUniform4f(vertexColorLocation, blueValue, 0.0f, redValue, 1.0f);
    // bind the second triangle's VAO & call draw function
    [ ... ]
}
```

<img src="/assets/images/shaders-notes-2.png" alt="Two triangles with red, blue, and green blended throughout them"/> 

Adding more information to the array representing the triangle by including colors corresponding to each vertex. It looks smoothly blended because during the rasterization stage of rendering, the colors are blended because of fragment interpolation.

<img src="/assets/images/shaders-notes-3.png" alt="One triangle with red, blue, and green blended throughout it"/> 

Drawing a triangle after implementing a Shader class to manage compiling, linking, and error checking the vertex shader, fragment shader, and shader program as well as setting uniforms.

---

### Exercises

<img src="/assets/images/shaders-ex-1.png" alt="One triangle, upside down"/> 

**1. Adjust the vertex shader so that the triangle is upside down.** 

*Note:* I modified the output vector of the vertex shader to have a negative y-value, which flips all the vertices across the x-axis (i.e. input ```(x, y, z)``` --> output ```(x, -y, z)```). This works without an additional translation of the vertices because NDC places ```(0, 0)``` directly in the center of the canvas.

Video of a triangle moving left and right: 
<a href="/assets/images/shaders-ex-2-video.mp4" title="Watch video">
  <img src="/assets/videos/shaders-ex-2.png" alt="Video: One triangle, horizontally offset by a uniform"/>
</a> 

**2. Specify a horizontal offset via a uniform and move the triangle to the right side of the screen in the vertex shader using this offset value.** 

*Note:* I thought it would be more interesting to make the triangle move back and forth between the right and left sides of the screen.  

I accomplished this with the following code: 

```c++
float horizontalOffset;
while (/* window is open */)
{
    [ ... ]
    myShader.use();
    horizontalOffset = sin(glfwGetTime()) / 2.0f;
    myShader.setFloat("horizontalOffset", horizontalOffset);`
    [ ... ]
}
```

<img src="/assets/images/shaders-ex-3.png" alt="One triangle, color equal to vertex position"/> 

**3. Output the vertex position to the fragment shader using the ```out``` keyword and set the fragment's color equal to this vertex position (see how even the vertex position values are interpolated across the triangle). Once you managed to do this; try to answer the following question: why is the bottom-left side of our triangle black?** 

The 'color' in the bottom left side of the triangle is ```(-0.43f, -0.25f, 0.0f)```, although the values for colors are only accepted in the range ```[0.0 - 1.0]``` which means any negative values would simply be clamped to the closest value in the range before rendering. Effectively, ```(-0.43f, -0.25f, 0.0f)``` becomes ```(0.0f, 0.0f, 0.0f)``` (black).

---

## [Textures](https://learnopengl.com/Getting-started/Textures)

### Notes

<img src="/assets/images/textures-notes-1.png" alt="Sunset gradient rectangle"/> 

Playing with the colors from the Sunny Swamp palette to create a sunset gradient.

<img src="/assets/images/textures-notes-2.png" alt="Sunset gradient rectangle, colors slightly different from before"/> 

I changed the way I pass colors into the fragment shader, so now I make use of the ```normalize``` function and pass each color component as a float ```[0.0f - 255.0f]```. It's interesting to see how much the colors change between my rounded off values and the precise ones.

<img src="/assets/images/textures-notes-3.png" alt="A rectangle with the container image rendered onto it"/> 

My first texture!

<img src="/assets/images/textures-notes-4.png" alt="A rectangle with the container image and a rainbow gradient blended"/> 

Modifying the fragment shader to output a mix between the vertex colors and the texture color by multiplying the two values. 

```c++
FragColor = texture(myTexture, TexCoord) * vec4(myColor, 1.0);
```

<img src="/assets/images/textures-notes-5.png" alt="A rectangle with a container image and an upside down happy face on it"/> 

Renderinng two textures onto the same target object.

<img src="/assets/images/textures-notes-6.png" alt="A rectangle with a container image and a rightside up happy face on it"/> 

After flipping each image vertically with: 

```c++
stbi_set_flip_vertically_on_load(true);
```

---

### Exercises

<img src="/assets/images/textures-ex-1-1.png" alt="Incorrect upside down and mirrored smiley"/> 

<img src="/assets/images/textures-ex-1-2.png" alt="Correctly flipped smiley face"/> 

**1. Make sure only the happy face looks in the other/reverse direction by changing the fragment shader.** 

My first attempt: 

```c++
FragColor = mix(texture(texture0, TexCoord), texture(texture1, -TexCoord), 0.2);
``` 

Appears correct since the image is flipped, although it's flipped across both axes instead of one, and upon further examination, the texture coordinates are technically no longer in the valid range for this approach. I suspect the only reason I still see the image is because of the texture wrapping parameters I have set to be repeating. I should be passing coordinates between ```0.0f``` and ```1.0f``` for both the x and y components, so first a vertical flip would give ```(x, -y)``` with x staying in the valid range while y now exists between ```-1.0f``` and ```0.0f```, so an additional translation is needed by ```(x, 1 - y)```. 

The correct solution now becomes: 

```c++
FragColor = mix(texture(texture0, TexCoord), texture(texture1, vec2(TexCoord.x, 1 - TexCoord.y)), 0.2);
```

<img src="/assets/images/textures-ex-2-1.png" alt="Four happy faces and four containers"/> 

<img src="/assets/images/textures-ex-2-2.png" alt="Four happy faces in a single container"/> 

<img src="/assets/images/textures-ex-2-3.png" alt="One container clamped at edge"/> 

<img src="/assets/images/textures-ex-2-4.png" alt="Mirrored faces and containers"/> 

**2. Experiment with the different texture wrapping methods by specifying texture coordinates in the range ```0.0f``` to ```2.0f``` instead of ```0.0f``` to ```1.0f```. See if you can display 4 smiley faces on a single container image clamped at its edge. See if you can experiment with other wrapping methods as well.** 

Four happy faces and four containers: 

```c++
FragColor = mix(texture(texture0, 2 * TexCoord), texture(texture1, 2 * TexCoord), 0.2);
```

Four happy faces in a single container: 

```c++
FragColor = mix(texture(texture0, TexCoord), texture(texture1, 2 * TexCoord), 0.2);
```

Four happy faces and one container clamped at its edge: 
```c++
// In main.cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
```

Four mirrored happy faces and four mirrored containers

<img src="/assets/images/textures-ex-3-1.png" alt="Magnified center pixels"/> 

<img src="/assets/images/textures-ex-3-2.png" alt="Zooming in more"/> 

<img src="/assets/images/textures-ex-3-3.png" alt="Maximum pixel clarity"/> 

**3. Try to display only the center pixels of the texture image on the rectangle in such a way that the individual pixels are getting visible by changing the texture coordinates. Try to set the texture filtering method to ```GL_NEAREST``` to see the pixels more clearly.** 

Final texture coordinates: 

```c++
0.51f, 0.51f,	// top right
0.49f, 0.51f,	// top left
0.49f, 0.49f,	// bottom left
0.51f, 0.49f,	// bottom right
```

Video of happy face and container changing visibility with arrow keys: 

<a href="/assets/videos/textures-ex-4-video.mp4">
  <img src="/assets/images/textures-ex-4.png" alt="Video: Using a uniform to vary visibility"/>
</a>

**4. Use a uniform variable as the mix function's third parameter to vary the amount the two textures are visible. Use the up and down arrow keys to change how much the container or the smiley face is visible.**

---

## [Transformations](https://learnopengl.com/Getting-started/Transformations)

### Notes

Video of rotating container with left/right arrow keys: 

<a href="/assets/videos/transformations-notes-1-video.mp4">
  <img src="/assets/images/transformations-notes-1.png" alt="Video: Matrix transformations to rotate the container"/>
</a> 

Playing with matrix transforms using [```glm```](https://github.com/g-truc/glm), a C++ header-only library for matrix, vector, and quaternion operations that are compatible with OpenGL. 

In the main render loop, I added tests for right and left arrow key presses, so the container rotates clockwise (right arrow) or counterclockwise (left arrow). 

```c++
while (/* window is open */)
{
    [ ... ]
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        transformMatrix = glm::rotate(transformMatrix, glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        transformMatrix = glm::rotate(transformMatrix, glm::radians(-0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    glUniformMatrix4fv(transformMatrixLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
    [ ... ]
}
```

*Note:* I made the color at each vertex change according to its new transformed position by modifying the vertex shader to output the vertex position (```gl_Position```) as the color to the fragment shader. This has the effect of making the colors on the container almost seem like lighting instead of a stationary filter that stays fixed on the same vertices. As the container rotates, the color at each vertex changes.

Video of two koi fish circling each other: 

<a href="/assets/videos/transformations-notes-2-1-video.mp4" alt="Video of two koi fish circling each other"/>
  <img src="/assets/images/transformations-notes-2-1.png">
</a> 

Frame from the scene that I was thinking of: 

<img src="/assets/images/transformations-notes-2-2.png" alt="Tui and La circling each other from ATLA"/> 

I thought of the animation of Tui and La circling each other in Avatar: The Last Airbender, then I decided to test my ability by recreating it. 

At first, I thought I would have to read ahead to the section on [instancing](https://learnopengl.com/Advanced-OpenGL/Instancing) in order to render the same texture on two different rectangles. I ended up rereading the transformations section again, until I realized that I could use the exact same rectangle, and simply transform the position and color before drawing it again. 

In the main render loop: 

```c++
while (/* window is open */)
{
    [ ... ]
    float t = glfwGetTime();
    transformMatrix = glm::mat4(1.0f);
    transformMatrix = glm::scale(transformMatrix, glm::vec3(0.5f, 0.5f, 1.0f));
    transformMatrix = glm::translate(transformMatrix, glm::vec3(sin(t), cos(t), 0.0f));
    transformMatrix = glm::rotate(transformMatrix, -t + glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(transformMatrixLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
    // draw the rectangle for the first koi fish

    transformMatrix = glm::mat4(1.0f);
    transformMatrix = glm::scale(transformMatrix, glm::vec3(0.5f, 0.5f, 1.0f));
    transformMatrix = glm::translate(transformMatrix, glm::vec3(sin(t + M_PI), cos(t + M_PI), 0.0f));
    transformMatrix = glm::rotate(transformMatrix, -t + glm::radians(-270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(transformMatrixLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
    // draw the rectangle for the second koi fish
    [ ... ]
}
```

Video of smiley face rotating in the corner of the window: 

<a href="/assets/videos/transformations-notes-3-video.mp4">
  <img src="/assets/images/transformations-notes-3.png" alt="Video of smiley face rotating in the corner of the window"/>
</a>

Rotating the smiley face over time in the corner of the window.

---

### Exercises

Video of smiley face moving in a circle: 

<a href="/assets/videos/transformations-ex-1.mp4">
  <img src="/assets/images/transformations-ex-1.png" alt="Video of smiley face moving in a circle"/> 
</a>

**1. Using the last transformation on the container, try switching the order around by first rotating and then translating. See what happens and try to reason why this happens.** 

This happens because matrix multiplication applies the visual transformations in right-to-left order, meaning the combined transformation matrix is first translating the image then rotating w.r.t. the origin ```(0.0, 0.0)```. Since the image is already at some coordinate ```(x, y)``` when the rotation occurs, it's no longer rotated on an axis that is centered on the image. 

In order to produce the same effect of rotating in place in the corner of the screen but still apply a rotation before a translation, it would be necessary to rotate the image relative to the translated location. For instance: 

```c++
smileyPosition = glm::vec2(0.75f, 0.75f);
transformMatrix = glm::rotate(transformMatrix, t, glm::vec3(smileyPosition, 1.0f));
transformMatrix = glm::translate(transformMatrix, glm::vec3(smileyPosition, 1.0f));
```

Video of two smiley faces in opposite corners, one shrinking/growing and the other rotating: 

<a href="/assets/videos/transformations-ex-2-video.mp4">
  <img src="/assets/images/transformations-ex-2.png" alt="Video of two smiley faces in opposite corners, one shrinking/growing and the other rotating"/>
</a>

**2. Try drawing a second container with another call to ```glDrawElements``` but place it at a different position using transformations only. Make sure this second container is placed at the top-left of the window and instead of rotating, scale it over time (using the ```sin``` function is useful here; note that using ```sin``` will cause the object to invert as soon as a negative scale is applied).

---

## [Coordinate Systems](https://learnopengl.com/Getting-started/Coordinate-Systems)

### Notes

<img src="/assets/images/coordinates-notes-1.png" alt="My first 3D render"/> 

My first 3D render!

Video of a cube rotating in a seemingly erratic, yet repeating manner: 

<a href="/assets/video/coordinates-notes-2-video.mp4" title="Watch video">
  <img src="/assets/images/coordinates-notes-2.jpg" alt="Video of a cube rotating in a seemingly erratic, yet repeating manner"/>
</a> 

My first cube! 

The reason why the cube looks off is because currently, OpenGL has no way to tell what should be in front and what should be in back, so the triangles are all drawn in the same order every frame making the cube look weird and very much like physics is broken. 

*Note:* I made the cube move in a more interesting pattern than what's demonstrated in the book by using ```sin``` and ```cos``` to determine the axis of rotation. 

In the main render loop: 

```c++
while (/* window is open */)
{
    [ ... ]
    float t = glfwGetTime();
    model = glm::mat4(1.0f);
    model = glm::rotate(model, t * glm::radians(50.0f), glm::vec3(0.25 * sin(t), 0.25 * cos(t), 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model);
    // draw the cube
    [ ... ]
}
```

Video of a cube rotating in a seemingly erratic, yet rotating manner: 

<a href="/assets/videos/coordinates-notes-3-video.mp4" title="Watch video">
  <img src="/assets/images/coordinates-notes-3.jpg" alt="Video of a cube rotating in a seemingly erratic, yet repeating manner"/>
</a> 

Here's how it looks after we let OpenGL know to actually perform depth testing. 

Accomplished with adding this at the beginning of ```main.cpp```: 

```c++
glEnable(GL_DEPTH_TEST);
``` 

And adding this to the render loop: 

```c++
glClear(GL_COLOR_BUFFER_BIT \| GL_DEPTH_BUFFER_BIT);
float t = glfwGetTime();
model = glm::mat4(1.0f);
model = glm::rotate(model, t * glm::radians(50.0f), glm::vec3(0.25f * sin(t), 0.25f * cos(t), 0.0f);
glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model));
// Draw the cube
```
