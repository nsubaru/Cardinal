export module Cardinal.Core:Comparing;

import :Types;
import :Math;

export namespace Cardinal::Core::Comparing
{
	/// <summary>
	/// Comparing result
	/// </summary>
	using ComparingResult = Int8;

	/// <summary>
	/// Null type
	/// </summary>
	using LiteralZero = NullptrT;

	/// <summary>
	/// Equality categories
	/// </summary>
	enum class ECompareEqual : ComparingResult {
		Equal = 0,
		Equivalent = Equal,
		NonEqual = 1,
		NonEquivalent = NonEqual
	};

	/// <summary>
	/// Ordering categories
	/// </summary>
	enum class ECompareOrdinal : ComparingResult {
		Less = -1,
		Greater = 1
	};

	/// <summary>
	/// Disordering categories
	/// </summary>
	enum class ECompareUnOrderable : ComparingResult {
		UnOrdered = Math::Min<Int8>() + 1
	};
}

export namespace std //-V1061
{
	using namespace Cardinal::Core::Comparing;
	using Cardinal::Core::Boolean;

	class strong_ordering;
	class partial_ordering;
	class weak_ordering;
	class weak_equality;
	class strong_equality;

#pragma region Weak equality comparing
	/// <summary>
	/// Weak equality
	/// </summary>
	class weak_equality {
		/// <summary>
		/// Comparing result
		/// </summary>
		ComparingResult value;

	public:
		/// <summary>
		/// Equality categories constructor
		/// </summary>
		/// <param name="equal">Equality category</param>
		constexpr explicit weak_equality(const ECompareEqual equal) : value(static_cast<ComparingResult>(equal)) {}

	public:
		/// <summary>
		/// Equality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Equal" or "Equality"</returns>
		friend constexpr Boolean operator==(const weak_equality left, LiteralZero) { //-V801
			return left.value == 0;
		}

		/// <summary>
		/// Inequality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is not "Equal" or "Equality"</returns>
		friend constexpr Boolean operator!=(const weak_equality left, LiteralZero) { //-V801
			return left.value != 0;
		}

		/// <summary>
		/// Equality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Equal" or "Equality"</returns>
		friend constexpr Boolean operator==(LiteralZero, const weak_equality left) { //-V801
			return 0 == left.value;
		}

		/// <summary>
		/// Inequality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is not "Equal" or "Equality"</returns>
		friend constexpr Boolean operator!=(LiteralZero, const weak_equality left) { //-V801
			return 0 != left.value;
		}

	public:
		/// <summary>
		/// Universal result comparing with 0
		/// </summary>
		/// <param name="left">Comparing result</param>
		/// <returns>Comparing result</returns>
		friend constexpr weak_equality operator<=>(const weak_equality left, LiteralZero) { //-V801
			return left;
		}

		/// <summary>
		/// Universal result comparing with 0
		/// </summary>
		/// <param name="left">Comparing result</param>
		/// <returns>Comparing result</returns>
		friend constexpr weak_equality operator<=>(LiteralZero, const weak_equality left) { //-V801
			return left;
		}

	public:
		static const weak_equality equivalent;
		static const weak_equality nonequivalent;
	};

	inline constexpr weak_equality weak_equality::equivalent(ECompareEqual::Equivalent);
	inline constexpr weak_equality weak_equality::nonequivalent(ECompareEqual::NonEquivalent);
#pragma endregion

#pragma region Strong equality comparing
	/// <summary>
	/// Strong equality
	/// </summary>
	class strong_equality {
		/// <summary>
		/// Comparing result
		/// </summary>
		ComparingResult value;

	public:
		/// <summary>
		/// Equality categories constructor
		/// </summary>
		/// <param name="equal">Equality category</param>
		constexpr explicit strong_equality(const ECompareEqual equal) : value(static_cast<ComparingResult>(equal)) {}

	public:
		/// <summary>
		/// Equality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Equal" or "Equality"</returns>
		friend constexpr Boolean operator==(const strong_equality left, LiteralZero) { //-V801
			return left.value == 0;
		}

