# Getting Started

## Hello Triangle

| Notes | Visual |
|-------|--------|
| I wanted to use my own colors, so I used [this](https://lospec.com/palette-list/sunnyswamp) palette. | &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![My first triangle](https://github.com/user-attachments/assets/96697d61-5c49-4dba-b90b-f83031987b65 "My first triangle")&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; |
| My first rectangle, drawn using ```glDrawElements```. |&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![My first rectangle](https://github.com/user-attachments/assets/48dbbb2f-08a8-477d-baba-b78246e884ad "My first rectangle")&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; |
| Wireframe of my first rectangle, drawn using ```glPolygonMode```. |&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![My first rectangle, in wireframe](https://github.com/user-attachments/assets/9c1ade1e-69f9-4277-a177-6653dc0642d5)&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; | 

### Exercises

| Excercise | Visual |
|-----------|--------|
| **1.** Try to draw 2 triangles next to each other using ```glDrawArrays``` by adding more vertices to your data. | &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![Excercise 1 from Hello Triangle](https://github.com/user-attachments/assets/1f486ffa-1754-47e1-990d-97406d0b5935 "Excercise 1 from Hello Triangle")&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; |
| **2.** Now create the same 2 triangles using two different ```VAO```s and ```VBO```s for their data. | &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![Excercise 2 from Hello Triangle](https://github.com/user-attachments/assets/c1b6d600-a11a-46d0-90bb-df3b1650e2f3 "Excercise 2 from Hello Triangle")&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; |
| **3.** Create two shader programs where the second program uses a different fragment shader that outputs the color yellow; draw both triangles again where one outputs the color yellow. | &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![Excercise 3 from Hello Triangle](https://github.com/user-attachments/assets/67acae05-d8e1-4b11-a6ab-3ff5ef4997ca "Excercise 3 from Hello Triangle")&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; |

---

## Shaders

| Notes | Visual |
|-------|--------|
| Using a uniform to set the color the fragment shader outputs, and updating it over time. | [Video: Two triangles shifting colors between red, purple, and blue](https://github.com/user-attachments/assets/4ed11f58-051a-4c51-93f9-bdfb0d9eced8) |
| I thought it would be more visually interesting to use two triangles, and change both the red and blue components of their color. The graphs of each triangle's color plotted over time look like this: | ![Color graph of the left triangle](https://github.com/user-attachments/assets/9f273239-1243-48f5-b3e9-2acfad19ea71 "Color graph of the left triangle") ![Color graph of the right triangle](https://github.com/user-attachments/assets/538b9bdc-6ee4-4c08-8bad-858272be1793 "Color graph of the right triangle") |
| In code, I accomplished this effect by swapping the red and blue component passed to the second fragment shader via uniform like so: | ```float timeValue;``` <br>```float redValue;``` <br>```float blueValue;``` <br><br>```int vertexColorLocation;``` <br>```int vertexColorLocation;``` <br>```while (/* window is open */)``` <br>```{``` <br>&emsp;&emsp;```[ ... ]``` <br>&emsp;&emsp;```timeValue = glfwGetTime();``` <br><br>&emsp;&emsp;```// scale & translate cos(x) --> range [0.0f - 1.0f]``` <br>&emsp;&emsp;```redValue = (0.5f * cos(timeValue)) + 0.5f;``` <br><br>&emsp;&emsp;```// add PI to make blueValue out of phase with redValue``` <br>&emsp;&emsp;```blueValue = (0.5f * cos(timeValue + M_PI)) + 0.5f;``` <br><br>&emsp;&emsp;```vertexColorLocation = glGetUniformLocation(shaderProgram, "customColor");``` <br>&emsp;&emsp;```glUseProgram(shaderProgram);``` <br><br>&emsp;&emsp;```glUniform4f(vertexColorLocation, redValue, 0.0f, blueValue, 1.0f);``` <br>&emsp;&emsp;```// bind the first triangle's VAO & call draw function``` <br><br>&emsp;&emsp;```glUniform4f(vertexColorLocation, blueValue, 0.0f, redValue, 1.0f);``` <br>&emsp;&emsp;```// bind the second triangle's VAO & call draw function``` <br>&emsp;&emsp;```[ ... ]``` <br>```}``` |
| Adding more information to the array representing the triangle by including colors corresponding to each vertex. It looks smoothly blended because during the rasterization stage of rendering, the colors are blended because of fragment interpolation. | ![Two triangles with red, blue, and green blended throughout them](https://github.com/user-attachments/assets/f6f06c0b-ae50-4422-a531-02e84dc04707 "Two triangles with red, blue, and green blended throughout them") |
| Drawing a triangle after implementing a Shader class to manage compiling, linking, and error checking the vertex shader, fragment shader, and shader program as well as setting uniforms. | ![One triangle with red, blue, and green blended throughout it](https://github.com/user-attachments/assets/6e32bdef-dbc8-4678-b18c-be0eb5a68e3c "One triangle with red, blue, and green blended throughout it") |

### Excercises

| Excercise | Visual |
|-----------|--------|
| **1.** Adjust the vertex shader so that the triangle is upside down. <br><br>*Note:* I modified the output vector of the vertex shader to have a negative y-value, which flips all the vertices across the x-axis (i.e. input ```(x, y, z)``` --> output ```(x, -y, z)```). This works without an additional translation of the vertices because NDC places ```(0, 0)``` directly in the center of the canvas. | &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;![One triangle, upside down](https://github.com/user-attachments/assets/735d0d7a-ea40-46d8-a7cd-38e723e467e4)&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; |
| **2.** Specify a horizontal offset via a uniform and move the triangle to the right side of the screen in the vertex shader using this offset value. <br><br>*Note:* I thought it would be more interesting to make the triangle move back and forth between the right and left sides of the screen. | [Video: One triangle, horizontally offset by a uniform](https://github.com/user-attachments/assets/3d35aca5-3e9e-4d2f-ba25-d6114920c683) |
| I accomplished this with the following code: | ```float horizontalOffset;``` <br>```while (/* window is open */)``` <br>```{``` <br>```[ ... ]``` <br>&emsp;&emsp;```myShader.use();``` <br>&emsp;&emsp;```horizontalOffset = sin(glfwGetTime()) / 2.0f;``` <br>&emsp;&emsp;```myShader.setFloat("horizontalOffset", horizontalOffset);``` <br>&emsp;&emsp;```[ ... ]``` <br>```}``` |
| **3.** Output the vertex position to the fragment shader using the ```out``` keyword and set the fragment's color equal to this vertex position (see how even the vertex position values are interpolated across the triangle). Once you managed to do this; try to answer the following question: why is the bottom-left side of our triangle black? <br><br>The 'color' in the bottom left side of the triangle is ```(-0.43f, -0.25f, 0.0f)```, although the values for colors are only accepted in the range ```[0.0 - 1.0]``` which means any negative values would simply be clamped to the closest value in the range before rendering. Effectively, ```(-0.43f, -0.25f, 0.0f)``` becomes ```(0.0f, 0.0f, 0.0f)``` (black). | ![One triangle, color equal to vertex position](https://github.com/user-attachments/assets/04bcef49-b885-4eef-b0e8-54c96610a303) |

## Textures

- Playing with the colors from the Sunny Swamp palette to create a sunset gradient.

    ![Sunset gradient rectangle](https://github.com/user-attachments/assets/28719b67-5733-4361-8013-e53f35c0bf65)


- I changed the way I pass colors into the fragment shader, so now I make use of the ```normalize``` function and pass each color component as a float ```[0.0f - 255.0f]```. It's interesting to see how much the colors change between my rounded off values and the precise ones.

    ![Sunset gradient rectangle, colors slightly different from before](https://github.com/user-attachments/assets/61043970-4edc-48c7-a27b-e2df0f5f9df5)


- My first texture!

    ![A rectangle with the container image rendered onto it](https://github.com/user-attachments/assets/d4bdbbc1-7e2a-45d7-b600-2017e7c942b1)

---
