workspace "ScriptAppLib"
	location "ScriptAppLib"
	configurations { "Debug", "Release" }

project "ScriptAppLib"
	location "ScriptAppLib"
	kind "SharedLib"
	language "C#"

	targetdir ("bin")
	objdir ("bin/obj")
   
	files
	{
		"ScriptAppLib/Source/**.cs",
		"ScriptAppLib/Properties/**.cs"
	}

	print(_PREMAKE_DIR .. "/Scripts/ScriptCoreLib.dll")

	links
	{
		_PREMAKE_DIR .. "/Scripts/ScriptCoreLib.dll"
	}
   
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"