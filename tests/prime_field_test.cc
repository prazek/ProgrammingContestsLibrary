// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "numeric.h"
#include "numeric/prime_field.h"
#include "io.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(prime_field_suite)

BOOST_AUTO_TEST_CASE(creation_test) {
  {
    numeric::prime_field<7> n(10);
    BOOST_CHECK_EQUAL(n.value(), 3);
  }

  {
    numeric::prime_field<5> n(10);
    BOOST_CHECK_EQUAL(n.value(), 0);
  }

  {
    numeric::prime_field<5> n(4);
    numeric::prime_field<5> m(-1);
    BOOST_CHECK_EQUAL(n.value(), 4);
    BOOST_CHECK_EQUAL(m.value(), 4);
    BOOST_CHECK_EQUAL(n.value(), m.value());
  }

  {
    uint64 value = (1uLL << 63) + uint64(1000 * 1000 * 1000);
    numeric::prime_field<5> n(value);
    BOOST_CHECK_EQUAL(n.value(), 3);
  }

  {
    int64 value = (1uLL << 62) + uint64(1000 * 1000 * 1000);
    numeric::prime_field<5> n(value);
    BOOST_CHECK_EQUAL(n.value(), 4);
  }

  {
    int64 value = (1uLL << 62) + uint64(1000 * 1000 * 1000);
    numeric::prime_field<5> n(-value);
    BOOST_CHECK_EQUAL(n.value(), 1);
  }

  {
    uint64 value = uint64(uint32_prime1) * uint64(uint32_prime1) + 100;
    numeric::prime_field<uint32_prime1> n(value);
    BOOST_CHECK_EQUAL(n.value(), 100);
  }

  {
    numeric::prime_field<uint32_prime1> n(uint32_prime1 - 1);
    numeric::prime_field<uint32_prime1> m(-1);
    BOOST_CHECK_EQUAL(n.value(), m.value());
  }
}

BOOST_AUTO_TEST_CASE(addition_test) {
  {
    numeric::prime_field<5> a(2);
    numeric::prime_field<5> b(3);
    BOOST_CHECK_EQUAL(a + a, 4);
    BOOST_CHECK_EQUAL(a + b, 0);
    BOOST_CHECK_EQUAL(b + b, 1);
    BOOST_CHECK_EQUAL(a + a + b, a);
  }

  {
    numeric::prime_field<uint32_prime1> a(uint32_prime1 - 1);
    numeric::prime_field<uint32_prime1> b(uint32_prime1 - 2);
    BOOST_CHECK_EQUAL(a + b, b + a);
    BOOST_CHECK_EQUAL(a + b + 3, 0);
    BOOST_CHECK_EQUAL(3 + a + b, 0);
    BOOST_CHECK_EQUAL(a + 3 + b, 0);
    BOOST_CHECK_EQUAL(a + a + b + 8, 4);
  }
}

BOOST_AUTO_TEST_CASE(substraction_test) {
  {
    numeric::prime_field<5> a(2);
    numeric::prime_field<5> b(3);
    BOOST_CHECK_EQUAL(a - a, 0);
    BOOST_CHECK_EQUAL(a - b, 4);
    BOOST_CHECK_EQUAL(b - a, 1);
    BOOST_CHECK_EQUAL(b - a - a, 4);
  }

  {
    numeric::prime_field<uint32_prime1> a(uint32_prime1 - 1);
    numeric::prime_field<uint32_prime1> b(uint32_prime1 - 2);
    BOOST_CHECK_EQUAL(a - b, 1);
    BOOST_CHECK_EQUAL(b - a, uint32_prime1 - 1);
    BOOST_CHECK_EQUAL(b - a + 1, 0);
    BOOST_CHECK_EQUAL(b - (uint32_prime1 + 1) + 3, 0);
  }
}

BOOST_AUTO_TEST_CASE(multiplication_test) {
  {
    numeric::prime_field<5> a(1);
    numeric::prime_field<5> b(2);
    BOOST_CHECK_EQUAL(a * a, a);
    BOOST_CHECK_EQUAL(a * 1, a);
    BOOST_CHECK_EQUAL(1 * a, a);
    BOOST_CHECK_EQUAL(a * 0, 0);
    BOOST_CHECK_EQUAL(a * b, b);
    BOOST_CHECK_EQUAL(b * b * b, 3);
  }

  {
    numeric::prime_field<uint32_prime1> a(uint32_prime1 - 1);
    numeric::prime_field<uint32_prime1> b(uint32_prime1 - 2);
    BOOST_CHECK_EQUAL(a * b, 2);
    BOOST_CHECK_EQUAL(a * a, 1);
    BOOST_CHECK_EQUAL(b * b * b * b, 16);
  }
}

BOOST_AUTO_TEST_SUITE_END()