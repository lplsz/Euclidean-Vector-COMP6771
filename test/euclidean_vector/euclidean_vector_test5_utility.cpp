#include "comp6771/euclidean_vector.hpp"
#include <algorithm>
#include <catch2/catch.hpp>

#include <vector>

/*
    Tests in this file tests if the utility functions have be implemently correctly.
    By that, it means the result computed is expected and appropriate exception
    is throw.

    All tests assume that casting to vector is correctly implemented.

    Rational: These functions can only do computation or throw exception. So it is sufficient to see
   if exception is thrown and if the computation is correct.

   All the expected value are computed using scientific calcultor.
*/

TEST_CASE("Norm") {
	SECTION("Zero Dimension: Have norm of 0") {
		auto const ev_zero_dim = comp6771::euclidean_vector(0);

		CHECK(comp6771::euclidean_norm(ev_zero_dim) == Approx(0));
	}

	SECTION("Non-zero Dimension: Match expected norm calculated by calculator") {
		SECTION("Test 1") {
			auto const ev = comp6771::euclidean_vector{1, 2, 3};

			CHECK(comp6771::euclidean_norm(ev) == Approx(3.7416573867739));
		}

		SECTION("Test 2") {
			auto const ev = comp6771::euclidean_vector{4.5, 6.771, -2.041, 25.11, 92.43, -608};

			CHECK(comp6771::euclidean_norm(ev) == Approx(615.55509836407));
		}
	}
}

/*
   Since we maintain a cache of the norm, use different mutating opertions and see if cache has been
   invalidated and if the norm is computed.
 */
TEST_CASE("Norm Cache Invalidation") {
	auto ev = comp6771::euclidean_vector{1, 2, 3};

	// Compute the norm, cause it to be cached
	CHECK(comp6771::euclidean_norm(ev) == Approx(3.7416573867739));

	SECTION("Non const []") {
		ev[1] = 5;

		CHECK(comp6771::euclidean_norm(ev) == Approx(5.9160797830996));
	}

	SECTION("+=") {
		ev += comp6771::euclidean_vector{6, 7, 8};

		CHECK(comp6771::euclidean_norm(ev) == Approx(15.842979517755));
	}

	SECTION("-=") {
		ev -= comp6771::euclidean_vector{9, 4, 25};

		CHECK(comp6771::euclidean_norm(ev) == Approx(23.494680248941));
	}

	SECTION("*=") {
		ev *= 2;

		CHECK(comp6771::euclidean_norm(ev) == Approx(7.4833147735479));
	}

	SECTION("/=") {
		ev /= 0.5;

		CHECK(comp6771::euclidean_norm(ev) == Approx(7.4833147735479));
	}

	SECTION("Non const at") {
		ev.at(1) = 5;

		CHECK(comp6771::euclidean_norm(ev) == Approx(5.9160797830996));
	}
}

TEST_CASE("Unit Vector") {
	SECTION("Test if the correct unit vector is return") {
		SECTION("Test 1") {
			auto const ev = comp6771::euclidean_vector{0, 8, 6, 0};
			auto const ev_unit_exp = std::vector<double>{0, 0.8, 0.6, 0};

			CHECK_THAT(static_cast<std::vector<double>>(comp6771::unit(ev)), Catch::Approx(ev_unit_exp));
		}

		SECTION("Test 2") {
			auto const ev = comp6771::euclidean_vector{7, 4, 6, -1, -5, 9};
			auto const ev_unit_exp = std::vector<double>{0.485362671,
			                                             0.277350098,
			                                             0.416025147,
			                                             -0.069337524,
			                                             -0.346687622,
			                                             0.624037720};

			CHECK_THAT(static_cast<std::vector<double>>(comp6771::unit(ev)), Catch::Approx(ev_unit_exp));
		}
	}

	SECTION("Exception: No dimension") {
		auto const ev_zero_dim = comp6771::euclidean_vector(0);

		CHECK_THROWS_MATCHES(comp6771::unit(ev_zero_dim),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("euclidean_vector with no dimensions does not "
		                                              "have a unit vector"));
	}

	SECTION("Exception: Norm == 0") {
		auto const ev_zero_norm = comp6771::euclidean_vector{0, 0, 0, 0};

		CHECK_THROWS_MATCHES(comp6771::unit(ev_zero_norm),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("euclidean_vector with zero euclidean normal "
		                                              "does not have a unit vector"));
	}
}

TEST_CASE("Dot Product") {
	SECTION("Zero Dimension: Return 0 as the dot product") {
		/* Two 0 dimension should gives dot product 0 */
		auto const ev1 = comp6771::euclidean_vector(0);
		auto const ev2 = comp6771::euclidean_vector(0);

		CHECK(comp6771::dot(ev1, ev2) == 0);
	}

	SECTION("Non zero Dimension: Return the sum of squares in magnitude") {
		SECTION("Test 1") {
			auto const ev = comp6771::euclidean_vector{1, 2};
			auto const ev_same_dim = comp6771::euclidean_vector{3, 4};

			CHECK(comp6771::dot(ev, ev_same_dim) == Approx(11));
		}

		SECTION("Test 2") {
			auto const ev = comp6771::euclidean_vector{-1.2, 2.5, 3.7, 4.5};
			auto const ev_same_dim = comp6771::euclidean_vector{-4.5, 3, 2.7, 1.5};

			CHECK(comp6771::dot(ev, ev_same_dim) == Approx(29.64));
		}
	}

	SECTION("Exception: Dimension not match") {
		auto const ev = comp6771::euclidean_vector{1, 2, 3, 4};
		auto const ev_diff_dim_1 = comp6771::euclidean_vector{2, 3, 4, 5, 6, 9, 0};
		auto const ev_diff_dim_2 = comp6771::euclidean_vector(0);

		CHECK_THROWS_MATCHES(comp6771::dot(ev, ev_diff_dim_1),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Dimensions of LHS(4) and RHS(7) do not "
		                                              "match"));

		CHECK_THROWS_MATCHES(comp6771::dot(ev, ev_diff_dim_2),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Dimensions of LHS(4) and RHS(0) do not "
		                                              "match"));
	}
}
