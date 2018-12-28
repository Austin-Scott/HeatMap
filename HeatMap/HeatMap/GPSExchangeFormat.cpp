#include "GPSExchangeFormat.h"

GPSExchangeFormat::GPSExchangeFormat(string filename)
{
	xml_document<> xmlDocument;
	xml_node<>* rootNode;
	ifstream file(filename, ios::binary);
	if (file) {
		cout << "\t*Loading file into memory..." << endl;

		filebuf* fbuf = file.rdbuf();
		size_t size = fbuf->pubseekoff(0, file.end, file.in);
		fbuf->pubseekpos(0, file.in);
		char* buffer = new char[size];
		fbuf->sgetn(buffer, size);
		file.close();

		string fileBuffer(buffer, size);
		delete[] buffer;

		char* cstring = new char[fileBuffer.length() + 1];
		for (int i = 0; i < fileBuffer.length(); i++) {
			cstring[i] = fileBuffer[i];
		}
		cstring[fileBuffer.length()] = '\0';

		cout << "\t*Parsing file..." << endl;

		xmlDocument.parse<0>(cstring);

		rootNode = xmlDocument.first_node("gpx")->first_node("trk");

		int trackPointCount = 0;

		for (xml_node<>* lapNode = rootNode->first_node("trkseg"); lapNode; lapNode = lapNode->next_sibling()) {
			for (xml_node<>* pointNode = lapNode->first_node("trkpt"); pointNode; pointNode = pointNode->next_sibling()) {
				track.emplace_back(stod(pointNode->first_attribute("lat")->value()), stod(pointNode->first_attribute("lon")->value()));
				trackPointCount++;
			}
		}
		delete[] cstring;

		cout << "\t*Successfully retrieved " << trackPointCount << " tracking points from file." << endl;
	}
	else {
		cerr << "Error: Couldn't open " << filename << " for reading." << endl;
	}
}

const vector<GeographicCoordinate>& GPSExchangeFormat::getTrack()
{
	return track;
}
