# Metaeorite

> A modular C++20 toolkit for translating between spacetime metrics, generalized constitutive relations,
> and realizable metamaterial geometries.

See [docs/THEORY.ipynb](docs/THEORY.ipynb) for a from-scratch primer, the scientific foundations and citations
behind each module, SymPy-verified derivations of the key equations, and a full equation ↔ algorithm ↔ code
mapping (including exactly what remains to be implemented).

See [docs/CloakValidation.ipynb](docs/CloakValidation.ipynb) for an end-to-end demonstration: validating this
toolkit's four transformation modules (`metric_to_maxwell`, `maxwell_to_metric`, `maxwell_to_geometry`,
`geometry_to_maxwell`) against a well-known published result (the Pendry–Schurig–Smith cylindrical invisibility
cloak), parallelized with `dask.distributed`, with human-readable JSON export and sketched MEEP/EinsteinPy
interoperability.

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

`metric_to_maxwell::transformationOpticsConstitutive` implements the spatial-only, orthonormal-frame
specialization of this correspondence (ε′ = μ′ = (Λ Λᵀ)/det Λ for a coordinate Jacobian Λ), validated against
the published closed-form [Pendry–Schurig–Smith cylindrical cloak](https://doi.org/10.1126/science.1126493) in
[tests/engines/TestTransformationOptics.cpp](tests/engines/TestTransformationOptics.cpp) and demonstrated end-to-end
(with `dask.distributed` parallelization, JSON export, and MEEP/EinsteinPy interoperability sketches) in
[docs/CloakValidation.ipynb](docs/CloakValidation.ipynb). The fully general `Metric`-input engine (arbitrary
coordinate bases, time-space mixing) is still future work.

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

`meta_atom::classifySymmetry` (backed by [spglib](https://github.com/spglib/spglib)) provides the symmetry-analysis
step as a reusable utility; wiring it into a concrete synthesis engine is still future work, but
[docs/CloakValidation.ipynb](docs/CloakValidation.ipynb) demonstrates it directly as a downstream analysis step
on the `radial-laminate` engine's synthesized/input geometries (a real point-group classification of a concrete
lattice + basis, not a placeholder).

Genericity across macroscopic configurations comes from this module's `IMaxwellToGeometryEngine` interface and
`engine_id` registry, not from any single engine trying to cover every topology: `radial-laminate` (concentric
dielectric shells, validated against the cylindrical cloak) and `planar-laminate` (a Cartesian flat-stack
sibling using the same `core::LaminateEffectiveMedium` mixing formulas but a different index convention) are two
independent, topology-specific engines registered side by side. Adding support for another macroscopic
configuration (e.g. spherical shells, wire-grid arrays) means registering a new engine, not generalizing an
existing one.

---

#### 4. Geometry → Maxwell

Estimates the effective constitutive relations associated with an existing metamaterial geometry.

This module forms the reverse of homogenization.

Input

- Geometry
- Material properties

Output

- Effective constitutive tensors
- Symmetry classification (`meta_atom::classifySymmetry`, backed by [spglib](https://github.com/spglib/spglib))
- Homogenized Maxwell equations

Mirrors `maxwell_to_geometry`'s engine-per-topology structure: `radial-laminate` and `planar-laminate` are each
registered here too, so a geometry synthesized by one direction's engine can be homogenized back by its
counterpart of the same name.

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

## Hierarchical Synthesis (planned)

A `maxwell_to_geometry` synthesis engine's constituent materials (e.g. `radial-laminate`'s host/inclusion) are
themselves just scalar permittivities supplied by the caller - they are not guaranteed to correspond to a single
real, off-the-shelf material. When one doesn't, that constituent's assumed permittivity can be treated as a
*target* for another synthesis call, recursively, using a different (more fundamental) constituent pair, until
every constituent at the deepest level is realizable. Design principles for this workflow:

- **No automatic recursion, no materials database.** Metaeorite will not decide for the user whether a given
  permittivity is "real" - the user drives the loop themselves, calling the same functions repeatedly.
- **The existing `ReconstructionResult::empty()` rejection mechanism *is* the termination signal.** It already
  rejects targets unrealizable by the chosen constituent pair (e.g. near the cloak's coordinate singularity -
  see `maxwell_to_geometry`'s scope notes above). A user's recursive loop simply stops descending, keeping the
  last successful level, when the next level's call returns empty.
- **Recurse into a constituent slot, not into a level's own effective tensor.** Re-homogenizing a geometry with
  `geometry_to_maxwell` and feeding that tensor straight back into `maxwell_to_geometry` is a fixed point (it
  reproduces the same geometry) and does not descend a level. The meaningful next-level target is one of the
  *constituent's* assumed scalar permittivities (host or inclusion), promoted to an isotropic constitutive
  target via `core::ConstitutiveRelations::isotropic(epsilon)`.
- **Per-call, not per-`engine_id`, constituent parameters.** The `"radial-laminate"` `maxwell_to_geometry` engine
  is additionally exposed as a directly-callable, parameterized free function,
  `maxwell_to_geometry::synthesizeRadialLaminate(constitutive, hostEpsilon, inclusionEpsilon, layerPeriod)`
  (`mo.synthesize_radial_laminate(...)` in Python), so each recursion level can use a different constituent pair
  instead of being locked to the `engine_id` registry's fixed-at-registration-time defaults.
- The unit cell at whichever level the recursion successfully terminates at is sufficient on its own for
  full-wave (e.g. FDTD + periodic boundary conditions) dispersion sampling - there is no need to additionally
  isolate or render the bare meta-atom, since by construction that unit cell already only contains real,
  physically obtainable constituents.

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

---

## Implementation

This repository contains a C++20 implementation of the architecture described above.

### Layout

```bash
modules/core/                 shared vocabulary: Metric, ConstitutiveRelations, IGeometry,
                              ReconstructionResult<T>, EngineRegistry<Interface>
modules/meta_atom/            meta-atom structure & symmetry-group determination: LatticeBasis,
                              SymmetryClassification, classifySymmetry (spglib-backed)
modules/metric_to_maxwell/    IMetricToMaxwellEngine + PlaceholderEngine
modules/maxwell_to_metric/    IMaxwellToMetricEngine + PlaceholderEngine
modules/maxwell_to_geometry/  IMaxwellToGeometryEngine + PlaceholderEngine
modules/geometry_to_maxwell/  IGeometryToMaxwellEngine + PlaceholderEngine
modules/pipeline/             Pipeline orchestrator, wired purely through the interfaces above
apps/metaeorite_cli/          demo executable exercising the full pipeline
tests/                        Catch2 unit + integration tests
```

Each transformation engine is expressed as an abstract interface (`I*Engine`) in `core`'s vocabulary
(`Metric`, `ConstitutiveRelations`, `IGeometry`). Modules depend only on `core`, never on each other, and are
composed by `Pipeline` purely via those interfaces (dependency inversion), so a new engine implementation can
replace `PlaceholderEngine` without touching any other module. `core::EngineRegistry<Interface>` provides a
string-keyed factory for selecting an implementation at runtime; each module exposes an explicit
`registerEngines()` function rather than relying on static-initialization tricks.

`PlaceholderEngine` in every module currently implements only the one exactly-known trivial case (flat/Minkowski
spacetime &harr; vacuum constitutive relations &harr; free space); anything else throws or returns an empty
`ReconstructionResult` with an explanatory note. This validates the architecture end-to-end ahead of the real
Plebanski transformation, effective medium theory, and symmetry analysis implementations.

Beyond the vacuum baseline, each module also ships a real, non-placeholder engine validated against the
published Pendry&ndash;Schurig&ndash;Smith cylindrical cloak (see
[docs/CloakValidation.ipynb](docs/CloakValidation.ipynb) for the full worked demonstration of all four):

- **`metric_to_maxwell`** &mdash; `transformation_optics_constitutive`: the standard transformation-optics
  formula $\varepsilon' = \mu' = (\Lambda\Lambda^\mathsf{T})/\det(\Lambda)$ for an orthonormal-frame Jacobian
  $\Lambda$, exposed as a free function (not registry-selected) plus `cylindricalCloakJacobian` for this test
  case specifically. Validated to floating-point precision against the closed-form cloak parameters.
- **`maxwell_to_metric`** &mdash; `"transformation-optics"` engine: inverts the above for diagonal,
  impedance-matched ($\varepsilon=\mu$) input via $g_{ii} = \varepsilon_{ii}/\det(\varepsilon)$. Rejects
  (returns an empty result with notes) non-diagonal, complex, or non-impedance-matched input rather than
  guessing. **Scope**: this closed-form inverse only holds for diagonal $\Lambda$; the general (non-orthonormal,
  time-space-mixing) Plebanski inverse remains future work.
- **`maxwell_to_geometry`** / **`geometry_to_maxwell`** &mdash; `"radial-laminate"` engine (both directions):
  synthesizes/homogenizes a concentric-shell laminate of two isotropic dielectrics using classical Wiener-bounds
  effective-medium theory (arithmetic mean tangential to the layers, harmonic mean perpendicular to them), per
  Cai, Chettiar, Kildishev & Shalaev, ["Optical cloaking with
  metamaterials"](https://doi.org/10.1038/nphoton.2007.28), *Nature Photonics* **1**, 224 (2007). The filling
  fraction is solved to match the tangential ($\varepsilon_\phi$) target exactly. **Scope/limitations, stated
  explicitly rather than silently ignored**: a single filling fraction cannot simultaneously match both Wiener
  bounds, so the radial ($\varepsilon_r$) component is only approximate (confidence-scored, not silently
  accepted); the axial ($\varepsilon_z$) component is not properly modeled by this 2D radial-laminate approach
  (approximated equal to the tangential value); and targets requiring a permittivity outside the achievable
  range of the chosen material pair (e.g. near the cloak's inner-boundary coordinate singularity) are correctly
  rejected rather than extrapolated.
- **`maxwell_to_geometry`** / **`geometry_to_maxwell`** &mdash; `"planar-laminate"` engine (both directions):
  the same Wiener-bounds mixing formulas applied to a Cartesian flat-stack instead of a concentric-shell
  topology (arithmetic mean in-plane, harmonic mean along the stack normal). Not tied to the cylindrical cloak
  or its `(r, φ, z)` index convention; exists to demonstrate that other macroscopic configurations are added as
  independent sibling engines rather than by generalizing `"radial-laminate"` itself.

### Dependencies

Fetched automatically at configure time via [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) &mdash;  
no manual install step required:

| Package | License | Used by | Why |
| --- | --- | --- | --- |
| [Eigen](https://eigen.tuxfamily.org) | MPL2 | `core` | Rank-2 tensor / linear algebra (constitutive tensors, coordinate transforms). |
| [xtensor](https://github.com/xtensor-stack/xtensor) (+ [xtl](https://github.com/xtensor-stack/xtl)) | BSD-3-Clause | `core` | General N-dimensional tensor storage for `Metric` components of arbitrary dimension. |
| [spglib](https://github.com/spglib/spglib) | BSD-3-Clause | `meta_atom` (`SymmetryClassifier`) | Crystal point-/space-group symmetry search, so `maxwell_to_geometry` / `geometry_to_maxwell` can classify unit-cell/meta-atom symmetry without reimplementing crystallographic symmetry detection from scratch. Linked privately; no public header depends on it. |
| [pybind11](https://github.com/pybind/pybind11) | BSD-3-Clause | `bindings/python` | Exposes the `metaeorite::api` C++ facade as the importable `metaeorite` Python package. |
| [Catch2](https://github.com/catchorg/Catch2) | BSL-1.0 | `tests` | Unit testing. |

Every dependency here is used to avoid re-implementing well-solved, general-purpose infrastructure (linear
algebra, tensor storage, symmetry search, Python bindings, testing) so this repository's own code can stay
focused on the novel part: the metric &harr; constitutive-relations &harr; geometry translation logic itself.
Domain-specific tools that overlap with adjacent fields &mdash; CAD kernels, full electromagnetic simulators,
general-relativity simulators &mdash; are deliberately **not** dependencies, since providing those is out of
scope for this project (see [Overview](#overview)); a downstream user is expected to pair this library's output
with such tools themselves.

#### Notebook / Python-side dependencies

[docs/CloakValidation.ipynb](docs/CloakValidation.ipynb) additionally uses these packages (installed separately
in the Python environment used to run the notebook &mdash; they are **not** required to build or use the core
C++ library or its Python bindings):

| Package | License | Why |
| --- | --- | --- |
| [dask](https://www.dask.org) / [distributed](https://distributed.dask.org) | BSD-3-Clause | Parallelizes the cloak-validation grid sweep across a resource-capped local cluster, demonstrating how larger/more expensive sweeps (finer grids, dispersive frequency sweeps) would scale. |
| [psutil](https://github.com/giampaolo/psutil) | BSD-3-Clause | Detects available CPU/RAM at run time so the Dask `LocalCluster` is sized conservatively for the host machine instead of assuming fixed resources. |
| [matplotlib](https://matplotlib.org) | PSF-based (BSD-style) | Plots the validated cloak parameters against the published closed form. |
| [EinsteinPy](https://einsteinpy.org) | MIT | Sketches the GR-side interoperability path (symbolic flatness check of the virtual-space background metric). |
| [MEEP](https://meep.readthedocs.io) (optional) | GPL-2.0 | Sketches the EM-FDTD interoperability path (radial permittivity profile as a MEEP material function); the notebook runs without it installed and prints the equivalent code instead. |

No new C++ JSON dependency was added for the notebook's human-readable/interoperable I/O: `ConstitutiveRelations`'
`epsilon`/`mu`/`xi`/`zeta` are already exposed as NumPy complex arrays via the existing pybind11 bindings
(`pybind11/eigen.h` + `pybind11/complex.h`), so JSON export/import is built entirely in Python from those.

### Building

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j"$(nproc)"
ctest --test-dir build --output-on-failure
./build/apps/metaeorite_cli/metaeorite_cli
```
