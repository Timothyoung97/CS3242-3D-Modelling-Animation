# CS3242 3D Modelling & Animation

## Assignment 1 Submission Report
- Name: Yang Shiyuan
- Matrix No: A0214269A
  
### Task Completion

| S/N | Task Details | Nature | Implementation |
|-|-|-|-|
|1| Computing Normal Vectors | Main | `operationLib.cpp` -> `generateFaceNormals()` |
|2| Computing Angle Statistics | Main | `operationLib.cpp` -> `generateStatistics()` |
|3| Write an OBJ File | Main | `main.cpp` -> `writeFile()`|
|4| Read some other type of file other than OBJ | Optional | `main.cpp` -> `readOffFile()` |
|5| Implement enext(), sym() | Main | `mesh.cpp` -> `enext()` & `sym()`|
|6| Implement org(), dest() | Main | `mesh.cpp` -> `org()` & `dest()` |
|7| Implement fnext() | Main | `mesh.cpp` -> `setupAdjList()`|
|8| Compute the Number of Components | Optional | `mesh.cpp` -> `processNumOfComponents()` |
|9| Implement orientTriangles() | Optional | `mesh.cpp` -> `orientTriangles()` |
|10| Compute Vertex Normal Vectors for Smooth Shading | Optional | `mesh.cpp` -> `generateVertexNormals()` |
|11| Visualize boundary edges | Optional | `mesh.cpp` -> `drawEdge()` |

### Boss Conquest

#### Theme: `Subdivision`
| S/N | Task Details | Status | Implementation |
|-|-|-|-|
|1| Barycentric Subdivision | Completed | `subdivisionLoop.cpp` & `mesh.cpp` -> `barycentricSubdivide()` |
|2| Loop Subdivision | Failed | `subdivisionLoop.cpp` & `mesh.cpp` -> `loopSubdivide()` |

#### Boss Update:
- Couldn't finish loop subdivision as I could not trace the error that causes the new vertex created during the subdivision to be outside the mesh.
- Hence any new object created after the loop subdivision will appear like a `forzen` creature. 🥶

### Reference
- Barycentric Subdivision Reference: https://ncatlab.org/nlab/show/subdivision 
- Loop Subdivision Reference: http://www.cs.cmu.edu/afs/cs/academic/class/15462-s14/www/lec_slides/Subdivision.pdf
- Off File Format: https://en.wikipedia.org/wiki/OFF_(file_format)

### Reflection
- I feel that this assignment is probably the most challenging one I have ever done. There was a lot of difficulty in the implementation of the `fnext` list as there are many steps leading up to the creation of the list, especially when we need to make sense of the raw vertices and faces, then to place them in the correct order.
- Though the assignment is difficult to implement, I feel that the most interesting part is to see our work in action visually.
