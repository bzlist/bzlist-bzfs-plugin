# BZList Plugin for BZFS

Provides real-time data to BZList.

This project uses Git submodules, after cloning run `git submodule update --init --recursive` or clone with the `--recursive` flag.

## Compiling

You must have [premake5](https://premake.github.io/download.html) to build the plugin.

Build:
```sh
premake5 build
```

Install:
```sh
sudo premake5 install
```

## Loading the plugin

To load the plugin, use the format:
```
-loadplugin BZList,API_KEY
```
Where `API_KEY` is the key you have received from me.

## Server Commands

Describe the server commands, if any, that this plugin exposes.

/sample on
  Turns the sample on

/sample off
  Turns the sample off

/sample powerlevel <powerlevel>
  Sets the power level to <powerlevel>, where <powerlevel> is a positive integer

## License

[MIT](LICENSE).
