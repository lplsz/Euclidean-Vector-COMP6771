#include "comp6771/euclidean_vector.hpp"
#include <algorithm>
#include <catch2/catch.hpp>
#include <sstream>
#include <vector>

/*
    Tests in this file test the class by mixing the uses of different operators and operation.

    These test assume the correctness of the contructors.

    All expected output is calculated by wolframAlpha.

    Rational: Sometimes unit testing is not sufficient to ensure the correctness of using all
    operations together. So it is worth testing. There's no need to test exception throwing as the
    unit test will be sufficient.
*/

TEST_CASE("Integration Test") {
	auto ev_four_dim_1 = comp6771::euclidean_vector{2.54, 7.98, -3.76, 4.25};
	auto ev_four_dim_2 = comp6771::euclidean_vector{-8.964, 5.2, 13.19, -5.2};

	/* Test: Casting to std::vector */
	CHECK_THAT(static_cast<std::vector<double>>(ev_four_dim_1),
	           Catch::Approx(std::vector<double>{2.54, 7.98, -3.76, 4.25}));

	CHECK_THAT(static_cast<std::vector<double>>(ev_four_dim_2),
	           Catch::Approx(std::vector<double>{-8.964, 5.2, 13.19, -5.2}));

	/* Test Friend functions */

	// Arithmetic Operators
	CHECK_THAT(static_cast<std::vector<double>>(ev_four_dim_1 * 0.3 + ev_four_dim_2 / (-2.5)),
	           Catch::Approx(std::vector<double>{4.3476, 0.314, -6.404, 3.355}));

	CHECK_THAT(static_cast<std::vector<double>>(ev_four_dim_1 / 3.598 - ev_four_dim_2 * (3.14)),
	           Catch::Approx(std::vector<double>{28.85290, -14.11010, -42.46162, 17.50921}));

	// Output Stream
	auto oss = std::ostringstream{};
	oss << (ev_four_dim_1 / 3.598 - ev_four_dim_2 * (3.14));
	CHECK(oss.str() == "[28.8529 -14.1101 -42.4616 17.5092]");

	// == !=
	CHECK(ev_four_dim_1 != ev_four_dim_2);
	CHECK(ev_four_dim_1 * 0.5 != ev_four_dim_2 * 3.14);

	CHECK(ev_four_dim_1 * 0.3 / 0.3 == ev_four_dim_1);

	/* Test member functions */
	ev_four_dim_1 += ev_four_dim_2;
	ev_four_dim_1 *= 3.14;

	CHECK_THAT(static_cast<std::vector<double>>(ev_four_dim_1),
	           Catch::Approx(std::vector<double>{-20.1714, 41.3852, 29.6102, -2.983}));

	/* Test Utility functions */

	// Norm & Unit Vector
	CHECK(comp6771::euclidean_norm(ev_four_dim_1) == Approx(54.8204));
	CHECK_THAT(static_cast<std::vector<double>>(comp6771::unit(ev_four_dim_1)),
	           Catch::Approx(std::vector<double>{-0.367953, 0.754923, 0.540131, -0.054414}));

	ev_four_dim_1[3] = 3.1415926;
	CHECK(comp6771::euclidean_norm(ev_four_dim_1) == Approx(54.8293));
	CHECK_THAT(static_cast<std::vector<double>>(comp6771::unit(ev_four_dim_1)),
	           Catch::Approx(std::vector<double>{-0.367894, 0.754801, 0.540043, 0.0572977}));

	// Dot product
	CHECK(comp6771::dot(ev_four_dim_1, ev_four_dim_2) == Approx(770.24172608));

	ev_four_dim_2 /= 0.45;
	CHECK_THAT(static_cast<std::vector<double>>(ev_four_dim_2),
	           Catch::Approx(std::vector<double>{-19.92, 11.5556, 29.3111, -11.5556}));

	CHECK(comp6771::dot(ev_four_dim_1, ev_four_dim_2) == Approx(1711.64965089144));
}