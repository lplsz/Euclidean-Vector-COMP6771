#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include <__functional_base>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>

#include <list>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what)
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		// Constructors
		euclidean_vector();

		explicit euclidean_vector(int);

		euclidean_vector(int, double);

		euclidean_vector(std::vector<double>::const_iterator, std::vector<double>::const_iterator);

		// enclidean_vector{} will invoke the default constructor
		euclidean_vector(std::initializer_list<double>);

		// Copy Constructor
		euclidean_vector(euclidean_vector const&);

		// Move Constructor
		euclidean_vector(euclidean_vector&&) noexcept;

		// Destructor
		~euclidean_vector() = default;

		// Operator Overload
		auto operator=(euclidean_vector const&) -> euclidean_vector&;
		auto operator=(euclidean_vector&&) noexcept -> euclidean_vector&;

		auto operator[](int const&) -> double&;
		auto operator[](int const&) const -> const double&;

		auto operator+() const -> euclidean_vector;
		auto operator-() const -> euclidean_vector;

		auto operator+=(euclidean_vector const&) -> euclidean_vector&;
		auto operator-=(euclidean_vector const&) -> euclidean_vector&;
		auto operator*=(double) -> euclidean_vector&;
		auto operator/=(double) -> euclidean_vector&;

		explicit operator std::vector<double>() const;
		explicit operator std::list<double>() const;

		// Member functions
		[[nodiscard]] auto at(int) const -> double;
		auto at(int) -> double&;
		[[nodiscard]] auto dimensions() const -> int;

		// Friends
		friend auto operator==(euclidean_vector const&, euclidean_vector const&) -> bool;
		friend auto operator!=(euclidean_vector const&, euclidean_vector const&) -> bool;
		friend auto operator+(euclidean_vector const&, euclidean_vector const&) -> euclidean_vector;
		friend auto operator-(euclidean_vector const&, euclidean_vector const&) -> euclidean_vector;
		friend auto operator*(euclidean_vector const&, double) -> euclidean_vector;
		friend auto operator/(euclidean_vector const&, double) -> euclidean_vector;
		friend auto operator<<(std::ostream&, euclidean_vector const&) -> std::ostream&;

	private:
		// ass2 spec requires we use double[]

		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitude_;
		std::size_t dimensions_;

		/* Stores the cached norm. -1 if the cache is invalid. */
		mutable double cached_norm_;

		// Helper functions

		// Swap the contents of two euclidea_vector
		static auto swap(euclidean_vector& first, euclidean_vector& second) noexcept {
			std::swap(first.dimensions_, second.dimensions_);
			std::swap(first.magnitude_, second.magnitude_);
			std::swap(first.cached_norm_, second.cached_norm_);
		}

		auto invalidate_cached_norm() {
			cached_norm_ = -1;
		}
		// Check if index in range, throw exception if not
		static auto index_check(euclidean_vector const& ev, int index) -> void;

		// Check if dimension of the two vectors matches, throw exception if not
		static auto dimensions_check(euclidean_vector const& first, euclidean_vector const& second)
		   -> void;

		static auto merge(euclidean_vector const& subject,
		                  euclidean_vector const& other,
		                  std::function<double(double, double)> const& func) -> void;
		static auto scale(euclidean_vector const& ev,
		                  double const& factor,
		                  std::function<double(double, double)> const& func) -> void;

		// Hidden friends
		friend auto euclidean_norm(euclidean_vector const& v) -> double;
	};

	// Utility functions

	/* Calling euclidean_norm invalidates any mutable references to the contents of the vector. */
	auto euclidean_norm(euclidean_vector const& v) -> double;
	auto unit(euclidean_vector const& v) -> euclidean_vector;
	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;

} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