		/// <summary>
		/// Inequality with "ECompareEqual::Equal"
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is not "Equal" or "Equality"</returns>
		friend constexpr Boolean operator!=(const strong_equality left, LiteralZero) { //-V801
			return left.value != 0;
		}

		/// <summary>
		/// Equality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Equal" or "Equality"</returns>
		friend constexpr Boolean operator==(LiteralZero, const strong_equality left) { //-V801
			return 0 == left.value;
		}

		/// <summary>
		/// Inequality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is not "Equal" or "Equality"</returns>
		friend constexpr Boolean operator!=(LiteralZero, const strong_equality left) { //-V801
			return 0 != left.value;
		}

	public:
		/// <summary>
		/// Universal result comparing with 0
		/// </summary>
		/// <param name="left">Comparing result</param>
		/// <returns>Comparing result</returns>
		friend constexpr strong_equality operator<=>(const strong_equality left, LiteralZero) { //-V801
			return left;
		}

		/// <summary>
		/// Universal result comparing with 0
		/// </summary>
		/// <param name="left">Comparing result</param>
		/// <returns>Comparing result</returns>
		friend constexpr strong_equality operator<=>(LiteralZero, const strong_equality left) { //-V801
			return left;
		}

	public:
		static const strong_equality equal;
		static const strong_equality nonequal;
		static const strong_equality equivalent;
		static const strong_equality nonequivalent;
	};

	inline constexpr strong_equality strong_equality::equal(ECompareEqual::Equal);
	inline constexpr strong_equality strong_equality::nonequal(ECompareEqual::NonEqual);
	inline constexpr strong_equality strong_equality::equivalent(ECompareEqual::Equivalent);
	inline constexpr strong_equality strong_equality::nonequivalent(ECompareEqual::NonEquivalent);

#pragma endregion

#pragma region Patial orderable comparing result
	/// <summary>
	/// Partial equality class result
	/// </summary>
	class partial_ordering {
	private:
		/// <summary>
		/// Equality result
		/// </summary>
		ComparingResult value;

		/// <summary>
		/// Ordering flag
		/// </summary>
		Boolean isOrdered;

	private:
		/// <summary>
		/// Field values internal constructor
		/// </summary>
		/// <param name="value">Equality result</param>
		/// <param name="isOrdered">Ordering flag</param>
		__forceinline constexpr partial_ordering(ComparingResult value, Boolean isOrdered) : value(value), isOrdered(isOrdered) {}

	public:
		/// <summary>
		/// Equality categories constructor
		/// </summary>
		/// <param name="equal">Equality categories</param>
		__forceinline constexpr partial_ordering(ECompareEqual equal) : value(static_cast<ComparingResult>(equal)), isOrdered(true) {}

		/// <summary>
		/// Ordering categories constructor
		/// </summary>
		/// <param name="equal">Ordering categories</param>
		__forceinline constexpr partial_ordering(ECompareOrdinal equal) : value(static_cast<ComparingResult>(equal)), isOrdered(true) {}

		/// <summary>
		/// Disordering categories constructor
		/// </summary>
		/// <param name="equal">Disordering categories</param>
		__forceinline constexpr partial_ordering(ECompareUnOrderable equal) : value(static_cast<ComparingResult>(equal)), isOrdered(false) {}

	public:
		/// <summary>
		/// Transforming into "weak_equality"
		/// </summary>
		/// <returns>"weak_equality" (result)</returns>
		__forceinline constexpr operator weak_equality() const {
			return weak_equality{ static_cast<ECompareEqual>(value != 0) };
		}

	public:

#pragma region Left side comparing
		/// <summary>
		/// Equality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Equal" or "Equality"</returns>
		__forceinline constexpr friend Boolean operator==(const partial_ordering left, LiteralZero) { //-V801
			return left.isOrdered && left.value == 0;
		}

		/// <summary>
		/// Inequality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is not "Equal" or "Equality"</returns>
		__forceinline constexpr friend Boolean operator!=(const partial_ordering left, LiteralZero) { //-V801
			return !left.isOrdered || left.value != 0;
		}

