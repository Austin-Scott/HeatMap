# HeatMap
![mapwithheatmap](***REMOVED***)
*The background image is from OpenStreetMap.org and is © OpenStreetMap contributors.* 

Show your friends and family what you've accomplished on foot or by bike by creating a Heat Map summary of all of your activities on Strava, Garmin Connect, or other GPS fitness sites! Here is a Heat Map of everywhere I ran in 2018. The "hotter" the path the more times I have crossed that location!

## The Interface
![gui](https://user-images.githubusercontent.com/12504656/50707304-ff071780-1025-11e9-95dd-b9266a6730ea.PNG)

## Getting Started Guide
### For Strava Users
Download a complete archive of your account by going to the [Account Download and Deletion page](https://www.strava.com/athlete/delete_your_account). **Downloading an archive will not harm your account in any way.** Click on the "Request Your Archive" button under the second step. After a couple of hours Strava will send you an email with a link to download a Zip file containing all of the activities you have ever uploaded to your account. Unzip this archive somewhere you can easily find again. Then launch the Heat Map application and select the "activities" subdirectory from the unzipped archive when you are prompted. Also, before selecting "Load Activities", make sure you have the "Automatically decompress .gz files" checkbox ticked. Strava tends to send activities that have been compressed with gzip and this option will let the application read these files as well. Select "Load Activities" and follow the on-screen instructions.
### For Garmin Connect Users
Coming soon...

### For anyone else
Just download all of your GPS activity files in any of the supported formats below and save them inside of a folder/directory. Then select that directory when you start the program and follow the on screen instructions!

## Beta Testing
Please note that this application is currently in Beta and there are probably bugs that I haven't caught yet. All feedback is accepted and encouraged!! Thank you for being a Beta tester!

## GPS file formats supported

| Format | GPS Tracks | Date Filtering | Activity Type Filtering | Average Speed Filtering | 
| --- | --- | --- | --- | --- |
| .tcx | Yes | Yes | Yes | Yes |
| .gpx | Yes | Yes *(If found in meta-data)* | **No** | **No** |
| .fit | Yes | Yes | Yes | Yes |

Files with the extensions ".tcx.gz", ".gpx.gz", and ".fit.gz" that are commonly found in Strava account archives are simply compressed versions of the above file types. This application will automatically decompress and read any files found in these formats.
