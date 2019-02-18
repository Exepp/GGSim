workspace "GGSim"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release"
	}

outDir = "%{cfg.system}_%{cfg.architecture}/%{cfg.buildcfg}/"

project "GGSim"
		location "GGSim"
		kind "ConsoleApp"
		language "C++"

		targetdir ("bin/" .. outDir .. "%{prj.name}")
		objdir ("bin_inter/" .. outDir .. "%{prj.name}")

		files
		{
			"%{prj.location}/src/main.cpp",
		}

		includedirs
		{

		}

		filter "system:windows"
			cppdialect "C++17"
			staticruntime "On"
			systemversion "latest"


		filter "configurations:Debug"
			defines "GGS_DEBUG"
			symbols "On"
		filter "configurations:Release"
			defines "GGS_RELEASE"
			optimize "On"