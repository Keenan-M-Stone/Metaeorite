# Metaeorite

> A modular C++20 toolkit for translating between spacetime metrics, generalized constitutive relations,
> and realizable metamaterial geometries.

---

## Overview

Metaeorite is an open-source scientific computing toolkit for computational transformation optics, intended for use
by researchers to aid in the designing metasurfaces, using the generalized constitutive relations of Maxwell's
 equations as the common intermediate language between geometry and spacetime.

This design separates the software into independent transformation modules that can be developed,
tested, and extended independently while remaining mathematically consistent.

The project focuses on providing computational tools—not full-wave electromagnetic simulation—for
exploring relationships between:

- General Relativity
- Transformation Optics
- Electromagnetic Metamaterials
- Effective Medium Theory
- Tensor Analysis
- Group and Representation Theory

---

## Scientific Philosophy

Transformation optics provides a mapping between curved spacetime and electromagnetic media.
Metaeorite separates this into two independent mathematical problems.

```uml
Spacetime Metric
        ↕
Transformation Optics
        ↕
Generalized Maxwell Equations
        ↕
Effective Medium Theory
        ↕
Metamaterial Geometry
```

The constitutive relations act as the common representation between all modules.
This architecture allows any module to be improved independently without requiring changes to the remainder of the software.

---

## Core Modules

Metaeorite consists of four primary transformation engines.

### 1. Metric → Maxwell

Uses the Plebanski formulation of transformation optics to convert a spacetime metric into the generalized
constitutive relations describing an equivalent electromagnetic medium.

Input

- Metric tensor
- Coordinate system

Output

- Permittivity tensor (ε)
- Permeability tensor (μ)
- Magnetoelectric coupling tensors (ξ, ζ)
- Generalized Maxwell formulation

---

### 2. Maxwell → Metric

Attempts the inverse transformation.

Given generalized constitutive relations, this module determines one or more compatible spacetime metrics.

Because this inverse problem is generally non-unique, the module produces a family of candidate metrics together with
information describing the assumptions made during reconstruction.

Input

- ε
- μ
- ξ
- ζ

Output

- Candidate metric tensors
- Confidence metrics
- Reconstruction assumptions

---

### 3. Maxwell → Geometry

Maps generalized constitutive relations to realizable metamaterial structures.

This module combines

- effective medium theory
- symmetry analysis
- group theory
- representation theory
- parameterized unit-cell libraries

to generate one or more candidate geometric realizations.

Input

- Constitutive tensors

Output

- Candidate unit cells
- Material assignments
- Fabrication parameters
- CAD geometry

---

#### 4. Geometry → Maxwell

Estimates the effective constitutive relations associated with an existing metamaterial geometry.

This module forms the reverse of homogenization.

Input

- Geometry
- Material properties

Output

- Effective constitutive tensors
- Symmetry classification
- Homogenized Maxwell equations

---

## Current Scope

The first implementation targets two-dimensional metasurfaces.
This decision reduces implementation complexity while preserving the overall architecture.
Internally, interfaces are designed such that extending to volumetric metamaterials requires
replacing computational backends rather than redesigning the public API.

Current assumptions include

- periodic unit cells
- planar structures
- linear media
- frequency-domain analysis

Future versions will support

- volumetric metamaterials
- multilayer structures
- nonlinear media
- dispersive materials
- time-varying media

---

## Scientific Foundations

Metaeorite is based on several complementary mathematical frameworks.

### Differential Geometry

Used for

- metrics
- coordinate transformations
- curvature
- tensor operations

### Transformation Optics

Provides the forward and inverse correspondence between spacetime metrics and generalized constitutive relations
through the Plebanski formulation.

### Effective Medium Theory

Provides the correspondence between constitutive tensors and realizable metamaterial structures.

### Group and Representation Theory

Used to classify structural symmetries and infer admissible constitutive tensors from geometric arrangements.

Rather than relying solely on numerical optimization, Metaeorite exploits symmetry constraints to reduce the search
space of candidate realizations.

---

## Architecture

```uml
                Metric
                   │
          ┌────────┴────────┐
          │ Metric → Maxwell│
          └────────┬────────┘
                   │
     Generalized Maxwell Equations
                   │
      ┌────────────┴────────────┐
      │                         │
Geometry → Maxwell      Maxwell → Geometry
      │                         │
      └────────────┬────────────┘
                   │
          Constitutive Relations
                   │
          ┌────────┴────────┐
          │ Maxwell → Metric│
          └─────────────────┘
```

Each transformation engine is implemented as an independent library with a well-defined interface.

No module depends directly on another domain representation except through the constitutive relations.

---

## Design Goals

Metaeorite is designed to be

- modular
- mathematically rigorous
- extensible
- parallelizable
- visualization-friendly
- suitable for research and education

The software emphasizes clear interfaces over monolithic implementations, allowing future algorithms to replace
existing modules without affecting user workflows.