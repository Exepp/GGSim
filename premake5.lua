workspace "GGSim"

	architecture "x64"

	language "C++"

	cppdialect "C++17"

	toolset "clang"

	configurations
	{
		"Debug",
		"Release"
	}

outDir = "%{cfg.system}_%{cfg.architecture}/%{cfg.buildcfg}/"

project "GGSim"

		location "GGSim"

		kind "ConsoleApp"


		targetdir ("bin/" .. outDir .. "%{prj.name}")

		objdir ("bin_inter/" .. outDir .. "%{prj.name}")


		files
		{
			"%{prj.location}/src/**",
			"%{prj.location}/include/**",
			"%{wks.location}/external/src/**",
		}

		includedirs
		{
			"%{prj.location}/include/",
			"%{wks.location}/external/include/",
			"%{wks.location}/external/include/rp3d/", -- only for rp3d library
		}

		libdirs
		{
			"%{wks.location}/external/lib/",
		}


		filter "configurations:Debug"
			links  { "glfw3_d", "reactphysics3d_d", "ECSpp_d" }
		filter "configurations:Release"
			links { "glfw3", "reactphysics3d", "ECSpp" }

		filter{}

		postbuildcommands 
		{
			"{COPY} %{prj.location}/src/shaders %{cfg.targetdir}"
		}

		links { "dl", "X11", "GL", "pthread", "Xrandr", "Xi" }


		filter "system:windows"
			staticruntime "On"
			systemversion "latest"


		filter "configurations:Debug"
			defines "GGS_DEBUG"
			symbols "On"
		filter "configurations:Release"
			defines "GGS_RELEASE"
			optimize "On"
