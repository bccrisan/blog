#pragma once

template
<
	typename T,
	template<typename> typename AcquirePolicy,
	template<typename> typename ReleasePolicy
>
class RAII
{
public:
	explicit RAII(T h) : m_handle(h) { AcquirePolicy<T>::Execute(m_handle); }
	RAII(const RAII&) = delete;
	RAII(RAII&&) = default;
	RAII& operator = (const RAII&) = delete;
	RAII& operator = (RAII&&) = default;
	~RAII() { ReleasePolicy<T>::Execute(m_handle); }

	operator T() const { return m_handle; }

private:
	T m_handle;
};

template<typename T> struct NoOpPolicy { static void Execute(T) noexcept {} };

template<typename T> struct PointerReleasePolicy { static void Execute(T ptr) noexcept { delete ptr; } };
template<typename T> struct ArrayReleasePolicy { static void Execute(T ptr) noexcept { delete[] ptr; } };

template<typename T> using arr_ptr_handle_t = RAII<T*, NoOpPolicy, ArrayReleasePolicy>;
template<typename T> using ptr_handle_t = RAII<T*, NoOpPolicy, PointerReleasePolicy>;