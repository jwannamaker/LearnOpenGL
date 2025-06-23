# LearnOpenGL

## Getting Started

### Hello Triangle 

My first triangle!

![My first triangle](https://github.com/user-attachments/assets/96697d61-5c49-4dba-b90b-f83031987b65)

*Note:* I wanted to use my own colors, so I used [this](https://lospec.com/palette-list/sunnyswamp) palette. I enjoy the fact I can open an image and pin the Properties tab in Visual Studio, then select the eyedropper tool to see the RGBA components of each color in the palette. From there, I calculated the normalized values of each component (i.e. input an integer in [0, 255] --> output a corresponding float in [0.0, 1.0]), then truncated the results past the hundredths. I believe I could improve this process by taking advantage of the ```normalized: (bool)``` parameter of ```glVertexAttribPointer```, so I wouldn't have to do that calculation before inputting my desired colors.

---

My first rectangle, drawn using ```glDrawElements```.

![My first rectangle](https://github.com/user-attachments/assets/48dbbb2f-08a8-477d-baba-b78246e884ad)

*Note:* I'm excited about the connection between working with *.obj files in previous projects, and using Element Buffer Objects because referencing the indices of each face by the order they appeared in the first array feels familiar for me. I learned that the storage savings by using EBOs (as opposed to storing duplicate vertices in adjacent triangles/faces) can be as great as 50%

---

Wireframe of my first rectangle, drawn using ```glPolygonMode```.

![My first rectangle, in wireframe](https://github.com/user-attachments/assets/9c1ade1e-69f9-4277-a177-6653dc0642d5)

---

#### Excercises

1. Try to draw 2 triangles next to each other using ```glDrawArrays``` by adding more vertices to your data.

![Excercise 1 from end of Hello Triangle](https://github.com/user-attachments/assets/1f486ffa-1754-47e1-990d-97406d0b5935)

---

2. Now create the same 2 triangles using two different VAOs and VBOs for their data.

![Excercise 2 from end of Hello Triangle](https://github.com/user-attachments/assets/c1b6d600-a11a-46d0-90bb-df3b1650e2f3)

---

3. Create two shader programs where the second program uses a different fragment shader that outputs the color yellow; draw both triangles again where one outputs the color yellow.

![Two triangles with two different shader programs](https://github.com/user-attachments/assets/67acae05-d8e1-4b11-a6ab-3ff5ef4997ca)

*Note:* Again, I use different colors than what's asked for on [learnopengl.com](https://learnopengl.com/Getting-started/Hello-Triangle), simply because I found it to be more aesthetically pleasing and the process was nearly identical.

---

### Shaders

Using a uniform to set the color the fragment shader outputs, and updating it over time.

[Two triangles shifting colors between red, purple, and blue](https://github.com/user-attachments/assets/4ed11f58-051a-4c51-93f9-bdfb0d9eced8)

*Note:* In the example, there's only one triangle and one component of the color that changes depending on the time. I thought it would be more visually interesting to instead use two triangles, and change both the red and blue components of their color. The graphs of each triangle's color plotted over time look like this:

![Color graph of the left triangle](https://github.com/user-attachments/assets/9f273239-1243-48f5-b3e9-2acfad19ea71)

![Color graph of the right triangle](https://github.com/user-attachments/assets/538b9bdc-6ee4-4c08-8bad-858272be1793)

In code, I accomplished this effect by swapping the red and blue component passed to the second fragment shader via uniform like so:

```c++
float timeValue;
float redValue;
float blueValue;
int vertexColorLocation;
while (/* window is open */)
{
    [ ... ]
    timeValue = glfwGetTime();
    redValue = (0.5f * cos(timeValue)) + 0.5f;	// scale & translate cos(x) --> range [0.0f, 1.0f]
    blueValue = (0.5 * cos(timeValue + M_PI)) + 0.5f; // add PI to make the blueValue out of phase with redValue

    vertexColorLocation = glGetUniformLocation(shaderProgram, "customColor");
    glUseProgram(shaderProgram);

    glUniform4f(vertexColorLocation, redValue, 0.0f, blueValue, 1.0f);
    // bind the first triangle's VAO & call draw function

    glUniform4f(vertexColorLocation, blueValue, 0.0f, redValue, 1.0f);
    // bind the second triangle's VAO & call draw function
    [ ... ]
}
```

---

Adding more information to the array representing the triangle by including colors corresponding to each vertex. 

![Two triangles with red, blue, and green blended throughout them](https://github.com/user-attachments/assets/f6f06c0b-ae50-4422-a531-02e84dc04707)

*Note:* It looks smoothly blended because during the rasterization stage of rendering, the colors are blended because of fragment interpolation.

---

Drawing a triangle after implementing a Shader class to manage compiling, linking, and error checking the vertex shader, fragment shader, and shader program.

![One triangle with red, blue, and green blended throughout it](https://github.com/user-attachments/assets/6e32bdef-dbc8-4678-b18c-be0eb5a68e3c)

---

#### Excercises

1. Adjust the vertex shader so that the triangle is upside down.

![One triangle, upside down](https://github.com/user-attachments/assets/735d0d7a-ea40-46d8-a7cd-38e723e467e4)

*Note:* To do this, I modified the output vector of the vertex shader to have a negative y-value, which flips all the vertices across the x-axis (i.e. input ```[x, y, z]``` --> output ```[x, -y, z]```. This works without an additional translation of the vertices because NDC places (0, 0) directly in the center of the canvas. 

---

2. Specify a horizontal offset via a uniform and move the triangle to the right side of the screen in the vertex shader using this offset value.

[One triangle, horizontally offset by a uniform](https://github.com/user-attachments/assets/3d35aca5-3e9e-4d2f-ba25-d6114920c683)

*Note:* I thought it would be more interesting to make the triangle move back and forth between the right and left sides of the screen. I accomplished this with the following:

```c++
float horizontalOffset;
while (/* window is open */)
{
    [ ... ]
    myShader.use();
    horizontalOffset = sin(glfwGetTime()) / 2.0f;
    myShader.setFloat("horizontalOffset", horizontalOffset);
    [ ... ]
}
```

---

3. Output the vertex position to the fragment shader using the ```out``` keyword and set the fragment's color equal to this vertex position (see how even the vertex position values are interpolated across the triangle). Once you managed to do this; try to answer the following question: why is the bottom-left side of our triangle black?

![One triangle, color equal to vertex position](https://github.com/user-attachments/assets/04bcef49-b885-4eef-b0e8-54c96610a303)

*Note:* The 'color' in the bottom left side of the triangle is ```[-0.43f, -0.25f, 0.0f]```, although the values for colors are only accepted in the range [0.0, 1.0] which means any negative values would simply be clamped to the closest value in the range before rendering. Effectively, ```[-0.43f, -0.25f, 0.0f]``` becomes ```[0.0f, 0.0f, 0.0f]``` (black).


