workspace "Game"
        architecture "x64"

        configurations {
            "Debug",
            "Release",
            "Retail"
        }

        startproject "Game"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

filter "configurations:Debug"
defines { "_DEBUG" }
-- debugdir "$(TargetDir)" -- This sets the working directory for debugging
-- debugdir (dirs.bin) -- This sets the working directory for debugging
-- debugdir ("%{cfg.buildtarget.abspath}///") -- This sets the working directory for debugging
filter "configurations:Release or Retail"
defines { "NDEBUG" }

include "Source/Engine"
include "Source/Game"