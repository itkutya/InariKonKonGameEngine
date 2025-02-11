#include "InariKonKon/Filesystem/Filesystem.hpp"

#include <fstream>

namespace ikk
{
    Filesystem::Filesystem(const std::filesystem::path& path) noexcept
    {
        //TODO:
        //Add options for opening the file...
        //& better error handeling
        std::ifstream file { path, std::ios::ate | std::ios::binary };

        if (!file.is_open())
            return;

        std::size_t fileSize = static_cast<std::size_t>(file.tellg());
        this->m_data.resize(fileSize);

        file.seekg(0);
        file.read(this->m_data.data(), static_cast<std::streamsize>(fileSize));

        file.close();
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