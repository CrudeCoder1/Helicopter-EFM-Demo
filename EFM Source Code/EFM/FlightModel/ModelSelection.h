#pragma once
#include <fstream>
#include <string>

namespace Ah6jModelSelection
{
    enum class Model
    {
        Ah6j,
        AH6J_UH_EFM
    };
    inline Model& current()
    {
        static Model value = Model::Ah6j;
        return value;
    }

    // Match FlightModelSelection.lua's data-only return string. Never execute Lua
    // in the DLL. Missing, unknown or malformed selections retain the legacy model.
    inline Model read(const char* path)
    {
        if (!path || !*path)
            return Model::Ah6j;
        std::ifstream file(path);
        std::string line;
        while (std::getline(file, line))
        {
            const auto first = line.find_first_not_of(" \t\r\n");
            if (first == std::string::npos || line.compare(first, 2, "--") == 0)
                continue;
            if (line.compare(first, 6, "return") != 0)
                return Model::Ah6j;
            const auto quote = line.find_first_not_of(" \t", first + 6);
            if (quote == std::string::npos || (line[quote] != '\'' && line[quote] != '"'))
                return Model::Ah6j;
            const auto end = line.find(line[quote], quote + 1);
            if (end == std::string::npos)
                return Model::Ah6j;
            const auto tail = line.find_first_not_of(" \t\r\n", end + 1);
            if (tail != std::string::npos && line.compare(tail, 2, "--") != 0)
                return Model::Ah6j;
            return line.substr(quote + 1, end - quote - 1) == "ah6j_oh6_cr3144_derivative"
                       ? Model::AH6J_UH_EFM
                       : Model::Ah6j;
        }
        return Model::Ah6j;
    }
    inline bool ah6jUhEfmSelected()
    {
        return current() == Model::AH6J_UH_EFM;
    }
} // namespace Ah6jModelSelection
