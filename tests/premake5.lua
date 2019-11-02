newoption {
	trigger     = "location",
	value       = "./",
	description = "Where to generate the project.",
}

if not _OPTIONS["location"] then
	_OPTIONS["location"] = "./"
end

location_dir = _OPTIONS["location"]

include(location_dir .. "conanbuildinfo.premake.lua")

workspace("HLA-tests")
	location(location_dir)
	conan_basic_setup()

function add_test(name)
	project(name)
		files{
			name .. ".cpp",
		}

		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		targetdir = location_dir .. "bin/%{cfg.buildcfg}"

		includedirs{
			"../include",
		}

		flags{"MultiProcessorCompile"}
        
        warnings "Extra"

        filter "toolset:msc"
            buildoptions{"/permissive-"}

		filter "configurations:Debug"
			defines { "DEBUG" }
			symbols "On"

		filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On"
end


test_files = os.matchfiles("./*.cpp")

for k,v in pairs(test_files) do
	add_test( string.sub(v, 0, string.len(v) - string.len(".cpp")) )
end