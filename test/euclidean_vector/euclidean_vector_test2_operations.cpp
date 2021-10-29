#include "comp6771/euclidean_vector.hpp"

#include <algorithm>
#include <catch2/catch.hpp>
#include <list>
#include <memory>
#include <sstream>
#include <vector>

/*
    Tests in the file test if the operations have the right behavior and if
    they throw the correct exception with correct message in appropriate
    situtation.

    These tests assumes that the constructors, dimensions getter and casting to std::vector are
    correct.
*/

/*
    Test case:
    - copy assign to an euclidean_vector(EV) of equal dimensions
    - copy assign to an euclidean_vector of different dimensions

   Assume the += operator is correctly implemented.

    Approach:
    1. Initialize some EVs of different dimensions that act as the copied from
    2. Initialize some EVs of different dimensions that act as the copied to
    3. Perform copy assignment
    4. Check that the copied to have contents equal to the copied from by casting them
       to std::vector
    5. Check that modifying the copied to will not affect the copied from (Ensure they are not
       connected underlyingly)

   Rational:
   - The semantics of copy assignment ensure that the copied from and to objects has the
   same magnitudes and has not underlying sharing data members. So the above test can ensure its
   behavior are correct.
   - Copy assignment can be perform between vectors of different internal states
   - Copying itself should not be a problem for robustness.
*/
TEST_CASE("Copy Assignment") {
	// Initialized copied from objects
	auto const ev1 = comp6771::euclidean_vector(0);
	auto const ev2 = comp6771::euclidean_vector{0.0, 1.5, -4.5, 40.0};
	auto const ev3 = comp6771::euclidean_vector{1.5, 3.0};

	auto const ev1_exp = std::vector<double>();
	auto const ev2_exp = std::vector<double>{0.0, 1.5, -4.5, 40.0};
	auto const ev3_exp = std::vector<double>{1.5, 3.0};

	// Initialized copied to objects
	auto ev1_copy = comp6771::euclidean_vector(10);
	auto ev2_copy = comp6771::euclidean_vector(9, -0.5);
	auto ev3_copy = comp6771::euclidean_vector(2, 0.5);

	// Invoke copy assignment operator
	ev1_copy = ev1;
	ev2_copy = ev2;
	ev3_copy = ev3;

	SECTION("Both the copy to object matches expected vector") {
		CHECK_THAT(static_cast<std::vector<double>>(ev1_copy), Catch::Approx(ev1_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev2_copy), Catch::Approx(ev2_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev3_copy), Catch::Approx(ev3_exp));

		CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev2), Catch::Approx(ev2_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev3), Catch::Approx(ev3_exp));
	}

	SECTION("Modifying the copied to will not affect the copied from") {
		ev2_copy += ev2;
		ev3_copy += ev3;

		CHECK_THAT(static_cast<std::vector<double>>(ev2), Catch::Approx(ev2_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev3), Catch::Approx(ev3_exp));
	}

	SECTION("Copy Assignment to itself") {
		ev1_copy = ev1_copy;
		ev2_copy = ev2_copy;
		ev3_copy = ev3_copy;

		CHECK_THAT(static_cast<std::vector<double>>(ev1_copy), Catch::Approx(ev1_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev2_copy), Catch::Approx(ev2_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev3_copy), Catch::Approx(ev3_exp));
	}
}

/*
    Test case:
    - move assign to an euclidean_vector(EV) of equal dimensions
    - move assign to an euclidean_vector of different dimensions

    Approach:
    1. Initialize some EVs of different dimensions that act as the moved from
    2. Initialize some EVs of different dimensions that act as the moved to
    3. Perform move assignment
    4. Check that the copied to have contents equal to the copied from by casting them
       to std::vector
    5. Check that the moved from has dimension of 0

   Rational:
   - Testing the above ensures correctness of the move assignment operator by testing all
   its behavior
*/
TEST_CASE("Move Assignment") {
	// Initialized moved from objects
	auto ev1 = comp6771::euclidean_vector(0);
	auto ev2 = comp6771::euclidean_vector{0.0, 1.5, -4.5, 40.0};
	auto ev3 = comp6771::euclidean_vector{1.5, 3.0};

	auto const ev1_exp = std::vector<double>();
	auto const ev2_exp = std::vector<double>{0.0, 1.5, -4.5, 40.0};
	auto const ev3_exp = std::vector<double>{1.5, 3.0};

	// Initialized moved to objects
	auto ev1_new = comp6771::euclidean_vector(10);
	auto ev2_new = comp6771::euclidean_vector(9, -0.5);
	auto ev3_new = comp6771::euclidean_vector(2, 0.5);

	// Invoke move assignment operator
	ev1_new = std::move(ev1);
	ev2_new = std::move(ev2);
	ev3_new = std::move(ev3);

	SECTION("The moved to object has the correct magnitudes") {
		CHECK_THAT(static_cast<std::vector<double>>(ev1_new), Catch::Approx(ev1_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev2_new), Catch::Approx(ev2_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev3_new), Catch::Approx(ev3_exp));
	}

	SECTION("The moved-from objects are in a state with 0 dimension (According to the spec)") {
		CHECK(ev1.dimensions() == 0);
		CHECK(ev2.dimensions() == 0);
		CHECK(ev3.dimensions() == 0);
	}
}

