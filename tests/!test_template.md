```cpp
#define BOOST_TEST_MODULE ExampleTest
#include <boost/test/unit_test.hpp>

#include "Example.h"

BOOST_AUTO_TEST_CASE(ExampleTest1)
{
	BOOST_CHECK_EQUAL_COLLECTIONS(f.begin(), f.end(), s.begin(), s.end());
	BOOST_CHECK_EQUAL(left, right);
}
```