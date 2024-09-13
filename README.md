# NTerm

## How to build the linux package
- Complete the build in `release` format
- copy the executable in the path `nterm/usr/bin` as nterm(i.e the name of the executable file should be nterm)
- make sure you do `chmod 755 ./linux-pkg/usr/bin/nterm`
- run `dpkg-deb --build linux-pkg`
- To install the app in your local computer: `sudo dpkg -i linux-pkg.deb`
- To remove the installed app use `sudo dpkg -r nterm`
