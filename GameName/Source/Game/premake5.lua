project "Game"
        kind "WindowedApp"
        language "C++"
        cppdialect "C++20"
        staticruntime "off"

        dependson ("Engine")

targetdir("../../Bin" .. outputdir .. "/%{prj.name}")
objdir ("../../Bin-int" .. outputdir .. "/%{prj.name}")

links {"Engine"}

        files{
            "../Game/**.h",
            "../Game/**.cpp",
        }  

        includedirs {
            "../Game/**"
        }

        externalincludedirs {
            "../External/",
            "../Engine/**",
        }

        defines {
            "WINDOWS"
        }

        libdirs {"../../Resources/**"}
       

filter {"configurations:Debug"}
        links {"CU-d.lib","DirectXTK-d.lib"}
            runtime "Debug"
            symbols "on"


filter {"configurations:Release"}
        links {"CU.lib","DirectXTK.lib"}
        runtime "Release"
        optimize "on"

filter {"configurations:Retail"}
        links {"CU.lib","DirectXTK.lib"}
        runtime "Release"
        optimize "on"

filter "system:windows"
        flags { 
            --	"FatalWarnings", -- would be both compile and lib, the original didn't set lib
                "FatalCompileWarnings",
                "MultiProcessorCompile",
            }
            links {
                "DXGI",
            }
    
            defines {
                "WIN32",
                -- "_CRT_SECURE_NO_WARNINGS", 
                -- "_LIB", 
                "_WIN32_WINNT=0x0602", --maybe update to: 0x0602  
            }