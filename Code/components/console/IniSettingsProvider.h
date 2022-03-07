// Copyright (C) 2022 TiltedPhoques SRL.
// For licensing information see LICENSE at the root of this distribution.
#pragma once

#include <filesystem>

namespace Console
{
void SaveSettingsToIni(const std::filesystem::path& path);
void LoadSettingsFromIni(const std::filesystem::path& path);
} // namespace base