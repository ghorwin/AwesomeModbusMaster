# Release procedure

## Preparations

- Increase version number and release date in `AwesomeModbusMaster/src/Constants.cpp`
- Update man-page:

[source,bash]
----
./AwesomeModbusMaster --man-page > ../../AwesomeModbusMaster/doc/AwesomeModbusMaster.1
----

- update manual


## First deb package for a new release/new version

When creating new release for first Debian/Ubuntu distro:

```bash
> scripts/update_source_code_archive.sh
```

Then change into distro-specific directory and run

```bash
> dch
```

to create new changelog entry. Edit the top line to reflect the new version number, e.g.

```
awesomemodbusmaster (0.9.5-1~jammy~ppa1) UNRELEASED; urgency=medium

  * update for release 0.9.5

 -- Andreas Nicolai <andreas.nicolai@gmx.net>  Thu, 22 Dec 2022 18:50:35 +0100

awesomemodbusmaster (0.9.4-1~jammy~ppa1) jammy; urgency=medium

  * first release for Jammy

 -- Andreas Nicolai <andreas.nicolai@gmx.net>  Thu, 19 May 2022 16:15:10 +0200

```

Note that the top line contains the tag UNRELEASED.

Next run

```bash
> dch --release
```

Now the line will read:

```
awesomemodbusmaster (0.9.5-1~jammy~ppa1) jammy; urgency=medium
```

Then:

```bash
> ../scripts/newUpstreamVersion.sh
Building source package 0.9.5-1~jammy~ppa1 for upstream version 0.9.5
Was the changelog updated correctly? Press any key or CTRL+C to abort!

(press key to continue)

....
Upload source archive to Launchpad? Remember to build binary package at least once for testing!

(press CTRL+C to aboard upload!!!)
```

and fix any warnings potentially issued.

Next build the package:

```bash
> ../scripts/buildBinary.sh
```

and if all is successful, again by:

```bash
> ../scripts/newUpstreamVersion.sh
# this time just press a key to start upload
```

## deb packages for other releases

Download already published original source code archive from launchpad, and extract:

```bash
> tar -xf awesomemodbusmaster_0.9.3.orig.tar.xz
```

Then follow procedure as described above.



