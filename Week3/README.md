# CS3242 3D Modelling and Animation

## Mesh Simplification

### Motivation
- Oversampled scan data
- Overtessellation: e.g. iso-surface extraction
- Multi-resolution hierarchies for
  - efficient geometry processing
  - level-of-detail (LOD) rendering
- Adaption to hardware capabilities

---

### Quantity-Quality Tradeoff

<img src="public\Screenshot 2022-02-03 171510.png" width=600>

---

### Wishful Thinking
- We want to reduce the number of vertices or triangles to a certain number, such that the difference of the two meshes is *minimal*
  - Impossible
- Look for sub-optimal solution
- More wishful thinking: additional fairness criteria:
  - normal deviation, triangle shape, scalar attributes    

--- 

### 2 main approaches
- Vertex Clustering
  - Reduce parts of the mesh into one vertex

<img src="public\Screenshot 2022-02-03 174654.png" width=600/>

- Iterative Decimation
  - Remove one edge/vertex a time

---

### Cluster Generation
- How to group vertices into groups?
  - Uniform 3D grid/partition

  <img src="public\Screenshot 2022-02-03 174927.png" width=600/>

---

### Computing a Representation
- Among a cluster of triangles and vertices
  - Compute one representative vertex and re-triangulate the whole cluster

- Average
  - Like low-pass filter
  
  <img src="public\Screenshot 2022-02-03 175156.png" width=600/>

- Median
  - The "closest" point to all
    - Ref: Geometric Median
  - Similar to sub sampling
  
  <img src="public\Screenshot 2022-02-03 175442.png" width=600/>

- Error Quadrics
  - The point that minimize the sum of distances to all planes of triangles
  - Feature preserving
  
  <img src="public\Screenshot 2022-02-03 175553.png" width=600>

  - Like distance-square equation 

  <img src="public\Screenshot 2022-02-03 175654.png" width=600/>

--- 

### Vertex Cluster
- Cluster Generation
- Compute a representation
- Retriangulate the cluster by
  - Edge contraction without destroying the representative
  - Or simply joining all the boundary vertices to the representative
- **Problem: Topology Changes**

  <img src="public\Screenshot 2022-02-03 175654.png" width=600/>

  <img src="public\Screenshot 2022-02-03 180216.png" width=600/>

---

### Iterative Decimation
- Set your goal
  - E.g. desired number of triangles, error, etc
- Repeat **Decimation Operation**:
  - Delete one vertex/edge at a time by vertex deletion/edge contraction
- Until no further reduction is possible or the goal(s) are reached
- Which one to pick? 
  - Shortest edge, least difference after decimation, curvature, triangle quality
  - User defined?
- Reminder: Have to update the priority queue.

<img src="public\Screenshot 2022-02-03 180537.png" width=600/>

### Potential Problems
- Topology Changes
  - Tackled by the link check
- Flipped triangles

<img src="public\Screenshot 2022-02-03 180700.png" width=600/>

---

### Quality Concerns
- Local error metrics
  - Average distance to from the deleted point to the new planes

  <img src="public\Screenshot 2022-02-03 181121.png" width=600>

- Global Error Metrics
  - Creating an "envelop" before decimation
  - Guarantees to stay within boundaries
  
  <img src="public\Screenshot 2022-02-03 181351.png" width=600/>

- Hausdroff distance
  - The maximum distance between all the minimal distances between two shapes
  - Two "sides" are different
  - Can take only one side

- Triangle quality
  - Triangle shape
  
  <img src="public\Screenshot 2022-02-03 181913.png" width=400/>
  
- Dihedral angles (change in angles)
- Valance balance
  - Degree of vertices

    <img src="public\Screenshot 2022-02-03 182033.png" width=500/>

- Color difference

  <img src="public\Screenshot 2022-02-03 182122.png" width=500/>

---

## Thickening

### "Lifting" up the surface
- Just move every vertex along the triangle normal
- Move every vertex according to the shared vertex normal
- Concave vertices move along with shared normal
- Convex vertices move along the triangle normal

<img src="public\Screenshot 2022-02-03 183547.png" width=600/>

<img src="public\Screenshot 2022-02-03 183637.png" width=600/>