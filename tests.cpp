#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <vector>
#include "serializer.h"

void debug_print(const char *format, ...) {
    va_list args;
    va_start(args, format);
    #if DEBUG
        vprintf(format, args);
    #endif
    va_end(args);
}

int main() {

    // Tests integer serialization
    {
        int in = 6, out = 10;
        serialize("data", &in, sizeof(in));
        deserialize("data", &out, sizeof(out));

        assert(out == in);
        // '6' is printed and not '10'. Therefore 'j' was correctly overwritten
        debug_print("expected: %d; got: %d\n", in, out);
        debug_print("//-------------------------------------------------//\n");
    }
    
    // Tests floating point serialization
    {
        double in = -0.543f, out = -345.43f;
        serialize("data", &in, sizeof(in));
        deserialize("data", &out, sizeof(out));
        assert(out == in);
        debug_print("expected: %lf; got: %f\n", in, out);
        debug_print("//-------------------------------------------------//\n");
    }

    // Tests char serialization
    {
        char in = 'z', out = '?';
        serialize("data", &in, sizeof(in));
        deserialize("data", &out, sizeof(out));
        assert(out == in);
        debug_print("expected: %c; got: %c\n", in, out);
        debug_print("//-------------------------------------------------//\n");
    }

    // Tests string serialization
    {
        char in[] = "Right Answer", out[sizeof(in)] = "Wrong Answer";
        serialize("data", in, sizeof(in));
        deserialize("data", out, sizeof(out));
        assert(strcmp(out, in) == 0);
        debug_print("expected: \"%s\"; got: \"%s\"\n", in, out);
        debug_print("//-------------------------------------------------//\n");
    }

    // Tests array serialization
    {
        long unsigned in[] = {1, 1, 2, 3, 5, 8, 13, 21}, out[sizeof(in)] = {0};
        serialize("data", in, sizeof(in));
        deserialize("data", out, sizeof(out));
        for (int i = 0; i < sizeof(in) / sizeof(*in); ++i) {
            assert(out[i] == in[i]);
            debug_print("expected: %d; got: %d\n", in[i], out[i]);
        }
        debug_print("//-------------------------------------------------//\n");
    }

    // Test struct serialization
    {
        struct {int i; char c;} in = {97, 'a'}, out = {98, 'b'};
        serialize("data", &in, sizeof(in));
        deserialize("data", &out, sizeof(out));
        assert(out.i == in.i && out.c == in.c);
        debug_print("expected: {%d, %c}; got: {%d, %c}\n",
                    in.i, out.i, in.c, out.c);
        debug_print("//-------------------------------------------------//\n");
    }

    // Tests pointer serialization
    {
        int a = -5, b = -78; 
        int * in = &a, * out = &b;
        serialize("data", &in, sizeof(in));
        deserialize("data", &out, sizeof(out));
        assert(out == in && *out == *in);
        debug_print("expected: address %p containing %d\n"
                    "got:      address %p containing %d\n",
                    in, *in, out, *out);
        debug_print("//-------------------------------------------------//\n");
    }

    // Tests heap allocated area
    {
        enum {length = 16}; 
        char * in  = (char *) calloc(length, sizeof(*in)); 
        char * out = (char *) calloc(length, sizeof(*out)); 
        const int size = length * sizeof(*in);
        
        strcpy(in,  "Right Answer!!!"); 
        strcpy(out, "Wrong Answer"); 

        serialize("data", in, size);
        deserialize("data", out, size);

        assert(memcmp(in, out, size) == 0);
        for (int i = 0; i < length; ++i) {
            debug_print("expected: %c; got: %c\n", in[i], out[i]);
        }
        debug_print("//-------------------------------------------------//\n");
        free(in);
        free(out);
    }

    // Tests array of structs serialization
    {
        typedef struct {int i; float f; char str[32];} Some_Struct; 
        Some_Struct array_in[] = {{-1, 0.1f, "AAAA"},
                                  {0,  3.0f, "BBBB"},
                                  {7, -77.7f, "CCCC"}};
        int size = sizeof(array_in) / sizeof(*array_in);

        serialize("data", array_in, sizeof(array_in));

        Some_Struct array_out[size];
        deserialize("data", array_out, sizeof(array_out));

        // 'array_in' and 'array_out' are equal
        for (int i = 0; i < size; ++i) {
            Some_Struct in = array_out[i], out = array_out[i];
            assert(in.i == out.i &&
                   in.f == out.f &&
                   strcmp(in.str, out.str) == 0);
            debug_print("expected {%d, %f, %s};\n"
                        "got      {%d, %f, %s}\n",
                        in.i, in.f, in.str, out.i, out.f, out.str);
        }
        debug_print("//-------------------------------------------------//\n");
    }
    
    // Tests nested struct serialization 
    {
        typedef struct {int day, month, year;}     Date;
        typedef struct {int hour, minute, second;} Time;
        typedef struct {Date date; Time time;}     TimeDate;

        Date date = {1, 4, 2021};
        Time time = {2, 39, 32};
        TimeDate in = {date, time};

        serialize("data", &in, sizeof(in));

        TimeDate out;
        deserialize("data", &out, sizeof(out));

        // 'in' and 'out' are equal
        assert(in.date.day    == out.date.day    &&
               in.date.month  == out.date.month  &&
               in.date.year   == out.date.year   &&
               in.time.hour   == out.time.hour   &&
               in.time.minute == out.time.minute &&
               in.time.second == out.time.second);

        debug_print("expected: %d/%d/%d %d:%d:%d; "
                    "got: %d/%d/%d %d:%d:%d\n",
                    in.date.day,   in.date.month,   in.date.year,
                    in.time.hour,  in.time.minute,  in.time.second,
                    out.date.day,  out.date.month,  out.date.year,
                    out.time.hour, out.time.minute, out.time.second);
        debug_print("//-------------------------------------------------//\n");
    }

   /* WARNING:
    * C++ container type (e.g. <vector>) serialization does not work!
    * After deserialization 'vector out' actually points to the same 
    * memory region of 'vector in'. It's not a copy!
    * In real cases it's gonna be even worse and 'out' will point to garbage
    * and likely cause segmentation fault errors.
    */
    {
        using namespace std;
        vector<int> in = {-5, -7, 0, 1, 3, 8, 12, -20, 39, 53};
        vector<int> out;
        int size = sizeof(in);
        serialize("data", &in, size);
        deserialize("data", &out, size);

        int old_value = in[0];
        in[0] = 1;

        for (auto i = in.begin(), j = out.begin();
             i != in.end() && j != out.end();
             ++i, ++j) {

            int expected = (i == in.begin()) ? old_value : *i; 

            // Uncomment to see the error.
            //assert(expected == *j /* out[0] should be equal to in[0] */);
            debug_print("expected: %d; got: %d\n", expected, *j);
        }
    }

    // In case you don't need the file anymore
    file_delete("data");
}
