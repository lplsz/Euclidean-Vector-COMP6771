#include "comp6771/euclidean_vector.hpp"

#include <algorithm>
#include <catch2/catch.hpp>

/*
    Tests in the file test if the members have the right behavior and if
    they throw the correct exception with correct message in appropriate
    situtation.

    These tests assumes that the constructors and casting to std::vector are
    correct.

   Rational: Test these member operators and functions's usage on const and non-const will be
   sufficient to ensure their correctness.
*/

/*
   Test case for non-const
   - Can access the value in the given dimension
   - Can modify the value in the given dimension
   - Is exception thrown for invalid index

   Test case for const
   - Can access the value in the given dimension
   - Is exception thrown for invalid index

   Rational:
   - Other than testing the non zero dimension vectors, since zero dimension vector has no
   magnitude so should always throw excpetion which should be tested.
   - Non const vector should be able to modify the magnitude given the reference returned.

*/
TEST_CASE("Subscript") {
	SECTION("Const Version") {
		SECTION("Non zero dimension") {
			auto const ev = comp6771::euclidean_vector{0.0, 1.5, -4.5, 40.0};

			SECTION("Return the correct value of the given magnitude") {
				CHECK(ev.at(0) == Approx(0.0));
				CHECK(ev.at(1) == Approx(1.5));
				CHECK(ev.at(2) == Approx(-4.5));
				CHECK(ev.at(3) == Approx(40.0));
			}

			SECTION("Exception: Index out of range") {
				CHECK_THROWS_MATCHES(ev.at(-1),
				                     comp6771::euclidean_vector_error,
				                     Catch::Matchers::Message("Index -1 is not valid for this "
				                                              "euclidean_vector object"));

				CHECK_THROWS_MATCHES(ev.at(4),
				                     comp6771::euclidean_vector_error,
				                     Catch::Matchers::Message("Index 4 is not valid for this "
				                                              "euclidean_vector "
				                                              "object"));
			}
		}

		SECTION("Exception: Access magnitude in zero Dimension vectors") {
			auto const ev = comp6771::euclidean_vector(0);

			CHECK_THROWS_MATCHES(ev.at(0),
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Index 0 is not valid for this "
			                                              "euclidean_vector object"));
		}
	}

	SECTION("Non-const Version") {
		SECTION("Non zero dimension") {
			auto ev = comp6771::euclidean_vector{0.0, 1.5, -4.5, 40.0};

			SECTION("Return the correct value of the given magnitude") {
				CHECK(ev.at(0) == Approx(0.0));
				CHECK(ev.at(1) == Approx(1.5));
				CHECK(ev.at(2) == Approx(-4.5));
				CHECK(ev.at(3) == Approx(40.0));
			}

			SECTION("Exception: Index out of range") {
				CHECK_THROWS_MATCHES(ev.at(-1),
				                     comp6771::euclidean_vector_error,
				                     Catch::Matchers::Message("Index -1 is not valid for this "
				                                              "euclidean_vector object"));

				CHECK_THROWS_MATCHES(ev.at(4),
				                     comp6771::euclidean_vector_error,
				                     Catch::Matchers::Message("Index 4 is not valid for this "
				                                              "euclidean_vector "
				                                              "object"));
			}

			SECTION("Modify the dimension in the given dimension") {
				ev.at(0) = 5.3;
				ev.at(1) = -3.19;
				ev.at(2) = 4.25;
				ev.at(3) = 8.18;

				// Verify
				auto ev_exp = std::vector<double>{5.3, -3.19, 4.25, 8.18};
				CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(ev_exp));
			}
		}

		SECTION("Zero Dimension") {
			auto ev = comp6771::euclidean_vector(0);

			CHECK_THROWS_MATCHES(ev.at(0),
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Index 0 is not valid for this "
			                                              "euclidean_vector object"));
		}
	}
}

/*
    Test if the member function dimensions() return the correct dimensions for const and non-const
    objects.

   Rational: This is a non-modifying function, so should be about to be invoked by both const and
   non-const objects.
*/
TEST_CASE("Dimensions()") {
	SECTION("Non-const: Dimension matches") {
		auto ev1 = comp6771::euclidean_vector(0);
		auto ev2 = comp6771::euclidean_vector(5, 0.5);
		auto ev3 = comp6771::euclidean_vector{4.5, 3, 6.7};

		CHECK(ev1.dimensions() == 0);
		CHECK(ev2.dimensions() == 5);
		CHECK(ev3.dimensions() == 3);
	}

	SECTION("Const: Dimension matches") {
		auto const ev1 = comp6771::euclidean_vector(0);
		auto const ev2 = comp6771::euclidean_vector(5, 0.5);
		auto const ev3 = comp6771::euclidean_vector{4.5, 3, 6.7};

		CHECK(ev1.dimensions() == 0);
		CHECK(ev2.dimensions() == 5);
		CHECK(ev3.dimensions() == 3);
	}
}