/*
   Test case for non-const
   - Can access the value in the given dimension
   - Can modify the value in the given dimension

   Test case for const
   - Can access the value in the given dimension

   Rational: Since the spec says these function will not throw exception and will terminate for
   invalid input, we only need to check that they perform correctly for valid inputes.
*/
TEST_CASE("Subscript") {
	SECTION("Const Version") {
		auto const ev = comp6771::euclidean_vector{0.0, 1.5, -4.5, 40.0};

		SECTION("Return the correct value of the given magnitude") {
			CHECK(ev[0] == Approx(0.0));
			CHECK(ev[1] == Approx(1.5));
			CHECK(ev[2] == Approx(-4.5));
			CHECK(ev[3] == Approx(40.0));
		}
	}

	SECTION("Non-const Version") {
		auto ev = comp6771::euclidean_vector{0.0, 1.5, -4.5, 40.0};

		SECTION("Return the correct value of the given magnitude") {
			CHECK(ev[0] == Approx(0.0));
			CHECK(ev[1] == Approx(1.5));
			CHECK(ev[2] == Approx(-4.5));
			CHECK(ev[3] == Approx(40.0));
		}

		SECTION("Modify the dimension in the given dimension") {
			ev[0] = 5.3;
			ev[1] = -3.19;
			ev[2] = 4.25;
			ev[3] = 8.18;

			// Check the change is correctly reflected
			CHECK(ev[0] == Approx(5.3));
			CHECK(ev[1] == Approx(-3.19));
			CHECK(ev[2] == Approx(4.25));
			CHECK(ev[3] == Approx(8.18));
		}
	}
}

/*
   Test if the unary operators are overloading correctly.
   Cases:
   - Whether the value is expected
   - Whether a copy is returned, modifying one will not affect the other
   - The original vector stays the same


   Assumes the *= operator is correctly overloaded.

   Rational:
   - In the case of the uanry plus, except for testing the the vector is returned with the
   correct dimension and magnitudes, we should also test if modifying one will not affect the
   other, so that we can ensure they have different underlying data members.
   - After return this copy, the original vector's content should remain the same.
*/
TEST_CASE("Unary Operations: Non-zero dimension vector") {
	auto ev1 = comp6771::euclidean_vector{5.3, -3.19, 4.25, 8.18};
	auto ev1_exp = std::vector<double>{5.3, -3.19, 4.25, 8.18};

	SECTION("Unary Plus +") {
		auto ev2 = (+ev1);
		auto ev2_exp = std::vector<double>{5.3, -3.19, 4.25, 8.18};

		SECTION("Returned vector has expected magnitudes") {
			REQUIRE_THAT(static_cast<std::vector<double>>(ev2), Catch::Approx(ev2_exp));
		}

		SECTION("The original vector should stay the same") {
			CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
		}

		// Modify ev1, check if ev2 stays the same
		SECTION("Modify one, the other stay the same") {
			ev1 *= 2;
			CHECK_THAT(static_cast<std::vector<double>>(ev2), Catch::Approx(ev2_exp));
		}
	}

	SECTION("Negation -") {
		SECTION("Returned vector has expected magnitudes") {
			auto ev2 = (-ev1);
			auto ev2_exp = std::vector<double>{-5.3, 3.19, -4.25, -8.18};
			CHECK_THAT(static_cast<std::vector<double>>(ev2), Catch::Approx(ev2_exp));
		}

		SECTION("The original vector should stay the same") {
			CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
		}
	}
}

/*
   Rational: The unary operator + and - make sense for a zero dimension vector, so we should test if
   they are behaving properly by producing a zero dimension vector.
*/
TEST_CASE("Unary Operations: Zero dimension vector") {
	auto const ev_zero_dim = comp6771::euclidean_vector(0);

	auto ev1 = (+ev_zero_dim);
	auto ev2 = (-ev_zero_dim);

	CHECK(ev1.dimensions() == 0);
	CHECK(ev2.dimensions() == 0);
}

