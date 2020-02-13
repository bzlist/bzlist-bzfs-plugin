workspace "BZList"
  configurations {"Release"}

  language "C++"
  targetdir "bin"
  targetprefix ""

  includedirs {"../../include"}

  filter "configurations:Release"
    optimize "On"
  
  newaction {
    ["trigger"] = "clean",
    ["description"] = "Delete generated project and build files",
    ["onStart"] =
      function()
	      print "Cleaning files..."
      end,
    ["execute"] =
      function()
	      os.execute("rm -rf obj bin Makefile *.make")
      end,
    ["onEnd"] =
      function()
	      print "Done."
      end
  }

  newaction {
    ["trigger"] = "install",
    ["description"] = "Install plugin",
    ["onStart"] =
      function()
	      print "Copying files..."
      end,
    ["execute"] =
      function()
        os.execute("chmod +x bin/BZList.so")
        os.execute("cp bin/BZList.so /usr/local/lib/bzflag")
      end,
    ["onEnd"] =
      function()
	      print "Done."
      end
  }

  newaction {
    ["trigger"] = "build",
    ["description"] = "Build plugin",
    ["onStart"] =
      function()
        print "Building plugin..."
      end,
    ["execute"] =
      function()
        os.execute("premake5 gmake2")
        os.execute("make")
      end,
    ["onEnd"] =
      function()
	      print "Done."
      end
  }

project "plugin_utils"
  kind "StaticLib"

  files {"../plugin_utils/*.cpp", "../plugin_utils/*.h"}
  buildoptions "-fPIC"

project "BZList"
  kind "SharedLib"

  files {"*.cpp", "*.h", "easywsclient/easywsclient.cpp"}
  includedirs {"../plugin_utils", "easywsclient"}
  links {"plugin_utils"}