		/// <summary>
		/// Comparing result with "ECompareOrdinal::Greater"
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Greater"</returns>
		__forceinline constexpr friend Boolean operator<(const partial_ordering left, LiteralZero) { //-V801
			return left.isOrdered && left.value < 0;
		}

		/// <summary>
		/// Comparing result with "ECompareOrdinal::Less"
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Greater"</returns>
		__forceinline constexpr friend Boolean operator>(const partial_ordering left, LiteralZero) { //-V801
			return left.isOrdered && left.value > 0;
		}

		/// <summary>
		/// Comparing result with "ECompareOrdinal::Greater" or "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Greater" or "Equal"</returns>
		__forceinline constexpr friend Boolean operator<=(const partial_ordering left, LiteralZero) { //-V801
			return left.isOrdered && left.value <= 0;
		}

		/// <summary>
		/// Comparing result with "ECompareOrdinal::Less" OR "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Greater" or "Equal"</returns>
		__forceinline constexpr friend Boolean operator>=(const partial_ordering left, LiteralZero) { //-V801
			return left.isOrdered && left.value >= 0;
		}
#pragma endregion

#pragma region Right side comparing
		/// <summary>
		/// Equality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Equal" or "Equality"</returns>
		__forceinline constexpr friend Boolean operator==(LiteralZero, const partial_ordering right) { //-V801
			return right.isOrdered && 0 == right.value;
		}

		/// <summary>
		/// Inequality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is not "Equal" or "Equality"</returns>
		__forceinline constexpr friend Boolean operator!=(LiteralZero, const partial_ordering right) { //-V801
			return !right.isOrdered || 0 != right.value;
		}

		/// <summary>
		/// Comparing result with "ECompareOrdinal::Greater"
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Greater"</returns>
		__forceinline constexpr friend Boolean operator<(LiteralZero, const partial_ordering right) { //-V801
			return right.isOrdered && 0 < right.value;
		}

		/// <summary>
		/// Comparing result with "ECompareOrdinal::Less"
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Greater"</returns>
		__forceinline constexpr friend Boolean operator>(LiteralZero, const partial_ordering right) { //-V801
			return right.isOrdered && 0 > right.value;
		}

		/// <summary>
		/// Comparing result with "ECompareOrdinal::Greater" or "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Greater" or "Equal"</returns>
		__forceinline constexpr friend Boolean operator<=(LiteralZero, const partial_ordering right) { //-V801
			return right.isOrdered && 0 <= right.value;
		}

		/// <summary>
		/// Comparing result with "ECompareOrdinal::Less" or "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Greater" or "Equal"</returns>
		__forceinline constexpr friend Boolean operator>=(LiteralZero, const partial_ordering right) { //-V801
			return right.isOrdered && 0 >= right.value;
		}
#pragma endregion

#pragma region Three-way comparing
		/// <summary>
		/// Universal result comparing with 0
		/// </summary>
		/// <param name="left">Comparing result</param>
		/// <returns>Comparing result</returns>
		__forceinline constexpr friend partial_ordering operator<=>(const partial_ordering left, LiteralZero) { //-V801
			return left;
		}

		/// <summary>
		/// Universal result comparing with 0
		/// </summary>
		/// <param name="left">Comparing result</param>
		/// <returns>Comparing result</returns>
		__forceinline constexpr friend partial_ordering operator<=>(LiteralZero, const partial_ordering left) { //-V801
			return partial_ordering{ static_cast<ComparingResult>(-left.value),  left.isOrdered };
		}
#pragma endregion

#pragma region Available values
		/// <summary>
		/// Value, is less in equality result
		/// </summary>
		static const partial_ordering less;

		/// <summary>
		/// Value, is equal in equality result
		/// </summary>
		static const partial_ordering equivalent;

		/// <summary>
		/// Value, is greater in equality result
		/// </summary>
		static const partial_ordering greater;

		/// <summary>
		/// Value, is unordered in equality result
		/// </summary>
		static const partial_ordering unordered;
#pragma endregion
	};

