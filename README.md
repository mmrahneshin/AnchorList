# Anchor List

[![DOI](https://zenodo.org/badge/961046497.svg)](https://doi.org/10.5281/zenodo.20474959)

This repository contains the source code for the "Anchor List" data structure as detailed in our paper. 

Traditionally, arrays provide $O(1)$ random access but perform poorly ($O(n)$) on arbitrary insertions and deletions. Linked lists, on the other hand, support $O(1)$ insertion and deletion but incur $O(n)$ random-access time. The **Anchor List** uses a practical implementation of the Order-Statistic Tree (OST) backed by Parity-Seeking 2-3 Red-Black Trees (PSRBT) to address this trade-off.

The data structure provides:
- Amortized $O(1)$ time for push and pop operations at both ends of the list.
- $O(\log n)$ time for access, insertion, and deletion at arbitrary indices.

## Repository Structure

The repository includes implementations of the Anchor List in three major programming languages:

*   **`C++/`**: A high-performance implementation in C++. Also features time metric test suites comparing `AnchorList` against `std::vector`, along with benchmark plotting scripts.
*   **`Java/`**: A Java implementation of the Parity-Seeking Red-Black Tree and the Anchor List, accompanied by its own test suite and plotting scripts.
*   **`Python/`**: A C-extension based module (`anchor_list`) exposing the data structure directly to Python, bridging fast low-level execution with a familiar API.

---

## C++ Implementation: Usage Guide

The C++ implementation provides the core high-performance structure and is built using `CMake`. It includes several test suites designed for correctness verification and performance benchmarking against `std::vector`.

### Building / Compiling

To build the project, make sure you have `CMake` and a suitable C++ compiler (like `g++` or `clang++`) installed.

```bash
cd C++

# Create a build directory
mkdir build
cd build

# Configure the project with CMake
cmake ..

# Compile the project
make
```

### Running Tests and Benchmarks

Once built, the executables will be available in the `build/` directory (categorized by the subdirectories like `test/` or `test.paper/`). You can run the tests or benchmark binaries directly:

```bash
# Run one of the compiled benchmark/test binaries
./test.paper/anchorList_vs_vector_half_remove
./test.paper/anchorList_vs_vector_remove_all
```

You can also use the included Python scripts in the `C++/` directory (e.g., `plot_time_ell_vs_vec.py`) to visualize the benchmark results after they are generated.

---

## Python Implementation: Usage Guide

The Python implementation is written directly as a C-extension for maximum performance while exposing a standard Sequence protocol to behave similarly to Python's built-in `list`.

### Installation / Building

To use `AnchorList` in Python, you need to compile the C-extension module first. Ensure you have standard C build tools (like `gcc`) and Python development headers installed.

Navigate to the `Python/` directory and use the provided `setup.py`:

```bash
cd Python

# To build the extension in place (creates a .so / .pyd file in the same directory):
python3 setup.py build_ext --inplace

# OR to install it globally/user-wide:
python3 setup.py install --user
```

### Usage Example

After compiling, you can import and use `AnchorList` just like standard Python sequences:

```python
from anchor_list import AnchorList

# 1. Initialize the structure
my_list = AnchorList()

# 2. Insert elements: insert(index, value)
my_list.insert(0, 100)
my_list.insert(1, 200)
my_list.insert(0, 50)   # Inserts at the beginning

# 3. Random Access: O(log n)
print(f"Element at index 1: {my_list[1]}")  # Output: 100

# 4. Length
print(f"Total elements: {len(my_list)}")    # Output: 3

# 5. Remove elements: remove(index)
my_list.remove(0)       # Removes the element at index 0 (which is 50)
print(f"New element at index 0: {my_list[0]}") # Output: 100
```

### Running Benchmarks

Each language directory contains benchmark scripts designed to evaluate operations like `push_back`, `push_front`, random insertion, and deletion against built-in types. In Python, you can find `test_list.py` which runs a comprehensive suite benchmarking `AnchorList` against Python's built-in `list`.

```bash
cd Python
python3 test_list.py
```
