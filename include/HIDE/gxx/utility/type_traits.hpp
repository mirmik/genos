
	
	template<class T, T v>
	struct integral_constant {
		static constexpr T value = v;
		typedef T value_type;
		typedef integral_constant type;
		constexpr operator value_type() { return value; }
		constexpr value_type operator()() const { return value; } 
	};
	
	template <bool B>
	using bool_constant = integral_constant<bool, B>;
	
	using true_type = bool_constant<true>;

	using false_type = bool_constant<false>;
	
	
	// Types small_ and big_ are guaranteed such that sizeof(small_) <	sizeof(big_)
	struct small_	{char dummy[1];};
	struct big_ 	{char dummy[2];};
	typedef small_ yes_tag;
	typedef big_ no_tag;
