#ifndef IKK_NON_COPYABLE_HPP
#define IKK_NON_COPYABLE_HPP

namespace ikk
{
    class NonCopyable
    {
    public:
		constexpr NonCopyable() noexcept = default;

		constexpr NonCopyable(const NonCopyable&) noexcept = delete;
		constexpr NonCopyable(NonCopyable&&) noexcept = default;

		constexpr NonCopyable& operator=(const NonCopyable&) noexcept = delete;
		constexpr NonCopyable& operator=(NonCopyable&&) noexcept = default;

		virtual constexpr ~NonCopyable() noexcept = default;
    private:
    };
}

#endif