# BZList Plugin for BZFS

Provides real-time data to BZList.

This project uses Git submodules, after cloning run `git submodule update --init --recursive` or clone with the `--recursive` flag.

## Compiling

You must have [premake5](https://premake.github.io/download.html) to build the plugin.

**Build**
```sh
premake5 build
```

**Install**
```sh
sudo premake5 install
```

## Loading the plugin

To load the plugin, use the format:
```
-loadplugin BZList,API_KEY
```
Where `API_KEY` is the key you have received from me.

## License

[MIT](LICENSE).
