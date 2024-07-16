#ifndef RABBIT_INTERVAL_H
#define RABBIT_INTERVAL_H

//Операции над интервалами.

#include <vector>
#include <algorithm>
#include <iterator>
#include <initializer_list>

#include <nos/print.h>
#include <nos/fprint.h>

namespace rabbit
{
	template<typename T> struct interval_union;

	//Интерал реализуется парой чисел, причем одно из них всегда меньше либо равно второму.
	//Допускается ситуация, когда минимум больше максимумак, но такой интервал не признается валидным.
	//Невалидные интервалы используются в булевых алгоритмах.
	template<typename T>
	class interval
	{
	public:
		T minimum;
		T maximum;

	public:
		interval() : minimum(1), maximum(-1) {} //невалидный.
		interval(T minimum, T maximum) : minimum(minimum), maximum(maximum) {}

		//Содержит ли точку (невключающая операция).
		bool in(T pnct) const
		{
			return minimum < pnct && pnct < maximum;
		}

		//Содержит ли точку (невключающая операция).
		//Данный вариант функции учитывает малый отступ для исключения крайних вариантов.
		bool in(T pnct, T prec) const
		{
			return minimum + prec < pnct && pnct < maximum - prec;
		}

		bool in_weak(T pnct, T prec) const
		{
			return minimum - prec < pnct && pnct < maximum + prec;
		}

		T length() const { return maximum - minimum; }

		//Проверка интервала на валидность.
		operator bool() const
		{
			return minimum <= maximum;
		}

		interval offset(T off) const
		{
			return interval(minimum + off, maximum + off);
		}

		interval reverse() const
		{
			return interval( - maximum, - minimum );
		}

		//Вырождается ли интервал в точку?
		//Вырожденные интервалы возникают, например, при пересечении смежных интервалов.
		bool is_degenerate() const
		{
			return minimum == maximum;
		}

		//Отношение порядка для выполнения операции сортировки.
		//Порядок вводится лексикографический.
		bool operator< (const interval& oth) const
		{
			return minimum < oth.minimum || maximum < oth.maximum;
		}

		//Определяет факт наличия пересечения с другим интервалом.
		bool is_intersected_with(const interval& oth) const
		{
			return maximum >= oth.minimum && minimum <= oth.maximum;
		}

		//Определяет факт наличия пересечения с другим интервалом.
		bool is_intersected_with_weak(const interval& oth, T prec) const
		{
			return maximum + prec >= oth.minimum && minimum - prec <= oth.maximum;
		}

		//Упрощенная операция пересечения.
		//В случае отсутствия пересечения возвращает невалидный интервал.
		interval simple_intersect(interval oth) const
		{
			return interval(MAX(minimum, oth.minimum), MIN(maximum, oth.maximum));
		}

		//Упрощенная операция слияния.
		//В случае отсутствия пересечения возвращает неверный результат (но валидный интервал).
		//Перед использованием нужно убедиться в наличии пересечения (is_intersected_with).
		//Если пересечения нет, результатом должны являться оба интервала.
		interval simple_combine(interval oth) const
		{
			return interval(MIN(minimum, oth.minimum), MAX(maximum, oth.maximum));
		}

		//Упрощенная операция разности.
		//Оба результата необходимо проверить на валидность.
		//Могут быть верны как оба сразу, так и не один.
		std::pair<interval, interval> simple_difference(interval oth) const
		{
			return std::pair<interval, interval> { { minimum, oth.minimum }, { oth.maximum, maximum } };
		}

		//Упрощенная операция симметрической разности.
		//В случае отсутствия пересечения возвращает неверный результат.
		//Результат может быть неупорядоченным.
		std::pair<interval, interval> simple_symmetric_difference(interval oth) const
		{
			return std::pair<interval, interval>
			{
				maximum > oth.maximum ? interval(oth.maximum, maximum) : interval(maximum, oth.maximum),
				minimum > oth.minimum ? interval(oth.minimum, minimum) : interval(minimum, oth.minimum),
			};
		}

		//Упрощенная операция разделения.
		//Возвращает невалидный результат при отсутствии пересечения.
		std::pair<interval, interval> simple_divide(T pnct) const
		{
			return std::pair<interval, interval>
			{
				interval(minimum, pnct), interval(pnct, maximum)
			};
		}

		//Операция пересечения двух интервалов.
		interval_union<T> intersect(interval oth) const
		{
			interval i = simple_intersect(oth);
			return i ? interval_union<T> { i } : interval_union<T>();
		}

		//Объединение двух интервалов.
		interval_union<T> combine(interval oth) const
		{
			interval intersection = simple_intersect(oth);
			return intersection ?
			       interval_union<T> { simple_combine(oth) } :
			       interval_union<T> { *this, oth };
		}

		//Разность двух интервалов.
		interval_union<T> difference(interval oth) const
		{
			interval_union<T> ret;
			interval intersection = simple_intersect(oth);

			if (intersection)
			{
				if (minimum != intersection.minimum)
				{
					ret.vec.push_back(interval(minimum, intersection.minimum));
				}

				if (maximum != intersection.maximum)
				{
					ret.vec.push_back(interval(intersection.maximum, maximum));
				}
			}
			return ret;
		}

