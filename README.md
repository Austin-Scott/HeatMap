# Heat Map ![](https://img.shields.io/github/release/Austin-Scott/HeatMap.svg) ![](https://img.shields.io/github/downloads/Austin-Scott/HeatMap/total.svg)
![render](https://user-images.githubusercontent.com/12504656/51348041-47cbc100-1a67-11e9-8361-83dcfdb6d2d1.png)
Show your friends and family what you've accomplished on foot or by bike by creating a Heat Map summary of all of your activities on Strava, Garmin Connect, or other GPS fitness sites!

## Getting Started Guide
### For Strava Users
Download a complete archive of your account by going to the [Account Download and Deletion page](https://www.strava.com/athlete/delete_your_account). **Downloading an archive will not harm your account in any way.** Click on the "Request Your Archive" button under the second step. After a couple of hours Strava will send you an email with a link to download a Zip file containing all of the activities you have ever uploaded to your account. Unzip this archive somewhere you can easily find again. Then launch the Heat Map application and select the "activities" subfolder from the unzipped archive when you are prompted. Also, before selecting "Load Activities", make sure you have the "Automatically decompress .gz files" checkbox ticked. Strava tends to send activities that have been compressed with gzip and this option will let the application read these files as well. Select "Load Activities" and follow the on-screen instructions.
### Garmin Connect users and anyone else
Just download all of your GPS activity files in any of the supported formats below and save them inside of a folder. Perhaps a tool such as [tapiriik](https://tapiriik.com/) can help by mass transfering all of your activities to dropbox. However, I have not tested that service. Select the directory when you start the program and follow the on screen instructions!

## Privacy Zones
To hide sensitive portions of your activities (such as locations of houses) from the map add privacy zones to the "privacy.txt" file. Here is an example that uses a rectangular privacy zone to hide any activities recorded in Texas and a circlular privacy zone to hide activities in New York City. All lines that begin with '#' are ignored.
~~~
#Set up privacy zones here. Each zone should be on its own line with data points seperated by a space. No quotes.
#
#To add a rectangular privacy zone specify the lower left coordinate followed by the upper right coordinate:
#	"lat lon lat lon"
#To add a circular privacy zone specify the center coordinate followed by the radius in kilometers:
#	"lat lon radius"

26.151493 -105.973976 36.946380 -94.289671
40.728995 -73.987993 20.921
~~~

## GPS file formats supported

| Format | GPS Tracks | Date Filtering | Activity Type Filtering | Average Speed Filtering | 
| --- | --- | --- | --- | --- |
| .tcx | Yes | Yes | Yes | Yes |
| .gpx | Yes | Yes *(If found in meta-data)* | **No** | **No** |
| .fit | Yes | Yes | Yes | Yes |

Files with the extensions ".tcx.gz", ".gpx.gz", and ".fit.gz" that are commonly found in Strava account archives are simply compressed versions of the above file types. This application will automatically decompress and read any files found in these formats.

## Map Quest Integration
The first time you render a heat map with the "download map from Map Quest" option selected you will be prompted to enter an API key. You will need to sign up for a free [Map Quest developer account](https://developer.mapquest.com/plan_purchase/steps/business_edition/business_edition_free/register) and [create a new key](https://developer.mapquest.com/user/me/apps). The key is then stored and retrieved inside "key.txt" so it doesn't need to be entered more than once. 

## The Interface
![gui](https://user-images.githubusercontent.com/12504656/51385500-185b9980-1ae5-11e9-89e9-94745ced1bb4.PNG)

When you first launch the program you will be greeted with a screen from where you must select a valid activity folder before proceeding. Once your activities have been loaded you will be presented with above main interface. This interface's controls have been organized into five subgroups and one main group. Here are the subgroups:
  * Configure Viewport
    * Manual
      * Manually set the viewport dimensions. You must specify the coordinate of the lower left and upper right coordinates of the generated heat map.
      * Please note that if you have the "Download map from Map Quest" option selected in the "Configure Renderer" subgroup the bounds of the resulting image will be modified to maintain aspect ratio and to snap your image to a map zoom level. However, everything inside of your specified bounds will be visible in the resulting image.
    * Ratio
      * By specifying just the coordinate of the bottom center of your viewport and a maximum latitude the program will automatically adjust the width of the viewport to maintain the correct aspect ratio in the resulting image.
      * If you are using a background map from Map Quest I recommend using either the Manual or Automatic option instead because the aspect ratio will be maintained regardless.
    * Automatic
      * Automatically computes the bounds of the viewport to make all activities that start within a specified radius of the median start point completely visible. Recommended option.
  * Filter By Activity Date
    * Filter activities from appearing on the Heat Map by their start date.
  * Filter By Activity Speed
    * Filter activities from appearing on the Heat Map by their average speed.
  * Filter By Activity Type
    * Filter activities from appearing on the Heat Map by their type.
    * Please note that many activities (especially ones recorded by Garmin devices) may falsely report their types as runs. If you run into this problem I recommend using the "filter by activity speed" option. By filtering activities slower than your slowest speed and faster than your fastest speed for a particular activity type you can acheive the same effect.
  * Configure Renderer
    * These options change the appearence of Heat Map. 
    * Width and Height specify the size of the resulting image.
    * The color values specify how heat appears on the map. Linear interpolation is used the smoothly adjust the heat color from the minimum to low to high and finally to maximum based on the number of activities to cross any point on the map. Background color is used when the "Download map from Map Quest" option is not selected to fill the background of the Heat Map. 

## Beta Testing
Please note that this application is currently in Beta and there are probably bugs that I haven't caught yet. All feedback is accepted and encouraged!! Thank you for being a Beta tester!
