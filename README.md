## Serializer
A very minimalist serializer for C.
Works with all basic data types, strings, arrays and structs.
Since data is saved in binary generated files are small and serialization and
deserialization are blazing fast but the generated files are not human readable.

# Recommended usage:
Just copy and paste functions serializer() and deserializer().
Putting "serializer.h" in the same folder of your source and using 
"#include "serializer.h""
is also an easy option.

"tests.cpp" goes over a few simple cases and can be used as a reference.

# Limitations:
Does not support structs containing pointers to heap allocated memory.
Does not support C++ container types.
