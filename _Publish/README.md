# _Publish

This directory stores the published library packages. Under this directory, the file `PublishVersion.txt` records the published library names and their versions.

## Version format

The version format is `m.a.b.c`.

### `m`

The major version number. When there is a major upgrade (this often means there are incompatible updates), or when `a` exceeds 999, modify this value.

### `a`

Interface and function version numbers. From 100 to 999, hundreds represent the interface revision, others represent the function addition and modification.

### `b`

Issue and code revision number. Modify this value when there are only bug fixes.

### `c`

Build number, can be ignored.

## Publish version

Whenever a library is released, the library package will be stored in this directory, and the publish information also needs to be recorded in `PublishVersion.txt`.

Assuming the library name is `NeatCore` and the library version is 1.100.0.0, the publish information is `MapLibrary    "NeatCore"        "1.100.0.0".`

## Package format

Assuming the library name is `NeatCore` and the library version is 1.100.0.0, the library package is stored in the `NeatCore` folder, and the library package name is `NeatCore-1.100.0.0.7z`.

The file hierarchy is as follows:

```
_Publish
	NeatCore
		NeatCore-1.100.0.0.7z
		NeatCore-1.100.1.0.7z
		NeatCore-1.101.0.0.7z
	NeatData
		NeatCore-1.100.0.0.7z
		NeatCore-1.200.0.0.7z
```

