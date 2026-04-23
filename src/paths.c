#include "paths.h"

typedef struct all_paths
{
    char* bed;
    char* bsd;
    char* bedgeo;
    char* geoarq;
    char* geoname;
    char* bedpm;
    char* pmarq;
    char* pmname;
    char* bedqry;
    char* qryarq;
    char* qryname;
    char* bsdgeosvg;
    char* bsdgeoqrysvg;
    char* bsdgeoqrytxt;
} all_paths;

void* createAllPaths()
{
    all_paths* ap = calloc(1, sizeof(all_paths));
    assert(ap != NULL);
    return ap;
}

void freeAllPaths(void* aptofree)
{
    if (!aptofree) return;
    all_paths* ap = aptofree;
    free(ap->bed);
    free(ap->bsd);
    free(ap->bedgeo);
    free(ap->geoarq);
    free(ap->geoname);
    free(ap->bedpm);
    free(ap->pmarq);
    free(ap->pmname);
    free(ap->bedqry);
    free(ap->qryarq);
    free(ap->qryname);
    free(ap->bsdgeosvg);
    free(ap->bsdgeoqrysvg);
    free(ap->bsdgeoqrytxt);
    free(ap);
}

char* prepareDir(char* dir)
{
    if (!dir) return NULL;
    size_t len = strlen(dir);
    if (len > 0 && dir[len - 1] != '/')
    {
        dir = realloc(dir, len + 2);
        assert(dir != NULL);
        dir[len]     = '/';
        dir[len + 1] = '\0';
    }
    return dir;
}

void prepareDoc(char* asread, char* onlyname)
{
    assert(asread != NULL);
    assert(onlyname != NULL);

    char* bar = strrchr(asread, '/');
    if (bar)
        strcpy(onlyname, bar + 1);
    else
        strcpy(onlyname, asread);

    char* dot = strchr(onlyname, '.');
    if (dot)
        *dot = '\0';
}

void setBed(void* ap, char* bed)           { ((all_paths*)ap)->bed = bed; }
void setBsd(void* ap, char* bsd)           { ((all_paths*)ap)->bsd = bsd; }
void setBedGeo(void* ap, char* bedgeo)     { ((all_paths*)ap)->bedgeo = bedgeo; }
void setGeoArq(void* ap, char* geoarq)     { ((all_paths*)ap)->geoarq = geoarq; }
void setGeoName(void* ap, char* geoname)   { ((all_paths*)ap)->geoname = geoname; }
void setBedPm(void* ap, char* bedpm)       { ((all_paths*)ap)->bedpm = bedpm; }
void setPmArq(void* ap, char* pmarq)       { ((all_paths*)ap)->pmarq = pmarq; }
void setPmName(void* ap, char* pmname)     { ((all_paths*)ap)->pmname = pmname; }
void setBedQry(void* ap, char* bedqry)     { ((all_paths*)ap)->bedqry = bedqry; }
void setQryArq(void* ap, char* qryarq)     { ((all_paths*)ap)->qryarq = qryarq; }
void setQryName(void* ap, char* qryname)   { ((all_paths*)ap)->qryname = qryname; }
void setBsdGeoSvg(void* ap, char* v)       { ((all_paths*)ap)->bsdgeosvg = v; }
void setBsdGeoQrySvg(void* ap, char* v)    { ((all_paths*)ap)->bsdgeoqrysvg = v; }
void setBsdGeoQryTxt(void* ap, char* v)    { ((all_paths*)ap)->bsdgeoqrytxt = v; }

char* getBed(void* ap)          { return ((all_paths*)ap)->bed; }
char* getBsd(void* ap)          { return ((all_paths*)ap)->bsd; }
char* getBedGeo(void* ap)       { return ((all_paths*)ap)->bedgeo; }
char* getGeoArq(void* ap)       { return ((all_paths*)ap)->geoarq; }
char* getGeoName(void* ap)      { return ((all_paths*)ap)->geoname; }
char* getBedPm(void* ap)        { return ((all_paths*)ap)->bedpm; }
char* getPmArq(void* ap)        { return ((all_paths*)ap)->pmarq; }
char* getPmName(void* ap)       { return ((all_paths*)ap)->pmname; }
char* getBedQry(void* ap)       { return ((all_paths*)ap)->bedqry; }
char* getQryArq(void* ap)       { return ((all_paths*)ap)->qryarq; }
char* getQryName(void* ap)      { return ((all_paths*)ap)->qryname; }
char* getBsdGeoSvg(void* ap)    { return ((all_paths*)ap)->bsdgeosvg; }
char* getBsdGeoQrySvg(void* ap) { return ((all_paths*)ap)->bsdgeoqrysvg; }
char* getBsdGeoQryTxt(void* ap) { return ((all_paths*)ap)->bsdgeoqrytxt; }