		//Разделение интервала точкой. Если точка не лежит внутри, возвращает неизмененный интервал.
		interval_union<T> divide(T pnct)
		{
			return in(pnct) ?
			       interval_union<T> { interval(minimum, pnct), interval(pnct, maximum) } :
			       interval_union<T> { *this };
		}

		bool operator== (const interval& oth) const
		{
			return minimum == oth.minimum && maximum == oth.maximum;
		}

		size_t print_to(nos::ostream& o) const
		{
			return fprint_to(o, "({},{})", minimum, maximum);
		}
	};

	//Комбинация интервалов.
	//Реализует замкнутый набор операций.
	template<typename T>
	class interval_union
	{
	private:
		//Группа пересечения.
		//Внутренняя структура, используемая в булевых алгоритмах.
		//Реализует операции над пересекающимися интервалами объединений.
		struct intersected_group
		{
			using begtype = typename std::vector<interval<T>>::const_iterator;
			using endtype = typename std::vector<interval<T>>::const_iterator;

			begtype begin_ait;
			endtype end_ait;
			begtype begin_bit;
			endtype end_bit;

			intersected_group(
			    begtype a,
			    endtype ea,
			    begtype b,
			    endtype eb
			) :
				begin_ait(a),
				end_ait(ea),
				begin_bit(b),
				end_bit(eb)
			{}

			operator bool()
			{
				return begin_ait != end_ait && begin_bit != end_bit;
			}

			//Использовать с find_intersected_group_weak.
			interval<T> combine()
			{
				if (begin_ait == end_ait) return *begin_bit;
				if (begin_bit == end_bit) return *begin_ait;

				T minimum = begin_ait->minimum;
				T maximum = begin_ait->maximum;

				for (auto it = begin_ait; it != end_ait; ++it)
				{
					if (it->minimum < minimum) minimum = it->minimum;
					if (it->maximum > maximum) maximum = it->maximum;
				}

				for (auto it = begin_bit; it != end_bit; ++it)
				{
					if (it->minimum < minimum) minimum = it->minimum;
					if (it->maximum > maximum) maximum = it->maximum;
				}

				return interval<T>(minimum, maximum);
			}

			//Использовать с find_intersected_group.
			interval_union intersect()
			{
				interval_union ret;
				if (begin_ait == end_ait) return ret;
				if (begin_bit == end_bit) return ret;

				auto ait = begin_ait;
				auto bit = begin_bit;

				while (1)
				{
					auto r = ait->simple_intersect(*bit);
					if (r) ret.vec.push_back(r);
					auto amax = ait->maximum;
					auto bmax = bit->maximum;
					if (amax < bmax)
					{
						if (++ait == end_ait) return ret;
					}
					else
					{
						if (++bit == end_bit) return ret;
					}
				}
			}

			//Использовать с find_intersected_group_weak.
			interval_union difference()
			{
				interval_union ret;
				if (begin_ait == end_ait) return ret;
				if (begin_bit == end_bit) { ret.vec.push_back(*begin_ait); return ret; }

				auto ait = begin_ait;
				auto bit = begin_bit;

				interval<T> accumulator;
				for (; ait != end_ait && bit != end_bit; ++ait)
				{
					accumulator = *ait;
					while (1)
					{
						auto ldiff = accumulator.simple_difference(*bit);

						if (ldiff.first) ret.vec.push_back(ldiff.first);
						if (ldiff.second)
						{
							accumulator = ldiff.second;
							++bit;
							if (bit == end_bit || !bit->is_intersected_with(*ait))
							{
								ret.vec.push_back(accumulator);
								break;
							}
						}
						else break;
					}
				}
			}
		};

	private:
		std::vector<interval<T>> vec;

	public:
		interval_union(const std::initializer_list<interval<T>> lst) : vec(lst) {}
		interval_union() = default;

		//Операция приведения к стандартному виду.
		//Проводит сортировку, объединяет взамно пересекающиеся интервалы.
		void normalize()
		{
			if (vec.empty()) return;
			std::vector<interval<T>> nvec;

			std::sort(vec.begin(), vec.end());
			auto accumulator = vec[0];
			for (int i = 1; i < vec.size(); ++i)
			{
				if (accumulator.is_intersected_with(vec[i])) accumulator =
					    accumulator.simple_combine(vec[i]);
				else
				{
					nvec.push_back(accumulator);
					accumulator = vec[i];
				}
			}
			nvec.push_back(accumulator);
			std::swap(vec, nvec);
		}

		template <class AI, class CAI, class BI, class CBI>
		intersected_group __find_intersected_group(
			AI  ait, 
			CAI end_ait, 
			BI  bit, 
			CBI end_bit)
		{
			auto a = ait;
			auto b = bit;
			auto anext = ait;
			auto bnext = bit;
			++anext;
			++bnext;

			while (1)
			{
				if (anext != end_ait) if ((anext)->is_intersected_with(*b)) { ++a; ++anext; continue; }
				if (bnext != end_bit) if ((bnext)->is_intersected_with(*a)) { ++b; ++bnext; continue; }
				return intersected_group(ait, anext, bit, bnext);
			}
		}

