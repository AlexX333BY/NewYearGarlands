# New Year Garlands
New Year Garland for Windows
### Running
0. Install [wxWidgets](https://www.wxwidgets.org/) and set `wxwin` environment variable to wxWidgets root;
1. Compile `service` and `client` as executables;
2. Run `service`:
    * if you've compiled it in `DEBUG` mode, just run it as usual application;
    * if you've compiled it in `RELEASE` mode, run it with `-i` argument, and then you can enable it in Windows Service Manager as `GarlandService`;
3. Run as many `client` instances as you want;
4. See garland show on your screen!
