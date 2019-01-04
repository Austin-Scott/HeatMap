# HeatMap
![mapwithheatmap](***REMOVED***)
*The background image is from OpenStreetMap.org and is © OpenStreetMap contributors.* 

Show your friends and family what you've accomplished on foot or by bike by creating a Heat Map summary of all of your activities on Strava, Garmin Connect, or other GPS fitness sites! Here is a Heat Map of everywhere I ran in 2018. The "hotter" the path the more times I have crossed that location!

## The Interface
![gui](https://user-images.githubusercontent.com/12504656/50707304-ff071780-1025-11e9-95dd-b9266a6730ea.PNG)


## GPS file formats supported

| Format | GPS Tracks | Date Filtering | Activity Type Filtering | Average Speed Filtering | 
| --- | --- | --- | --- | --- |
| .tcx | Yes | Yes | Yes | Yes |
| .gpx | Yes | Yes *(If found in meta-data)* | **No** | **No** |
| .fit | Yes | Yes | Yes | Yes |

Files with the extensions ".tcx.gz", ".gpx.gz", and ".fit.gz" that are commonly found in Strava account archives are simply compressed versions of the above file types. This application will automatically decompress and read any files found in these formats.
