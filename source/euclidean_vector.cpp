// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "comp6771/euclidean_vector.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <system_error>
#include <utility>

#include <experimental/iterator>

namespace comp6771 {
	// Constructors
	euclidean_vector::euclidean_vector()
	: euclidean_vector(1, 0) {}

	euclidean_vector::euclidean_vector(int dimensions)
	: euclidean_vector(dimensions, 0) {}

	euclidean_vector::euclidean_vector(int dimensions, double magnitude)
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	: magnitude_{std::make_unique<double[]>(static_cast<std::size_t>(dimensions))}
	, dimensions_{static_cast<std::size_t>(dimensions)}
	, cached_norm_{-1} {
		std::fill(magnitude_.get(), magnitude_.get() + dimensions_, magnitude);
	}

	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator begin,
	                                   std::vector<double>::const_iterator end)
	: euclidean_vector(static_cast<int>(std::distance(begin, end))) {
		std::copy(begin, end, magnitude_.get());
	}

	// For an empty initializer list, the default constructor is called.
	euclidean_vector::euclidean_vector(std::initializer_list<double> list)
	: euclidean_vector(static_cast<int>(list.size())) {
		std::copy(list.begin(), list.end(), magnitude_.get());
	}

	// Copy Constructor
	euclidean_vector::euclidean_vector(euclidean_vector const& original)
	: euclidean_vector(original.dimensions()) {
		std::copy(original.magnitude_.get(),
		          original.magnitude_.get() + original.dimensions_,
		          magnitude_.get());

		cached_norm_ = original.cached_norm_;
	}

	// Move Constructor
	euclidean_vector::euclidean_vector(euclidean_vector&& other) noexcept
	: magnitude_{std::exchange(other.magnitude_, nullptr)}
	, dimensions_{std::exchange(other.dimensions_, 0)}
	, cached_norm_{std::exchange(other.cached_norm_, -1)} {}

	// Operator Overload
	auto euclidean_vector::operator=(euclidean_vector const& original) -> euclidean_vector& {
		auto other = euclidean_vector(original);
		swap(*this, other);
		return *this;
	}

	auto euclidean_vector::operator=(euclidean_vector&& other) noexcept -> euclidean_vector& {
		// Avoid self assignment
		if (this == std::addressof(other)) {
			return *this;
		}

		swap(*this, other);

		// Reset the moved from object
		other.magnitude_.reset();
		other.dimensions_ = 0;
		other.cached_norm_ = -1;

		return *this;
	}

	auto euclidean_vector::operator[](int const& index) -> double& {
		assert(index >= 0 && index < dimensions());
		invalidate_cached_norm();

		return magnitude_[static_cast<std::size_t>(index)];
	}

	auto euclidean_vector::operator[](int const& index) const -> const double& {
		assert(index >= 0 && index < dimensions());

		return magnitude_[static_cast<std::size_t>(index)];
	}

	auto euclidean_vector::operator+() const -> euclidean_vector {
		return *this;
	}

	auto euclidean_vector::operator-() const -> euclidean_vector {
		auto copy = euclidean_vector(*this);

		std::transform(copy.magnitude_.get(),
		               copy.magnitude_.get() + copy.dimensions_,
		               copy.magnitude_.get(),
		               std::negate<>());

		return copy;
	}

	auto euclidean_vector::operator+=(euclidean_vector const& other) -> euclidean_vector& {
		merge(*this, other, std::plus<>());
		invalidate_cached_norm();
		return *this;
	}

	auto euclidean_vector::operator-=(euclidean_vector const& other) -> euclidean_vector& {
		merge(*this, other, std::minus<>());
		invalidate_cached_norm();
		return *this;
	}

	auto euclidean_vector::operator*=(double factor) -> euclidean_vector& {
		scale(*this, factor, std::multiplies<>());
		invalidate_cached_norm();
		return *this;
	}

	auto euclidean_vector::operator/=(double factor) -> euclidean_vector& {
		if (factor == 0) {
			throw euclidean_vector_error("Invalid vector division by 0");
		}

		scale(*this, factor, std::divides<>());
		invalidate_cached_norm();
		return *this;
	}

	euclidean_vector::operator std::vector<double>() const {
		return std::vector<double>(magnitude_.get(), magnitude_.get() + dimensions_);
	}

	euclidean_vector::operator std::list<double>() const {
		return std::list<double>(magnitude_.get(), magnitude_.get() + dimensions_);
	}

	// Member functions
	[[nodiscard]] auto euclidean_vector::at(int index) const -> double {
		euclidean_vector::index_check(*this, index);

		return magnitude_[static_cast<std::size_t>(index)];
	}

	auto euclidean_vector::at(int index) -> double& {
		euclidean_vector::index_check(*this, index);
		invalidate_cached_norm();

		return magnitude_[static_cast<std::size_t>(index)];
	}

	[[nodiscard]] auto euclidean_vector::dimensions() const -> int {
		return static_cast<int>(dimensions_);
	}

	// Friends
	auto operator==(euclidean_vector const& first, euclidean_vector const& second) -> bool {
		// Identity check
		if (std::addressof(first) == std::addressof(second)) {
			return true;
		}

		if (first.dimensions_ != second.dimensions_) {
			return false;
		}

		return std::equal(first.magnitude_.get(),
		                  first.magnitude_.get() + first.dimensions_,
		                  second.magnitude_.get(),
		                  second.magnitude_.get() + second.dimensions_,
		                  [](double const& f, double const& s) {
			                  return std::fabs(f - s) < std::numeric_limits<double>::epsilon();
		                  });
	}

	auto operator!=(euclidean_vector const& first, euclidean_vector const& second) -> bool {
		return not(first == second);
	}

	auto operator+(euclidean_vector const& first, euclidean_vector const& second) -> euclidean_vector {
		auto first_copy = euclidean_vector(first);
		first_copy += second;
		return first_copy;
	}

	auto operator-(euclidean_vector const& first, euclidean_vector const& second) -> euclidean_vector {
		auto first_copy = euclidean_vector(first);
		first_copy -= second;
		return first_copy;
	}

	auto operator*(euclidean_vector const& ev, double factor) -> euclidean_vector {
		auto ev_copy = euclidean_vector(ev);
		ev_copy *= factor;
		return ev_copy;
	}

	auto operator/(euclidean_vector const& ev, double factor) -> euclidean_vector {
		auto ev_copy = euclidean_vector(ev);
		ev_copy /= factor;
		return ev_copy;
	}

	auto operator<<(std::ostream& os, euclidean_vector const& ev) -> std::ostream& {
		auto oss = std::ostringstream{};
		oss << '[';

		std::copy(ev.magnitude_.get(),
		          ev.magnitude_.get() + ev.dimensions_,
		          std::experimental::make_ostream_joiner(oss, " "));

		oss << "]";

		os << oss.str();

		return os;
	}

	// Helper functions
	auto euclidean_vector::index_check(euclidean_vector const& ev, int index) -> void {
		if (index < 0 or index >= ev.dimensions()) {
			throw euclidean_vector_error("Index " + std::to_string(index)
			                             + " is not valid for this euclidean_vector object");
		}
	}

	auto euclidean_vector::dimensions_check(euclidean_vector const& first,
	                                        euclidean_vector const& second) -> void {
		if (first.dimensions() != second.dimensions()) {
			throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(first.dimensions())
			                             + ") and RHS(" + std::to_string(second.dimensions())
			                             + ") do not match");
		}
	}

	// Merge <other> into <subject> using <func>
	auto euclidean_vector::merge(euclidean_vector const& subject,
	                             euclidean_vector const& other,
	                             std::function<double(double, double)> const& func) -> void {
		// Error Checking
		euclidean_vector::dimensions_check(subject, other);

		// Perform mutation
		std::transform(subject.magnitude_.get(),
		               subject.magnitude_.get() + subject.dimensions_,
		               other.magnitude_.get(),
		               subject.magnitude_.get(),
		               func);
	}

	// Scale <this> by <factor> using <func>
	auto euclidean_vector::scale(euclidean_vector const& ev,
	                             double const& factor,
	                             std::function<double(double, double)> const& func) -> void {
		// Perform mutation
		std::transform(ev.magnitude_.get(),
		               ev.magnitude_.get() + ev.dimensions_,
		               ev.magnitude_.get(),
		               [&](double const& i) { return func(i, factor); });
	}

	// Utility Functions
	auto euclidean_norm(euclidean_vector const& v) -> double {
		if (v.cached_norm_ != -1) {
			return v.cached_norm_;
		}

		auto dot_product = std::inner_product(v.magnitude_.get(),
		                                      v.magnitude_.get() + v.dimensions_,
		                                      v.magnitude_.get(),
		                                      0.0);

		auto norm = std::sqrt(dot_product);
		v.cached_norm_ = norm;

		return norm;
	}

	auto unit(euclidean_vector const& v) -> euclidean_vector {
		if (v.dimensions() == 0) {
			throw euclidean_vector_error("euclidean_vector with no dimensions does not have a unit "
			                             "vector");
		}

		auto norm = euclidean_norm(v);
		if (norm == 0) {
			throw euclidean_vector_error("euclidean_vector with zero euclidean normal does not have a "
			                             "unit vector");
		}

		auto v_copy = euclidean_vector(v);
		v_copy /= norm;

		return v_copy;
	}

	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double {
		if (x.dimensions() != y.dimensions()) {
			throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(x.dimensions())
			                             + ") and RHS(" + std::to_string(y.dimensions())
			                             + ") do not match");
		}

		// Dot product of two 0-dimension vectors yield 0
		if (x.dimensions() == 0) {
			return 0;
		}

		auto dot_product = std::inner_product(&(x[0]), &(x[0]) + x.dimensions(), &(y[0]), 0.0);

		return dot_product;
	}

} // namespace comp6771
