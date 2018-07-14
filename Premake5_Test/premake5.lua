---------------------------------
-- [ WORKSPACE CONFIGURATION   --
---------------------------------
local projectName = "Oxana"

  workspace = projectName         -- Solution Name
  configurations { "Debug", "Release" }   -- Optimization/General config mode in VS
  platforms { "x64" }                     -- Dropdown platforms section in VS

  -- _ACTION is the argument passed into premake5 when you run it.
  local project_action = "UNDEFINED"
  if _ACTION ~= nill then
    project_action = _ACTION
  end

  -- Where the project files (vs project, solution, etc) go
  location( "project_" .. project_action)

  -------------------------------
  -- [ COMPILER/LINKER CONFIG] --
  -------------------------------
  flags "FatalWarnings" -- comment if you don't want warnings to count as errors
  warnings "Extra"

  -- see 'filter' in the wiki pages
  filter "configurations:Debug"    defines { "DEBUG" }  flags { "Symbols" }
  filter "configurations:Release"  defines { "NDEBUG" } optimize "On"

  filter { "platforms:*64" } architecture "x64"

  -- when building any visual studio project
  filter { "system:windows", "action:vs*"}
    flags         { "MultiProcessorCompile", "NoMinimalRebuild" }
    linkoptions   { "/ignore:4099" }      -- Ignore library pdb warnings when running in debug

  filter {} -- clear filter when you know you no longer need it!
  

  -------------------------------
  -- [ PROJECT CONFIGURATION ] --
  ------------------------------- 
  project = projectName
    kind "WindowedApp" -- "WindowedApp" removes console
    language "C++"
    targetdir "bin_%{cfg.buildcfg}_%{cfg.platform}" -- where the output binary goes.
    targetname = projectName -- the name of the executable saved to targetdir


    --------------------------------------
    -- [ PROJECT FILES CONFIGURATIONS ] --
    --------------------------------------
    local SourceDir = "./source/";
    local UserDir = "./user/";
    -- what files the visual studio project/makefile/etc should know about
    files
    { 
      SourceDir .. "**.h", SourceDir .. "**.hpp", 
      SourceDir .. "**.c", SourceDir .. "**.cpp", SourceDir .. "**.tpp",
      
      UserDir .. "**.h", UserDir .. "**.hpp", 
      UserDir .. "**.c", UserDir .. "**.cpp", UserDir .. "**.tpp",      
    }

    -- Exclude template files from project (so they don't accidentally get compiled)
    filter { "files:**.tpp" }
      flags {"ExcludeFromBuild"}

    filter {} -- clear filter!

    -- setting up visual studio filters (basically virtual folders).
    vpaths 
    {
      ["Source/*"] = { SourceDir .. "**.c", SourceDir .. "**.cxx", SourceDir .. "**.cpp", SourceDir .. "**.h", SourceDir .. "**.hxx", SourceDir .. "**.hpp" },
      ["User/*"] = { SourceDir .. "**.c", SourceDir .. "**.cxx", SourceDir .. "**.cpp" },
    }

    -- where to find header files that you might be including, mainly for library headers.
    includedirs
    {
      SourceDir -- include root source directory to allow for absolute include paths
      -- include the headers of any libraries/dlls you need
    }


    -------------------------------------------
    -- [ PROJECT DEPENDENCY CONFIGURATIONS ] --
    -------------------------------------------
    libdirs
    {
      -- add dependency directories here
      "./lib"
    }

    -- Debug
    filter { "configurations:Debug" }   
    links
    {
      -- add depedencies (libraries) here
      "sfml-main-d.lib",
      "sfml-graphics-s-d.lib",
      "sfml-window-s-d.lib",
      "sfml-system-s-d.lib",
      "opengl32.lib",
      "winmm.lib",
      "gdi32.lib",
      "freetype.lib",
      "jpeg.lib"      
    }    
    
    -- Release
    filter { "configurations:Release" }        
    links
    {
      -- add depedencies (libraries) here
      "sfml-main-d.lib",
      "sfml-graphics-s.lib",
      "sfml-window-s.lib",
      "sfml-system-s.lib",
      "opengl32.lib",
      "winmm.lib",
      "gdi32.lib",
      "freetype.lib",
      "jpeg.lib"      
    }