/*
   Test if the compound operators are overloading correctly.
   Approach (Each respetive session):
   - Test if these operator will correctly update the value of the source euclidean vector
   - Test if performing them on themselves will produce the correct value
   - Test if the correct exception is thrown with the correct message

   Rational: Testing the above can ensure the correctness of these operators as these are all the
   behaviors they have. The behavior when adding two same vector object also should produce the
   right result.
*/
TEST_CASE("Compound Operators") {
	auto ev1 = comp6771::euclidean_vector{5.3, -3.19, 4.25, 8.18};
	auto ev2_same_dimension = comp6771::euclidean_vector{1.0, -5.19, 9.25, 0.0};
	auto ev3_diff_dimension = comp6771::euclidean_vector{5.2, 13.14};

	SECTION("+=: Match expected output") {
		SECTION("Add another instance") {
			ev1 += ev2_same_dimension;
			auto ev1_exp = std::vector<double>{6.3, -8.38, 13.5, 8.18};
			REQUIRE_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
		}

		SECTION("Plus itself") {
			ev1 += ev1;
			auto ev1_exp = std::vector<double>{10.6, -6.38, 8.5, 16.36};
			CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
		}
	}

	SECTION("-=: Match expected output") {
		SECTION("Minus another instance") {
			ev1 -= ev2_same_dimension;
			auto ev1_exp = std::vector<double>{4.3, 2.0, -5.0, 8.18};
			REQUIRE_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
		}

		SECTION("Minus itself") {
			ev1 -= ev1;
			auto ev1_exp = std::vector<double>{0.0, 0.0, 0.0, 0.0};
			CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
		}
	}

	SECTION("*=: Match expected output") {
		SECTION("Time non zero double") {
			ev1 *= 2.0;
			auto ev1_exp = std::vector<double>{10.6, -6.38, 8.5, 16.36};
			CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
		}

		SECTION("Time 0") {
			ev1 *= 0;
			auto ev1_exp = std::vector<double>{0, 0, 0, 0};
			CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
		}
	}

	SECTION("/=: Match expected output") {
		ev1 /= 2.0;
		auto ev1_exp = std::vector<double>{2.65, -1.595, 2.125, 4.09};
		CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
	}

	SECTION("Exception of += and -=: Not matching dimension") {
		CHECK_THROWS_MATCHES(ev1 += ev3_diff_dimension,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Dimensions of LHS(4) and RHS(2) do not "
		                                              "match"));

		CHECK_THROWS_MATCHES(ev1 -= ev3_diff_dimension,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Dimensions of LHS(4) and RHS(2) do not "
		                                              "match"));
	}

	SECTION("Exception of /=: Division by 0") {
		CHECK_THROWS_MATCHES(ev1 /= 0,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Invalid vector division by 0"));
	}

	SECTION("0 dimension: The result between two 0 dimensions should yield a 0 dimension "
	        "vector") {
		auto zero_dim_1 = comp6771::euclidean_vector(0);
		auto zero_dim_2 = comp6771::euclidean_vector(0);

		SECTION("+=") {
			CHECK_NOTHROW(zero_dim_1 += zero_dim_2);
			CHECK(zero_dim_1.dimensions() == 0);

			CHECK_NOTHROW(zero_dim_1 += zero_dim_1);
			CHECK(zero_dim_1.dimensions() == 0);
		}

		SECTION("-=") {
			CHECK_NOTHROW(zero_dim_1 -= zero_dim_2);
			CHECK(zero_dim_1.dimensions() == 0);

			CHECK_NOTHROW(zero_dim_1 -= zero_dim_1);
			CHECK(zero_dim_1.dimensions() == 0);
		}

		SECTION("*=") {
			CHECK_NOTHROW(zero_dim_1 *= 2.0);
			CHECK(zero_dim_1.dimensions() == 0);

			CHECK_NOTHROW(zero_dim_1 *= 2.0);
			CHECK(zero_dim_1.dimensions() == 0);
		}

		SECTION("/=") {
			CHECK_NOTHROW(zero_dim_1 /= 2.0);
			CHECK(zero_dim_1.dimensions() == 0);

			CHECK_NOTHROW(zero_dim_1 /= 2.0);
			CHECK(zero_dim_1.dimensions() == 0);
		}
	}
}

/*
   Cast euclidean_vectors to vector and check if the conversion is correct.
   Test cases:
   - Check if 0 dimensions euclidean_vectors work
   - Check if non-0 dimensions euclidean_vectors work
*/
TEST_CASE("Vector Type Conversion") {
	SECTION("Zero dimensions: Empty vector") {
		auto const ev1 = comp6771::euclidean_vector(0);
		auto const ev1_exp = std::vector<double>();

		CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
	}

	SECTION("Non-zero dimensions") {
		auto const ev1 = comp6771::euclidean_vector{5.3, -3.19, 4.25, 8.18};
		auto const ev1_exp = std::vector<double>{5.3, -3.19, 4.25, 8.18};

		CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
	}
}

/*
   Cast euclidean_vectors to list and check if the conversion is correct by compare to a manual
   constructed list. Test cases:
   - Check if 0 dimensions euclidean_vectors work
   - Check if non-0 dimensions euclidean_vectors work
*/
TEST_CASE("List Type Conversion") {
	SECTION("Zero dimensions: Empty List") {
		auto const ev1 = comp6771::euclidean_vector(0);
		auto const ev1_list = static_cast<std::list<double>>(ev1);
		auto const ev1_exp = std::list<double>();

		CHECK(ev1_list == ev1_exp);
	}

	SECTION("Non-zero dimensions") {
		auto const ev1 = comp6771::euclidean_vector{5.3, -3.19, 4.25, 8.18};
		auto const ev1_list = static_cast<std::list<double>>(ev1);

		auto const ev1_exp = std::list<double>{5.3, -3.19, 4.25, 8.18};

		CHECK(ev1_list == ev1_exp);
	}
}