# Serializer
A very minimalist serializer for C.
Works with all basic types as well as strings, arrays and structs.\
Since data is saved in binary, generated files are small and serialization and
deserialization are blazing fast.\
But, as a result, the generated files are not human readable.

## Recommended usage:
1. Just copy and paste functions `serializer()` and `deserializer()` from "serializer.h".

1. Putting file "serializer.h" in the same folder of your source and including
it is also an easy option\
i.e. `#include "serializer.h"`

"tests.cpp" goes over a few simple cases and can be used as a reference.

### Tests Compilation:
A simple `g++ tests.cpp -o serializer` should do the job.\
If you want to see some output define `DEBUG`.\
For example: `g++ tests.cpp -o serializer -DDEBUG`

## Limitations:
* Does not support structs containing pointers to heap allocated memory.
* Does not support C++ container types.
