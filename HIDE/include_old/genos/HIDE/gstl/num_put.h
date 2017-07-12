
#ifndef GENOS_NUM_PUT_H
	#define GENOS_NUM_PUT_H
	
	
	#include <math.h>
	#include <genos/gstl/io/ios.h>
	
	namespace genos{
		
		const char* __hex_char_table_lo();	
		const char*  __hex_char_table_hi();
		
		template <class _Integer>
		inline char* __write_base_positive_backward(char* __ptr, ios_base::fmtflags __flags, uint8_t __base, _Integer __x) {
			bool _uppercase = __flags & ios_base::uppercase;
			for (; __x != 0; __x /= __base)
			{
				char tmp = (char)((int)(__x % __base));
				*--__ptr = (tmp < 10) ? (tmp + '0') : (tmp - 10 + (_uppercase ? 'A' : 'a'));
			};
			return __ptr;
		}
		
		template <class _Integer>
		inline char* __write_base_backward(char* __ptr, ios_base::fmtflags __flags, uint8_t __base, _Integer __x) {
			constexpr bool _signed = (_Integer(-1)) < 0;
			bool neg = __x < 0;
			__ptr = __write_base_positive_backward(__ptr, __flags, __base, neg ? -__x : __x);
			if 	(_signed)
			{
				if (neg) *--__ptr = '-';
				else if (__flags & ios_base::showpos) *--__ptr = '+';
			};
			return __ptr;
		}
		
		template<int N> constexpr __umax_int_t MASK		(){return (MASK<N-1>() * 0x100 + 0xFF);};
		template<> 		constexpr __umax_int_t MASK<0>	(){return 0;};
		
		template <class _Integer>
		char* __write_integer_backward(char* __buf, ios_base::fmtflags __flags, uint8_t _base, _Integer __x) {
			char* __ptr = __buf;
			
			if (__x == 0) {
				*--__ptr = '0';
			}
			else {
				switch (__flags & ios_base::basefield) {
					case ios_base::oct:
					{
						const char* __table_ptr = (__flags & ios_base::uppercase) ? __hex_char_table_hi() : __hex_char_table_lo();
						
						constexpr __umax_int_t mask = MASK<sizeof(_Integer)>(); 
						__umax_int_t temp(__x & mask);	
						for(; temp != 0; temp>>=3)
						{
							*--__ptr = '0' + (temp & 0b111);
						};
						
						if (__flags & ios_base::showbase) {
							*--__ptr = '0';
						}
					}
					break;
					case ios_base::hex:
					{
						const char* __table_ptr = (__flags & ios_base::uppercase) ? __hex_char_table_hi() : __hex_char_table_lo();
						constexpr __umax_int_t mask = MASK<sizeof(_Integer)>(); 
						__umax_int_t temp(__x & mask);		 
						
						if (__flags & ios_base::alldigits)
						for(int j=0; j < sizeof(_Integer) * 2 ; temp>>=4, j++)
						{
							*--__ptr = *(__table_ptr + (temp & 0xF));
						}
						else 
						for(;temp != 0 ; temp>>=4)
						{
							*--__ptr = *(__table_ptr + (temp & 0xF));
						};
						
						if (__flags & ios_base::showbase) {
							*--__ptr = __table_ptr[16];
							*--__ptr = '0';
						}
					}
					break;
					case ios_base::bin:
					{
						constexpr __umax_int_t mask = MASK<sizeof(_Integer)>(); 
						__umax_int_t temp(__x & mask);	
						if (__flags & ios_base::alldigits)
						for(int j=0; j < sizeof(_Integer) * 2 ; temp>>=4, j++)
						{
							*--__ptr = (temp & 0b00001000) ? '1': '0';
							*--__ptr = (temp & 0b00000100) ? '1': '0';
							*--__ptr = (temp & 0b00000010) ? '1': '0';
							*--__ptr = (temp & 0b00000001) ? '1': '0';
						}
						else 
						for(;temp != 0 ; temp>>=4)
						{
							*--__ptr = (temp & 0b00001000) ? '1': '0';
							*--__ptr = (temp & 0b00000100) ? '1': '0';
							*--__ptr = (temp & 0b00000010) ? '1': '0';
							*--__ptr = (temp & 0b00000001) ? '1': '0';
						};
						
						if (__flags & ios_base::showbase) {
							*--__ptr = 'b';
							*--__ptr = '0';
						}
					}
					break;
					case ios_base::dec:
					{
						__ptr = __write_base_backward(__buf, __flags, 10, __x);		
					}
					break;
					default:
					{
						__ptr = __write_base_backward(__buf, __flags, _base, __x);
					}
					break;
					
				}
			}
			
			return __ptr;
		};
		
		
		
		
		
		
		
		template<typename T>
		char* __write_float_backward(char* buf, ios_base::fmtflags flags, uint8_t precision, uint8_t base, T x)
		{
			char* ptr = buf;
			if (x==0) {*(--ptr)='0'; return ptr;}; 
			
			uint64_t h2;
			float x_unn;
			uint64_t h1 = (uint64_t) x;
			float x_un = x - h1;
			if (x_un == 0) goto print_high;
			
			
			if (precision==0) precision=2;
			x_unn = (x_un * pow(base, precision) + 0.5);
			h2 = x_unn;
			
			
			
			ptr = __write_base_backward(ptr, flags, base, h2);
			*(--ptr) = '.';
			print_high:
			ptr = __write_base_backward(ptr, flags, base, h1);
			
			if (x < 0)
			*(--ptr) = '-';
			else if (flags & ios_base::showpos)
			*(--ptr) = '+';
			
			return ptr;
		};
		
		
		
		
		
		
		
		
		
		
		
		
		
	};
#endif