		//Поиск группы пересечения. Интерпретирует непересекающиеся интервалы как группы.
		template <class AI, class CAI, class BI, class CBI>
		intersected_group find_intersected_group_weak(
		    AI  ait,
		    CAI end_ait,
		    BI  bit,
		    CBI end_bit)

		{
			if (ait == end_ait)
			{
				auto ebit = bit;
				return intersected_group(ait, ait, bit, ++ebit);
			}

			if (bit == end_bit)
			{
				auto eait = ait;
				return intersected_group(ait, ++eait, bit, bit);
			}

			if ( ! ait->is_intersected_with(*bit) )
			{
				if (*ait < *bit)
				{
					auto eait = ait;
					return intersected_group(ait, ++eait, bit, bit);
				}
				else
				{
					auto ebit = bit;
					return intersected_group(ait, ait, bit, ++ebit);
				}
			}

			return __find_intersected_group(ait, end_ait, bit, end_bit);
		};

		//Поиск группы пересечения. Непересекающиеся интервалы отбрасываются.
		template <class AI, class CAI, class BI, class CBI>
		intersected_group find_intersected_group(
		    AI  ait,
		    CAI eait,
		    BI  bit,
		    CBI ebit)
		{
			while (1)
			{
				if (ait == eait || bit == ebit) return intersected_group(eait, eait, ebit, ebit);
				if ( ! ait->is_intersected_with(*bit) )
				{
					if (*ait < *bit) ++ait; else ++bit;
				}
				else break;
			}

			return __find_intersected_group(ait, eait, bit, ebit);
		};

		//Булево объединение.
		interval_union combine(const interval_union& oth) const
		{
			interval_union ret;

			auto ait = vec.begin();
			auto eait = vec.end();
			auto bit = oth.vec.begin();
			auto ebit = oth.vec.end();
			for (; ait != eait || bit != ebit;)
			{
				auto group = interval_union<double>::find_intersected_group_weak(ait, eait, bit, ebit);
				ret.vec.push_back(group.combine());
				ait = group.end_ait;
				bit = group.end_bit;
			}
			return ret;
		}

		//Булево пересечение.
		interval_union intersect(const interval_union& oth) const
		{
			interval_union ret;

			auto ait = vec.begin();
			auto eait = vec.end();
			auto bit = oth.vec.begin();
			auto ebit = oth.vec.end();
			for (; ait != eait || bit != ebit;)
			{
				auto group = interval_union<double>::find_intersected_group(ait, eait, bit, ebit);
				if (!group) return ret;
				auto r = group.intersect();
				std::copy(r.vec.begin(), r.vec.end(), std::back_inserter(ret.vec));
				ait = group.end_ait;
				bit = group.end_bit;
			}
			return ret;
		}

		//Булева разность.
		interval_union difference(const interval_union& oth) const
		{
			interval_union ret;

			auto ait = vec.begin();
			auto eait = vec.end();
			auto bit = oth.vec.begin();
			auto ebit = oth.vec.end();
			for (; ait != eait || bit != ebit;)
			{
				auto group = interval_union<double>::find_intersected_group_weak(ait, eait, bit, ebit);
				auto r = group.difference();
				std::copy(r.vec.begin(), r.vec.end(), std::back_inserter(ret.vec));
				ait = group.end_ait;
				bit = group.end_bit;
			}
			return ret;
		}

		bool operator==(const interval_union& oth) const { return vec == oth.vec; }

		auto begin() { return vec.begin(); }
		auto end() { return vec.end(); }
		auto begin() const { return vec.begin(); }
		auto end() const { return vec.end(); }
		auto cbegin() const { return vec.cbegin(); }
		auto cend() const { return vec.cend(); }

		size_t print_to(nos::ostream& o) const
		{
			return nos::print_to(o, vec);
		}

		friend class interval<T>;
	};

	template <typename T>
	struct directed_interval : public interval<T>
	{
		bool reverse;
		directed_interval(T strt, T stop) : interval<T>(strt, stop)
		{
			reverse = interval<T>::minimum > interval<T>::maximum;
			if (reverse) std::swap(interval<T>::minimum, interval<T>::maximum);
		}
		directed_interval() = default;

		T start() const { return reverse ? interval<T>::maximum : interval<T>::minimum; }
		T finish() const { return reverse ? interval<T>::minimum : interval<T>::maximum; }

		T proc(double prc) const
		{
			//assert(prc >= 0-0.00001 && prc <=1+0.000001);
			return reverse ?
			       (prc * interval<T>::minimum + (1 - prc) * interval<T>::maximum) :
			       (prc * interval<T>::maximum + (1 - prc) * interval<T>::minimum);
		}

		double to_proc(T param) const
		{
			//gxx::println(interval<T>::minimum, param <= interval<T>::maximum);
			//assert(param >= interval<T>::minimum && param <= interval<T>::maximum);
			auto koeff = (param - interval<T>::minimum) / interval<T>::length();
			return reverse ? 1 - koeff : koeff;

		}
	};
}

#endif