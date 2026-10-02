# Xed packaging for Debian

This repository contains Debian packaging for [Linux Mint's xed](https://github.com/linuxmint/xed), targeting upstream tag **3.9.0** and source version **3.9.0-1**. The inspected tag points to commit `9be0bdcd6f17c119d41543440066da9287c12d0e`. Upstream code is downloaded separately.

The source builds `xed`, `xed-common`, and `xed-dev`. Installing `xed` pulls in shared data and built-in plugins; development files remain separate. Debhelper generates debug symbol packages automatically.

The build requirements select the coordinated GIRepository 2.0, libpeas and PyGObject transition in current Debian unstable. The Debian patches provide the matching include, link and namespace-registration behavior.

## Build and check

Use a minimal Debian unstable build VM and a separate Debian Cinnamon desktop VM for interactive tests. Install `build-essential`, `devscripts`, `dpkg-dev`, `lintian`, `sbuild`, and `autopkgtest` on the build VM. Configure an unstable sbuild/schroot testbed before using the isolated commands below.

From this packaging checkout:

```sh
uscan --download-current-version --destdir ..
mkdir ../xed-3.9.0
tar -xf ../xed_3.9.0.orig.tar.gz -C ../xed-3.9.0 --strip-components=1
# Replace the extracted Linux Mint packaging completely.
rm -rf ../xed-3.9.0/debian
cp -a debian ../xed-3.9.0/
cd ../xed-3.9.0
sudo apt build-dep .
dpkg-buildpackage -us -uc
lintian -i -I --pedantic ../xed_3.9.0-1_*.changes
sbuild -d unstable ../xed_3.9.0-1.dsc
autopkgtest ../xed_3.9.0-1_amd64.changes -- schroot unstable-amd64-sbuild
```

The `mkdir` deliberately fails if the build tree already exists; start with a fresh tree for each source preparation. Replace `amd64` and the testbed name with your configured architecture and schroot.

Use the original upstream archive, without a `+ds` repack just to remove `debian/`. Source format `3.0 (quilt)` replaces that directory when extracting the Debian source package. This packaging-only repository does not assume imported upstream or pristine-tar branches. `debian/watch` discovers numbered upstream tags; inspect and update the changelog before packaging a newer release.

The runtime test edits a document through the private typelib and exercises the installed Join Lines plugin. The development test compiles, links and runs a small document-editing program. Upstream dogtail tests require an interactive desktop and are skipped during the package build.

Install the resulting runtime packages with APT on the separate Cinnamon VM. Test opening, saving where applicable, help, printing, thumbnails and plugins, including Wayland and X11 sessions where available. Automated smoke checks do not cover all interactive behavior.

## Salsa and submission

The intended Salsa project is `https://salsa.debian.org/Overseer/xed`. Push the packaging history there and set the CI configuration path to `debian/salsa-ci.yml` under **Settings → CI/CD → General pipelines**. The standard Salsa recipe is retained.

The existing WNPP request is [#830598](https://bugs.debian.org/830598). Claim it as an ITP before requesting sponsorship.

Keep the changelog `UNRELEASED` during preparation. After clean Debian unstable builds, installed-package tests, desktop checks and copyright review pass, finalize it for `unstable`, build and sign a source upload on the machine holding your signing key, and upload it to mentors.debian.net for sponsor review. GitHub commits and Salsa CI do not upload to Debian. Do not commit binaries; any test binary release should include the matching source, `.changes`, `.buildinfo` and checksums.

Update the `Vcs-*` fields to the exact Salsa URL after making that project canonical.
