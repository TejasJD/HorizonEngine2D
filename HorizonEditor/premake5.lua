workspace "ScriptCoreLib"
	location "ScriptCoreLib"
	configurations { "Debug", "Release" }

project "ScriptCoreLib"
	location "ScriptCoreLib"
	kind "SharedLib"
	language "C#"

	targetdir ("Scripts")
	objdir ("Scripts/obj")
   
	files
	{
		"ScriptCoreLib/Source/**.cs",
		"ScriptCoreLib/Properties/**.cs"
	}
   
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"
	
		