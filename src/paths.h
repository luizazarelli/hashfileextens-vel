#include "libs.h"

#ifndef PATHS_H
#define PATHS_H

void* createAllPaths();

void freeAllPaths(void* ap);

char* prepareDir(char* dir);

void prepareDoc(char* asread, char* onlyname);

void setBed(void* ap, char* bed);

void setBsd(void* ap, char* bsd);

void setBedGeo(void* ap, char* bedgeo);

void setGeoArq(void* ap, char* geoarq);

void setGeoName(void* ap, char* geoname);

void setBedPm(void* ap, char* bedpm);

void setPmArq(void* ap, char* pmarq);

void setPmName(void* ap, char* pmname);

void setBedQry(void* ap, char* bedqry);

void setQryArq(void* ap, char* qryarq);

void setQryName(void* ap, char* qryname);

void setBsdGeoSvg(void* ap, char* bsdgeosvg);

void setBsdGeoQrySvg(void* ap, char* bsdgeoqrysvg);

void setBsdGeoQryTxt(void* ap, char* bsdgeoqrytxt);

char* getBed(void* ap);

char* getBsd(void* ap);

char* getBedGeo(void* ap);

char* getGeoArq(void* ap);

char* getGeoName(void* ap);

char* getBedPm(void* ap);

char* getPmArq(void* ap);

char* getPmName(void* ap);

char* getBedQry(void* ap);

char* getQryArq(void* ap);

char* getQryName(void* ap);

char* getBsdGeoSvg(void* ap);

char* getBsdGeoQrySvg(void* ap);

char* getBsdGeoQryTxt(void* ap);

#endif
