project "Engine"
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"
        staticruntime "off"

        targetdir("../../Bin" .. outputdir .. "/%{prj.name}")
        objdir ("../../Bin-int" .. outputdir .. "/%{prj.name}")

        files{
            -- "../Engine/**.h",
            -- "../Engine/**.cpp",
            "../Engine/**.hlsl",
            "../Engine/**.hlsli",
			"src/**.h",
       		"src/**.cpp",
        }  


        defines {
            "WINDOWS",
        }
        
        externalincludedirs {
            "src",
            "../External/",
			"..",            
        }

            libdirs {"../../Resources/**"}

filter {"configurations:Debug"}
            runtime "Debug"
        links {"CU-d.lib", "DirectXTK-d.lib"}
            symbols "on"


filter {"configurations:Release"}
        runtime "Release"
        links {"CU.lib","DirectXTK.lib"}
        optimize "on"

filter {"configurations:Retail"}
        links {"CU.lib","DirectXTK.lib"}
        runtime "Release"
        optimize "on" 


filter "system:windows"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"
		warnings "Extra"

		flags { 
			"FatalCompileWarnings",
			"MultiProcessorCompile"
		}
		links {
			"DXGI",
			"dxguid",
			"d3d11",
		}

		defines {
			"WIN32",
			"_CRT_SECURE_NO_WARNINGS", 
			"_LIB", 
			"_WIN32_WINNT=0x0602", 
			"_SILENCE_CXX20_CISO646_REMOVED_WARNING",
			'NOMINMAX'
		}


        shadermodel("5.0")

   filter("files:**PS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Pixel")

	filter("files:**VS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Vertex")

	filter("files:**GS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Geometry")
	
	filter("files:**CS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Compute")