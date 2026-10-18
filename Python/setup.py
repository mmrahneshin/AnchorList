from setuptools import setup, Extension

anchor_list_module = Extension(
    name="anchor_list",
    sources=[
        "anchor_list_module.c",  # Python wrapper (renamed for consistency)
        "anchor_list.c",  # Core logic
        "indexed_PPS23RB_binaryTree.c",
        "indexed_PPS23RB_binaryTree_node.c",
    ],
    include_dirs=["."],
    extra_compile_args=["-g", "-O0"],  # Enable debug symbols, disable optimizations
)

setup(
    name="anchor_list",
    version="0.1",
    description="Anchor list using a red-black tree",
    ext_modules=[anchor_list_module],
)
