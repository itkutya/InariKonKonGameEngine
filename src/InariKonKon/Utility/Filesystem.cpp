#include "InariKonKon/Utility/Filesystem.hpp"

#include <fstream>

#include "InariKonKon/Utility/Log.hpp"

namespace ikk
{
    Filesystem::Filesystem(const std::filesystem::path& path) noexcept : m_path(path)
    {
        std::ifstream file { path, std::ios::ate | std::ios::binary };

        if (file.is_open())
        {
            std::size_t fileSize = static_cast<std::size_t>(file.tellg());
            this->m_data.resize(fileSize);

            file.seekg(0);
            file.read(this->m_data.data(), static_cast<std::streamsize>(fileSize));

            file.close();
        }
        else
            Log(std::format("Faild to open file: {}", path.c_str()).c_str(), Log::ERROR, Log::ALL);
    }

    const std::filesystem::path& Filesystem::getPath() const noexcept
    {
        return this->m_path;
    }

    const std::string& Filesystem::getContents() const noexcept
    {
        return this->m_data;
    }
}