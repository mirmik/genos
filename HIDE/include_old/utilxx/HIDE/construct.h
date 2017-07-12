#ifndef GENOS_CONSTRUCT_H
#define GENOS_CONSTRUCT_H

template<typename T, typename ... Args>
void construct(T* ptr, Args ... args) {
	ptr->T(args ...);
}

template<typename T>
void destruct(T* ptr, Args ... args) {
	ptr->~T(args ...);
}

#endif