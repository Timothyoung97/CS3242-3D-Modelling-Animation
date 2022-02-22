# CS3242 3D Modelling and Animation

## Self-intersection and Shape Matching

### Self-intersection
- created after modification
  - E.g. simplification (too simple primitive)

    <img src="public\Screenshot 2022-02-07 192058.png" width=600/>

---

### Some requirements for mesh
- No self-intersection
- Water tight
  - A closed manifold with no self-intersection
- Important for prototyping (3D printing), manufacturing

---

### Definition of Self-intersection
- Considered as NO self-intersection if the mesh is a simplicial complex
- A set of points S is said to be affinely independent (a.i.) if any point `x` in S is not linear combination of S - {`x`}.
  - basically, a point cannot be produced by the combination of all other points in the set

    <img src="public\Screenshot 2022-02-07 193827.png" width=600/>

---

### Simplicial Complex

[wiki_resource](https://en.wikipedia.org/wiki/Simplex#Elements)
[wiki_resource](https://en.wikipedia.org/wiki/Simplicial_complex)

- A `d-simplex σ` is a convex hull of an a.i. set S
  - S has `d + 1` points
- A `simplex τ` is a face of `σ` if `τ` is the convex hull of T ⊆ S. Then `τ ≤ σ`.
- A simplicial complex `K` is a collection of simplices such that
  - `𝜎 ∈ 𝐾 and 𝜏 ≤ 𝜎 ⇒ τ ∈ 𝐾`, and
  - `𝜎1, 𝜎2 ∈ 𝐾 ⇒ 𝜎1 ∩ 𝜎2 ≤ 𝜎1, 𝜎2`

    <img src="public\Screenshot 2022-02-07 194335.png" width=600/>

  