	inline constexpr partial_ordering partial_ordering::less(ECompareOrdinal::Less);
	inline constexpr partial_ordering partial_ordering::equivalent(ECompareEqual::Equivalent);
	inline constexpr partial_ordering partial_ordering::greater(ECompareOrdinal::Greater);
	inline constexpr partial_ordering partial_ordering::unordered(ECompareUnOrderable::UnOrdered);
#pragma endregion

#pragma region Weak orderable comparing result
	/// <summary>
	/// Weak ordering result
	/// </summary>
	class weak_ordering {
	private:
		/// <summary>
		/// Weak ordering result value
		/// </summary>
		ComparingResult value;

	private:
		/// <summary>
		/// Comparing value internal constructor
		/// </summary>
		/// <param name="value">Comparing result</param>
		__forceinline constexpr weak_ordering(ComparingResult value) : value(value) {}

	public:
		/// <summary>
		/// Equality category constructor
		/// </summary>
		/// <param name="equal">Equality category</param>
		__forceinline constexpr weak_ordering(ECompareEqual equal) : value(static_cast<ComparingResult>(equal)) {}

		/// <summary>
		/// Ordering category constructor
		/// </summary>
		/// <param name="equal">Ordering category</param>
		__forceinline constexpr weak_ordering(ECompareOrdinal equal) : value(static_cast<ComparingResult>(equal)) {}

	public:
		/// <summary>
		/// Transforming to partial ordering result
		/// </summary>
		__forceinline constexpr operator partial_ordering() const {
			return partial_ordering{ static_cast<ECompareOrdinal>(this->value) };
		}
	public:

#pragma region Left side comparing
		/// <summary>
		/// Equality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Equal" or "Equality"</returns>
		__forceinline constexpr friend Boolean operator==(const weak_ordering left, LiteralZero) { //-V801
			return left.value == 0;
		}

