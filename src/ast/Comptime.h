/**
 * @file CompTime.h
 * @author Conlan Wesson
 * @copyright (c) 2026, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include <cstdint>
#include <variant>

namespace AST {

/**
 * Type for compile-time evaluation.
 * @ingroup ast
 */
template<typename... Types>
class CompTime {
	private:
		/**
		 * Helper for is_oneof.
		 * @warning Do not use this directly, use is_oneof.
		 */
		template<typename A, typename T, typename... S>
		struct _is_oneof {
			enum { value = std::is_same<T, A>::value || _is_oneof<A, S...>::value };
		};

		template<typename A>
		struct _is_oneof<A, void> {
			enum { value = false };
		};

		/**
		 * Template to determine if A is of the type in T.
		 * @tparam A Type to test for.
		 * @tparam T List of types to check against.
		 * @note T must not contain void.
		 */
		template<typename A, typename... T>
		struct is_oneof {
			enum {
				/**
				 * true if A is in T, false otherwise.
				 */
				value = _is_oneof<A, T..., void>::value
			};
		};

	public:
		/**
		 * Constructor.
		 * @tparam T Initial type to store.
		 * @param v Initial value to store.
		 */
		template<typename T,
				std::enable_if_t<is_oneof<T, Types...>::value, bool> = true>
		explicit CompTime(T v) :
			value(v) {}
		
		/**
		 * Constructor.  Initialize to an empty state.
		 */
		CompTime() :
			value() {}

		/**
		 * Check if the given type is being stored.
		 * @tparam T Type to check for.
		 * @return true if the given type is currently being stored, false otherwise.
		 */
		template<typename T,
				std::enable_if_t<is_oneof<T, Types...>::value, bool> = true>
		bool has() const {
			return std::holds_alternative<T>(value);
		}
		
		/**
		 * Get the value being stored.
		 * @tparam T Type to check for.
		 * @return The stored value.
		 */
		template<typename T,
				std::enable_if_t<is_oneof<T, Types...>::value, bool> = true>
		T get() const {
			if(std::holds_alternative<T>(value)){
				return std::get<T>(value);
			}else{
				return 0;
			}
		}
		
		/**
		 * Set the value being stored.
		 * @tparam T Type to store.
		 * @param v Value to store
		 */
		template<typename T,
				std::enable_if_t<is_oneof<T, Types...>::value, bool> = true>
		void set(T v) {
			value = v;
		}

	private:
		std::variant<std::monostate, Types...> value; ///< Storage for the current value.
};

}
