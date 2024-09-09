# NTerm

## How to build the linux package
- Complete the build in `release` format
- copy the executable in the path `nterm/usr/bin` as nterm(i.e the name of the executable file should be nterm)
- run `dpkg-deb --build linux-pkg`
- To install the app in your local computer: `sudo dpkg -i nterm.deb`
- To remove the installed app use `sudo dpkg -r nterm`
