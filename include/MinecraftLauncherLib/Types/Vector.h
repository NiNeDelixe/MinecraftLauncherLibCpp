#ifndef TYPES_VECTOR_H_
#define TYPES_VECTOR_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <stdlib.h>
#include <stdio.h>

#include <vector>

/* Declare a vector of type `TYPE`. */
#define VECTOR_OF(TYPE) struct { \
    TYPE *data; \
    size_t size; \
    size_t capacity; \
}

/* Initialize `VEC` with `N` capacity. */
#define VECTOR_INIT_CAPACITY(VEC, N) do { \
    (VEC).data = malloc((N) * sizeof(*(VEC).data)); \
    if (!(VEC).data) { \
        fputs("malloc failed!\n", stderr); \
        abort(); \
    } \
    (VEC).size = 0; \
    (VEC).capacity = (N); \
} while (0)

/* Initialize `VEC` with zero elements. */
#define VECTOR_INIT(VEC) VECTOR_INIT_CAPACITY(VEC, 1)

/* Get the amount of elements in `VEC`. */
#define VECTOR_SIZE(VEC) (VEC).size

/* Get the amount of elements that are allocated for `VEC`. */
#define VECTOR_CAPACITY(VEC) (VEC).capacity

/* Test if `VEC` is empty. */
#define VECTOR_EMPTY(VEC) ((VEC).size == 0)

/* Push `VAL` at the back of the vector. This function will reallocate the buffer if
    necessary. */
#define VECTOR_PUSH_BACK(VEC, VAL) do { \
    if ((VEC).size + 1 > (VEC).capacity) { \
        size_t n = (VEC).capacity * 2; \
        void *p = realloc((VEC).data, n * sizeof(*(VEC).data)); \
        if (!p) { \
            fputs("realloc failed!\n", stderr); \
            abort(); \
        } \
        (VEC).data = p; \
        (VEC).capacity = n; \
    } \
    (VEC).data[VECTOR_SIZE(VEC)] = (VAL); \
    (VEC).size += 1; \
} while (0)

    /* Get the value of `VEC` at `INDEX`. */
#define VECTOR_AT(VEC, INDEX) (VEC).data[INDEX]

/* Get the value at the front of `VEC`. */
#define VECTOR_FRONT(VEC) (VEC).data[0]

/* Get the value at the back of `VEC`. */
#define VECTOR_BACK(VEC) (VEC).data[VECTOR_SIZE(VEC) - 1]

#define VECTOR_FREE(VEC) do { \
    (VEC).size = 0; \
    (VEC).capacity = 0; \
    free((VEC).data); \
} while(0)

namespace MCLCPPLIB_NAMESPACE
{
	namespace types
	{
		template<typename T>
		class MCLLibVector
		{
        public:
            MCLLibVector()
            {
                do 
                {
                    (this->data_struct).data = static_cast<T*>(malloc((1) * sizeof(*(this->data_struct).data))); 
                    if (!(this->data_struct).data) 
                    {
                        fputs("malloc failed!\n", (__acrt_iob_func(2)));
                        abort();
                    } 
                    (this->data_struct).size = 0;
                    (this->data_struct).capacity = (1);
                } while (0);
            }

            MCLLibVector(size_t size)
            {
                do 
                {
                    (this->data_struct).data = malloc((size) * sizeof(*(this->data_struct).data)); 
                    if (!(this->data_struct).data) 
                    {
                        fputs("malloc failed!\n", (__acrt_iob_func(2)));
                        abort();
                    } 
                    (this->data_struct).size = 0;
                    (this->data_struct).capacity = (size);
                } while (0);
            }

            ~MCLLibVector()
            {
                do 
                {
                    (this->data_struct).size = 0;
                    (this->data_struct).capacity = 0;
                    free((this->data_struct).data);
                } while (0);
            }

        public:
            bool isEmpty()
            {
                return ((this->data_struct).size == 0);
            }

            void pushBack(const T& value)
            {
                do 
                {
                    if ((this->data_struct).size + 1 > (this->data_struct).capacity) 
                    {
                        size_t n = (this->data_struct).capacity * 2;
                        void* p = realloc((this->data_struct).data, n * sizeof(*(this->data_struct).data)); 
                        if (!p) 
                        {
                            fputs("realloc failed!\n", (__acrt_iob_func(2))); 
                            abort();
                        } 
                        (this->data_struct).data = static_cast<T*>(p);
                        (this->data_struct).capacity = n;
                    } 
                    (this->data_struct).data[(this->data_struct).size] = (value);
                    (this->data_struct).size += 1;
                } while (0);
            }

        public:
            T& operator[](size_t index)
            {
                if (index > data_struct.capacity)
                {
                    throw std::length_error("Index out of memory");
                }
                return this->data_struct.data[index];
            }

        private:
            struct DATA
            {
                T* data; size_t size; size_t capacity;
            } data_struct;
		};

		template<typename T>
		using Vector = MCLLibVector<T>;
	}
}

#endif // !TYPES_VECTOR_H_