		/// <summary>
		/// Inequality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is not "Equal" or "Equality"</returns>
		__forceinline constexpr friend Boolean operator!=(const weak_ordering left, LiteralZero) { //-V801
			return left.value != 0;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Greater"
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Greater"</returns>
		__forceinline constexpr friend Boolean operator<(const weak_ordering left, LiteralZero) { //-V801
			return left.value < 0;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Less"
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Greater"</returns>
		__forceinline constexpr friend Boolean operator>(const weak_ordering left, LiteralZero) { //-V801
			return left.value > 0;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Greater" or "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Greater" or "Equal"</returns>
		__forceinline constexpr friend Boolean operator<=(const weak_ordering left, LiteralZero) { //-V801
			return left.value <= 0;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Less" or "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Greater" or "Equal"</returns>
		__forceinline constexpr friend Boolean operator>=(const weak_ordering left, LiteralZero) { //-V801
			return left.value >= 0;
		}
#pragma endregion

#pragma region Right side comparing
		/// <summary>
		/// Equality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Equal" or "Equality"</returns>
		__forceinline constexpr friend Boolean operator==(LiteralZero, const weak_ordering right) { //-V801
			return 0 == right.value;
		}

		/// <summary>
		/// Inequality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is not "Equal" or "Equality"</returns>
		__forceinline constexpr friend Boolean operator!=(LiteralZero, const weak_ordering right) { //-V801
			return 0 != right.value;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Greater"
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Greater"</returns>
		__forceinline constexpr friend Boolean operator<(LiteralZero, const weak_ordering right) { //-V801
			return 0 < right.value;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Less"
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Greater"</returns>
		__forceinline constexpr friend Boolean operator>(LiteralZero, const weak_ordering right) { //-V801
			return 0 > right.value;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Greater" or "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Greater" or "Equal"</returns>
		__forceinline constexpr friend Boolean operator<=(LiteralZero, const weak_ordering right) { //-V801
			return 0 <= right.value;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Less" or "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Greater" or "Equal"</returns>
		__forceinline constexpr friend Boolean operator>=(LiteralZero, const weak_ordering right) { //-V801
			return 0 >= right.value;
		}
#pragma endregion

#pragma region Three-way comparing
		/// <summary>
		/// Universal result comparing with 0
		/// </summary>
		/// <param name="left">Comparing result</param>
		/// <returns>Comparing result</returns>
		__forceinline constexpr friend weak_ordering operator<=>(const weak_ordering left, LiteralZero) { //-V801
			return left;
		}

		/// <summary>
		/// Universal result comparing with 0
		/// </summary>
		/// <param name="left">Comparing result</param>
		/// <returns>Comparing result</returns>
		__forceinline constexpr friend weak_ordering operator<=>(LiteralZero, const weak_ordering left) { //-V801
			return weak_ordering{ static_cast<ComparingResult>(-left.value) };
		}
#pragma endregion

#pragma region Available values
		/// <summary>
		/// Value, is less in comparing result
		/// </summary>
		static const weak_ordering less;

		/// <summary>
		/// Value, is equal in comparing result
		/// </summary>
		static const weak_ordering equivalent;

		/// <summary>
		/// Value, is freater in comparing result
		/// </summary>
		static const weak_ordering greater;
#pragma	endregion
	};

	inline constexpr weak_ordering weak_ordering::less(ECompareOrdinal::Less);
	inline constexpr weak_ordering weak_ordering::equivalent(ECompareEqual::Equivalent);
	inline constexpr weak_ordering weak_ordering::greater(ECompareOrdinal::Greater);
#pragma endregion

#pragma region Strong orderable comparing
	/// <summary>
	/// Strong ordering result
	/// </summary>
	class strong_ordering {
	private:
		/// <summary>
		/// Strong ordering result value
		/// </summary>
		ComparingResult value;

	private:
		/// <summary>
		/// Comparing result internal constructor
		/// </summary>
		/// <param name="value">Comparing result</param>
		__forceinline constexpr strong_ordering(ComparingResult value) : value(value) {}

	public:
		/// <summary>
		/// Equality category constructor
		/// </summary>
		/// <param name="equal">Equality category</param>
		__forceinline constexpr strong_ordering(ECompareEqual equal) : value(static_cast<ComparingResult>(equal)) {}

		/// <summary>
		/// Ordering category constructor
		/// </summary>
		/// <param name="equal">Ordering category</param>
		__forceinline constexpr strong_ordering(ECompareOrdinal equal) : value(static_cast<ComparingResult>(equal)) {}

	public:
		/// <summary>
		/// Transform to partial ordering result
		/// </summary>
		/// <returns>Partial ordering</returns>
		__forceinline constexpr operator partial_ordering() const {
			return partial_ordering{ static_cast<ECompareOrdinal>(this->value) };
		}

		/// <summary>
		/// Transform to weak ordering result
		/// </summary>
		/// <returns>Weak ordering</returns>
		__forceinline constexpr operator weak_ordering() const {
			return weak_ordering{ static_cast<ECompareOrdinal>(this->value) };
		}

		/// <summary>
		/// Transform to weak equality result
		/// </summary>
		/// <returns>Weak equality</returns>
		__forceinline constexpr operator weak_equality() const {
			return weak_equality{ static_cast<ECompareEqual>(this->value) };
		}

		/// <summary>
		/// Transform to strong equality result
		/// </summary>
		/// <returns>Strong equality</returns>
		__forceinline constexpr operator strong_equality() const {
			return strong_equality{ static_cast<ECompareEqual>(this->value) };
		}
	public:

#pragma region Left side comparing
		/// <summary>
		/// Equality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Equal" or "Equality"</returns>
		__forceinline constexpr friend Boolean operator==(const strong_ordering left, LiteralZero) { //-V801
			return left.value == 0;
		}

		/// <summary>
		/// Inequality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is not "Equal" or "Equality"</returns>
		__forceinline constexpr friend Boolean operator!=(const strong_ordering left, LiteralZero) { //-V801
			return left.value != 0;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Greater"
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Greater"</returns>
		__forceinline constexpr friend Boolean operator<(const strong_ordering left, LiteralZero) { //-V801
			return left.value < 0;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Less"
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Greater"</returns>
		__forceinline constexpr friend Boolean operator>(const strong_ordering left, LiteralZero) { //-V801
			return left.value > 0;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Greater" or "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Greater" or "Equal"</returns>
		__forceinline constexpr friend Boolean operator<=(const strong_ordering left, LiteralZero) { //-V801
			return left.value <= 0;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Less" or "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="left">Ordering value</param>
		/// <returns>True if equality result is "Greater" or "Equal"</returns>
		__forceinline constexpr friend Boolean operator>=(const strong_ordering left, LiteralZero) { //-V801
			return left.value >= 0;
		}
#pragma endregion

#pragma region Right side comparing
		/// <summary>
		/// Equality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Equal" or "Equality"</returns>
		__forceinline constexpr friend Boolean operator==(LiteralZero, const strong_ordering right) { //-V801
			return 0 == right.value;
		}

		/// <summary>
		/// Inequality with "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is not "Equal" or "Equality"</returns>
		__forceinline constexpr friend Boolean operator!=(LiteralZero, const strong_ordering right) { //-V801
			return 0 != right.value;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Greater"
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Greater"</returns>
		__forceinline constexpr friend Boolean operator<(LiteralZero, const strong_ordering right) { //-V801
			return 0 < right.value;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Less"
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Greater"</returns>
		__forceinline constexpr friend Boolean operator>(LiteralZero, const strong_ordering right) { //-V801
			return 0 > right.value;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Greater" or "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Greater" or "Equal"</returns>
		__forceinline constexpr friend Boolean operator<=(LiteralZero, const strong_ordering right) { //-V801
			return 0 <= right.value;
		}

		/// <summary>
		/// Coparing result with "ECompareOrdinal::Less" or "ECompareEqual::Equal" 
		/// </summary>
		/// <param name="right">Ordering value</param>
		/// <returns>True if equality result is "Greater" or "Equal"</returns>
		__forceinline constexpr friend Boolean operator>=(LiteralZero, const strong_ordering right) { //-V801
			return 0 >= right.value;
		}
#pragma endregion

#pragma region Three-way comparing
		/// <summary>
		/// Universal result comparing with 0
		/// </summary>
		/// <param name="left">Comparing result</param>
		/// <returns>Comparing result</returns>
		__forceinline constexpr friend strong_ordering operator<=>(const strong_ordering left, LiteralZero) { //-V801
			return left;
		}

		/// <summary>
		/// Universal result comparing with 0
		/// </summary>
		/// <param name="left">Comparing result</param>
		/// <returns>Comparing result</returns>
		__forceinline constexpr friend strong_ordering operator<=>(LiteralZero, const strong_ordering left) { //-V801
			return strong_ordering{ static_cast<ComparingResult>(-left.value) };
		}
#pragma endregion

#pragma region Available values
		/// <summary>
		/// Value, is less in comparing result
		/// </summary>
		static const strong_ordering less;

		/// <summary>
		/// Value, is equivalent in comparing result
		/// </summary>
		static const strong_ordering equivalent;

		/// <summary>
		/// Value, is equal in comparing result
		/// </summary>
		static const strong_ordering equal;

		/// <summary>
		/// Value, is greater in comparing result
		/// </summary>
		static const strong_ordering greater;
#pragma	endregion
	};

	inline constexpr strong_ordering strong_ordering::less(ECompareOrdinal::Less);
	inline constexpr strong_ordering strong_ordering::equivalent(ECompareEqual::Equivalent);
	inline constexpr strong_ordering strong_ordering::equal(ECompareEqual::Equal);
	inline constexpr strong_ordering strong_ordering::greater(ECompareOrdinal::Greater);
#pragma endregion
}

export namespace Cardinal::Core::Comparing 
{
	using WeakEquality = std::weak_equality;
	using StrongOrdering = std::strong_ordering;
	using WeakOrdering = std::weak_ordering;
	using PartialOrdering = std::partial_ordering;
	using StrongEquality = std::strong_equality;
}

export namespace Cardinal::Core
{
	using WeakEquality = std::weak_equality;
	using StrongOrdering = std::strong_ordering;
	using WeakOrdering = std::weak_ordering;
	using PartialOrdering = std::partial_ordering;
	using StrongEquality = std::strong_equality;
}