#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <list>
#include <memory>
#include <sstream>
#include <vector>

/*
    Tests in this file is for the purpose of testing all the constructors.

    It relies on the overloaded cast to vector to be correct as
    there is the need to inspect the magnitudes.

    Testing the move constructor assumes that the dimensions() getter works.

    General approach:
    1. Construct a euclidean_vector
    2. Construct the expected casting to a std::vector or std::list
    3. Compare if they are equal

   Rational: Testing the magnitudes matches will be sufficient to test simple constructors
*/

TEST_CASE("Default Constructor: 1 Dimension of magnitude 0") {
	auto const ev1 = comp6771::euclidean_vector();

	auto const ev1_exp = std::vector<double>{0.0};
	CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
}

TEST_CASE("Single argument constructor: Take in the size and make magnitudes 0") {
	auto const ev1 = comp6771::euclidean_vector(0);
	auto const ev2 = comp6771::euclidean_vector(2);
	auto const ev3 = comp6771::euclidean_vector(5);

	auto const ev1_exp = std::vector<double>();
	auto const ev2_exp = std::vector<double>{0.0, 0.0};
	auto const ev3_exp = std::vector<double>{0.0, 0.0, 0.0, 0.0, 0.0};

	CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
	CHECK_THAT(static_cast<std::vector<double>>(ev2), Catch::Approx(ev2_exp));
	CHECK_THAT(static_cast<std::vector<double>>(ev3), Catch::Approx(ev3_exp));
}

TEST_CASE("Num dimensions - magnitudes constructor") {
	auto const ev1 = comp6771::euclidean_vector(0, 1.5);
	auto const ev2 = comp6771::euclidean_vector(3, 1.5);
	auto const ev3 = comp6771::euclidean_vector(5, 4.5);

	auto const ev1_exp = std::vector<double>();
	auto const ev2_exp = std::vector<double>{1.5, 1.5, 1.5};
	auto const ev3_exp = std::vector<double>{4.5, 4.5, 4.5, 4.5, 4.5};

	CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
	CHECK_THAT(static_cast<std::vector<double>>(ev2), Catch::Approx(ev2_exp));
	CHECK_THAT(static_cast<std::vector<double>>(ev3), Catch::Approx(ev3_exp));
}

TEST_CASE("Vector Iterator constructor") {
	auto const ev1_exp = std::vector<double>();
	auto const ev2_exp = std::vector<double>{0.0, 1.5, -4.5, 40.0};
	auto const ev3_exp = std::vector<double>{1.5, 3.0};

	auto const ev1 = comp6771::euclidean_vector(ev1_exp.begin(), ev1_exp.end());
	auto const ev2 = comp6771::euclidean_vector(ev2_exp.begin(), ev2_exp.end());
	auto const ev3 = comp6771::euclidean_vector(ev3_exp.begin(), ev3_exp.end());

	CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
	CHECK_THAT(static_cast<std::vector<double>>(ev2), Catch::Approx(ev2_exp));
	CHECK_THAT(static_cast<std::vector<double>>(ev3), Catch::Approx(ev3_exp));
}

TEST_CASE("Initialiser List constructor") {
	auto const ev1 = comp6771::euclidean_vector({});
	auto const ev2 = comp6771::euclidean_vector{0.0, 1.5, -4.5, 40.0};
	auto const ev3 = comp6771::euclidean_vector{1.5, 3.0};

	auto const ev1_exp = std::vector<double>();
	auto const ev2_exp = std::vector<double>{0.0, 1.5, -4.5, 40.0};
	auto const ev3_exp = std::vector<double>{1.5, 3.0};

	CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
	CHECK_THAT(static_cast<std::vector<double>>(ev2), Catch::Approx(ev2_exp));
	CHECK_THAT(static_cast<std::vector<double>>(ev3), Catch::Approx(ev3_exp));
}

/*
   Rational:
   - Other then checking if the constructed euclidean_vectors are equal, check
   if modifying the copy will not change the original. To ensure that the copied to object doesn't
   share the same data member with the copied to object.

   Assume the += operator is correctly overloaded.
*/
TEST_CASE("Copy constructor") {
	auto const ev1 = comp6771::euclidean_vector(0);
	auto const ev2 = comp6771::euclidean_vector{0.0, 1.5, -4.5, 40.0};
	auto const ev3 = comp6771::euclidean_vector{1.5, 3.0};

	// Invoke copy constructor
	auto ev1_copy = ev1;
	auto ev2_copy = ev2;
	auto ev3_copy = ev3;

	auto const ev1_exp = std::vector<double>();
	auto const ev2_exp = std::vector<double>{0.0, 1.5, -4.5, 40.0};
	auto const ev3_exp = std::vector<double>{1.5, 3.0};

	SECTION("Both the copy and the original version should match the expecting vector") {
		CHECK_THAT(static_cast<std::vector<double>>(ev1_copy), Catch::Approx(ev1_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev2_copy), Catch::Approx(ev2_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev3_copy), Catch::Approx(ev3_exp));

		CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(ev1_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev2), Catch::Approx(ev2_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev3), Catch::Approx(ev3_exp));
	}

	SECTION("Check if modifying one will not affect the other.") {
		ev2_copy += ev2_copy;
		ev3_copy += comp6771::euclidean_vector{2.0, 3.5};

		CHECK_THAT(static_cast<std::vector<double>>(ev2), Catch::Approx(ev2_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev3), Catch::Approx(ev3_exp));
	}
}

/* Check_nothrow */

/*
   Rational: All move constructor does is to take the data member of the moved from object and leave
   it in an default state (0 dimension in this case). So we just need to see if the moved to objects
   contain the expected value and if the moved from object is left in a default state.
*/
TEST_CASE("Move constructor") {
	auto ev1 = comp6771::euclidean_vector(0);
	auto ev2 = comp6771::euclidean_vector{0.0, 1.5, -4.5, 40.0};
	auto ev3 = comp6771::euclidean_vector{1.5, 3.0};

	// Invoke move constructor
	auto const ev1_new = std::move(ev1);
	auto const ev2_new = std::move(ev2);
	auto const ev3_new = std::move(ev3);

	// Construct expected vectors
	auto const ev1_exp = std::vector<double>();
	auto const ev2_exp = std::vector<double>{0.0, 1.5, -4.5, 40.0};
	auto const ev3_exp = std::vector<double>{1.5, 3.0};

	// Check that
	SECTION("The moved to object should have the same magnitudes as the moved from object's "
	        "original magnitudes") {
		CHECK_THAT(static_cast<std::vector<double>>(ev1_new), Catch::Approx(ev1_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev2_new), Catch::Approx(ev2_exp));
		CHECK_THAT(static_cast<std::vector<double>>(ev3_new), Catch::Approx(ev3_exp));
	}

	// Check that
	SECTION("The moved-from object should be in a state with 0 dimension") {
		CHECK(ev1.dimensions() == 0);
		CHECK(ev2.dimensions() == 0);
		CHECK(ev3.dimensions() == 0);
	}
}