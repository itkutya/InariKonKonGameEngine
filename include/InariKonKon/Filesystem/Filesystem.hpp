#pragma once

#include <filesystem>
#include <string>

namespace ikk
{
    class Filesystem
    {
    public:
        explicit Filesystem(const std::filesystem::path& path) noexcept;

        ~Filesystem() noexcept = default;

        Filesystem(const Filesystem&) noexcept = default;
        Filesystem(Filesystem&&) noexcept = default;

        Filesystem& operator=(const Filesystem&) noexcept = default;
        Filesystem& operator=(Filesystem&&) noexcept = default;

        const std::filesystem::path& getPath() const noexcept;
        const std::string& getContents() const noexcept;
    private:
        std::filesystem::path m_path;
        std::string m_data;
    };
}