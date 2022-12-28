# WeatherDeskBand
[Japanese](https://github.com/Nusudo-junior/WeatherDeskBand/blob/master/README_ja.md)

Display the weather forecast provided by Open-Meteo on your taskbar. 

![sample](https://user-images.githubusercontent.com/106300228/209774652-f477dbae-8592-4dd2-9acc-84e0fafea9b3.jpg)

Open-Meteo: https://open-meteo.com/en

## Description
This is a deskband to display the current weather and temperature and today's high and low provided by Open-Meteo. 
The weather forecast is updated every 6 hours from midnight. You can also update it by clicking the deskband.

## Usage

### Install/Uninstall

Download a release version and run `install.bat`. Right-click the taskbar and select `toolbar`->`Weather DeskBand` on the menu.

<img src="https://user-images.githubusercontent.com/106300228/209650853-bc9efbd9-a27e-4870-b8e9-5c7f430bf06c.jpg" alter="register" width="30%" height="30%" >

If you want to uninstall, run `uninstall.bat` and restart the explorer.

### Change URL (Default URL: Osaka, Japan)
Make URL on [Open-Meteo](https://open-meteo.com/en/docs#api_form). 
Note that you must check or change the parameters below.
- Hourly Weather Variables: Temperature (2 m), Weathercode
- Daily Weather Variables: Maximum Temperature (2 m), Minimum Temperature (2 m), Sunrise, Sunset
- Timeformat: Unix timestamp

Open the `WeatherDeskBand.xml` and change the `src` value in `URL` to the URL you made.

<img src="https://user-images.githubusercontent.com/106300228/209780563-e7c02970-8eaa-4f61-8f6a-0a1bdb6e7420.jpg" alter="xml" width="60%" height="60%" >

### Change Icon

Save an icon in `images` folder. Open `WeatherDeskBand.xml` and change the `filename` value in `Icon` element.

## Requirement
Windows10 x64

## Build
- Visual Studio 2022
- cpprestsdk 2.10.18
- nlohmann.json.decomposed 3.11.2

## Contact

Mail:u849256c\[at\]ecs.osaka-u.ac.jp

## License
MIT

## Reference
- https://learn.microsoft.com/ja-jp/windows/win32/shell/band-objects
- http://eternalwindows.jp/shell/shellex/shellex13.html
- https://github.com/KMConner/TaskbarTweet
