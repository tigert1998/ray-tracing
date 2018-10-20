solution "ray-tracing"
configurations { "Debug", "Release" }
configuration "Debug"
    targetdir "../bin/Debug"
configuration "Release"
    targetdir "../bin/Release"

project "ray-tracing"
    kind "ConsoleApp"
    language "C++"
    linkoptions {
      "-lpthread"
    }
    buildoptions { "-std=c++17" }

    includedirs {
        "/usr/local/include", 
        "../include", 
        "../include/texture_related" 
    }
    files {
        "../include/**.h",
        "../src/**.cpp"
    }
