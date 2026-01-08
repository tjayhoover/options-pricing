# Options Pricing
## Reverse Mode AD Framework

 I will be storing the nodes from the computational graph in a vector. They will be inserted as the operations are completed in the correct order, resulting in a topological sort of the computation graph. A reverse path can easily be executed by traversing the tape (the vector).

## Python bindings (pybind11)

This project can optionally build a Python extension using pybind11. The module is named `options_pricing_py` and exposes a small subset of the C++ API: `Variable`, `sin`, `cos`, `exp`, and two helper functions `tape_execute_backpass()` and `tape_nodes_count()`.

Build steps (Windows / PowerShell):

1. Install pybind11 and ensure a matching Python development environment is available. Using vcpkg or a system package manager is recommended. If you installed pybind11 via pip, prefer the CMake-config aware installation (or point CMake to pybind11's config).

2. From the project root (where `CMakeLists.txt` is):

```powershell
# out-of-source build
cmake -S . -B build -D CMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

3. If `pybind11` was found by CMake, the extension `options_pricing_py` will be built and the generated `.pyd` (Windows) file will be placed in the build tree. You can then run the example script in `python_scripts/using_bindings.py` with the same Python interpreter used to build the module.

If pybind11 isn't found, the build will still produce the `options_pricing` executable but the Python extension won't be built.

## Next Project Steps

### 1. Add a `backward()` helper
- **Goal:** one-call gradient computation.
- **Behavior:**
  - (a) Zero all adjoints (recommended)
  - (b) Set output adjoint seed (default `1.0`)
  - (c) Run `tape_execute_backpass()`
- **Python API:**  
  - `op.backward(z)`  
  - or `op.backward(z, seed=1.0)`

---

### 2. Add `log` and `sqrt` primitives
- Implement **forward + backward** rules in C++.
- Expose to Python:
  - `op.log(x)`
  - `op.sqrt(x)`
- Add domain checks:
  - `log(x)` requires `x > 0`
  - `sqrt(x)` requires `x >= 0`

---

### 3. Support literal numeric types in operator overloads
- Enable:
  - `Variable op double`
  - `double op Variable`
- Cover operators:
  - `+`, `-`, `*`, `/`
- Keep implementation minimal:
  - templated overloads constrained to arithmetic types  
  - or explicit `double` overloads
- Result: users no longer need to wrap constants as `Variable`.

---

### 4. Implement Hessian–Vector Products (HVP)
- **Goal:** compute second-order derivatives efficiently without building a full Hessian.
- Likely implementation:
  - Add per-variable fields:
    - `dot` (forward directional derivative)
    - `adj_dot` (reverse directional adjoint)
  - Extend each primitive op with second-order backprop rules.
- **Python API ideas:**
  - `x.set_dot(v)`
  - `op.backward_hvp(z)` or `op.hvp(z)`
- Should return:
  - first-order adjoints
  - second-order directional results (`Hv`)

---

### 5. Add comprehensive tests (C++ + Python)

#### C++ tests
- Primitive ops:
  - `+`, `-`, `*`, `/`, `sin`, `cos`, `exp`, `log`, `sqrt`
- Analytic gradient checks where available
- Central finite-difference checks for general expressions

#### Python tests
- Simple expressions with known derivatives
- Monte Carlo European call:
  - price vs Black–Scholes
  - delta vs Black–Scholes
- Tape hygiene:
  - repeated runs do not drift
  - tape does not grow unexpectedly (requires `tape_reset()`)

---
