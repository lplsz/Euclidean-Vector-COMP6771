#include "comp6771/euclidean_vector.hpp"

#include <algorithm>
#include <catch2/catch.hpp>
#include <sstream>
#include <vector>

/*
    Tests in this file tests if the friends overloaded operator have be implemently correctly.
    It matches the computed result with the expected output calculate by calculator and also see if
    appropriate exception is throw.

    Rational: These operator is can only do computation or throw exception.
*/

/*
   Test cases:
   - 0 dimension EVs are equal
   - A vector is equals to itself: Reflective
   - Two vectors are equal in the number of dimensions but different in the magnitude in each
   dimension are not equal vectors.
   - Two vecters differ in dimensions number are not equal vectors
*/
TEST_CASE("Equal Operator") {
	auto const ev_zero_dim = comp6771::euclidean_vector(0);

	SECTION("Zero dimension") {
		auto const ev = comp6771::euclidean_vector(0);

		CHECK(ev_zero_dim == ev);
		CHECK(ev == ev);
	}

	SECTION("Non Zero dimension") {
		auto const ev = comp6771::euclidean_vector{0.5, -3.5, 4.25, 3.09};
		auto const ev_same = comp6771::euclidean_vector{0.5, -3.5, 4.25, 3.09};

		auto const ev_diff_1 = comp6771::euclidean_vector{0.5, -3.5, -4.25, 3.09};
		auto const ev_diff_2 = comp6771::euclidean_vector{0.5, -3.5};

		SECTION("Same dimensions, different magnitudes") {
			CHECK_FALSE(ev == ev_diff_1);
		}

		SECTION("Different dimensions") {
			CHECK_FALSE(ev == ev_zero_dim);
			CHECK_FALSE(ev == ev_diff_2);
		}

		SECTION("Equal to itself") {
			CHECK(ev == ev);
		}

		SECTION("Equal in dimensions and magnitudes") {
			CHECK(ev == ev_same);
		}
	}
}

/*
   Rational: Basically negating the tests in the == cases.
*/
TEST_CASE("Not Equal") {
	auto const ev_zero_dim = comp6771::euclidean_vector(0);

	SECTION("Zero dimension") {
		auto const ev = comp6771::euclidean_vector(0);

		CHECK_FALSE(ev_zero_dim != ev);
		CHECK_FALSE(ev != ev);
	}

	SECTION("Non Zero dimension") {
		auto const ev = comp6771::euclidean_vector{0.5, -3.5, 4.25, 3.09};
		auto const ev_same = comp6771::euclidean_vector{0.5, -3.5, 4.25, 3.09};

		auto const ev_diff_1 = comp6771::euclidean_vector{0.5, -3.5, -4.25, 3.09};
		auto const ev_diff_2 = comp6771::euclidean_vector{0.5, -3.5};

		SECTION("Same dimensions, different magnitudes") {
			CHECK(ev != ev_diff_1);
		}

		SECTION("Different dimensions") {
			CHECK(ev != ev_zero_dim);
			CHECK(ev != ev_diff_2);
		}

		SECTION("Equal to itself") {
			CHECK_FALSE(ev != ev);
		}

		SECTION("Equal in dimensions and magnitudes") {
			CHECK_FALSE(ev != ev_same);
		}
	}
}

