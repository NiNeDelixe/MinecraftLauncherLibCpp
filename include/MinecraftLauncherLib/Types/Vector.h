#ifndef TYPES_VECTOR_H_
#define TYPES_VECTOR_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include <string>
#include <memory>

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
                    this->data = this->allocat.allocate(1 * sizeof(*this->data));
                    if (!this->data)
                    {
                        //fputs("malloc failed!\n", (__acrt_iob_func(2)));
                        //abort();
                    }
                    this->m_size = 0;
                    this->capacity = 1;
                } while (0);
            }

            MCLLibVector(size_t size)
            {
                do 
                {
                    this->data = this->allocat.allocate(size * sizeof(*this->data));
                    if (!this->data) 
                    {
                        //fputs("malloc failed!\n", (__acrt_iob_func(2)));
                        //abort();
                    } 
                    this->m_size = 0;
                    this->capacity = size;
                } while (0);
            }

            ~MCLLibVector()
            {
                do 
                {
                    this->allocat.deallocate(this->data, this->capacity);
                    this->m_size = 0;
                    this->capacity = 0;
                } while (0);
            }

        public:
            size_t size()
            {
                return this->m_size;
            }

            bool empty()
            {
                return this->m_size == 0;
            }

            void push_back(const T& value)
            {
                if (this->size + 1 > this->capacity) 
                {
                    size_t n = this->capacity * 2;
                    this->allocat.deallocate(this->data, this->capacity);
                    this->data = this->allocat.allocate(n * sizeof(*this->data));
                    if (!this->data)
                    {
                        fputs("realloc failed!\n", (__acrt_iob_func(2))); 
                        abort();
                    } 
                    this->capacity = n;
                } 
                //new(this->data + this->size, value);
                this->data[this->m_size] = value;
                this->m_size += 1;
            }

        private:
            template<class... ARGS>
            T& emplaceOneAtBack(ARGS&&... _Val)
            {

            }

            

        public:
            T& operator[](size_t index)
            {
                if (index > this->capacity)
                {
                    throw std::length_error("Index out of memory");
                }
                return this->data[index];
            }

        private:
            T* data;
            size_t m_size;
            size_t capacity;
            

            std::allocator<T> allocat;
		};

        template<typename T>
        MCLLibVector<T>& operator+=(MCLLibVector<T>& vec, const T& new_value)
        {
            vec.push_back(new_value);
            return vec;
        }


		template<typename T>
		using Vector = std::vector<T, std::allocator<T>>;

	}
    template<typename T>
    types::Vector<T>& operator+=(types::Vector<T>& vec, const T& new_value)
    {
        vec.push_back(new_value);
        return vec;
    }
}

#endif // !TYPES_VECTOR_H_
