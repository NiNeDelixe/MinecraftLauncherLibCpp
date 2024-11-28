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
        private:
            template <typename ITERATOR>
            class Iterator {
                friend class MCLLibVector;

            public:
                typedef ITERATOR iterator_type;
                typedef std::random_access_iterator_tag iterator_category;
                typedef iterator_type value_type;
                typedef iterator_type& reference;
                typedef iterator_type* pointer;
                typedef ptrdiff_t difference_type;

            private:
                Iterator(ITERATOR* ptr) : m_ptr(ptr) {}

            public:
                Iterator(const Iterator& other)
                    : m_ptr(other.m_ptr) {}

                Iterator& operator++() {
                    m_ptr++;
                    return *this;
                }

                Iterator operator++(int) {
                    Iterator<ITERATOR> temp = *this;
                    m_ptr++;
                    return temp;
                }

                Iterator operator+(const size_t index) noexcept {
                    for (size_t i = 0; i < index; ++i)
                    {
                        ++this->value;
                    }

                    return *this;
                }
                Iterator operator-(const size_t index) noexcept {
                    for (size_t i = index - 1; i >= 0; --i)
                    {
                        --this->value;
                    }

                    return *this;
                }

                Iterator& operator+=(const size_t index) noexcept {
                    *this = *this + index;
                    return *this;
                }
                Iterator& operator-=(const size_t index) noexcept {
                    *this = *this - index;
                    return *this;
                }

                Iterator& operator[](const size_t index) noexcept { return this + index; }

                Iterator::reference& operator*() const {
                    return *m_ptr;
                }

                Iterator::pointer* operator->() const {
                    return m_ptr;
                }

                bool operator==(const Iterator& other) const {
                    return m_ptr == other.m_ptr;
                }

                bool operator!=(const Iterator& other) const {
                    return m_ptr != other.m_ptr;
                }

                bool operator>(const Iterator& other) const noexcept { return this->value > other.value; }
                bool operator>=(const Iterator& other) const noexcept { return !(this < other); }
                bool operator<(const Iterator& other) const noexcept { return this->value < other.value; }
                bool operator<=(const Iterator& other) const noexcept { return !(this > other); }

            private:
                iterator_type* m_ptr;
            };

        private:
            void reallocate(size_t new_capacity) {
                T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
                for (size_t i = 0; i < m_size; ++i) {
                    new (new_data + i) T(std::move(data[i])); // Перемещаем существующие элементы
                    data[i].~T(); // Вызываем деструкторы для старых объектов
                }
                ::operator delete(data); // Освобождаем старую память
                data = new_data;
                m_capacity = new_capacity;
            }

        public:
            MCLLibVector()
                : data(nullptr), m_size(0), m_capacity(0) {}
            /*{
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
            }*/

            explicit MCLLibVector(size_t size)
                : data(static_cast<T*>(::operator new(size * sizeof(T)))), m_size(0), m_capacity(size) {}

            /*MCLLibVector(size_t size)
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
            }*/

            ~MCLLibVector()
            {
                clear();
                ::operator delete(data);
                /*do 
                {
                    (this->data_struct).size = 0;
                    (this->data_struct).capacity = 0;
                    free((this->data_struct).data);
                } while (0);*/
            }

        public:
            bool isEmpty()
            {
                return (size() == 0);
            }

            void pushBack(const T& value)
            {
               /* do 
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
                } while (0);*/
                if (m_size >= m_capacity) {
                    reallocate(m_capacity == 0 ? 1 : m_capacity * 2);
                }
                new (data + m_size) T(value);
                ++m_size;
            }

            void push_back(const T& value)
            {
                pushBack(value);
            }

            bool empty()
            {
                return isEmpty();
            }

            size_t size()
            {
                return m_size;
            }

            void clear() 
            {
                for (size_t i = 0; i < m_size; ++i) 
                {
                    data[i].~T();
                }
                m_size = 0;
            }

            typedef Iterator<T> iterator;
            typedef Iterator<const T> const_iterator;
            iterator begin() {
                return iterator(data);
            }

            iterator end() {
                return iterator(data + m_size);
            }

            const_iterator cbegin() const {
                return const_iterator(data);
            }

            const_iterator cend() const {
                return const_iterator(data + m_size);
            }

        public:
            T& operator[](size_t index)
            {
                if (index > m_capacity)
                {
                    throw std::length_error("Index out of memory");
                }
                return data[index];
            }

            const T& operator[](size_t index) const 
            {
                if (index > m_capacity)
                {
                    throw std::out_of_range("Index out of bounds");
                }
                return data[index];
            }

        private:
            T* data;
            size_t m_size;
            size_t m_capacity;
		};

		template<typename T, typename ALLOC = std::allocator<T>>
		using Vector = std::vector<T, ALLOC>;
	}
}

#endif // !TYPES_VECTOR_H_