/*
   Test cases:
   - If performing these operators yield the expected result
   - If exception is thrown in appriopriate situtaion with the correct message
   - If 0 dimension is handled correctly

   Assumes casting to vector is implementing correctly.

   Rational: For arithmetic operators just check if they are invoked correctly and product the right
   result and throw the right exception.
*/
TEST_CASE("+ - * /: Test vectors with non zero Dimension") {
	auto const ev = comp6771::euclidean_vector{0.5, -3.5, 4.25, 3.09};
	auto const ev_same_dim_1 = comp6771::euclidean_vector{1.5, 10.2, 3.14, 5.2};
	auto const ev_same_dim_2 = comp6771::euclidean_vector{1, -1, 1, -1};

	SECTION("+") {
		CHECK_THAT(static_cast<std::vector<double>>(ev + ev_same_dim_1),
		           Catch::Approx(std::vector<double>{2, 6.7, 7.39, 8.29}));

		CHECK_THAT(static_cast<std::vector<double>>(ev + ev_same_dim_1 + ev_same_dim_2),
		           Catch::Approx(std::vector<double>{3, 5.7, 8.39, 7.29}));
	}

	SECTION("-") {
		CHECK_THAT(static_cast<std::vector<double>>(ev - ev_same_dim_1),
		           Catch::Approx(std::vector<double>{-1, -13.7, 1.11, -2.11}));

		CHECK_THAT(static_cast<std::vector<double>>(ev - ev_same_dim_1 - ev_same_dim_2),
		           Catch::Approx(std::vector<double>{-2, -12.7, 0.11, -1.11}));
	}

	SECTION("*") {
		CHECK_THAT(static_cast<std::vector<double>>(ev * 0),
		           Catch::Approx(std::vector<double>{0, 0, 0, 0}));

		CHECK_THAT(static_cast<std::vector<double>>(ev * 2),
		           Catch::Approx(std::vector<double>{1, -7, 8.5, 6.18}));
	}

	SECTION("/") {
		CHECK_THAT(static_cast<std::vector<double>>(ev / 2),
		           Catch::Approx(std::vector<double>{0.25, -1.75, 2.125, 1.545}));

		CHECK_THAT(static_cast<std::vector<double>>(ev / 2 / 0.2),
		           Catch::Approx(std::vector<double>{1.25, -8.75, 10.625, 7.725}));
	}

	SECTION("Exceptions") {
		auto const ev_diff_dim_1 = comp6771::euclidean_vector{0.5, -3.5};
		auto const ev_diff_dim_2 = comp6771::euclidean_vector(0);

		SECTION("Plus and minus: Mismatch dimensions") {
			CHECK_THROWS_MATCHES(ev + ev_diff_dim_1,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Dimensions of LHS(4) and RHS(2) do not "
			                                              "match"));

			CHECK_THROWS_MATCHES(ev + ev_diff_dim_2,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Dimensions of LHS(4) and RHS(0) do not "
			                                              "match"));

			CHECK_THROWS_MATCHES(ev - ev_diff_dim_1,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Dimensions of LHS(4) and RHS(2) do not "
			                                              "match"));

			CHECK_THROWS_MATCHES(ev - ev_diff_dim_2,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Dimensions of LHS(4) and RHS(0) do not "
			                                              "match"));
		}

		SECTION("/: Division by 0") {
			CHECK_THROWS_MATCHES(ev / 0,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Invalid vector division by 0"));
		}
	}
}

/*
   Check if performing these operations on 0 dimension vectors yield 0 dimension vector. And
   exception will be thrown when appropriate.

   Rational: Since this case is only for zero dimension vectors, we don't need to check for
   exception thrown due to dimension mismatch.
*/
TEST_CASE("+ - * /: Test vectors with zero dimension") {
	auto const ev_zero_dim_1 = comp6771::euclidean_vector(0);
	auto const ev_zero_dim_2 = comp6771::euclidean_vector({});

	SECTION("All results should vectors of zero dimension") {
		CHECK((ev_zero_dim_1 + ev_zero_dim_2).dimensions() == 0);
		CHECK((ev_zero_dim_1 - ev_zero_dim_2).dimensions() == 0);
		CHECK((ev_zero_dim_1 * 2).dimensions() == 0);
		CHECK((ev_zero_dim_1 / 2).dimensions() == 0);
	}

	SECTION("Exception: Division by zero") {
		CHECK_THROWS_MATCHES(ev_zero_dim_1 / 0,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Invalid vector division by 0"));
	}
}

/*
   Test cases:
   - Output a 0-dimension vector gives []
   - Output non-zero dimension vector and compare with expected output

   Rational: These tests covers all use case of this oeprator
*/
TEST_CASE("Output Stream") {
	SECTION("Zero Dimension: Yield []") {
		auto const ev_zero_dim_1 = comp6771::euclidean_vector(0);

		auto oss = std::ostringstream{};
		oss << ev_zero_dim_1;
		CHECK(oss.str() == "[]");
	}

	SECTION("Non-zero Dimension: Yield magnitudes of the inputted euclidean vector") {
		auto const ev = comp6771::euclidean_vector{3, 5.7, 8.39, 7.29};

		auto oss = std::ostringstream{};
		oss << ev;
		CHECK(oss.str() == "[3 5.7 8.39 7.29]");
	}
}