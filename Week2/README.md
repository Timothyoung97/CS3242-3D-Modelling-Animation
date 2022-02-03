# CS3242 3D Modelling and Animation

## Trist Structure and Mesh Modifications

### Various 3D Object Representations
1. Polygonal meshes
   1. Using "soups" of polygons (*triangles) to model ("approximate") object.
   2. Question: 
      1. Density of triangle to used
      2. Quality of triangle
2. Parametric patches
   1. Bézier curve (from CS3241)
3. Constructive solid geometry
   1. Representation of objects formed by boolean operations (Union, Intersection, Difference)
   2. Alternative:
      1. Ray tracing (trace rays that hit the primitive objects)
   3. Problem:
      1. Poor Quality of mesh
4. Spatial subdivision techniques
   1.  3D array of voxels
   2.  If the object occupies a voxel, turns the voxel on
5. Implicit representation
   1. Formulas

---

### What is a Mesh?
- A mesh is a collection of vertices, edges and triangles.
- Each edge must belong to at least one face.
- Each vertex must belong to at least one edge
- An edge is a boundary edge if it only belongs to one face.

---

### Property of a Mesh
- Manifold
- Orientability
- Topology

---

### Manifold
- A mesh is a manifold if
  - Every edge is adjacent to one (boundary) or two faces
  - For every vertex, its adjacent polygons form a disk (internal vertex) or a half-disk (boundary vertex)
- A mesh is a polyhedron if
  - It is a manifold mesh and it is closed (no boundary)
  - Every vertex belongs to a cyclically ordered set of faces (local shape is a disk)

---

### Orientation of Faces
- Each face can be assigned an orientation by defining the ordering of its vertices
- Orientation can be clockwise or counter-clockwise
- The orientation determines the normal direction of face. Usually counter-clockwise order is the "front" side.
- A mesh is well orientated (orientable) if all faces can be oriented consistently (all CCW or all CW) such that each edge has 2 opposite orientations for its two adjacent faces.
- Not every mesh can be well oriented.
  - e.g. Klein bottle, Mobius strip

--- 

### Euler-Poincare Characteristic (help to determine topology)
- The Euler-Poincare characteristic X is defined for the surfaces of polyhedra, according to the formula
  - X = V - E + F
  - where V, E, F are respectively the number of vertices, edges and faces in the given polyhedron
- More general rule
  - V - E + F = 2(C - G) -B
  - where:
    - V: Number of vertices
    - E: Number of edges
    - F: Number of faces
    - C: Number of connected components
    - G: Number of genus (holes, handles)
    - B: Number of boundaries
- Sphere like object, X = 2.
- Torus like object, X = 0.

---

### Topology
- Topology is a major area of mathematics concerned with properties that are preserved under continuous deformation of objects, such as deformations that involve stretching, but no tearing or gluing.

---

### Triangulation DS (for oriented triangle)

<img src="public\Screenshot 2022-02-03 153533.png" width=600/>

---

### Oriented Triangles
- In R^3, an edge can be shared by more than 1 triangle
- `fnext` operation: (using right-hand rule)
  - `fnext(abc) -> abd`
  - `fnext(abd) -> abe`
  
<img src="public\Screenshot 2022-02-03 153920.png" width=400/>

--- 

### Triangulation DS

<img src="public\Screenshot 2022-02-03 154123.png" width=600/>

- Point array: All vertices become integer index
- Triangle array: Mapping of vertex indexes
- *tIdx* represents a triangle without orientation
- Total only 6 version of orientation
- In empty slots, we store the 6 different `fnext()` information of the current tIdx

---

### An Oriented Triangle
- An unoriented triangle can be simply represented by an integer *tIdx* that is its index in the triangle list
  - i.e. an integer
- However, to incorporate with the version number *v* (0 to 5), we can encode an oriented triangle into a large integer by:
  - `OrTri ot = (tIdx << 3) | v;`
- So triangle index of an oriented triangle is:
  - `int tIdx = ot >> 3; (or ot/8)`
- And its version number is:
  - `int tVersion = ot && (0000111b);`

---

### Operators
- The operation `org` return the first vertex of the triangle `abc`, i.e. `a`
  - Note that `org(bca)` will return `b`
- The operation `enext`, as mentioned in the previous picture will return the triangle "rotated"
  - `enext(abc)` return `bca`
- The operation `sym` will flip the triangle
  - `sym(abc)` will return `bac`

---

## Mesh Operation (Assuming Manifolds)

### Two different "Topology" here
- Can deem "topology" as connectivity
- 2 kinds 
  - Mesh connectivity
  - Surface connectivity

---

### Mesh Modification
- Geometric
  - Does not change the mesh connectivity
    - e.g. UV map
    - Smoothing
  - Problem:
    - Topology may be destroyed if the vertex is moved too much to the side
  
---

### Topological
- Topological changes to the mesh connectivity
- Not topological changes to the underlying surface
- However, the underlying surface of the mesh does not change
- Possibly change of the underlying space

<img src="public\Screenshot 2022-02-03 162847.png" width=600/>

---

### Underlying Space
- The "space" occupied by the object 

<img src="public\Screenshot 2022-02-03 163111.png" width=600/>

---

### Topological Changes
- Topological changes of the mesh connectivity
  - No change of the underlying space
  
  <img src="public\Screenshot 2022-02-03 163428.png" width=400/>

- Topological changes to the surface
  - Change of u.s
  
  <img src="public\Screenshot 2022-02-03 163611.png" width=400/>

---

## Modification Operations

### Vertex Insertion
- Simply adding a new vertex (Delete existing triangle and add the new triangle)
- The new vertex can fall on
  - A triangle
  - An edge
  - An existing vertex (undesirable)
- Usually, the *position* of the new vertex help to achieve some goals
  - E.g. mesh quality

---

### Barycentric Subdivision
- A "Standard" way of subdividing a triangle
- The new vertices are the centroids of the elements they lie on (not a good method)

<img src="public\Screenshot 2022-02-03 164658.png" width=600/>

<img src="public\Screenshot 2022-02-03 165211.png" width=600>

### Partial Barycentric Subdivision
- A possibility to apply the subdivision partially

<img src="public\Screenshot 2022-02-03 164906.png" width=600/>

---

### Loop Subdivision

<img src="public\Screenshot 2022-02-03 164929.png" width=600/>

---

### Edge Swapping
- If an edge `ab` is shared by two triangles `abc` and `abd`
- Delete the two triangles and create two new ones
  - `abc` and `adb`
- A very useful technique in mesh refinement

<img src="public\Screenshot 2022-02-03 165535.png" width=600/>

---

### Edge Contraction
- For an edge `ab`, that is shared by two triangles
- Merge the two vertices `a` and `b` into a new one `c`
- Delete the two triangles
- And "stitch" the empty hole

<img src="public\Screenshot 2022-02-03 165736.png" width=600/>

- In general, you can only contract an edge `ab` if `Lk(a) ∩ Lk(n) = Lk(ab)`

---